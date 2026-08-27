#!/usr/bin/env python3
"""
Print scratch-pad BD patches and PREEMPT opcode page references in an aiebu ELF.

Usage: python3 read_bd.py <elf>

Two ELF layouts are supported:

Merged format (.ctrltext.<col>.<name_id>, one large section per column):
  The symbol's shndx identifies the column's ctrltext section directly.
  r_off is a byte offset relative to the start of that section (not absolute VMA).
  Each APPLY_OFFSET_57 relocation sits at r_off and is preceded by a 16-byte
  placeholder prefix, so the actual BD starts at:
    offset = r_off + 16
  Page index comes from the 8KB page header at the containing page slot.
  BD words: [addr_hi[24:0], addr_lo[31:0], len_words]

Per-page format (.ctrltext.<col>.<page> + .ctrldata.<col>.<page>):
  Detected via ELF OS/ABI 0x46. The symbol's shndx identifies the ctrltext page.
  r_off encodes the ctrldata offset as: r_off = ctrltext_size + data_offset - 16
    => data_offset = r_off - ctrltext_size + 16; BD is in the paired ctrldata.
  Page index comes from the ctrltext section name (.ctrltext.<col>.<page>).
  BD words: [addr_hi[24:0], addr_lo[31:0], len_words]
"""
import os
import struct
import sys

PAGE_LENGTH = 0x2000
PAGE_HEADER = 16
PREEMPT_OPCODE = 0x19
ALIGN_OPCODE = 0xA5
OSABI_AIE2PS_GROUP = 0x46
ELF_VERSION_CONFIG = 0x21

def u16(d, o):
    return struct.unpack_from('<H', d, o)[0]

def u32(d, o):
    return struct.unpack_from('<I', d, o)[0]

def load_elf(path):
    raw = open(path, 'rb').read()
    assert raw[:4] == b'\x7fELF', "Not an ELF"
    bits = 32 if raw[4] == 1 else 64
    if bits == 32:
        shoff = struct.unpack_from('<I', raw, 32)[0]
        esz, n, sx = struct.unpack_from('<HHH', raw, 46)
    else:
        shoff = struct.unpack_from('<Q', raw, 40)[0]
        esz, n, sx = struct.unpack_from('<HHH', raw, 58)

    secs = []
    for i in range(n):
        b = raw[shoff + i * esz: shoff + i * esz + esz]
        if bits == 32:
            nm, ty, fl, addr, off, sz = struct.unpack_from('<IIIIII', b)
            lk = struct.unpack_from('<I', b, 24)[0]
            entsz = struct.unpack_from('<I', b, 36)[0]
        else:
            nm, ty = struct.unpack_from('<II', b)
            fl, addr, off, sz = struct.unpack_from('<QQQQ', b, 8)
            lk = struct.unpack_from('<I', b, 40)[0]
            entsz = struct.unpack_from('<Q', b, 56)[0]
        secs.append(dict(nm=nm, ty=ty, off=off, sz=sz, lk=lk, entsz=entsz,
                         addr=addr, data=raw[off:off + sz]))

    shstr = secs[sx]['data']
    for s in secs:
        e = shstr.index(b'\x00', s['nm'])
        s['name'] = shstr[s['nm']:e].decode()
    elf_merged = raw[7] != OSABI_AIE2PS_GROUP and raw[8] >= ELF_VERSION_CONFIG
    return secs, bits, elf_merged

def _is_merged(sec, elf_merged):
    """True when this ctrltext section belongs to a merged-section ELF."""
    if not elf_merged:
        return False
    parts = sec['name'].split('.')
    return len(parts) >= 3 and parts[1] == 'ctrltext'

def _section_page_num(sec):
    """Return page index encoded in a per-page ctrltext section name.

    Matches ctrlcode util.get_pagenum(): second numeric component of the name,
    e.g. .ctrltext.0.3 -> 3, .ctrltext.0.3.0 -> 3.
    """
    nums = [int(p) for p in sec['name'].split('.') if p.isdigit()]
    return nums[1] if len(nums) >= 2 else 0

def _merged_page_num(sec_data, offset):
    """Return page index from the 8KB page header at offset's page slot."""
    page_base = (offset // PAGE_LENGTH) * PAGE_LENGTH
    if page_base + 4 <= len(sec_data):
        return u16(sec_data, page_base + 2)
    return offset // PAGE_LENGTH

def _bd_page_num(ct_sec, sec, offset, elf_merged):
    """Return the page index for a scratch-pad BD at offset within its section."""
    if _is_merged(ct_sec, elf_merged):
        return _merged_page_num(sec['data'], offset)
    return _section_page_num(ct_sec)

def _isa_spec_path():
    here = os.path.dirname(os.path.abspath(__file__))
    return os.path.normpath(os.path.join(here, '..', '..', '..',
                                          'specification', 'aie2ps', 'isa-spec.yaml'))

def load_opcode_sizes():
    """Build opcode-byte -> instruction-size map from isa-spec.yaml."""
    import yaml

    with open(_isa_spec_path(), 'r', encoding='utf-8') as f:
        spec = yaml.safe_load(f)

    sizes = {ALIGN_OPCODE: 1}
    for op in spec.get('operations', []):
        width = 16  # opcode byte + pad byte
        for arg in op.get('arguments', []):
            if arg.get('type') == 'patch_buf':
                continue
            if arg.get('type') in ('register', 'barrier'):
                width += 8
            else:
                width += arg.get('width', 0)
        sizes[op['opcode']] = width // 8
    return sizes

def _parse_preempt(data, pos):
    """Decode PREEMPT at data[pos:]; returns (id, save_page, restore_page)."""
    if pos + 8 > len(data) or data[pos] != PREEMPT_OPCODE:
        return None
    preempt_id = u16(data, pos + 2)
    save_page = u16(data, pos + 4)
    restore_page = u16(data, pos + 6)
    return preempt_id, save_page, restore_page

def _walk_page(data, page_base, page_idx, col, opcode_sizes, out):
    pos = PAGE_HEADER
    limit = min(len(data), PAGE_LENGTH)
    while pos < limit:
        op = data[pos]
        if op == ALIGN_OPCODE:
            pos += 1
            continue
        size = opcode_sizes.get(op)
        if size is None or pos + size > limit:
            break
        if op == PREEMPT_OPCODE:
            decoded = _parse_preempt(data, pos)
            if decoded:
                preempt_id, save_page, restore_page = decoded
                out.append(dict(col=col, page=page_idx,
                                offset=page_base + pos,
                                preempt_id=preempt_id,
                                save_page=save_page,
                                restore_page=restore_page))
        pos += size

def scan_preempt_opcodes(secs, opcode_sizes, elf_merged):
    """Find every PREEMPT opcode in .ctrltext.* sections."""
    out = []
    for sec in secs:
        name = sec['name']
        parts = name.split('.')
        if len(parts) < 3 or parts[1] != 'ctrltext':
            continue

        col = parts[2]
        data = sec['data']

        if _is_merged(sec, elf_merged):
            for page_idx in range(0, len(data), PAGE_LENGTH):
                page = data[page_idx:page_idx + PAGE_LENGTH]
                _walk_page(page, page_idx, page_idx // PAGE_LENGTH,
                           col, opcode_sizes, out)
        else:
            _walk_page(data, 0, _section_page_num(sec), col, opcode_sizes, out)

    out.sort(key=lambda r: (int(r['col']), r['page'], r['offset']))
    return out

def print_scratchpad_bds(secs, bits, elf_merged):
    SHT_DYNSYM, SHT_RELA = 11, 4
    dsym = next((s for s in secs if s['ty'] == SHT_DYNSYM), None)
    rela = next((s for s in secs if s['ty'] == SHT_RELA), None)
    if not dsym or not rela:
        print("No dynsym or rela section")
        sys.exit(1)

    strtab = secs[dsym['lk']]['data']
    entsz = dsym['entsz'] or (16 if bits == 32 else 24)
    spm = {}
    for i in range(0, len(dsym['data']), entsz):
        if bits == 32:
            nm, val, sz, info, other, shndx = struct.unpack_from('<IIIBBH', dsym['data'], i)
        else:
            nm, info, other, shndx = struct.unpack_from('<IBBH', dsym['data'], i)
        e = strtab.index(b'\x00', nm)
        name = strtab[nm:e].decode()
        if name == 'scratch-pad-mem':
            spm[i // entsz] = shndx

    sec_by_idx = {i: s for i, s in enumerate(secs)}
    rsz = rela['entsz'] or (12 if bits == 32 else 24)
    fmt = lambda b: f"{b/1024/1024:.2f}MB" if b >= 1024 * 1024 else f"{b//1024}KB"

    print(f"\n\nList to all shim BDs:")
    print(f"{'Col':<6} {'Page':<6} {'r_off':<12} {'address':>14}  {'len(bytes)':>12}  size")
    print("-" * 68)

    for i in range(0, len(rela['data']), rsz):
        if bits == 32:
            r_off, r_info = struct.unpack_from('<II', rela['data'], i)
            sym_idx = r_info >> 8
        else:
            r_off, r_info = struct.unpack_from('<QQ', rela['data'], i)
            sym_idx = r_info >> 32
        if sym_idx not in spm:
            continue

        shndx = spm[sym_idx]
        ct_sec = sec_by_idx.get(shndx)
        if not ct_sec:
            continue

        if _is_merged(ct_sec, elf_merged):
            sec = ct_sec
            offset = r_off + 16
        else:
            sec = sec_by_idx.get(shndx + 1)
            if not sec:
                continue
            offset = r_off - ct_sec['sz'] + 16

        if offset < 0 or offset + 12 > sec['sz']:
            continue

        w0 = u32(sec['data'], offset)
        w1 = u32(sec['data'], offset + 4)
        w2 = u32(sec['data'], offset + 8)
        addr = ((w0 & 0x1FFFFFF) << 32) | w1
        lb = w2 * 4
        col = ct_sec['name'].split('.')[2] if ct_sec['name'].count('.') >= 2 else '?'
        page = _bd_page_num(ct_sec, sec, offset, elf_merged)
        print(f"{col:<6} {page:<6} 0x{r_off:<10x} 0x{addr:>12x}  0x{lb:>10x}  {fmt(lb)}")

def print_preempt_opcodes(records):
    print(f"List to all preempt opcodes:")
    print(f"{'Col':<6} {'Page':<6} {'Offset':<10} {'Id':<8} {'SavePage':<10} {'RestorePage':<12}")
    print("-" * 60)
    if not records:
        print("(none)")
        return
    for r in records:
        print(f"{r['col']:<6} {r['page']:<6} 0x{r['offset']:<8x} "
              f"0x{r['preempt_id']:04x}   {r['save_page']:<10} {r['restore_page']:<12}")

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <elf>")
        sys.exit(1)

    secs, bits, elf_merged = load_elf(sys.argv[1])
    opcode_sizes = load_opcode_sizes()

    print_preempt_opcodes(scan_preempt_opcodes(secs, opcode_sizes, elf_merged))
    print_scratchpad_bds(secs, bits, elf_merged)

if __name__ == '__main__':
    main()

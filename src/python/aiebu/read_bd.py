#!/usr/bin/env python3
"""
Print address and length for every BD patched with 'scratch-pad-mem' in an aiebu ELF.

Usage: python3 read_bd.py <elf>

Two ELF layouts are supported:

Merged format (.ctrltext.<col>.0, one large section per column):
  The symbol's shndx identifies the column's ctrltext section directly.
  r_off is a section-local VMA (offset from the section's own load address).
  Each APPLY_OFFSET_57 relocation sits at (r_off - sec.addr) and is preceded
  by a 16-byte placeholder prefix, so the actual BD starts at:
    offset = (r_off - sec.addr) + 16
  BD words: [addr_hi[24:0], addr_lo[31:0], len_words]

Per-page format (.ctrltext.<col>.<page> + .ctrldata.<col>.<page>):
  The symbol's shndx identifies the ctrltext page section.
  r_off encodes the ctrldata offset as: r_off = ctrltext_size + data_offset - 16
    => data_offset = r_off - ctrltext_size + 16; BD is in the paired ctrldata.
  BD words: [addr_hi[24:0], addr_lo[31:0], len_words]
"""
import sys, struct

def u32(d, o): return struct.unpack_from('<I', d, o)[0]

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
        b = raw[shoff + i*esz: shoff + i*esz + esz]
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
                         addr=addr, data=raw[off:off+sz]))

    shstr = secs[sx]['data']
    for s in secs:
        e = shstr.index(b'\x00', s['nm'])
        s['name'] = shstr[s['nm']:e].decode()
    return secs, bits

def _is_merged(sec):
    """True if merged format: section name has exactly 3 dots, last part is '0'.

    Merged: .ctrltext.<col>.0  (4 name components when split on '.')
    Per-page: .ctrltext.<col>.<page>.0  (5 name components)
    """
    parts = sec['name'].split('.')
    return len(parts) == 4 and parts[1] == 'ctrltext' and parts[3] == '0'

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <elf>"); sys.exit(1)

    secs, bits = load_elf(sys.argv[1])

    # dynsym: find all 'scratch-pad-mem' symbol indices -> their section index
    SHT_DYNSYM, SHT_RELA = 11, 4
    dsym = next((s for s in secs if s['ty'] == SHT_DYNSYM), None)
    rela = next((s for s in secs if s['ty'] == SHT_RELA), None)
    if not dsym or not rela:
        print("No dynsym or rela section"); sys.exit(1)

    strtab = secs[dsym['lk']]['data']
    entsz  = dsym['entsz'] or (16 if bits == 32 else 24)
    spm    = {}  # sym_idx -> shndx
    for i in range(0, len(dsym['data']), entsz):
        if bits == 32:
            nm, val, sz, info, other, shndx = struct.unpack_from('<IIIBBH', dsym['data'], i)
        else:
            nm, info, other, shndx = struct.unpack_from('<IBBH', dsym['data'], i)
        e    = strtab.index(b'\x00', nm)
        name = strtab[nm:e].decode()
        if name == 'scratch-pad-mem':
            spm[i // entsz] = shndx

    sec_by_idx = {i: s for i, s in enumerate(secs)}
    rsz = rela['entsz'] or (12 if bits == 32 else 24)

    fmt = lambda b: f"{b/1024/1024:.2f}MB" if b >= 1024*1024 else f"{b//1024}KB"

    print(f"{'Col':<6} {'r_off':<12} {'address':>14}  {'len(bytes)':>12}  size")
    print("-" * 60)

    for i in range(0, len(rela['data']), rsz):
        if bits == 32:
            r_off, r_info = struct.unpack_from('<II', rela['data'], i)
            sym_idx = r_info >> 8
        else:
            r_off, r_info = struct.unpack_from('<QQ', rela['data'], i)
            sym_idx = r_info >> 32
        if sym_idx not in spm:
            continue

        shndx  = spm[sym_idx]
        ct_sec = sec_by_idx.get(shndx)
        if not ct_sec:
            continue

        if _is_merged(ct_sec):
            # Merged format: shndx points to this column's ctrltext section.
            # r_off is a raw byte offset into that section data.
            # The BD follows a 16-byte APPLY_OFFSET_57 placeholder prefix.
            sec    = ct_sec
            offset = r_off + 16
        else:
            # Per-page format: BD is in the paired ctrldata section (shndx + 1).
            # r_off encodes: r_off = ctrltext_size + data_offset - 16
            sec = sec_by_idx.get(shndx + 1)
            if not sec:
                continue
            offset = r_off - ct_sec['sz'] + 16

        if offset < 0 or offset + 12 > sec['sz']:
            continue

        # BD layout: word[0]=addr_hi [24:0], word[1]=addr_lo [31:0], word[2]=len_words
        w0 = u32(sec['data'], offset)
        w1 = u32(sec['data'], offset + 4)
        w2 = u32(sec['data'], offset + 8)
        addr  = ((w0 & 0x1FFFFFF) << 32) | w1
        lb    = w2 * 4
        col   = sec['name'].split('.')[2] if sec['name'].count('.') >= 2 else '?'
        print(f"{col:<6} 0x{r_off:<10x} 0x{addr:>12x}  0x{lb:>10x}  {fmt(lb)}")

if __name__ == '__main__':
    main()

---
description: Inspect one or more aie2ps or aie4* ELFs and list all PDI IDs referenced by load_pdi instructions. For config ELFs groups by kernel/instance.
---

Run the Python script below using the Bash tool. One or more ELF file paths are passed as `$ARGUMENTS` (space-separated, the text after `/get_pdi_detail` on the command line).

## Task

For each ELF path provided: parse the ELF, verify it is an AIE ELF, and report every `load_pdi` instruction found in all ctrltext sections. For config ELFs with kernel/instance info, group by kernel:instance. Print a separator between ELFs when more than one is given.

## ELF layout reference

### Page header (16 bytes, present in ALL config ctrltext sections ABI >= 0x03)
- Bytes 0-1: `0xFF 0xFF` (magic)
- Bytes 2-3: page index (uint16_t)
- Bytes 4-5: OOO length (uint16_t)
- Bytes 6-7: reserved
- Bytes 8-11: current page length (uint32_t)
- Bytes 12-13: in-order page length (uint16_t)
- Bytes 14-15: reserved
- Code starts at byte **16** in every config ctrltext section

### Non-config ELF (ABI 0x02): no page header, code starts at byte 0

### `load_pdi` instruction encoding (12 bytes total)
```
byte  0:   opcode = 0x1a (26)
byte  1:   pad
bytes 2-3: pad (16-bit)
bytes 4-7: pdi_id (uint32_t, little-endian)
bytes 8-9: page_ref (uint16_t) — page index of PDI data
bytes 10-11: pad
```
`EOF` = opcode `0xFF`, 4 bytes total. Stops code scan within a page.

### Instruction sizes (1 byte opcode + 1 byte implicit pad + arg bytes)
| Opcode | Name                   | Bytes |
|--------|------------------------|-------|
| 0x00   | start_job              | 8     |
| 0x07   | end_job                | 4     |
| 0x08   | yield                  | 4     |
| 0x09   | uc_dma_write_des_sync  | 4     |
| 0x0e   | apply_offset_57        | 8     |
| 0x10   | mov                    | 8     |
| 0x16   | nop                    | 4     |
| 0x17   | start_job_deferred     | 8     |
| 0x18   | launch_job             | 4     |
| 0x19   | preempt                | 8     |
| 0x1a   | **load_pdi**           | **12**|
| 0x1b   | load_last_pdi          | 4     |
| 0x1f   | start_cond_job_preempt | 8     |
| 0x20   | load_cores_cp          | 8     |
| 0x21   | rel_acq_sync           | 12    |
| 0x22   | uc_dma_mask_poll_ext   | 24    |
| 0x23   | apply_offset_pl        | 8     |
| 0xa5   | align (1 byte, no pad) | 1     |
| 0xff   | eof                    | 4     |

### ABI version → page format
- `0x02` (non-config): no page header, code starts at offset 0, one ctrltext per page
- `0x03`, `0x10`, `0x20` (config, separate-page): 16-byte page header, code at offset 16
- `0x21+` (config, merged-page): 16-byte page header per 8KB page, N pages per section

### Kernel/instance mapping (config ELFs, ABI >= 0x03)
`.symtab` (16-byte entries, ELF32): `st_name(4) st_value(4) st_size(4) st_info(1) st_other(1) st_shndx(2)`
- Kernel: `st_info & 0xf == 2` (STT_FUNC), `st_shndx == 0`, mangled name → demangle with `_Z\d+(\w+)`
- Instance: `st_info & 0xf == 1` (STT_OBJECT), `st_shndx == kernel_sym_index`
- Group index (last component of `.ctrltext.COL.PAGE.GRP`) maps to (kernel, instance) in declaration order

## Python script to run

```python
import struct, subprocess

OSABI_NAMES = {
    0x40:'aie2ps', 0x45:'aie2p', 0x46:'aie2ps/aie4-group',
    0x4B:'aie4',   0x56:'aie4a', 0x69:'aie4z',
}
INSN_SIZES = {
    0x00:8,  0x01:8,  0x02:4,  0x03:12, 0x04:12, 0x05:12,
    0x06:8,  0x07:4,  0x08:4,  0x09:4,  0x0b:12, 0x0c:8,
    0x0d:4,  0x0e:8,  0x0f:8,  0x10:8,  0x11:4,  0x12:8,
    0x13:4,  0x14:8,  0x15:4,  0x16:4,  0x17:8,  0x18:4,
    0x19:8,  0x1a:12, 0x1b:4,  0x1c:8,  0x1f:8,
    0x20:8,  0x21:12, 0x22:24, 0x23:8,  0xff:4,
}

def scan_page(sec_data, code_start, code_end, col, page_idx):
    """Return list of (col, page_idx, pdi_id, page_ref) for every load_pdi in [code_start, code_end)."""
    results = []
    i = code_start
    while i < code_end and i < len(sec_data):
        op = sec_data[i]
        if op == 0xff: break
        if op == 0xa5: i += 1; continue
        if op == 0x1a and i + 12 <= len(sec_data):
            pdi_id   = struct.unpack_from('<I', sec_data, i + 4)[0]
            page_ref = struct.unpack_from('<H', sec_data, i + 8)[0]
            results.append((col, page_idx, pdi_id, page_ref))
        sz = INSN_SIZES.get(op, 0)
        if sz == 0: break
        i += sz
    return results

def get_pdi_detail(elf_path):
    with open(elf_path, 'rb') as f:
        data = f.read()

    if data[:4] != b'\x7fELF':
        print(f"Not an ELF file: {elf_path}"); return
    osabi  = data[7]
    abiver = data[8]
    if osabi not in OSABI_NAMES:
        print(f"Not an AIE ELF (OSABI=0x{osabi:02x})"); return

    is_merged  = (abiver >= 0x21)
    is_config  = (abiver >= 0x03)
    elf_type   = "merged-config" if is_merged else ("config" if is_config else "non-config")
    print(f"ELF: {elf_path}")
    print(f"  OSABI: 0x{osabi:02x}  ABI Version: 0x{abiver:02x}"
          f"  Platform: {OSABI_NAMES[osabi]}  Type: {elf_type}")
    print()

    # Parse ELF32 section headers
    e_shoff   = struct.unpack_from('<I', data, 32)[0]
    shentsize = struct.unpack_from('<H', data, 46)[0]
    shnum     = struct.unpack_from('<H', data, 48)[0]
    shstrndx  = struct.unpack_from('<H', data, 50)[0]
    shstr_off = struct.unpack_from('<I', data, e_shoff + shstrndx * shentsize + 16)[0]

    sections = {}
    for i in range(shnum):
        h = e_shoff + i * shentsize
        name_idx, sh_type = struct.unpack_from('<II', data, h)
        sh_offset, sh_size = struct.unpack_from('<II', data, h + 16)
        name = data[shstr_off + name_idx: shstr_off + name_idx + 80].split(b'\x00')[0].decode()
        sections[name] = {'type': sh_type, 'offset': sh_offset, 'size': sh_size, 'index': i}

    # ── Kernel / instance hierarchy from .symtab ──────────────────────────────
    # ELF writer order: kernel_A, instance_A0, instance_A1, ..., kernel_B, ...
    # Instance st_shndx = symtab index of its owning kernel symbol.
    # group_idx assigned sequentially across all instances in declaration order,
    # matching the last component of .ctrltext.COL.PAGE.GRP section names.
    kernel_order        = []   # [(sym_idx, mangled_name), ...]
    instances_by_kernel = {}   # sym_idx -> [instance_name, ...]
    group_map           = {}   # group_idx -> (kernel_name, instance_name)

    if is_config and '.strtab' in sections and '.symtab' in sections:
        strtab_off = sections['.strtab']['offset']
        sym_off    = sections['.symtab']['offset']
        sym_size   = sections['.symtab']['size']
        for si in range(sym_size // 16):
            s        = sym_off + si * 16
            st_name  = struct.unpack_from('<I', data, s)[0]
            st_info  = data[s + 12]
            st_shndx = struct.unpack_from('<H', data, s + 14)[0]
            st_type  = st_info & 0xf
            sym_name = data[strtab_off + st_name: strtab_off + st_name + 80].split(b'\x00')[0].decode()
            if not sym_name:
                continue
            if st_type == 2:   # STT_FUNC → kernel
                kernel_order.append((si, sym_name))
                instances_by_kernel[si] = []
            elif st_type == 1: # STT_OBJECT → instance; st_shndx = owning kernel's sym index
                if st_shndx in instances_by_kernel:
                    instances_by_kernel[st_shndx].append(sym_name)

        # Demangle kernel names via c++filt
        mangled = [k for _, k in kernel_order]
        try:
            r = subprocess.run(['c++filt'] + mangled, capture_output=True, text=True, check=True)
            demangled = r.stdout.strip().splitlines()
        except Exception:
            demangled = mangled
        kernel_order = [(idx, demangled[i] if i < len(demangled) else raw)
                        for i, (idx, raw) in enumerate(kernel_order)]

        grp = 0
        for k_idx, k_name in kernel_order:
            for i_name in instances_by_kernel.get(k_idx, []):
                group_map[grp] = (k_name, i_name)
                grp += 1

    # ── Always print kernel / instance structure ───────────────────────────────
    if kernel_order:
        print("Kernels / Instances:")
        for k_idx, k_name in kernel_order:
            print(f"  Kernel: {k_name}")
            for i_name in instances_by_kernel.get(k_idx, []):
                print(f"    Instance: {i_name}")
        print()

    # ── Scan ctrltext sections ─────────────────────────────────────────────────
    # Collect hits keyed by (col, section_name) so we can display per-section.
    # Also record section → group_idx for kernel/instance labelling.
    PAGE_SIZE = 8192
    sec_hits    = {}   # sec_name -> list of (col, page_idx, pdi_id, page_ref)
    sec_grp_idx = {}   # sec_name -> group_idx (if encoded in name)
    sec_col     = {}   # sec_name -> col

    for sec_name in sorted(sections):
        if not sec_name.startswith('.ctrltext') or sections[sec_name]['type'] != 1:
            continue
        sec      = sections[sec_name]
        sec_data = data[sec['offset']: sec['offset'] + sec['size']]
        parts    = sec_name.split('.')  # ['','ctrltext','COL','PAGE'(,'GRP')]
        col      = int(parts[2]) if len(parts) > 2 else 0
        grp_idx  = int(parts[4]) if len(parts) > 4 else None
        sec_col[sec_name]     = col
        sec_grp_idx[sec_name] = grp_idx
        hits = []
        if is_merged:
            for pg in range(sec['size'] // PAGE_SIZE):
                base = pg * PAGE_SIZE
                hits.extend(scan_page(sec_data, base + 16, base + PAGE_SIZE, col, pg))
        else:
            base_page  = int(parts[3]) if len(parts) > 3 else 0
            code_start = 16 if is_config else 0
            hits.extend(scan_page(sec_data, code_start, sec['size'], col, base_page))
        sec_hits[sec_name] = hits

    # ── Print load_pdi results ─────────────────────────────────────────────────
    any_hits = any(sec_hits.values())

    if group_map:
        # Group sections by (kernel, instance) using group_idx from section name,
        # falling back to col-based ordering when group_idx is absent.
        print("load_pdi references:")
        for k_idx, k_name in kernel_order:
            printed_kernel = False
            for i_name in instances_by_kernel.get(k_idx, []):
                # Find the group index for this instance
                g = next((gi for gi, (gk, gi_n) in group_map.items()
                          if gk == k_name and gi_n == i_name), None)
                # Collect matching sections: prefer explicit grp_idx match,
                # fall back to all sections with no grp_idx (col-only naming)
                # Prefer sections with explicit group index match; fall back to all
                # col-only sections (same instance runs on every hardware column).
                explicit = [s for s in sorted(sec_hits) if sec_grp_idx[s] == g]
                if explicit:
                    matching = explicit
                else:
                    matching = sorted(
                        [s for s in sec_hits if sec_grp_idx[s] is None],
                        key=lambda s: sec_col[s]
                    )
                hits_for_inst = [(s, h) for s in matching for h in sec_hits[s]]
                if not printed_kernel:
                    print(f"  Kernel: {k_name}")
                    printed_kernel = True
                print(f"    Instance: {i_name}")
                if hits_for_inst:
                    for sec_name, (col, page_idx, pdi_id, page_ref) in hits_for_inst:
                        print(f"      {sec_name}  page {page_idx}:  "
                              f"load_pdi  pdi_id=0x{pdi_id:08x} ({pdi_id})  page_ref={page_ref}")
                else:
                    print(f"      (no load_pdi)")
    else:
        # No kernel/instance info — print flat by section
        print("load_pdi references:")
        if not any_hits:
            print("  (none)")
        else:
            for sec_name in sorted(sec_hits):
                for col, page_idx, pdi_id, page_ref in sec_hits[sec_name]:
                    print(f"  {sec_name}  page {page_idx}:  "
                          f"load_pdi  pdi_id=0x{pdi_id:08x} ({pdi_id})  page_ref={page_ref}")


import shlex
args = shlex.split("$ARGUMENTS")
if not args:
    print("Usage: /get_pdi_detail <elf> [elf2 elf3 ...]")
else:
    for idx, path in enumerate(args):
        if idx > 0:
            print("\n" + "─" * 60)
        get_pdi_detail(path)
```

# Kernel Metadata
## TA Build
```
ryzenai_main_daily_latest
```

## AIE Compiler
```
AI Engine Compiler
                                     Version 2026.2 (linux64-bit)
                                     SW Build dc35d75 (main) on 2026-09-09-16:41:30
                          Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
                          Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
```

## Kernels Included
```
run_flash_mha_uint16: 65
```
## PM Size
```
32062
```

## Dual-PDI Provenance

This is an inherited PDI, not a newly compiled or regenerated PDI. Its six
non-documentation files are byte-for-byte copies of
`prebuilt/fp16_pdi_with_pdi_combination_86/` at
`7bff3c289bd3ec2fd824f01a6696e22fe42d6293`. That payload and its FlashMHA
source are unchanged from the #3707 merge,
`06269d3c8ab90b82b5b95a8727c1b42fc1aee5e6`.

The inherited machine code and listings still name `run_flash_mha_uint16`.
The `batched-mha` PDI 1003 manifest intentionally exposes the distinct entrypoint
`run_flash_mha_uint16_batched` at the same numeric dispatch slot, **65**.
Runtime dispatch uses that slot; the entrypoint separates PDI selection without
patching the validated machine code. This family must not be combined with
the original uint16 or uint8 FlashMHA implementation in a single PDI.

The reproducible source family is now
`kernel/flash_mha_qdq_uint16_batched/`. Its wrapper and 1D kernel preserve
the source above except for the wrapper entrypoint and include-guard names;
`mha_affine_u16.hpp` is unchanged. The current
`kernel/flash_mha_common/mha_stream_params.h` remains shared: the 128-byte QDQ
record's original eight words and the stream parameter layout are unchanged.
An isolated rebuild of the renamed source neither publishes a replacement PDI
nor establishes binary equivalence to this inherited payload. Publishing a
replacement requires separate hardware and numerical validation.

The original `single-mha` source family and PDI 86 are restored from
`13e026039b52140b98cd4bb337319fb2a838d1ec`, whose kernel/PDI files also match
the immediate pre-#3707 revision,
`886f86f6783274938ceff624166267f47adc573b`. The restored source differs only
by trailing-whitespace cleanup and a final newline in `mha_kernel_1d_u16.hpp`;
the restored PDI 86 payload remains byte-identical to those revisions.

### Inherited Source Git Blob IDs

| Original source path under `kernel/flash_mha_qdq_uint16/` | Git blob |
| --- | --- |
| `flash_mha_wrapper.cc` | `789b6b0cbd008f523e9c2ce403f796dddd7d07a5` |
| `mha_kernel_1d_u16.hpp` | `c400df779e9fa05e9dda7866499e4a9ca6bf318a` |
| `mha_affine_u16.hpp` | `0850e236420ed3d0e9489b6d0dd2966691725d11` |

### Inherited Payload SHA-256

`kernel.md` is excluded because this provenance section is new.

| File | SHA-256 |
| --- | --- |
| `0_0.lst` | `5bd3a11b5f06ac7b66b7aea77aeb10e0601184a818b4e285c8b3a70bff6e4618` |
| `aie_asm_elfs.asm` | `ae14bd37d0be9845d081f3e084cbc29eee7ab2152eafe0ac311cb55ced42f500` |
| `aie_asm_init.asm` | `33c29a0b4625d3ad50f6093e884d3c36e993d8fa8921c136c71586abe09573ae` |
| `elf_ctrl_pkt.bin` | `85848079893c4cea0002cf7c2ea4875ee39b918265d436f7a10254b9a3817f18` |
| `packet_size.json` | `acfbbca1f7e6b4e5f428e917e49d3d90d18bf157d8604efecdbed11924c2a9f3` |
| `pdi.asm` | `93895df9aa1678bd394f2e5da197bdc9336500e8f493d3d29fed4956b5061ed3` |

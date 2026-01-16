# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc.

# Re-enable save-restore test in phase2 of decoupling of aiebu from aie-rt
#add_subdirectory(save-restore)

# MD5 name-based uuid generation was corrected to be
# identical on all endian systems in 1.71.0 only.
if (${Boost_VERSION} VERSION_GREATER_EQUAL "1.71.0")
  add_subdirectory(basic)
  add_subdirectory(rich)
  add_subdirectory(fused)
  add_subdirectory(pmctrlpkt)
  add_subdirectory(dump)
  add_subdirectory(mladf_txn)
  add_subdirectory(dma_txn_ctrlpkt_pmload)
  add_subdirectory(transform)
endif()

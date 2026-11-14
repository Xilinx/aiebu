// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#include "aie4_preprocessor_input.h"
#include <cstdint>

namespace aiebu {
void
aie4_preprocessor_input::
patch_ctrl57_aie4_addend(std::vector<char>& buf, uint32_t offset, uint64_t addend) const
{
  auto* bd = reinterpret_cast<uint32_t*>(buf.data() + offset);
  const uint64_t bd0 = bd[1]; // NOLINT
  const uint64_t bd1 = bd[2]; // NOLINT
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
  uint64_t arg = (bd1 & 0xFFFFFFFF) + ((bd0 & 0x1FFFFFF) << 32);
  arg += addend;
  bd[2] = static_cast<uint32_t>(arg & 0xFFFFFFFF);
  bd[1] = static_cast<uint32_t>(((arg >> 32) & 0x1FFFFFF) | (bd0 & 0xFE000000));
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}

void
aie4_preprocessor_input::
patch_ctrlpkt_addend(std::vector<char>& buf, uint32_t offset, uint64_t addend) const
{
  patch_ctrl57_aie4_addend(buf, offset, addend);
}

}

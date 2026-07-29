// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_PREPROCESSOR_PREBUILT_SAVE_RESTORE_H_
#define AIEBU_PREPROCESSOR_PREBUILT_SAVE_RESTORE_H_

#include <cstddef>
#include <cstdint>

namespace aiebu {

// Pointers refer to generated arrays with static storage.
struct prebuilt_blob
{
  const std::uint8_t* data;
  std::size_t size;
};

struct prebuilt_save_restore
{
  prebuilt_blob save;
  prebuilt_blob restore;
};

} // namespace aiebu

#endif // AIEBU_PREPROCESSOR_PREBUILT_SAVE_RESTORE_H_

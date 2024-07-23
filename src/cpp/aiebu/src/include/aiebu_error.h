// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ERROR_H_
#define _AIEBU_ERROR_H_

#include <system_error>
#include "aiebu.h"

#if defined(_WIN32)
#define DRIVER_DLLESPEC __declspec(dllexport)
#else
#define DRIVER_DLLESPEC __attribute__((visibility("default")))
#endif

namespace aiebu {

class error : public std::system_error
{
public:

  enum class DRIVER_DLLESPEC error_code : int
  {
    invalid_asm = aiebu_invalid_asm,
    invalid_patch_schema = aiebu_invalid_patch_schema,
    invalid_patch_buffer_type = aiebu_invalid_batch_buffer_type,
    invalid_buffer_type = aiebu_invalid_buffer_type,
    invalid_offset = aiebu_invalid_offset,
    internal_error = aiebu_invalid_internal_error
  };

  DRIVER_DLLESPEC
  error(error_code ec, const std::error_category& cat, const std::string& what = "");

  DRIVER_DLLESPEC
  explicit
  error(error_code ec, const std::string& what = "");

  // Retrive underlying code for return plain error code
  [[nodiscard]]
  DRIVER_DLLESPEC
  int
  value() const;

  [[nodiscard]]
  DRIVER_DLLESPEC
  int
  get() const;

  [[nodiscard]]
  DRIVER_DLLESPEC
  int
  get_code() const;
};

}

#endif //_AIEBU_ERROR_H_

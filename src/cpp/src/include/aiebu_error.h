// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ERROR_H_
#define _AIEBU_ERROR_H_

#include <system_error>
#include "aiebu.h"

#define DRIVER_DLLESPEC __attribute__((visibility("default")))

namespace aiebu {

class DRIVER_DLLESPEC error : public std::system_error
{
public:

  enum class error_code : int
  {
    invalid_asm = aiebu_invalid_asm,
    invalid_patch_schema = aiebu_invalid_patch_schema,
    invalid_patch_buffer_type = aiebu_invalid_batch_buffer_type,
    invalid_buffer_type = aiebu_invalid_buffer_type,
    invalid_offset = aiebu_invalid_offset,
    internal_error = aiebu_invalid_internal_error
  };

  error(error_code ec, const std::error_category& cat, const std::string& what = "");

  explicit
  error(error_code ec, const std::string& what = "");

  // Retrive underlying code for return plain error code
  int
  value() const;

  int
  get() const;

  int
  get_code() const;
};

}

#endif //_AIEBU_ERROR_H_

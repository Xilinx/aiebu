// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include <string>
#include "aiebu_error.h"
namespace aiebu {

error::
error(error_code ec, const std::error_category& cat, const std::string& what)
  : std::system_error(static_cast<int>(ec), cat, what) {}

error::
error(error_code ec, const std::string& what)
  : system_error(static_cast<int>(ec), std::system_category(), what) {}

// Retrive underlying code for return plain error code
int
error::
value() const
{
  return code().value();
}

int
error::
get() const
{
  return value();
}

int
error::
get_code() const
{
  return value();
}

}

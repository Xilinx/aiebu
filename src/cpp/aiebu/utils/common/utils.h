// SPDX-License-Identifier: MIT
// Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

#ifndef __AIEBU_UTILITIES_UTILS_H_
#define __AIEBU_UTILITIES_UTILS_H_

// Include files
// Please keep these to the bare minimum

#include <string>
#include <vector>
#include <cxxopts.hpp>

#include "target.h"
#include "aiebu_assembler.h"

namespace aiebu::utilities {

  std::vector<const char*>
  vector_of_string_to_vector_of_char(const std::vector<std::string>& args);

}

#endif //__AIEBU_UTILITIES_UTILS_H_

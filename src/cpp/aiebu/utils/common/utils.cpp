// SPDX-License-Identifier: MIT
// Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

#include <iostream>
#include <filesystem>
#include "utils.h"

std::vector<const char*>
aiebu::utilities::vector_of_string_to_vector_of_char(const std::vector<std::string>& args)
{
  std::vector<const char*> char_vec;
  for (auto& arg : args)
    char_vec.push_back(arg.c_str());

  return char_vec;
}

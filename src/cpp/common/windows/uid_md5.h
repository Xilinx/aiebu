// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_COMMON_WINDOWS_UID_MD5_H_
#define _AIEBU_COMMON_WINDOWS_UID_MD5_H_

#include <BaseTsd.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

namespace aiebu {

#define MD5LEN 16

class uid_md5
{
  ULONG_PTR hProv = 0;
  ULONG_PTR hHash = 0;
public:
  uid_md5();
  void update(const std::vector<uint8_t>& data);
  std::string calculate();
  ~uid_md5();
};
}
#endif //_AIEBU_COMMON_WINDOWS_UID_MD5_H_

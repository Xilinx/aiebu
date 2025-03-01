// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_COMMON_LINUX_UID_MD5_H_
#define _AIEBU_COMMON_LINUX_UID_MD5_H_

#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <openssl/md5.h>

namespace aiebu {

class uid_md5 {
    MD5_CTX context;

public:
  uid_md5()
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    MD5_Init(&context);
#pragma GCC diagnostic pop
  }

  void update(const std::vector<uint8_t>& data)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    MD5_Update(&context, data.data(), data.size());
#pragma GCC diagnostic pop
  }

  std::string calculate()
  {
    unsigned char s[16];
    // MD5_Final erases the context.
    // Creating local copy of context, so calculate() return same md5sum on every call.
    MD5_CTX local_context = context;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    MD5_Final(s, &local_context);
#pragma GCC diagnostic pop
    std::stringstream md5;
    md5 << std::hex << std::setfill('0');
    for (auto &byte: s)
    {
      md5 << std::setw(2) << (int)byte;
    }
    return md5.str();
  }
};

}
#endif //_AIEBU_COMMON_LINUX_UID_MD5_H_

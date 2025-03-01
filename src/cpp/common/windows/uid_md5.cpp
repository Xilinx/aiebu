// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "windows/uid_md5.h"
#include <windows.h>
#include <wincrypt.h>
#include "aiebu_error.h"

namespace aiebu {

#define MD5LEN 16

uid_md5::
uid_md5()
{
  // Get a handle to the crypto provider
  if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
    throw error(error::error_code::internal_error, "Error: CryptAcquireContext!!!");
  }

  // Create a hash object
  if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
    CryptReleaseContext(hProv, 0);
    throw error(error::error_code::internal_error, "Error: CryptCreateHash!!!");
  }
}

void
uid_md5::
update(const std::vector<uint8_t>& data)
{
  // Hash the input string
  if (!CryptHashData(hHash, (BYTE*)data.data(), data.size(), 0))
    throw error(error::error_code::internal_error, "Error: CryptHashData!!!");
}

std::string
uid_md5::
calculate()
{
  BYTE rgbHash[MD5LEN];
  DWORD dwDataLen = MD5LEN;
  std::string strMD5;

  // Get the hash value
  if (!CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &dwDataLen, 0))
    throw error(error::error_code::internal_error, "Error: CryptGetHashParam!!!");

  std::stringstream md5;
  md5 << std::hex << std::setfill('0');
  for (auto &byte: rgbHash)
  {
    md5 << std::setw(2) << (int)byte;
  }

  return md5.str();
}

uid_md5::
~uid_md5()
{
  if (hHash)
    CryptDestroyHash(hHash);

  if (hProv)
    CryptReleaseContext(hProv, 0);
}

}

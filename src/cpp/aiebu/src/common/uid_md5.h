// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_COMMON_UID_MD5_H_
#define _AIEBU_COMMON_UID_MD5_H_

#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>

namespace aiebu {

class uid_md5 {
    boost::uuids::detail::md5 hasher;

public:
  uid_md5()
  {
  }

  void update(const std::vector<uint8_t>& data)
  {
      hasher.process_bytes(data.data(), data.size());
  }

  std::string calculate()
  {
    // Creating local copy of context, so calculate() return same md5sum on every call.
    boost::uuids::detail::md5 hasher_copy = hasher;
    boost::uuids::detail::md5::digest_type digest;

    hasher_copy.get_digest(digest);

    std::stringstream md5;
    // Different boost versions model digest_type differently:
    // 1. typedef unsigned int(digest_type)[4];
    // 2. typedef unsigned char digest_type[16];
    // The code sets the print width to number of chars needed to print the element type
    // used by digest_type. This results in the same string representation for both cases
    // which matches with that reported by command line md5sum utility

    md5 << std::hex << std::setfill('0');
    for (auto ele : digest) {
        md5 << std::setw(sizeof(ele) * 2) << (unsigned int)ele;
    }
    return md5.str();
  }
};

}
#endif //_AIEBU_COMMON_UID_MD5_H_

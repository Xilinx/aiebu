// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_COMMON_UID_MD5_H_
#define _AIEBU_COMMON_UID_MD5_H_

#include <vector>
#include <iostream>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>

namespace aiebu {

unsigned constexpr md5_size = 16;

class uid_md5 {
  boost::uuids::detail::md5 hasher;
  std::vector<char> sig = std::vector<char>(md5_size, 0);

public:
  uid_md5() = default;

  void update(const std::vector<uint8_t>& data)
  {
    hasher.process_bytes(data.data(), data.size());
  }

  const std::vector<char>& calculate()
  {
    // Creating local copy of context, so calculate() return same md5sum on every call.
    boost::uuids::detail::md5 hasher_copy = hasher;
    boost::uuids::detail::md5::digest_type digest;

    hasher_copy.get_digest(digest);
    std::memcpy(sig.data(), digest, md5_size);
    return sig;

    /*
        std::stringstream md5;
        // Different boost versions model digest_type differently:
        // 1. typedef unsigned int(digest_type)[4];
        // 2. typedef unsigned char digest_type[16];
        // The code sets the print width to number of chars needed to print the
    element type
        // used by digest_type. This results in the same string representation
    for both cases
        // which matches with that reported by command line md5sum utility

        md5 << std::hex << std::setfill('0');
        for (auto ele : digest) {
          md5 << std::setw(sizeof(ele) * 2) << (unsigned int)ele;
        }
        return md5.str();
    */
  }

  [[nodiscard]] std::string str() const {
    std::stringstream md5;

    std::cout << std::hex << std::setfill('0');
    md5 << std::hex << std::setfill('0');
    for (auto ele : sig) {
      auto c = (unsigned char)ele;
      md5 << std::setw(sizeof(ele) * 2) << (unsigned int)c;
      std::cout << std::setw(sizeof(ele) * 2) << (unsigned int)c << "\n";
    }
    return md5.str();
  }
};

}
#endif //_AIEBU_COMMON_UID_MD5_H_

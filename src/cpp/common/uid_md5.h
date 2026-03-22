// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_COMMON_UID_MD5_H_
#define _AIEBU_COMMON_UID_MD5_H_

#include <vector>
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

    constexpr size_t element_size = sizeof(digest[0]);

    // Early-exit doesn't work here. If sizeof(digest[0]] == 1
    // then the compiler would complain about unreachable code
    // because the if is compiled at compile time.
    if constexpr (element_size != 1) {
      // Different boost versions model digest_type differently:
      // 1. typedef unsigned int(digest_type)[4];
      // 2. typedef unsigned char digest_type[16];
      // For case 1. the following code swaps the bytes of each integer in
      // the signature. This solves the little endian issue of integer bytes
      // stored in the reverse order than in which they are printed.
      auto tcurr = sig.begin();
      auto done = sig.end();
      while (tcurr < done) {
        auto tend = tcurr + element_size;
        std::reverse(tcurr, tend);
        tcurr = tend;
      }
    }

    return sig;
  }

  [[nodiscard]] std::string str() const {
    std::stringstream md5;

    md5 << std::hex << std::setfill('0');
    for (auto ele : sig) {
      auto c = (unsigned char)ele;
      md5 << std::setw(sizeof(ele) * 2) << (unsigned int)c;
    }
    return md5.str();
  }
};

}
#endif //_AIEBU_COMMON_UID_MD5_H_

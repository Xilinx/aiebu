// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_PASSMANAGER_H_
#define AIEBU_PASSMANAGER_H_

#include <boost/property_tree/json_parser.hpp>

#include <elfio/elfio.hpp>
#include <elfio/elfio_section.hpp>

namespace aiebu {

class passmanager {
  class passmanager_impl;
private:
  std::unique_ptr<passmanager_impl> m_impl;

public:
  passmanager(ELFIO::elfio &elf, const boost::property_tree::ptree &spec,
              bool debug = false);

  ~passmanager();

  passmanager(const passmanager &) = delete;
  passmanager(passmanager &&) = delete;
  passmanager &operator=(const passmanager &) = delete;
  passmanager& operator=(passmanager &&) = delete;

  void run_transforms();
};

}

#endif

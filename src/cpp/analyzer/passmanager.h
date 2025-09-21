// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_PASSMANAGER_H_
#define AIEBU_PASSMANAGER_H_

#include <elfio/elfio.hpp>
#include <elfio/elfio_section.hpp>

namespace aiebu {

inline bool is_ctrldata(const std::string& name)
{
  return !name.compare(".ctrldata");
}

inline bool is_pm_ctrlpkt(const std::string& name)
{
  return !name.substr(0,8).compare(".ctrlpkt");
}


class passmanager {
private:
  ELFIO::elfio &m_elf;
  bool m_debug;

private:
  void run_transforms(ELFIO::section *psec);
  void adjust_relocations();

public:
  explicit passmanager(ELFIO::elfio &elf, bool debug) : m_elf(elf), m_debug(debug) {}

  void run_transforms() {
    // Currently we are running a precannded sequence of transforms.
    ELFIO::Elf_Half sec_num = m_elf.sections.size();
    for ( int i = 0; i < sec_num; ++i ) {
      ELFIO::section *psec = m_elf.sections[i];
      if (psec->get_type() != ELFIO::SHT_PROGBITS)
        continue;
      if (is_pm_ctrlpkt(psec->get_name()) || is_ctrldata(psec->get_name()))
        continue;
      run_transforms(psec);
    }
    adjust_relocations();
  }

};

}

#endif

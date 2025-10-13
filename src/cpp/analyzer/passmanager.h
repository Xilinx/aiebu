// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_PASSMANAGER_H_
#define AIEBU_PASSMANAGER_H_

#include "aiebu/aiebu_error.h"
#include <sstream>

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
  void fixup_section_addresses() {
    for (auto & seg : m_elf.segments) {
      seg->set_virtual_address( seg->get_offset());
      seg->set_physical_address( seg->get_offset());
    }
    for (auto & sec : m_elf.sections) {
      if (sec->get_type() != ELFIO::SHT_PROGBITS)
        continue;
      if (is_pm_ctrlpkt(sec->get_name()) || is_ctrldata(sec->get_name()))
        continue;
    }
  }
  void run_transforms(ELFIO::section *psec);
  void adjust_relocations();

public:
  explicit passmanager(ELFIO::elfio &elf, bool debug) : m_elf(elf), m_debug(debug) {}

  void adjust_addresses() {
    std::ostringstream nullstream;
    m_elf.save(nullstream);
    nullstream.str("");
    // Update the address of the each section to match its offset
    for (auto &sec : m_elf.sections) {
      sec->set_address(sec->get_offset());
    }

    for (auto &seg : m_elf.segments) {
      if (!seg->get_sections_num())
        continue;
      if (seg->get_sections_num() != 1)
        throw error(error::error_code::invalid_elf,
                    "A segment with multiple sections encountered\n");

      // Update the address of the each segment which has a section
      auto offset = m_elf.sections[seg->get_section_index_at(0)]->get_offset();
      seg->set_virtual_address(offset);
      seg->set_physical_address(offset);
    }
  }

  void run_transforms() {
    // Currently we are running a precannded sequence of transforms.
    for (auto & section : m_elf.sections) {
      if (section->get_type() != ELFIO::SHT_PROGBITS)
        continue;
      if (is_pm_ctrlpkt(section->get_name()) || is_ctrldata(section->get_name()))
        continue;
      run_transforms(section.get());
    }
    adjust_addresses();
    adjust_relocations();
  }

};

}

#endif

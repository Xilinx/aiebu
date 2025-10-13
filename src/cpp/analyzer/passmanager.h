// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_PASSMANAGER_H_
#define AIEBU_PASSMANAGER_H_

#include "aiebu/aiebu_error.h"
#include <sstream>

#include <elfio/elfio.hpp>
#include <elfio/elfio_section.hpp>

namespace aiebu {

#ifndef EM_AIECTRLCODE
constexpr ELFIO::Elf_Half EM_AIECTRLCODE = 269; // AMD / Xilinx AIEngine ctrlcode
#endif

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

  bool is_legacy_elf_with_unset_address() const
  {
    for (auto &seg : m_elf.segments) {
      if (seg->get_virtual_address() != 0)
        return false;
      if (seg->get_physical_address() != 0)
        return false;
    }
    return true;
  }


  void upgrade_legacy_elf_assign_adddress()
  {
    // Upgrading address in exisiting ELF does not work. We need to create a new
    // ELF with contents from exisiting ELF and then
    // [1] update the flags
    // [2] drop segment entries for sections that are not used for execution
    // [3] update the address
    //
    ELFIO::elfio nbin;
    nbin.create(ELFIO::ELFCLASS32, ELFIO::ELFDATA2LSB);
    nbin.set_os_abi(m_elf.get_os_abi());
    nbin.set_abi_version(m_elf.get_abi_version());
    nbin.set_type( m_elf.get_type() );
    nbin.set_machine( EM_AIECTRLCODE);
    nbin.set_flags(m_elf.get_flags());

    std::vector<std::pair<int, size_t>> offsets;
    for (auto &sec : m_elf.sections) {
      // The following two sections are automatically added by ELFIO
      if (sec->get_name() == "")
        continue;
      if (sec->get_name() == ".shstrtab")
        continue;
      nbin.sections.add(sec->get_name());
    }

    for (auto &sec : m_elf.sections) {
      auto offset = sec->get_offset();
      const std::string name = sec->get_name();
      auto it = std::find_if(nbin.sections.begin(), nbin.sections.end(), [&name](auto &n) {
        return n->get_name() == name;
      });

      *it = std::move(sec);
      if ((*it)->get_type() != ELFIO::SHT_PROGBITS) {
        // Remove the ALLOC flags from sections except text and data
        auto flags = (*it)->get_flags();
        flags &= ~ELFIO::SHF_ALLOC;
        (*it)->set_flags(flags);
      }
      offsets.emplace_back((*it)->get_index(), offset);
    }

    for (auto &seg : m_elf.segments) {
      auto type = seg->get_type();
      if ((type != ELFIO::PT_LOAD) && (type != ELFIO::PT_PHDR))
        continue;
      auto nseg = nbin.segments.add();
      size_t offset = seg->get_offset();
      nseg->set_type(seg->get_type());
      nseg->set_flags(type);
      nseg->set_align(seg->get_align());
      auto it = std::find_if(offsets.begin(), offsets.end(), [offset](std::pair<int, size_t> n) {
        return n.second == offset;
      });
      if (it == offsets.end())
        continue;
      // Bind the segment to its section
      nseg->add_section_index(it->first, seg->get_align());
    }

    adjust_addresses(nbin);
#if 0
    // Force the layout of the ELF
    std::ostringstream nullstream;
    nbin.save(nullstream);
    nullstream.str("");

    for (auto & sec : nbin.sections) {
      std::cout << '[' << sec->get_index() << "] " << sec->get_name() << ": 0x" << std::hex << sec->get_offset() << ": 0x" << std::hex << sec->get_address() << std::dec << "\n";;
    }

    for (auto & seg : nbin.segments) {
      std::cout << '[' << seg->get_index() << "] " << std::hex << seg->get_offset() << std::dec << '(' << seg->get_sections_num() << ")\n";;
    }

    // Update the address of the each section to match its offset
    for (auto &sec : nbin.sections) {
      sec->set_address(sec->get_offset());
    }

    for (auto &seg : nbin.segments) {
      if (!seg->get_sections_num())
        continue;
      // Update the address of the each segment which has a section
      auto offset = nbin.sections[seg->get_section_index_at(0)]->get_offset();
      seg->set_virtual_address(offset);
      seg->set_physical_address(offset);
    }
#endif
    // Force the layout of the ELF
    std::ostringstream nullstream;
    nullstream.str("");
    nbin.save(nullstream);

    for (auto & sec : nbin.sections) {
      std::cout << '[' << sec->get_index() << "] " << sec->get_name() << ": 0x" << std::hex << sec->get_offset() << ": 0x" << std::hex << sec->get_address() << std::dec << "\n";;
    }

    for (auto & seg : nbin.segments) {
      std::cout << '[' << seg->get_index() << "] " << std::hex << seg->get_offset() << std::dec << '(' << seg->get_sections_num() << ")\n";;
    }

    m_elf = std::move(nbin);
    nullstream.str("");
    m_elf.save(nullstream);
  }

  void adjust_addresses(ELFIO::elfio &nelf) {
    std::ostringstream nullstream;
    nelf.save(nullstream);
    nullstream.str("");
    // Update the address of the each section to match its offset
    for (auto &sec : nelf.sections) {
      sec->set_address(sec->get_offset());
    }

    for (auto &seg : nelf.segments) {
      if (!seg->get_sections_num())
        continue;
      if (seg->get_sections_num() != 1)
        throw error(error::error_code::invalid_elf,
                    "Segment " + std:: to_string(seg->get_index()) + " with multiple sections encountered\n");

      // Update the address of the each segment which has a section
      auto offset = nelf.sections[seg->get_section_index_at(0)]->get_offset();
      seg->set_virtual_address(offset);
      seg->set_physical_address(offset);
    }
  }


public:
  explicit passmanager(ELFIO::elfio &elf, bool debug) : m_elf(elf), m_debug(debug) {}

  void run_transforms() {
    if (is_legacy_elf_with_unset_address()) {
      upgrade_legacy_elf_assign_adddress();
    }

//    m_elf.save("/tmp/a.elf");
    // Currently we are running a precannded sequence of transforms.
    for (auto &section : m_elf.sections) {
      if (section->get_type() != ELFIO::SHT_PROGBITS)
        continue;
      if (is_pm_ctrlpkt(section->get_name()) || is_ctrldata(section->get_name()))
        continue;
      run_transforms(section.get());
    }
//    m_elf.save("/tmp/b.elf");
//    adjust_addresses(m_elf);
    adjust_relocations();
  }
};

}

#endif

// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_PASSMANAGER_H_
#define AIEBU_PASSMANAGER_H_

#include "aiebu/aiebu_error.h"
#include <sstream>
#include <boost/property_tree/json_parser.hpp>

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

inline void elf_debug_dump(const ELFIO::elfio &nbin) {
  for (auto & sec : nbin.sections) {
    std::cout << '[' << sec->get_index() << "] " << sec->get_name() << ": 0x" << std::hex << sec->get_offset() << ": 0x" << std::hex << sec->get_address() << std::dec << "\n";;
  }

  for (auto & seg : nbin.segments) {
    std::cout << '[' << seg->get_index() << "] " << std::hex << seg->get_offset() << std::dec << '(' << seg->get_sections_num() << ")\n";;
  }
}

class passmanager {
private:
  // Passed by the client but updated by this transformer
  ELFIO::elfio &m_elf;
  // m_bin is a private copy of a working ELF object used by
  // upgrade_legacy_elf_assign_adddress() which is std::move'd back into
  // client's ELF object m_elf.
  // Please note that m_bin object's life cycle should really have beeen
  // limited to scope of upgrade_legacy_elf_assign_adddress() but it here
  // it has class scope life cycle. This is because of as yet unresolved
  // bug where when m_nbin was going out of scope after std::move in
  // upgrade_legacy_elf_assign_adddress(), [m_elf = std::move(m_bin)]
  // failures where seen inside ELFIO
  // when using m_elf. The bug seems to be in std::move of ELFIO header
  // object -- from m_nbin to m_elf -- which should be debugged separately.
  ELFIO::elfio m_nbin;
  boost::property_tree::ptree m_spec;
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
    m_nbin.create(ELFIO::ELFCLASS32, ELFIO::ELFDATA2LSB);
    m_nbin.set_os_abi(m_elf.get_os_abi());
    m_nbin.set_abi_version(m_elf.get_abi_version());
    m_nbin.set_type( m_elf.get_type() );
    m_nbin.set_machine( EM_AIECTRLCODE);
    m_nbin.set_flags(m_elf.get_flags());

    std::vector<std::pair<int, size_t>> offsets;
    for (auto &sec : m_elf.sections) {
      // The following two sections are automatically added by ELFIO
      if (sec->get_name() == "")
        continue;
      if (sec->get_name() == ".shstrtab")
        continue;
      m_nbin.sections.add(sec->get_name());
    }

    // Populate the new ELF object with relevant sections and segments from
    // existing ELF object
    for (auto &sec : m_elf.sections) {
      auto offset = sec->get_offset();
      const std::string name = sec->get_name();
      auto it = std::find_if(m_nbin.sections.begin(), m_nbin.sections.end(), [&name](auto &n) {
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
      auto nseg = m_nbin.segments.add();
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

    // Force the layout of the new ELF
    std::ostringstream nullstream;
    if (!m_nbin.save(nullstream))
      throw error(error::error_code::internal_error, "ELF layout generation failed\n");
    nullstream.str("");

    adjust_addresses(m_nbin);

    // Force the layout of the ELF
    nullstream.str("");
    if (!m_nbin.save(nullstream))
      throw error(error::error_code::internal_error, "ELF layout generation failed\n");

    if (m_debug)
      elf_debug_dump(m_nbin);
    m_elf = std::move(m_nbin);
    nullstream.str("");
    if (!m_elf.save(nullstream))
      throw error(error::error_code::internal_error, "ELF layout generation failed\n");

    if (m_debug)
      elf_debug_dump(m_elf);
  }

  void adjust_addresses(ELFIO::elfio &nelf) {
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

  void pretransform()
  {
    if (!is_legacy_elf_with_unset_address())
      return;

    upgrade_legacy_elf_assign_adddress();
    std::ostringstream nullstream;
    if (!m_elf.save(nullstream))
      throw error(error::error_code::internal_error, "ELF layout generation failed\n");
    nullstream.str("");
  }

public:
  explicit passmanager(ELFIO::elfio &elf, const boost::property_tree::ptree &spec,
                       bool debug = false) : m_elf(elf), m_spec(std::move(spec)), m_debug(debug) {}

  void run_transforms() {
    pretransform();
    // Currently we are running a precaned sequence of transforms.
    for (auto &section : m_elf.sections) {
      if (section->get_type() != ELFIO::SHT_PROGBITS)
        continue;
      if (is_pm_ctrlpkt(section->get_name()) || is_ctrldata(section->get_name()))
        continue;
      run_transforms(section.get());
    }

    std::ostringstream nullstream;
    if (!m_elf.save(nullstream))
      throw error(error::error_code::internal_error, "ELF layout generation failed\n");

    adjust_addresses(m_elf);
    nullstream.str("");
    adjust_relocations();
  }
};

}

#endif

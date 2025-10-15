// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include <boost/format.hpp>
#include <cxxopts.hpp>
#include <string>
#include <iostream>

#include "analyzer/passmanager.h"
#include "common/file_utils.h"
#include "common/utils.h"

namespace aiebu {

cxxopts::ParseResult main_helper(int argc, const char* const *argv,
                                 const std::string & executable,
                                 const std::string & description)
{
  std::string target_name;
  std::vector<std::string> subcmd_options;
  cxxopts::Options global_options(executable, description);

  try {
    global_options.add_options()(
        "j,transform",
        "Name of JSON file with requested ctrlcode transform patterns",
        cxxopts::value<std::string>()->default_value("unspecified"))(
        "o,output", "Name of the output ELF file",
        cxxopts::value<std::string>()->default_value("unspecified.elf"))(
        "filename", "Input file name", cxxopts::value<std::string>())(
        "help,h", "show help message and exit",
        cxxopts::value<bool>()->default_value("false"))
      ("d,debug", "dump ctrlcode after each transform", cxxopts::value<bool>()->default_value("false"))
      ("v,version", "show version and exit", cxxopts::value<bool>()->default_value("false"));
    global_options.parse_positional({"filename"});

    auto result = global_options.parse(argc, argv);

    subcmd_options = result.unmatched();

    if (result.count("version")) {
      std::cout << version_string();
      return {};
    }

    if (result.count("help")) {
      std::cout << global_options.help({"", executable}) << std::endl;
      return {};
    }

    if (!result.count("filename"))
      throw cxxopts::exceptions::missing_argument("filename");

    return result;
  }
  catch (const cxxopts::exceptions::exception& e) {
    auto errMsg = boost::format("Error parsing options: %s\n") % e.what() ;
    throw std::runtime_error(errMsg.str());
  }
}

bool is_legacy_elf_with_unset_address(const ELFIO::elfio &ebin)
{
  for (auto &seg : ebin.segments) {
    if (seg->get_virtual_address() != 0)
      return false;
    if (seg->get_physical_address() != 0)
      return false;
  }
  return true;
}

ELFIO::elfio upgrade_legacy_elf_assign_adddress(ELFIO::elfio &ebin)
{
  // Upgrading address in exisiting ELF does not work. We need to create a new
  // ELF with contents from exisiting ELF and then
  // [1] update the flags
  // [2] drop segment entries for sections that are not used for execution
  // [3] update the address
  //
  ELFIO::elfio nbin;
  nbin.create(ELFIO::ELFCLASS32, ELFIO::ELFDATA2LSB);
  nbin.set_os_abi(ebin.get_os_abi());
  nbin.set_abi_version(ebin.get_abi_version());
  nbin.set_type( ebin.get_type() );
  nbin.set_machine( EM_AIECTRLCODE);
  nbin.set_flags(ebin.get_flags());

  std::vector<std::pair<int, size_t>> offsets;
  for (auto &sec : ebin.sections) {
    // The following two sections are automatically added by ELFIO
    if (sec->get_name() == "")
      continue;
    if (sec->get_name() == ".shstrtab")
      continue;
    nbin.sections.add(sec->get_name());
  }

  for (auto &sec : ebin.sections) {
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

  for (auto &seg : ebin.segments) {
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

  // Force the layout of the ELF
  std::ostringstream nullstream;
  if (!nbin.save(nullstream))
    throw error(error::error_code::internal_error, "ELF layout generation failed\n");
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

  // Force the layout of the ELF
  nullstream.str("");
  if (!nbin.save(nullstream))
    throw error(error::error_code::internal_error, "ELF layout generation failed\n");

  for (auto & sec : nbin.sections) {
    std::cout << '[' << sec->get_index() << "] " << sec->get_name() << ": 0x" << std::hex << sec->get_offset() << ": 0x" << std::hex << sec->get_address() << std::dec << "\n";;
  }

  for (auto & seg : nbin.segments) {
    std::cout << '[' << seg->get_index() << "] " << std::hex << seg->get_offset() << std::dec << '(' << seg->get_sections_num() << ")\n";;
  }

  return nbin;
}

} //namespace aiebu::utilities

int main(int argc, char* argv[])
{
  const std::string executable = "aiebu-transform";
  // -- Program Description
  const std::string description = "aiebu ctrlcode transforming utility (aiebu-transform) for aie binaries";

  cxxopts::ParseResult result;

  try {
    result = aiebu::main_helper(argc, argv, executable, description);
  } catch (const std::exception& e) {
    std::cout << e.what();
    return 1;
  }

  if (!result.arguments().size())
    return 1;

  const std::vector<char> buffer = aiebu::readfile(result["filename"].as<std::string>());
  if (aiebu::identify_buffer_type(buffer) !=
      aiebu::aiebu_assembler::buffer_type::elf_aie2)
    return 1;

  ELFIO::elfio ebin;
  ebin.load(result["filename"].as<std::string>());

  // Run the transforms
  aiebu::passmanager passm(ebin, result["debug"].as<bool>());
  passm.run_transforms();

  // Now save the ELF with transformed ctrlcode
  if(!ebin.save(result["output"].as<std::string>()))
    return 1;

  return 0;
}

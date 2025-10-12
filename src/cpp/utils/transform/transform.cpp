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

  ELFIO::Elf_Half seg_num = ebin.segments.size();
  for ( int i = 0; i < seg_num; ++i ) {
    ELFIO::segment *pseg = ebin.segments[i];
    auto count = pseg->get_sections_num();
    std::cout << "seg[" << i << "] 0x" << std::hex << pseg->get_offset() << std::dec << "\n";
    for (auto s = 0; s < count; s++) {
      auto idx = pseg->get_section_index_at(s);
      auto sec = ebin.sections[idx];
      auto off = sec->get_offset();
      std::cout << sec->get_name() << " " << off << "\n";
    }
    pseg->set_virtual_address( pseg->get_offset());
    pseg->set_physical_address( pseg->get_offset());
  }


  ELFIO::elfio nbin;
  nbin.create(ELFIO::ELFCLASS32, ELFIO::ELFDATA2LSB);
  nbin.set_os_abi(ebin.get_os_abi());
  nbin.set_abi_version(ebin.get_abi_version());
  nbin.set_type( ebin.get_type() );
  nbin.set_machine( ebin.get_machine());
  nbin.set_flags(ebin.get_flags());

  size_t cursor = 0x4000000;
  std::vector<std::pair<int, size_t>> offsets;
  for (auto &sec : ebin.sections) {
    if (sec->get_name() == "")
      continue;
    if (sec->get_name() == ".shstrtab")
      continue;
    nbin.sections.add(sec->get_name());
  }

  auto nsec = nbin.sections.begin();
  for (auto & sec : ebin.sections) {
    cursor  = (cursor + sec->get_addr_align() - 1) & ~(sec->get_addr_align() - 1);
    offsets.emplace_back(sec->get_index(), sec->get_offset());
    cursor += sec->get_size();
    //const std::string name = sec->get_name();
    //auto it = std::find_if(nbin.sections.begin(), nbin.sections.end(), [&name](auto &n) {
    //  return n->get_name() == name;
    //});

    *nsec = std::move(sec);
    (*nsec)->set_address((*nsec)->get_offset());
    if ((*nsec)->get_type() != ELFIO::SHT_PROGBITS) {
      auto flags = (*nsec)->get_flags();
      flags &= ~ELFIO::SHF_ALLOC;
      (*nsec)->set_flags(flags);
    }
    nsec++;
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
    nseg->add_section_index(it->first, seg->get_align());
    nseg->set_virtual_address(it->second);
    nseg->set_physical_address(it->second);
  }

  for (auto & sec : nbin.sections) {
    std::cout << '[' << sec->get_index() << "] " << sec->get_name() << ": 0x" << std::hex << sec->get_offset() << std::dec << "\n";;
  }

  for (auto & seg : nbin.segments) {
    std::cout << '[' << seg->get_index() << "] " << std::hex << seg->get_offset() << std::dec << '(' << seg->get_sections_num() << ")\n";;
  }


  nbin.save(result["output"].as<std::string>());
  // We fail in the save even without any transforms. First this needs to be
  // debugged and resolved before we can save any transformed ctrlcode. Once
  // this bug is fixed comment out the ebi.save() from here.
  int i = 0;
  std::cin >> i;
//  ebin.save(result["output"].as<std::string>());
  // Run the transforms
  aiebu::passmanager passm(nbin, result["debug"].as<bool>());
  passm.run_transforms();
  // Now save the ELF with transformed ctrlcode
  nbin.save(result["output"].as<std::string>());
  return 0;
}

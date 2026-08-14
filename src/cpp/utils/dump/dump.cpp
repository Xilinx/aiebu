// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include <boost/format.hpp>
#include <cxxopts.hpp>
#include <set>
#include <string>
#include <iostream>
#include <map>

#include "aiebu/aiebu_error.h"
#include "analyzer/reporter.h"
#include "analyzer/packets.h"
#include "tools/debug_tools.h"
#include "common/file_utils.h"
#include "common/utils.h"

namespace aiebu {

static const std::set<std::string>
targets = { //NOLINT
    "aie2ps",
    "aie4",
    "aie4a",
    "aie4z",
    "aie2asm",
    "aie2txn",
    "aie2dpu",
    "unspecified"
};

static const std::map<aiebu::aiebu_assembler::buffer_type, std::string>
buffer_type_table = { // NOLINT
  {aiebu::aiebu_assembler::buffer_type::blob_instr_dpu, "aie2-dpu-ctrlcode"},
  {aiebu::aiebu_assembler::buffer_type::blob_instr_prepost, "aie2-ctrlpkt"},
  {aiebu::aiebu_assembler::buffer_type::blob_instr_transaction, "aie2-ctrlcode"},
  {aiebu::aiebu_assembler::buffer_type::blob_control_packet, "aie2p-ctrlpkt"},
  {aiebu::aiebu_assembler::buffer_type::blob_control_packet_aie2, "aie2-ctrlpkt"},
  {aiebu::aiebu_assembler::buffer_type::asm_aie2ps, "aie2ps-ctrlcode-asm"},
  {aiebu::aiebu_assembler::buffer_type::asm_aie2, "aie2-ctrlcode-asm"},
  {aiebu::aiebu_assembler::buffer_type::elf_aie2, "aie2-elf"},
  {aiebu::aiebu_assembler::buffer_type::elf_aie2ps, "aie2ps-elf"},
  {aiebu::aiebu_assembler::buffer_type::elf_aie4, "aie4-elf"},
  {aiebu::aiebu_assembler::buffer_type::elf_aie4a, "aie4a-elf"},
  {aiebu::aiebu_assembler::buffer_type::elf_aie4z, "aie4z-elf"},
  {aiebu::aiebu_assembler::buffer_type::elf_aie2ps_config, "aie2ps-config-elf"},
  {aiebu::aiebu_assembler::buffer_type::elf_aie4_config, "aie4-config-elf"},
  {aiebu::aiebu_assembler::buffer_type::elf_aie4a_config, "aie4a-config-elf"},
  {aiebu::aiebu_assembler::buffer_type::elf_aie4z_config, "aie4z-config-elf"},
  {aiebu::aiebu_assembler::buffer_type::pdi_aie2, "aie2-pdi"},
  {aiebu::aiebu_assembler::buffer_type::pdi_aie2ps, "aie2ps-pdi"},
  {aiebu::aiebu_assembler::buffer_type::unspecified, "unknown"},
  {aiebu::aiebu_assembler::buffer_type::blob_aie2ps, "aie2ps-binary"},
  {aiebu::aiebu_assembler::buffer_type::blob_aie4, "aie4-binary"},
  {aiebu::aiebu_assembler::buffer_type::blob_aie4a, "aie4a-binary"},
  {aiebu::aiebu_assembler::buffer_type::blob_aie4z, "aie4z-binary"},
};


cxxopts::ParseResult main_helper(int argc, const char* const *argv,
                                 const std::string & executable,
                                 const std::string & description)
{
  std::string target_name;
  std::vector<std::string> subcmd_options;
  cxxopts::Options global_options(executable, description);

  try {
    global_options.add_options()
      ("a,archive-headers", "Display archive header information", cxxopts::value<bool>()->default_value("false"))
      ("f,file-headers", "Display the contents of the overall file header", cxxopts::value<bool>()->default_value("false"))
      ("p,private-headers", "Display opcode frequency in the control code binary", cxxopts::value<bool>()->default_value("false"))
      ("x,all-headers", "Display contents of all elf headers", cxxopts::value<bool>()->default_value("false"))
      ("d,disassemble", "Display assembler contents of ctrltext sections if elf file is provided or display control packet in assembled format if control packet binary file is provided", 
        cxxopts::value<bool>()->default_value("false"))
      ("H,help", "show help message and exit", cxxopts::value<bool>()->default_value("false"))
      ("m,architecture", "Specify the target architecture as MACHINE (aie2ps/aie4/aie4a/aie4z/aie2asm/aie2txn/aie2dpu). Required for binary files or legacy ELFs (OS/ABI=0x46) to select correct ISA.",
        cxxopts::value<std::string>()->default_value("unspecified"))
      ("D,disassemble-all", "Display assembler contents of all sections", cxxopts::value<bool>()->default_value("false"))
      ("t,syms", "Display contents of the symbols table(s)", cxxopts::value<bool>()->default_value("false"))
      ("r,reloc", "Display relocation entries in the file", cxxopts::value<bool>()->default_value("false"))
      ("P,private","Display object format specific contents\narg:\ntrace-probe   Display probe information\nopcode-info   Display opcode information\n", 
        cxxopts::value<std::string>()->default_value("unspecified"))
      ("pc", "Program counter for opcode-info", cxxopts::value<std::string>()->default_value("unspecified"))
      ("page-index", "Page index for opcode-info", cxxopts::value<std::string>()->default_value("unspecified"))
      ("uc-index", "uC (column) index for opcode-info", cxxopts::value<std::string>()->default_value("unspecified"))
      ("filename", "Input file name", cxxopts::value<std::string>())
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
      std::cout << "Supported platforms and ELF identification:\n";
      std::cout << "\n";
      std::cout << "  Platform     OS/ABI  ABI Version  Notes\n";
      std::cout << "  -----------  ------  -----------  ------------------------------------------\n";
      std::cout << "  aie2p        0x45    0x02         AIE2P non-config ELF\n";
      std::cout << "  aie2p        0x45    0x21         AIE2P config ELF (.target directive)\n";
      std::cout << "  aie2ps/aie4  0x46    0x02         Legacy group non-config (use -m to select)\n";
      std::cout << "  aie2ps/aie4  0x46    0x03         Legacy group config (without .target)\n";
      std::cout << "  aie2ps       0x40    0x20/0x21    AIE2PS config ELF (.target aie2ps)\n";
      std::cout << "  aie4         0x4B    0x20/0x21    AIE4 config ELF (.target aie4)\n";
      std::cout << "  aie4a        0x56    0x20/0x21    AIE4A config ELF (.target aie4a)\n";
      std::cout << "  aie4z        0x69    0x20/0x21    AIE4Z config ELF (.target aie4z)\n";
      std::cout << "\n";
      std::cout << "  Note: Legacy non-config ELFs (OS/ABI=0x46, version=0x02) cannot be\n";
      std::cout << "  distinguished by OSABI alone. Use -m aie4/aie4a/aie4z to override.\n";
      return {};
    }

    target_name = result["architecture"].as<std::string>();

    if (targets.find(target_name) == targets.end())
      throw cxxopts::exceptions::incorrect_argument_type(target_name);

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
  const std::string executable = "aiebu-dump";
  // -- Program Description
  const std::string description = "aiebu dumping utility (aiebu-dump) for aie binaries";

  cxxopts::ParseResult result;

  try {
    result = aiebu::main_helper(argc, argv, executable, description);
  } catch (const std::exception& e) {
    std::cout << e.what();
    return 1;
  }

  if (!result.arguments().size())
    return 0;

  std::vector<char> buffer;
  aiebu::aiebu_assembler::buffer_type type;
  std::string target_arch;
  try {
    buffer = aiebu::readfile(result["filename"].as<std::string>());
    type = aiebu::identify_buffer_type(buffer);
    target_arch = result["architecture"].as<std::string>();
  } catch (const std::exception& e) {
    std::cerr << "Error reading file: " << e.what() << "\n";
    return 1;
  }

  // Print detected ELF OSABI/version/platform info for ELF files
  //if (buffer.size() >= 52 &&
  //    static_cast<unsigned char>(buffer[0]) == 0x7f &&
  //    buffer[1] == 'E' && buffer[2] == 'L' && buffer[3] == 'F') {
    //auto osabi = static_cast<unsigned char>(buffer[7]);
    //auto abiversion = static_cast<unsigned char>(buffer[8]);
    //auto detected_it = aiebu::buffer_type_table.find(type);
    //std::string detected_platform = (detected_it != aiebu::buffer_type_table.end())
    //                                  ? detected_it->second : "unknown";
    //std::cout << result["filename"].as<std::string>() << ":\n";
    //std::cout << boost::format(";  ELF OS/ABI:    0x%02x\n") % static_cast<unsigned>(osabi);
    //std::cout << boost::format(";  ABI Version:   0x%02x\n") % static_cast<unsigned>(abiversion);
    //std::cout << ";  Platform:      " << detected_platform << "\n";
    // Legacy group ELF (OSABI=0x46) is shared by aie2ps/aie4 family — version does not distinguish them
    //if (osabi == 0x46 && target_arch == "unspecified")
    //  std::cout << "  Note: Legacy ELF (OS/ABI=0x46) detected as aie2ps by default. Use -m aie4/aie4a/aie4z to override.\n";
  //}

  // For binary files (unspecified), convert to architecture-specific buffer type
  if (type == aiebu::aiebu_assembler::buffer_type::unspecified) {
    if (target_arch == "unspecified") {
      std::cout << "Warning: Binary file detected without specified architecture.\n";
      std::cout << "Please use -m option to specify target (aie2ps/aie4/aie4a/aie4z) for correct disassembly.\n";
      std::cout << "Example: aiebu-dump -d -m aie2ps input.bin\n";
      std::cout << "Defaulting to aie2ps...\n\n";
      target_arch = "aie2ps";
    }

    // Set buffer type based on target architecture
    if (target_arch == "aie4") {
      type = aiebu::aiebu_assembler::buffer_type::blob_aie4;
    } else if (target_arch == "aie4a") {
      type = aiebu::aiebu_assembler::buffer_type::blob_aie4a;
    } else if (target_arch == "aie4z") {
      type = aiebu::aiebu_assembler::buffer_type::blob_aie4z;
    } else {
      // Default to aie2ps for any other case
      type = aiebu::aiebu_assembler::buffer_type::blob_aie2ps;
    }
  }

  // For legacy ELFs (osabi_aie2ps_group=0x46, version=0x02) aie2ps and aie4 share the
  // same OSABI, so the -m option is the only way to distinguish them.
  if (type == aiebu::aiebu_assembler::buffer_type::elf_aie2ps) {
    if (target_arch == "aie4")
      type = aiebu::aiebu_assembler::buffer_type::elf_aie4;
    else if (target_arch == "aie4a")
      type = aiebu::aiebu_assembler::buffer_type::elf_aie4a;
    else if (target_arch == "aie4z")
      type = aiebu::aiebu_assembler::buffer_type::elf_aie4z;
  }

  try {
  // Handle private/debug tool options (trace-probe, opcode-info)
  std::string private_opt = result["private"].as<std::string>();
  if (private_opt != "unspecified" && !private_opt.empty()) {
    if (type != aiebu::aiebu_assembler::buffer_type::elf_aie2ps &&
        type != aiebu::aiebu_assembler::buffer_type::elf_aie4 &&
        type != aiebu::aiebu_assembler::buffer_type::elf_aie4a &&
        type != aiebu::aiebu_assembler::buffer_type::elf_aie4z &&
        type != aiebu::aiebu_assembler::buffer_type::elf_aie2ps_config &&
        type != aiebu::aiebu_assembler::buffer_type::elf_aie4_config &&
        type != aiebu::aiebu_assembler::buffer_type::elf_aie4a_config &&
        type != aiebu::aiebu_assembler::buffer_type::elf_aie4z_config)
      throw aiebu::error(aiebu::error::error_code::invalid_buffer_type, "Invalid ELF buffer for debug tools");

    aiebu::debug_tools debug_tools(type, buffer);

    if (private_opt == "trace-probe") {
      // trace-probe option
      debug_tools.write_trace_probes(std::cout);
    }
    else if (private_opt == "opcode-info") {
      // opcode-info option
      std::string pc_str = result["pc"].as<std::string>();
      std::string page_str = result["page-index"].as<std::string>();
      std::string uc_str = result["uc-index"].as<std::string>();

      debug_tools.write_opcode_information(
        std::cout, result["filename"].as<std::string>(), pc_str, page_str, uc_str
      );
    }
    else {
      throw aiebu::error(aiebu::error::error_code::invalid_input, "Invalid private mode argument");
    }
  }
  else if (type == aiebu::aiebu_assembler::buffer_type::blob_control_packet ||
      type == aiebu::aiebu_assembler::buffer_type::blob_control_packet_aie2) {
    if (result["disassemble"].as<bool>()) {
      aiebu::packets packetprint(buffer.data(), static_cast<uint64_t>(buffer.size()), type);
      std::cout << packetprint.get_dump() << std::endl;
    }
  }
  else {
    aiebu::reporter rep(type, buffer);
    if (result["all-headers"].as<bool>()) {
      if (type == aiebu::aiebu_assembler::buffer_type::elf_aie2) {
        rep.elf_summary(std::cout);
      }
    }
    else if (result["disassemble"].as<bool>()) {
      if (type == aiebu::aiebu_assembler::buffer_type::elf_aie2 ||
          type == aiebu::aiebu_assembler::buffer_type::elf_aie2ps ||
          type == aiebu::aiebu_assembler::buffer_type::elf_aie4 ||
          type == aiebu::aiebu_assembler::buffer_type::elf_aie4a ||
          type == aiebu::aiebu_assembler::buffer_type::elf_aie4z ||
          type == aiebu::aiebu_assembler::buffer_type::blob_instr_transaction ||
          type == aiebu::aiebu_assembler::buffer_type::blob_aie2ps ||
          type == aiebu::aiebu_assembler::buffer_type::blob_aie4 ||
          type == aiebu::aiebu_assembler::buffer_type::blob_aie4a ||
          type == aiebu::aiebu_assembler::buffer_type::blob_aie4z) {
        rep.disassemble(std::cout, false);
      }
    }
    else if (result["disassemble-all"].as<bool>()) {
      if (type == aiebu::aiebu_assembler::buffer_type::elf_aie2 ||
          type == aiebu::aiebu_assembler::buffer_type::blob_aie2ps ||
          type == aiebu::aiebu_assembler::buffer_type::blob_aie4 ||
          type == aiebu::aiebu_assembler::buffer_type::blob_aie4a ||
          type == aiebu::aiebu_assembler::buffer_type::blob_aie4z) {
        rep.disassemble(std::cout, true);
      }
      else if (type == aiebu::aiebu_assembler::buffer_type::elf_aie2ps ||
               type == aiebu::aiebu_assembler::buffer_type::elf_aie4 ||
               type == aiebu::aiebu_assembler::buffer_type::elf_aie4a ||
               type == aiebu::aiebu_assembler::buffer_type::elf_aie4z) {
        rep.disassemble(result["filename"].as<std::string>(), true);
      }
    }
    else if (result["private-headers"].as<bool>()) {
      rep.ctrlcode_summary(std::cout);
    }
  }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}

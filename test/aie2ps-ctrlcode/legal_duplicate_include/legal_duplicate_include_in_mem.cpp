// SPDX-License-Identifier: MIT
// Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.
//
// Assembles this testcase from memory using
// aiebu_assembler(buffer_type::aie2ps_config, config_json, file_artifact, flags).

#include "aiebu/aiebu_assembler.h"
#include "aiebu/aiebu_error.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {

// Minimal xrt-kernels config: one kernel, one instance; paths match artifact keys.
constexpr char k_config_json[] = R"json({
  "xrt-kernels": [
    {
      "name": "CTRL",
      "arguments": [
        { "name": "arg0", "type": "char *", "offset": "0x00" },
        { "name": "arg1", "type": "char *", "offset": "0x08" },
        { "name": "arg2", "type": "char *", "offset": "0x10" },
        { "name": "arg3", "type": "char *", "offset": "0x18" }
      ],
      "instance": [
        {
          "id": "inst0",
          "ctrl_code_file": "./ml_asm/merged_control.asm",
          "patch_info_file": "./external_buffer_id.json"
        }
      ]
    }
  ]
}
)json";

void read_file(const std::filesystem::path& path, std::vector<char>& out)
{
  if (!std::filesystem::exists(path))
    throw std::runtime_error("file not found: " + path.string());
  std::ifstream in(path, std::ios::binary);
  const auto sz = std::filesystem::file_size(path);
  out.resize(static_cast<std::size_t>(sz));
  in.read(out.data(), static_cast<std::streamsize>(sz));
}

void add_asm_tree(const std::filesystem::path& root, aiebu::file_artifact& artifact)
{
  const std::pair<const char*, const char*> dirs[] = {
      {"../ml_asm/", "ml_asm"},
      {"../asm/", "asm"},
  };
  for (const auto& ent : dirs) {
    const std::filesystem::path dir = root / ent.second;
    if (!std::filesystem::is_directory(dir))
      throw std::runtime_error("not a directory: " + dir.string());
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
      if (!entry.is_regular_file() || entry.path().extension() != ".asm")
        continue;
      std::vector<char> buf;
      read_file(entry.path(), buf);
      const std::string key = std::string(ent.first) + entry.path().filename().string();
      artifact.add_vfile(key, buf);
    }
  }
}

void usage()
{
  std::cerr << "Usage: legal_duplicate_include_in_mem <legal_duplicate_include_dir> <output1.elf> <output2.elf>\n";
}

} // namespace

int main(int argc, char** argv)
{
  if (argc != 4)  {
    usage();
    return 2;
  }

  const std::filesystem::path root = argv[1];
  const std::filesystem::path out_elf1 = argv[2];
  const std::filesystem::path out_elf2 = argv[3];

  try {
    std::vector<char> merged_asm;
    read_file(root / "ml_asm" / "merged_control.asm", merged_asm);

    std::vector<char> patch_json;
    read_file(root / "external_buffer_id.json", patch_json);

    std::vector<char> config_json;
    config_json.assign(k_config_json, k_config_json + std::strlen(k_config_json));

    aiebu::file_artifact artifact;
    std::string main_key("./ml_asm/merged_control.asm");
    artifact.add_vfile(main_key, merged_asm);
    std::string patch_key("./external_buffer_id.json");
    artifact.add_vfile(patch_key, patch_json);
    add_asm_tree(root, artifact);

    const std::vector<std::string> flags = {"disabledump"};

    aiebu::aiebu_assembler as1(aiebu::aiebu_assembler::buffer_type::aie2ps_config,
                              config_json,
                              artifact,
                              flags);

    const std::vector<char> elf1 = as1.get_elf();
    std::ofstream out1(out_elf1, std::ios::binary);
    out1.write(elf1.data(), static_cast<std::streamsize>(elf1.size()));
    if (!out1)
      throw std::runtime_error("failed to write " + out_elf1.string());

    // Run the same test again with another instance of assembler. Even though
    // the files are same for the second run, there should not be a duplicate
    // file error.

    aiebu::aiebu_assembler as2(aiebu::aiebu_assembler::buffer_type::aie2ps_config,
                              config_json,
                              artifact,
                              flags);

    const std::vector<char> elf2 = as2.get_elf();
    std::ofstream out(out_elf2, std::ios::binary);
    out.write(elf2.data(), static_cast<std::streamsize>(elf2.size()));
    if (!out)
      throw std::runtime_error("failed to write " + out_elf2.string());



  } catch (const aiebu::error& ex) {
    std::cerr << "aiebu::error: " << ex.what() << " (" << ex.get_code() << ")\n";
    return 1;
  } catch (const std::exception& ex) {
    std::cerr << "error: " << ex.what() << '\n';
    return 1;
  }

  return 0;
}

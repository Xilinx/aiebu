#!/usr/bin/env python3
"""
Generate AIE4 save/restore header file from assembly source files.
Converts entire .asm file content to byte arrays for embedding in C++ header.
"""

import os
import sys

def read_file_as_bytes(filepath, label_prefix=None):
    """
    Read entire file content and return as list of bytes.
    Optionally prepend a label (e.g., "save:" or "restore:") and append .endl directive.
    """
    bytes_list = []

    if not os.path.exists(filepath):
        print(f"Warning: File not found: {filepath}")
        return bytes_list

    with open(filepath, 'rb') as f:
        content = f.read()

    # Prepend label and append .endl if label_prefix specified
    if label_prefix:
        # Remove trailing colon for .endl directive (e.g., "save:" -> "save")
        label_name = label_prefix.rstrip(':')
        label_bytes = (label_prefix + "\n").encode('utf-8')
        endl_bytes = ("\n.endl " + label_name + "\n").encode('utf-8')
        content = label_bytes + content + endl_bytes

    bytes_list = list(content)

    return bytes_list

def format_bytes_array(name, bytes_list):
    """Format bytes as an inline constexpr array."""
    lines = [f"inline constexpr std::array<std::uint8_t, {len(bytes_list)}> {name} = {{{{"]
    for offset in range(0, len(bytes_list), 16):
        chunk = bytes_list[offset:offset + 16]
        suffix = "," if offset + 16 < len(bytes_list) else ""
        lines.append("  " + ", ".join(f"0x{byte:02x}" for byte in chunk) + suffix)
    lines.append("}};")
    return "\n".join(lines)

def generate_header(asm_dir, output_file):
    """Generate the header file from assembly files."""

    # Define the mapping: key -> (save_file, restore_file)
    # Keys 1, 2, 3: partition-based selection (1c, 2c, 3c)
    # Keys 10, 12, 14: group-based selection for multi-column mode
    #   - Key 10: 1c0 files for attach_to_group 0
    #   - Key 12: 1c1 files for attach_to_group 2
    #   - Key 14: 1c2 files for attach_to_group 4
    file_mappings = {
        1: ("aie4_save_1c.asm", "aie4_restore_1c.asm"),
        2: ("aie4_save_2c.asm", "aie4_restore_2c.asm"),
        3: ("aie4_save_3c.asm", "aie4_restore_3c.asm"),
        10: ("aie4_save_1c0.asm", "aie4_restore_1c0.asm"),
        12: ("aie4_save_1c1.asm", "aie4_restore_1c1.asm"),
        14: ("aie4_save_1c2.asm", "aie4_restore_1c2.asm"),
    }

    # Parse all files
    data = {}
    for key, (save_file, restore_file) in file_mappings.items():
        save_path = os.path.join(asm_dir, save_file)
        restore_path = os.path.join(asm_dir, restore_file)

        # Determine label names based on key:
        # Keys 1, 2, 3 (single-col): all use save_1 / restore_1
        # Keys 10, 12, 14 (multi-col): save_1/save_2/save_3 based on index
        #   - Key 10 (group 0) -> save_1
        #   - Key 12 (group 2) -> save_2
        #   - Key 14 (group 4) -> save_3
        if key <= 3:
            # Single-column mode: always use save_1 / restore_1
            save_label = "save_1:"
            restore_label = "restore_1:"
        else:
            # Multi-column mode: index = group/2 + 1
            group = key - 10
            index = group // 2 + 1
            save_label = f"save_{index}:"
            restore_label = f"restore_{index}:"

        save_bytes = read_file_as_bytes(save_path, save_label)
        restore_bytes = read_file_as_bytes(restore_path, restore_label)

        if save_bytes or restore_bytes:
            data[key] = (save_bytes, restore_bytes)
            print(f"Key {key}: save={len(save_bytes)} bytes ({save_label}), restore={len(restore_bytes)} bytes ({restore_label})")

    # Generate header content
    # Using C++17 inline variable at file scope to avoid MSVC "function too large" error
    header = """// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_AIE4_PREEMPTION_FILES_H
#define AIEBU_AIE4_PREEMPTION_FILES_H

#include "preprocessor/prebuilt_save_restore.h"

#include <array>
#include <cstdint>
#include <map>
#include <vector>
#include <string>

// C++17 inline variable - single instance across all translation units
inline const std::map<uint32_t, std::pair<std::vector<std::string>, std::vector<std::string>>> aie4_save_restore_shimbd_map = {
  {1, {{"DMAWRITE_data_125", "DMAWRITE_data_126"},
       {"DMAWRITE_data_0", "DMAWRITE_data_1", "DMAWRITE_data_2", "DMAWRITE_data_3"}}},
  {2, {{"DMAWRITE_data_250", "DMAWRITE_data_251", "DMAWRITE_data_252", "DMAWRITE_data_253"},
       {"DMAWRITE_data_0", "DMAWRITE_data_1", "DMAWRITE_data_2", "DMAWRITE_data_3", "DMAWRITE_data_4", "DMAWRITE_data_5", "DMAWRITE_data_6", "DMAWRITE_data_7"}}},
  {3, {{"DMAWRITE_data_375", "DMAWRITE_data_376", "DMAWRITE_data_377", "DMAWRITE_data_378", "DMAWRITE_data_379", "DMAWRITE_data_380"},
       {"DMAWRITE_data_0", "DMAWRITE_data_1", "DMAWRITE_data_2", "DMAWRITE_data_3", "DMAWRITE_data_4", "DMAWRITE_data_5", "DMAWRITE_data_6", "DMAWRITE_data_7", "DMAWRITE_data_8", "DMAWRITE_data_9", "DMAWRITE_data_10", "DMAWRITE_data_11"}}},
  {10, {{"DMAWRITE_data_125", "DMAWRITE_data_126"}, {"DMAWRITE_data_0", "DMAWRITE_data_1", "DMAWRITE_data_2", "DMAWRITE_data_3"}}},
  {12, {{"DMAWRITE_data_125", "DMAWRITE_data_126"}, {"DMAWRITE_data_0", "DMAWRITE_data_1", "DMAWRITE_data_2", "DMAWRITE_data_3"}}},
  {14, {{"DMAWRITE_data_125", "DMAWRITE_data_126"}, {"DMAWRITE_data_0", "DMAWRITE_data_1", "DMAWRITE_data_2", "DMAWRITE_data_3"}}}
};

inline const std::map<uint32_t, std::pair<std::vector<std::string>, std::vector<std::string>>>&
get_aie4_save_restore_shimbd()
{
  return aie4_save_restore_shimbd_map;
}

inline const std::map<uint32_t, std::pair<std::vector<std::string>, std::vector<std::string>>> aie4_save_restore_membd_map = {
  {1, {{"DMAWRITE_data_127", "DMAWRITE_data_128"},
       {"DMAWRITE_data_4", "DMAWRITE_data_5", "DMAWRITE_data_6", "DMAWRITE_data_7"}}},
  {2, {{"DMAWRITE_data_254", "DMAWRITE_data_255", "DMAWRITE_data_256", "DMAWRITE_data_257"},
       {"DMAWRITE_data_8", "DMAWRITE_data_9", "DMAWRITE_data_10", "DMAWRITE_data_11", "DMAWRITE_data_12", "DMAWRITE_data_13", "DMAWRITE_data_14", "DMAWRITE_data_15"}}},
  {3, {{"DMAWRITE_data_381", "DMAWRITE_data_382", "DMAWRITE_data_383", "DMAWRITE_data_384", "DMAWRITE_data_385", "DMAWRITE_data_386"},
       {"DMAWRITE_data_12", "DMAWRITE_data_13", "DMAWRITE_data_14", "DMAWRITE_data_15", "DMAWRITE_data_16", "DMAWRITE_data_17", "DMAWRITE_data_18", "DMAWRITE_data_19", "DMAWRITE_data_20", "DMAWRITE_data_21", "DMAWRITE_data_22", "DMAWRITE_data_23"}}},
  {10, {{"DMAWRITE_data_127", "DMAWRITE_data_128"}, {"DMAWRITE_data_4", "DMAWRITE_data_5", "DMAWRITE_data_6", "DMAWRITE_data_7"}}},
  {12, {{"DMAWRITE_data_127", "DMAWRITE_data_128"}, {"DMAWRITE_data_4", "DMAWRITE_data_5", "DMAWRITE_data_6", "DMAWRITE_data_7"}}},
  {14, {{"DMAWRITE_data_127", "DMAWRITE_data_128"}, {"DMAWRITE_data_4", "DMAWRITE_data_5", "DMAWRITE_data_6", "DMAWRITE_data_7"}}}
};

inline const std::map<uint32_t, std::pair<std::vector<std::string>, std::vector<std::string>>>&
get_aie4_save_restore_membd()
{
  return aie4_save_restore_membd_map;
}

namespace aiebu {
namespace prebuilt {

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
"""

    sorted_keys = sorted(data.keys())
    for key in sorted_keys:
        save_bytes, restore_bytes = data[key]
        header += format_bytes_array(f"aie4_save_{key}", save_bytes) + "\n\n"
        header += format_bytes_array(f"aie4_restore_{key}", restore_bytes) + "\n\n"
        header += f"""inline constexpr prebuilt_save_restore aie4_save_restore_{key} = {{
  {{aie4_save_{key}.data(), aie4_save_{key}.size()}},
  {{aie4_restore_{key}.data(), aie4_restore_{key}.size()}}
}};

"""

    header += """// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
} // namespace prebuilt
} // namespace aiebu

inline constexpr const aiebu::prebuilt_save_restore*
get_aie4_save_restore(uint32_t key) noexcept
{
  switch (key) {
"""

    for key in sorted_keys:
        header += f"""  case {key}:
    return &aiebu::prebuilt::aie4_save_restore_{key};
"""

    header += """  default:
    return nullptr;
  }
}

#endif // AIEBU_AIE4_PREEMPTION_FILES_H
"""

    # Write output file
    with open(output_file, 'w') as f:
        f.write(header)

    print(f"Generated: {output_file}")

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    # Assembly files are in the same directory as this script
    asm_dir = script_dir

    # Default output path is current working directory
    output_file = "aie4_save_restore_map_prebuilt.h"

    # Allow command line override
    if len(sys.argv) > 1:
        output_file = sys.argv[1]

    if len(sys.argv) > 2:
        asm_dir = sys.argv[2]

    generate_header(asm_dir, output_file)

if __name__ == "__main__":
    main()

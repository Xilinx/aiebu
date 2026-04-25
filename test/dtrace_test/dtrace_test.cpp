// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace.h"

#include <iostream>
#include <filesystem>
#include <memory>
#include <string>
#include <fstream>
#include <vector>

static const uint64_t TRACE_CTRL_CODE_BASE = 0x200000;
static const uint64_t TRACE_CTRL_CODE_SIZE = 16384; // 8kB
static const uint32_t word_byte_shift = 32;

int 
run_dtrace_test(const std::string& script_file, 
    const std::string& map_file, const std::string& output_file) 
{
    if (!std::filesystem::exists(script_file)) {
        std::cerr << "ERROR: test files not found: " << script_file;
        return 1;
    }

    uint32_t dtrace_buffer_length = 0;
    uint32_t buffers_length = 0;

    // Create dtrace config
    dtrace_config_t config;
    config.script_file = script_file.c_str();
    config.map_data = map_file.c_str();
    config.log_level = 0U;   // dtrace_error
    config.output_fmt = 0U;  // python format

    // get dtrace handle using create_dtrace_handle api from libcert_dtrace
    void* dtrace_handle = create_dtrace_handle(&config);
    if (!dtrace_handle) {
        std::cerr << "[ERROR]: dtrace compiler failed";
        return 1;
    }
    else {
        // get dtrace number of column using get_dtrace_col_numbers api from libcert_dtrace
        get_dtrace_col_numbers(dtrace_handle, &buffers_length);

        // allocate dtrace information buffer
        std::unique_ptr<uint32_t[]> dtrace_buffer = std::make_unique<uint32_t[]>(TRACE_CTRL_CODE_SIZE); // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
        std::unique_ptr<uint64_t[]> buffers = std::make_unique<uint64_t[]>(buffers_length); // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)

        // get dtrace information using get_dtrace_buffer_size api from libcert_dtrace
        get_dtrace_buffer_size(dtrace_handle, buffers.get());

        for (uint32_t i = 0; i < buffers_length; ++i) {
            auto length = static_cast<uint32_t>(buffers[i] >> word_byte_shift);
            // auto uC_index = static_cast<uint32_t>(buffers[i] & mask_32);
            dtrace_buffer_length += length;
        }

        // create dtrace buffer using populate_dtrace_buffer api from libcert_dtrace
        populate_dtrace_buffer(dtrace_handle, dtrace_buffer.get(), TRACE_CTRL_CODE_BASE);

        // create control_buffer.dat file
        if (dtrace_buffer_length > 0) {
            std::ofstream control_buffer_file(output_file, std::ios::binary);
            control_buffer_file.write(reinterpret_cast<const char*>(dtrace_buffer.get()), 
                static_cast<std::streamsize>(dtrace_buffer_length * sizeof(uint32_t)));
            control_buffer_file.close();
        }
        else {
            std::cerr << "[ERROR]: dtrace compiler - failed buffer length zero";
            return 1;
        }

        // destroy dtrace handle using destroy_dtrace_handle api from libcert_dtrace
        destroy_dtrace_handle(dtrace_handle);
        return 0;
    }
}

int main(int argc, char** argv) 
{
    try {
        if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " <testcase_path> <output_file>";
            return 1;
        }

        std::string script_file = std::string(argv[1]) + "/script.ct";
        std::string map_file = std::string(argv[1]) + "/map.json";
        std::string output_file = argv[2];

        if (!std::filesystem::exists(map_file)) {
            throw std::runtime_error("[ERROR]: map file does not exist: " + map_file);
            return 1; 
        }

        std::ifstream map_file_stream(map_file, std::ios::binary);
        std::vector<uint8_t> map_buffer((std::istreambuf_iterator<char>(map_file_stream)), 
            std::istreambuf_iterator<char>());
        map_file_stream.close();
        std::string map_data = std::string(map_buffer.begin(), map_buffer.end());

        int ret = run_dtrace_test(script_file, map_data, output_file);
        return ret;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR]: Exception: " << e.what();
        return 1;
    }
}

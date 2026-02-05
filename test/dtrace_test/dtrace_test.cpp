// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include <iostream>
#include <filesystem>
#include <memory>
#include <string>
#include <fstream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#define dlopen(lib, flags) LoadLibrary(lib)
#define dlsym(handle, symbol) GetProcAddress((HMODULE)(handle), (symbol))
#define dlclose(handle) FreeLibrary((HMODULE)(handle))
#else
#include <dlfcn.h>
#endif

static const uint64_t TRACE_CTRL_CODE_BASE = 0x200000;
static const uint64_t TRACE_CTRL_CODE_SIZE = 16384; // 8kB
static const uint32_t word_byte_shift = 32;
// static const uint32_t mask_32 = 0xFFFFFFFF; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
using get_dtrace_col_numbers_t = void* (*)(const char*, const char*, uint32_t*, uint32_t);
using get_dtrace_buffer_size_t = void (*)(void*, uint64_t*);
using populate_dtrace_buffer_t = void (*)(void*, uint32_t*, uint64_t);

int 
run_dtrace_test(const std::string& dtrace_lib_path, const std::string& script_file, 
    const std::string& map_file, const std::string& output_file) 
{
    if (!std::filesystem::exists(script_file)) {
        std::cerr << "ERROR: test files not found: " << script_file << std::endl;
        return 1;
    }

    // load dtrace compiler
    void* dtrace_lib_handle = dlopen(dtrace_lib_path.c_str(), RTLD_LAZY);
    if (!dtrace_lib_handle) {
        std::cerr << "[ERROR]: failed to load dtrace library" << std::endl;
        return 1;
    }

    // load and check functions from dtrace compiler
    auto get_dtrace_col_numbers = 
        reinterpret_cast<get_dtrace_col_numbers_t>(dlsym(dtrace_lib_handle, "get_dtrace_col_numbers_with_log"));
    auto get_dtrace_buffer_size = 
        reinterpret_cast<get_dtrace_buffer_size_t>(dlsym(dtrace_lib_handle, "get_dtrace_buffer_size"));
    auto populate_dtrace_buffer = 
        reinterpret_cast<populate_dtrace_buffer_t>(dlsym(dtrace_lib_handle, "populate_dtrace_buffer"));
    if (!get_dtrace_col_numbers || !get_dtrace_buffer_size || !populate_dtrace_buffer) {
        std::cerr << "[ERROR]: failed to load dtrace functions" << std::endl;
        dlclose(dtrace_lib_handle);
        return 1;
    }

    uint32_t dtrace_buffer_length = 0;
    uint32_t buffers_length = 0;
    uint32_t dtrace_log_level = 1; // default log level
    // get dtrace number of column using get_dtrace_col_numbers api from libdtrace.so
    void* dtrace_handle = 
        get_dtrace_col_numbers(script_file.c_str(), map_file.c_str(), &buffers_length, dtrace_log_level);
    if (!dtrace_handle) {
        std::cerr << "[ERROR]: dtrace compiler failed" << std::endl;
        dlclose(dtrace_lib_handle);
        return 1;
    }
    else {
        // allocate dtrace information buffer
        std::unique_ptr<uint32_t[]> dtrace_buffer = std::make_unique<uint32_t[]>(TRACE_CTRL_CODE_SIZE); // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
        std::unique_ptr<uint64_t[]> buffers = std::make_unique<uint64_t[]>(buffers_length); // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)

        // get dtrace information using get_dtrace_buffer_size api from libdtrace.so
        get_dtrace_buffer_size(dtrace_handle, buffers.get());

        for (uint32_t i = 0; i < buffers_length; ++i) {
            auto length = static_cast<uint32_t>(buffers[i] >> word_byte_shift);
            // auto uC_index = static_cast<uint32_t>(buffers[i] & mask_32);
            dtrace_buffer_length += length;
        }

        // create dtrace buffer using populate_dtrace_buffer api from libdtrace.so
        populate_dtrace_buffer(dtrace_handle, dtrace_buffer.get(), TRACE_CTRL_CODE_BASE);

        // create control_buffer.dat file
        if (dtrace_buffer_length > 0) 
        {
            std::ofstream control_buffer_file(output_file, std::ios::binary);
            control_buffer_file.write(reinterpret_cast<const char*>(dtrace_buffer.get()), 
                static_cast<std::streamsize>(dtrace_buffer_length * sizeof(uint32_t)));
            control_buffer_file.close();
        }
        dlclose(dtrace_lib_handle);
        return 0;
    }
}

int main(int argc, char** argv) 
{
    try {
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << "<dtrace_lib_path> <testcase_path> <output_file>" << std::endl;
            return 1;
        }

        std::string dtrace_lib_path = argv[1];
        std::string script_file = std::string(argv[2]) + "/script.ct";
        std::string map_file = std::string(argv[2]) + "/map.json";
        std::string output_file = argv[3];

        if (!std::filesystem::exists(map_file)) {
            throw std::runtime_error("[ERROR]: map file does not exist: " + map_file);
            return 1; 
        }

        std::ifstream map_file_stream(map_file, std::ios::binary);
        std::vector<uint8_t> map_buffer((std::istreambuf_iterator<char>(map_file_stream)), 
            std::istreambuf_iterator<char>());
        map_file_stream.close();
        std::string map_data = std::string(map_buffer.begin(), map_buffer.end());

        int ret = run_dtrace_test(dtrace_lib_path, script_file, map_data, output_file);
        return ret;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR]: Exception: " << e.what() << std::endl;
        return 1;
    }
}

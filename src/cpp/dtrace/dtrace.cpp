// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

// This file implements the dtrace public APIs for creating dtrace control buffer, 
// dtrace memory buffer and dtrace result file.
#include "dtrace.h"
#include "control/control.h"

#include <cstring>
#include <exception>
#include <filesystem>
#include <memory>
#include <stdexcept>

extern "C" {

// Intial parse to create control buffer and memory buffer
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
static std::unique_ptr<dtrace::control> g_control = nullptr;

/**
 * @struct dtrace_buffer_info
 *
 * @brief 
 * Typed dtrace_buffer_info used to hold dtarce buffer information.
 *
 * @details
 * Contains members that specify virtual and physical address of dtrace buffer,
 * control buffer and the memory buffer.
 */
struct dtrace_buffer_info {
    uint32_t* buffer_addr = nullptr;
    uint64_t buffer_dma_addr = 0;
    std::vector<uint32_t> control_buffer; 
    std::vector<uint32_t> mem_buffer;
};
// multiple uC dtrace
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
static std::unordered_map<uint32_t, dtrace_buffer_info> g_dtrace_buffer_info_map;

//---------------------------multiple uC dtrace---------------------------
uint32_t 
get_dtrace_col_numbers(const char* script_file, const char* map_data, 
    uint32_t* buffers_length)
{
    try
    {
        // Initialize the memory host address map and dtrace compiler control object
        // set the log level to default error level
        uint32_t log_level = 1;
        g_control = 
            std::make_unique<dtrace::control>(script_file, map_data, log_level);

        // Get the number of uC in the script file
        uint32_t number_uC = g_control->m_control_uC_indices.size();
        *buffers_length = number_uC;

        return 0; // Success
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
        return 1; // Failure
    }
}

uint32_t 
get_dtrace_col_numbers_with_log(const char* script_file, const char* map_data, 
    uint32_t* buffers_length, uint32_t log_level)
{
    try
    {
        // Initialize the memory host address map and dtrace compiler control object
        // and set the log level
        g_control = 
            std::make_unique<dtrace::control>(script_file, map_data, log_level);

        // Get the number of uC in the script file
        uint32_t number_uC = g_control->m_control_uC_indices.size();
        *buffers_length = number_uC;

        return 0; // Success
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
        return 1; // Failure
    }
}

void 
get_dtrace_buffer_size(uint64_t* buffers)
{
    try
    {
        uint32_t buffer_index = 0;
        // Control buffer size and memory buffer size for each uC
        for (const auto& uC_index : g_control->m_control_uC_indices)
        {
            // Get control buffer and memory buffer size and populate the map
            // with the dtrace_buffer_info for uC_index
            dtrace_buffer_info l_dtrace_buffer_info;
            l_dtrace_buffer_info.buffer_addr = nullptr;
            l_dtrace_buffer_info.buffer_dma_addr = 0; 
            l_dtrace_buffer_info.control_buffer = g_control->create_control_buffer(uC_index);
            l_dtrace_buffer_info.mem_buffer = g_control->create_mem_buffer(uC_index);

            uint32_t length = 
                l_dtrace_buffer_info.control_buffer.size() + l_dtrace_buffer_info.mem_buffer.size();
            // Update the control buffer and memory buffer length and uC index in buffers array
            buffers[buffer_index] = (static_cast<uint64_t>(length) << 32) | uC_index; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            buffer_index++;

            g_dtrace_buffer_info_map[uC_index] = std::move(l_dtrace_buffer_info);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }
}

void 
populate_dtrace_buffer(uint32_t* dtrace_buffer, uint64_t dtrace_buffer_dma)
{
    try
    {
        // Initialize the memory host address map
        std::unordered_map<uint32_t, uint64_t> mem_host_addr_map;

        uint64_t uC_buffer_dma_addr = dtrace_buffer_dma;
        // Update control buffer with mem host addr if mem buffer is present
        if (g_control->m_mem_action_present) {
            for (const auto& uC_index : g_control->m_control_uC_indices)
            {
                // Get the dtrace_buffer_info for the given uC_index
                dtrace_buffer_info& l_dtrace_buffer_info = g_dtrace_buffer_info_map[uC_index];

                uC_buffer_dma_addr += l_dtrace_buffer_info.control_buffer.size() * sizeof(uint32_t);
                if (!l_dtrace_buffer_info.mem_buffer.empty()) {
                    mem_host_addr_map[uC_index] = uC_buffer_dma_addr;

                    uC_buffer_dma_addr += l_dtrace_buffer_info.mem_buffer.size() * sizeof(uint32_t);
                }
                l_dtrace_buffer_info.buffer_dma_addr = uC_buffer_dma_addr;

            }
            // Patch the control buffer with memory host address
            g_control->patch_control_buffer(mem_host_addr_map);  

            // Update control buffer and memory buffer in the global structure after patching
            for (auto& [uC_index, l_dtrace_buffer_info] : g_dtrace_buffer_info_map)
            {
                l_dtrace_buffer_info.control_buffer = g_control->create_control_buffer(uC_index);
                l_dtrace_buffer_info.mem_buffer = g_control->create_mem_buffer(uC_index);
            }
        }

        uint32_t* uC_buffer_addr = dtrace_buffer;
        // Control buffer and memory buffer for each uC
        for (const auto& uC_index : g_control->m_control_uC_indices)
        {    
            // Get the dtrace_buffer_info for the given uC_index
            dtrace_buffer_info& l_dtrace_buffer_info = g_dtrace_buffer_info_map[uC_index];

            // Buffer address for the current uC index
            l_dtrace_buffer_info.buffer_addr = uC_buffer_addr;
            uC_buffer_addr += 
                l_dtrace_buffer_info.control_buffer.size() + l_dtrace_buffer_info.mem_buffer.size();

            std::vector<uint32_t> buffer;
            buffer.insert(
                buffer.end(), 
                l_dtrace_buffer_info.control_buffer.begin(), 
                l_dtrace_buffer_info.control_buffer.end()
            );
            if (!l_dtrace_buffer_info.mem_buffer.empty())
            {
                buffer.insert(
                    buffer.end(), 
                    l_dtrace_buffer_info.mem_buffer.begin(), 
                    l_dtrace_buffer_info.mem_buffer.end()
                );
            }
            std::memcpy(
                l_dtrace_buffer_info.buffer_addr, buffer.data(), 
                buffer.size() * sizeof(uint32_t)
            );
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }
}

void 
get_dtrace_result_file(const char* result_file)
{
    try
    {
        // Initialize the result buffers and memory buffers vector
        std::unordered_map<uint32_t, std::vector<uint32_t>> result_buffers;
        std::unordered_map<uint32_t, std::vector<uint32_t>> mem_buffers;

        // Iterate over all result buffer and memory buffer for each uC in global map
        for (const auto& [uC_index, l_dtrace_buffer_info] : g_dtrace_buffer_info_map)
        {
            std::vector<uint32_t> buffer(
                l_dtrace_buffer_info.control_buffer.size() + l_dtrace_buffer_info.mem_buffer.size()
            );

            // Copy the buffer from the dtrace buffer address
            std::memcpy(
                buffer.data(),
                l_dtrace_buffer_info.buffer_addr,
                (l_dtrace_buffer_info.control_buffer.size() + l_dtrace_buffer_info.mem_buffer.size()) * sizeof(uint32_t)
            );

            // Get control_buffer and mem_buffer from the buffer
            std::vector<uint32_t> control_buffer(
                buffer.begin(),
                buffer.begin() + l_dtrace_buffer_info.control_buffer.size() // NOLINT(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
            );
            std::vector<uint32_t> mem_buffer(
                buffer.begin() + l_dtrace_buffer_info.control_buffer.size(), // NOLINT(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
                buffer.end()
            );
            
            // Populate result buffers and memory buffers vector
            result_buffers[uC_index] = std::move(control_buffer);
            mem_buffers[uC_index] = std::move(mem_buffer);
        }
        // Create the result file
        g_control->create_result_file(result_buffers, mem_buffers, result_file);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }
}

}
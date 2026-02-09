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

/**
 * @struct dtrace_buffer_info
 *
 * @brief 
 * Typed dtrace_buffer_info used to hold dtrace buffer information.
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

/**
 * @struct dtrace_command_handle
 *
 * @brief 
 * dtrace command handle to maintain dtrace information for multiple commands.
 *
 * @details
 * Each command handle maintains its own dtrace control and buffer information maps.
 * This allows multiple dtrace commands to operate independently without interference.
 */
struct dtrace_command_handle {
    // Intial parse to create control buffer and memory buffer
    std::unique_ptr<dtrace::control> g_control = nullptr;
    // multiple uC dtrace
    std::unordered_map<uint32_t, dtrace_buffer_info> g_dtrace_buffer_info_map;
};

dtrace_handle_t 
get_dtrace_col_numbers(const char* script_file, const char* map_data, 
    uint32_t* buffers_length)
{
    try
    {
        // Create new dtrace handle
        auto handle = std::make_unique<dtrace_command_handle>();

        // Initialize the memory host address map and dtrace compiler control object
        // set the log level to default error level
        uint32_t log_level = 1;
        handle->g_control = 
            std::make_unique<dtrace::control>(script_file, map_data, log_level);

        // Get the number of uC in the script file
        uint32_t number_uC = handle->g_control->m_control_uC_indices.size();
        *buffers_length = number_uC;

        return static_cast<dtrace_handle_t>(handle.release());
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
        return nullptr; // Failure
    }
}

dtrace_handle_t 
get_dtrace_col_numbers_with_log(const char* script_file, const char* map_data, 
    uint32_t* buffers_length, uint32_t log_level)
{
    try
    {
        // Create new dtrace handle
        auto handle = std::make_unique<dtrace_command_handle>();

        // Initialize the memory host address map and dtrace compiler control object
        // and set the log level
        handle->g_control = 
            std::make_unique<dtrace::control>(script_file, map_data, log_level);

        // Get the number of uC in the script file
        uint32_t number_uC = handle->g_control->m_control_uC_indices.size();
        *buffers_length = number_uC;

        return static_cast<dtrace_handle_t>(handle.release());
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
        return nullptr; // Failure
    }
}

void 
get_dtrace_buffer_size(dtrace_handle_t dtrace_handle, uint64_t* buffers)
{
    try
    {
        // dtrace handle
        auto* handle = static_cast<dtrace_command_handle*>(dtrace_handle);

        uint32_t buffer_index = 0;
        // Control buffer size and memory buffer size for each uC
        for (const auto& uC_index : handle->g_control->m_control_uC_indices)
        {
            // Get control buffer and memory buffer size and populate the map
            // with the dtrace_buffer_info for uC_index
            dtrace_buffer_info l_dtrace_buffer_info;
            l_dtrace_buffer_info.buffer_addr = nullptr;
            l_dtrace_buffer_info.buffer_dma_addr = 0; 
            l_dtrace_buffer_info.control_buffer = handle->g_control->create_control_buffer(uC_index);
            l_dtrace_buffer_info.mem_buffer = handle->g_control->create_mem_buffer(uC_index);

            uint32_t length = 
                l_dtrace_buffer_info.control_buffer.size() + l_dtrace_buffer_info.mem_buffer.size();
            // Update the control buffer and memory buffer length and uC index in buffers array
            buffers[buffer_index] = (static_cast<uint64_t>(length) << 32) | uC_index; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
            buffer_index++;

            handle->g_dtrace_buffer_info_map[uC_index] = std::move(l_dtrace_buffer_info);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }
}

void 
populate_dtrace_buffer(dtrace_handle_t dtrace_handle, uint32_t* dtrace_buffer, 
    uint64_t dtrace_buffer_dma)
{
    try
    {
        // dtrace handle
        auto* handle = static_cast<dtrace_command_handle*>(dtrace_handle);

        // Initialize the memory host address map
        std::unordered_map<uint32_t, uint64_t> mem_host_addr_map;

        uint64_t uC_buffer_dma_addr = dtrace_buffer_dma;
        // Update control buffer with mem host addr if mem buffer is present
        if (handle->g_control->m_mem_action_present) {
            for (const auto& uC_index : handle->g_control->m_control_uC_indices)
            {
                // Get the dtrace_buffer_info for the given uC_index
                dtrace_buffer_info& l_dtrace_buffer_info = handle->g_dtrace_buffer_info_map[uC_index];

                uC_buffer_dma_addr += l_dtrace_buffer_info.control_buffer.size() * sizeof(uint32_t);
                if (!l_dtrace_buffer_info.mem_buffer.empty()) {
                    mem_host_addr_map[uC_index] = uC_buffer_dma_addr;

                    uC_buffer_dma_addr += l_dtrace_buffer_info.mem_buffer.size() * sizeof(uint32_t);
                }
                l_dtrace_buffer_info.buffer_dma_addr = uC_buffer_dma_addr;

            }
            // Patch the control buffer with memory host address
            handle->g_control->patch_control_buffer(mem_host_addr_map);  

            // Update control buffer and memory buffer in the global structure after patching
            for (auto& [uC_index, l_dtrace_buffer_info] : handle->g_dtrace_buffer_info_map)
            {
                l_dtrace_buffer_info.control_buffer = handle->g_control->create_control_buffer(uC_index);
                l_dtrace_buffer_info.mem_buffer = handle->g_control->create_mem_buffer(uC_index);
            }
        }

        uint32_t* uC_buffer_addr = dtrace_buffer;
        // Control buffer and memory buffer for each uC
        for (const auto& uC_index : handle->g_control->m_control_uC_indices)
        {    
            // Get the dtrace_buffer_info for the given uC_index
            dtrace_buffer_info& l_dtrace_buffer_info = handle->g_dtrace_buffer_info_map[uC_index];

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
get_dtrace_result_file(dtrace_handle_t dtrace_handle, const char* result_file)
{
    try
    {
        // dtrace handle unique pointer
        auto handle = std::unique_ptr<dtrace_command_handle>(
            static_cast<dtrace_command_handle*>(dtrace_handle)
        );

        // Initialize the result buffers and memory buffers vector
        std::unordered_map<uint32_t, std::vector<uint32_t>> result_buffers;
        std::unordered_map<uint32_t, std::vector<uint32_t>> mem_buffers;

        // Iterate over all result buffer and memory buffer for each uC in global map
        for (const auto& [uC_index, l_dtrace_buffer_info] : handle->g_dtrace_buffer_info_map)
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
        handle->g_control->create_result_file(result_buffers, mem_buffers, result_file);

        // Iterate over and copy back modified buffers for each uC in global map
        for (const auto& [uC_index, l_dtrace_buffer_info] : handle->g_dtrace_buffer_info_map)
        {
            std::vector<uint32_t> buffer;
            buffer.insert(
                buffer.end(), result_buffers[uC_index].begin(), result_buffers[uC_index].end()
            );
            buffer.insert(
                buffer.end(), mem_buffers[uC_index].begin(), mem_buffers[uC_index].end()
            );
            
            // Populate modified buffer back to the dtrace buffer address
            std::memcpy(
                l_dtrace_buffer_info.buffer_addr,
                buffer.data(),
                buffer.size() * sizeof(uint32_t)
            );
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }
}

}
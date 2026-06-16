// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026 Advanced Micro Devices, Inc. All rights reserved.

// This file implements the dtrace public APIs for creating dtrace control buffer,
// dtrace memory buffer and dtrace result file.
#include "dtrace.h"
#include "utils.h"

#include "control/control.h"

#include <cstring>
#include <exception>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

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
    std::unordered_map<uint32_t, dtrace::dtrace_buffer_info> g_dtrace_buffer_info_map;
};

dtrace_handle_t
create_dtrace_handle(const std::string& script_file, const std::string& map_data, uint32_t log_level,
    uint32_t output_fmt)
{
    try
    {
        // Validate script file path and map data
        if (script_file.empty())
        {
            std::cerr << "[DTRACE] [ERROR] : Invalid dtrace config script data";
            return nullptr;
        }

        if (map_data.empty())
        {
            std::cerr << "[DTRACE] [ERROR] : Invalid dtrace config map data";
            return nullptr;
        }

        // Create new dtrace handle
        auto handle = std::make_unique<dtrace_command_handle>();

        dtrace::set_log_level(log_level);
        dtrace::set_output_format(output_fmt);

        // Initialize the memory host address map and dtrace compiler control object
        handle->g_control = std::make_unique<dtrace::control>(script_file, map_data);

        // Returns an opaque raw handle.
        // Transfer ownership to caller; caller must call destroy_dtrace_handle().
        return static_cast<dtrace_handle_t>(handle.release());
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
        return nullptr; // Failure
    }
}

void
get_dtrace_col_numbers(dtrace_handle_t dtrace_handle, uint32_t* buffers_length)
{
    try
    {
        // dtrace handle
        auto* handle = static_cast<dtrace_command_handle*>(dtrace_handle);

        // Get the number of uC in the script file
        auto number_uC = static_cast<uint32_t>(handle->g_control->m_control_uC_indices.size());
        *buffers_length = number_uC;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
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
            dtrace::dtrace_buffer_info l_dtrace_buffer_info;
            l_dtrace_buffer_info.buffer_addr = nullptr;
            l_dtrace_buffer_info.buffer_dma_addr = 0; 
            l_dtrace_buffer_info.control_buffer = handle->g_control->create_control_buffer(uC_index);
            l_dtrace_buffer_info.mem_buffer = handle->g_control->create_mem_buffer(uC_index);

            auto length = static_cast<uint32_t>(
                l_dtrace_buffer_info.control_buffer.size() + l_dtrace_buffer_info.mem_buffer.size());
            // Update the control buffer and memory buffer length and uC index in buffers array
            buffers[buffer_index] = 
                (static_cast<uint64_t>(length) << dtrace::dtrace_ctrl::forth_byte_shift) | uC_index;
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
                dtrace::dtrace_buffer_info& l_dtrace_buffer_info = handle->g_dtrace_buffer_info_map[uC_index];

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
            dtrace::dtrace_buffer_info& l_dtrace_buffer_info = handle->g_dtrace_buffer_info_map[uC_index];

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
get_dtrace_result_file(dtrace_handle_t dtrace_handle, const std::string& result_file)
{
    try
    {
        // dtrace handle
        auto* handle = static_cast<dtrace_command_handle*>(dtrace_handle);

        // Create the result file
        handle->g_control->create_result_file(handle->g_dtrace_buffer_info_map, result_file);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }
}

std::string
get_dtrace_result_buffer(dtrace_handle_t dtrace_handle)
{
    try
    {
        auto* handle = static_cast<dtrace_command_handle*>(dtrace_handle);

        // Update the result buffer with the given result key and result buffer data
        nlohmann::ordered_json result_json = nlohmann::ordered_json::object();
        handle->g_control->create_result_buffer(handle->g_dtrace_buffer_info_map, result_json);

        // Return the result buffer as JSON string
        return result_json.dump();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
        return "null";
    }
}

void
destroy_dtrace_handle(dtrace_handle_t dtrace_handle)
{
    try
    {
        // dtrace handle destruction
        std::unique_ptr<dtrace_command_handle> handle(
            static_cast<dtrace_command_handle*>(dtrace_handle)
        );
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }
}

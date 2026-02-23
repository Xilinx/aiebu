// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

// This file defines the control class which is responsible for creating control buffers and result files.
#include "control.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#define htole32(x) (x)
#else
#include <endian.h>
#endif

namespace dtrace
{

//-------------------------Log Level-------------------------//
// variable to store the current log level
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
static dtrace_log_level g_current_log_level = dtrace_log_level::dtrace_error;

// Function to set the log level
void set_log_level(uint32_t log_level) 
{
    g_current_log_level = static_cast<dtrace_log_level>(log_level);
}

// Function to get the current log level
dtrace_log_level get_current_log_level() 
{
    return g_current_log_level;
}

//-------------------------control::control-------------------------//
/**
 * control() - Constructor with script file and map data
 *
 * @param script_file
 *  Script file containing the probe and action details.
 * @param map_data
 *  Map data containing details of control code.
 *
 * This constructor initializes the control object by reading and parsing a script
 * file, and setting up control and memory buffers based on the parsed data.
 */
control::
control(std::string script_file, const std::string& map_data, uint32_t log_level)
    : m_pager(true)
    , m_parser(map_data)
    , m_num_uCs(0)
    , m_mem_action_present(false)
{
    // Set the log level
    dtrace::set_log_level(log_level);
    DTRACE_INFO("DTRACE LOG LEVEL " << log_level);

    // Read the script file
    // Open file in "at end" mode to check size
    std::ifstream file(script_file, std::ios::ate);
    if (!file.is_open())
        DTRACE_ERROR("DTRACE_CONTROL_SCRIPT_FILE_NOT_FOUND", "script file: " << script_file);

    // Check script file empty
    if (file.tellg() == 0)
        DTRACE_ERROR("DTRACE_CONTROL_SCRIPT_FILE_EMPTY", "script file: " << script_file);

    // Reset to the beginning of the file
    file.seekg(0, std::ios::beg);

    std::string line;
    std::vector<std::string> file_lines;
    while (std::getline(file, line))
        file_lines.push_back(line);

    file.close();

    // Compile the script
    for (const auto& line : file_lines)
        m_parser.parse_line(line);

    // Check if any uC indices were parsed
    if (m_parser.m_uC_indices.empty()) {
        DTRACE_ERROR("DTRACE_CONTROL_NO_uC_INDICES", "script file: " << script_file);
    }

    // Set last uC index to the number of uCs
    m_num_uCs = *m_parser.m_uC_indices.rbegin() + 1;

    for (const auto& uC : m_parser.m_uC_indices)
    {
        // Initialize control and memory buffer for uC
        m_control_uC_indices.insert(uC);
        m_control_buffers[uC] = {};
        m_mem_buffers[uC] = {};
        m_mem_action_locations[uC] = {};

        // Initialize control buffer
        m_control_buffers.at(uC).push_back(
            (dtrace::probe::probe_ctrl::link_end << dtrace::dtrace_ctrl::second_byte_shift) | 
            (dtrace::probe::probe_type::begin << dtrace::dtrace_ctrl::first_byte_shift)
        );
        m_control_buffers.at(uC).push_back(
            (dtrace::probe::probe_ctrl::link_end << dtrace::dtrace_ctrl::second_byte_shift) | 
            (dtrace::probe::probe_type::end << dtrace::dtrace_ctrl::first_byte_shift)
        );
        m_control_buffers.at(uC).push_back(
            (dtrace::probe::probe_ctrl::link_end << dtrace::dtrace_ctrl::second_byte_shift) | 
            (dtrace::probe::probe_type::jprobe << dtrace::dtrace_ctrl::first_byte_shift)
        );
        m_control_buffers.at(uC).push_back(
            (dtrace::probe::probe_ctrl::link_end << dtrace::dtrace_ctrl::second_byte_shift) | 
            (dtrace::probe::probe_type::tracepoint << dtrace::dtrace_ctrl::first_byte_shift)
        );

        // Create control block buffer
        for (const auto& key : m_parser.m_probe_order.at(uC))
        {
            const auto& probe = m_parser.m_probes.at(uC).at(key);
            // Enable the probe and get the mem action locations
            try 
            {
                const auto mem_action_locations = 
                    probe->enable(m_control_buffers.at(uC), m_mem_buffers.at(uC));

                // Add the mem action locations for probe to control member locations variable
                m_mem_action_locations.at(uC).insert(
                    m_mem_action_locations.at(uC).end(),
                    mem_action_locations.begin(),
                    mem_action_locations.end()
                );
                DTRACE_INFO("Enabled probe " << probe->create_string() << " for uC index " << uC);
            } 
            catch (const std::exception& e) 
            {
                DTRACE_ERROR("DTRACE_PROBE_ENABLE_FAILED", "Failed to enable probe " 
                    << probe->create_string() << " for uC index " << uC << ".Exception: " << e.what());
            }
        }

        // Memory buffer present flag
        if (m_mem_buffers.at(uC).size() > 0)
            m_mem_action_present = true;

        // Trace control block paging
        m_control_buffers[uC] = m_pager.paging(m_control_buffers.at(uC), uC);
    }
}

//-------------------------control::create_control_buffer-------------------------//
/**
 * create_control_buffer() - Creates a control buffer for the specified uC.
 *
 * @param uC 
 *  uC identifier for which the control buffer is to be created.
 * @return 
 *  Vector (uint32_t) containing the control buffer in little-endian format.
 *
 * This function generates a control block buffer by converting the items 
 * for the given uC to little-endian format.
 */
std::vector<uint32_t> 
control::
create_control_buffer(uint32_t uC) const
{
    std::vector<uint32_t> buffer;
    if (m_control_buffers.find(uC) == m_control_buffers.end())
    {
        DTRACE_ERROR("DTRACE_CONTROL_BUFFER_CREATE_FAILED", 
            "Control buffer for uC " << uC << " not found.");
        return buffer;
    }
    for (const auto& item : m_control_buffers.at(uC))
    {
        uint32_t little_endian_item = htole32(item); // little-endian
        buffer.push_back(little_endian_item);
    }
    return buffer;
}

//-------------------------control::create_mem_buffer-------------------------//
/**
 * create_mem_buffer() - Creates a memory buffer for a specified uC.
 *
 * @param uC 
 *  uC for which the memory buffer is to be created.
 * @return 
 *  Vector (uint32_t) containing the memory buffer in little-endian format.
 *
 * This function generates a memory buffer for the given uC by converting
 * each item in the uC to little-endian format and storing it in a vector.
 */
std::vector<uint32_t> 
control::
create_mem_buffer(uint32_t uC) const
{
    std::vector<uint32_t> buffer;
    if (m_mem_buffers.find(uC) == m_mem_buffers.end())
    {
        DTRACE_ERROR("DTRACE_CONTROL_MEMORY_BUFFER_CREATE_FAILED", 
            "Memory buffer for uC " << uC << " not found.");
        return buffer;
    }
    for (const auto& item : m_mem_buffers.at(uC))
    {
        uint32_t little_endian_item = htole32(item); // little-endian
        buffer.push_back(little_endian_item);
    }
    return buffer;
}


//-------------------------control::patch_control_buffer-------------------------//
/**
 * patch_control_buffer() - Patches control buffer with memory host address.
 *
 * @param mem_host_addr_map
 *  Map containing memory host addresses indexed by uC.
 *
 * This function iterates through the control buffers for each uC and patches
 * the memory host address. It updates the control buffer with the updated
 * memory host address by combining the high and low values from the control
 * buffer and the host address from the mem_host_addr_map.
 */
void
control::
patch_control_buffer(std::unordered_map<uint32_t, uint64_t>& mem_host_addr_map)
{
    for (const auto& uC : m_control_uC_indices)
    {
        if (m_mem_action_locations.find(uC) == m_mem_action_locations.end())
            DTRACE_ERROR("DTRACE_CONTROL_MEM_ACTION_LOCATIONS_NOT_FOUND", 
                "Memory action locations for uC " << uC << " not found.");

        if (m_mem_action_locations.at(uC).empty()) 
        {
            DTRACE_INFO("No memory action locations for uC " << uC << ", skip patching.");
            continue; // No memory action locations for this uC, skip patching
        }

        // pager action location mapping
        const auto& mapping = m_pager.get_action_location_mapping(uC);

        for (const auto& location_index : m_mem_action_locations.at(uC))
        {
            uint32_t action_type = 
                (location_index >> dtrace::dtrace_ctrl::second_byte_shift) & dtrace::dtrace_ctrl::mask_32;
            // adding 2 for mem_host_addr location
            uint32_t location = (location_index & dtrace::dtrace_ctrl::mask_16) + 2;

            if (action_type == dtrace::action::action_type::mem_write)
            {
                uint32_t location_h = mapping.at(location);
                uint32_t location_l = mapping.at(location + 1);
                // Extract high and low values from control buffer for uC
                uint64_t mem_host_addr = 
                    static_cast<uint64_t>(m_control_buffers.at(uC).at(location_h)) << 
                    dtrace::dtrace_ctrl::forth_byte_shift | 
                    m_control_buffers.at(uC).at(location_l);

                // mem_host_addr for uC
                mem_host_addr += mem_host_addr_map.at(uC);
                // Update memory host address high and low values
                m_control_buffers.at(uC).at(location_h) = static_cast<uint32_t>(
                    (mem_host_addr >> dtrace::dtrace_ctrl::forth_byte_shift) & 
                    dtrace::dtrace_ctrl::mask_32);
            
                m_control_buffers.at(uC).at(location_l) = static_cast<uint32_t>(
                    mem_host_addr & dtrace::dtrace_ctrl::mask_32);
            }
            else if (action_type == dtrace::action::action_type::reg_mask_write)
            {
                uint32_t location_addr = mapping.at(location);
                // Extract header word from control buffer for action to understand mode
                uint32_t location_header = mapping.at(location - 3);
                uint32_t action_header = m_control_buffers.at(uC).at(location_header);
                uint32_t mode = 
                    (action_header >> dtrace::dtrace_ctrl::first_byte_shift) & dtrace::dtrace_ctrl::mask_8;

                if (mode == 1) 
                {// HIGH
                    uint64_t patched_mem_host_addr = m_control_buffers.at(uC).at(location_addr) +
                        ((mem_host_addr_map.at(uC) >> dtrace::dtrace_ctrl::forth_byte_shift) & dtrace::dtrace_ctrl::mask_32);

                    m_control_buffers.at(uC).at(location_addr) = static_cast<uint32_t>(
                        patched_mem_host_addr & dtrace::dtrace_ctrl::mask_32
                    );
                }
                else if (mode == 2)
                {// LOW
                    uint64_t patched_mem_host_addr = m_control_buffers.at(uC).at(location_addr) +
                        ((mem_host_addr_map.at(uC) & dtrace::dtrace_ctrl::mask_32));

                    m_control_buffers.at(uC).at(location_addr) = static_cast<uint32_t>(
                        patched_mem_host_addr & dtrace::dtrace_ctrl::mask_32
                    );
                }

                // Clear first byte in action header to indicate patching done
                action_header &= ~(dtrace::dtrace_ctrl::mask_8 << dtrace::dtrace_ctrl::first_byte_shift);
                m_control_buffers.at(uC).at(location_header) = action_header;
            }
        }
    }
}


//-------------------------control::create_result_file-------------------------//
/**
 * create_result_file() - Creates a result file from the given result and memory buffers.
 *
 * @param result_buffers 
 *  Map containing result buffers indexed by uC.
 * @param mem_buffers 
 *  Map containing memory buffers indexed by uC.
 * @param output_file 
 *  Path to the output file where the Python script will be written.
 *
 * This function generates a Python script that processes the provided result and 
 * memory buffers. It iterates through the probes and their associated actions,
 * serializing the actions into the output file.
 */
void 
control::
create_result_file(std::unordered_map<uint32_t, std::vector<uint32_t>>& result_buffers, 
    std::unordered_map<uint32_t, std::vector<uint32_t>>& mem_buffers, 
    const std::string& output_file) const
{
    uint32_t uC_index = 0;  // uC_index 0 for begin and end probes
    std::vector<std::pair<std::shared_ptr<dtrace::action::action>, uint32_t>> actions;

    // Probe - begin
    if (m_parser.m_probes.at(uC_index).find("begin") != m_parser.m_probes.at(uC_index).end())
    {
        for (const auto& action : m_parser.m_probes.at(uC_index).at("begin")->m_actions)
            actions.emplace_back(action, uC_index);
    }

    // Probe - Jprobe and Tracepoint
    for (const auto& uC : m_parser.m_uC_indices)
    {
        for (const auto& probe_name : m_parser.m_probe_order.at(uC))
        {
            if (probe_name == "begin" || probe_name == "end")
                continue;
        
            const auto& probe = m_parser.m_probes.at(uC).at(probe_name);
            for (const auto& action : probe->m_actions)
                actions.emplace_back(action, uC);
        }
    }

    // Probe - end
    if (m_parser.m_probes.at(uC_index).find("end") != m_parser.m_probes.at(uC_index).end())
    {
        for (const auto& action : m_parser.m_probes.at(uC_index).at("end")->m_actions)
            actions.emplace_back(action, uC_index);
    }

    // Create python script
    std::ofstream output(output_file);
    if (!output)
        DTRACE_ERROR("DTRACE_CONTROL_RESULT_FILE_NOT_FOUND", "result file: " << output_file);
        
    output << "#! /usr/bin/env python3\n";
    output << "import sys\n\n";
    output << "if __name__ == '__main__':\n";
    for (const auto& item : actions)
    {
        const auto& action = item.first;
        uint32_t uC_index = item.second;
        try 
        {
            output << action->serialize(
                result_buffers.at(uC_index), 
                mem_buffers.at(uC_index), 
                m_pager.get_action_location_mapping(uC_index)
            );
        } 
        catch (const std::exception& e) 
        {
            DTRACE_ERROR("DTRACE_ACTION_SERIALIZE_FAILED", "Failed to serialize action " 
                << action->create_string() << " for uC index " << uC_index << ". Exception: " << e.what() 
            );
        }
    }
    output << "  " << "sys.exit(0)\n";
    output.close();
}

} // namespace dtrace

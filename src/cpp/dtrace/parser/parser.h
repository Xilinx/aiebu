// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef PARSER_H
#define PARSER_H

// This file contains the declaration of the parser class which is responsible 
// for parsing and managing trace actions and probes.
#include "dtrace/action/action_control.h"
#include "dtrace/probe/probe_control.h"
#include "dtrace/utils.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <cstdint>
#include <map>
#include <memory>
#include <boost/regex.hpp>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace dtrace::parser
{

//-------------------------State-------------------------//
/**
 * @enum state_type
 * 
 * @brief
 * Enumeration representing the state of the parser.
 *
 * @var state_type::probe
 *  Represents the probe state.
 * @var state_type::action_open
 *  Represents the action open state.
 * @var state_type::action
 *  Represents the action state.
 * @var state_type::action_close
 *  Represents the action close state.
 */
enum class state_type
{
    probe = 0,
    action_open = 1,
    action = 2,
    action_close = 3,
    buffer_open = 4,
};

//-------------------------Parser-------------------------//
/**
 * @class parser
 *
 * @brief
 * dtrace::parser::parser is responsible for parsing and managing trace actions and probes.
 *
 * @details
 * The parser class handles the parsing of script file, manages the state of the
 * parser, and stores information about probes and actions. It provides methods 
 * to parse lines of script file, expand profile, jprobe, and tracepoint probes, 
 * and create actions based on the parsed data.
 */
class parser
{
private:
    state_type m_state;
    bool m_begin_exist;
    bool m_end_exist;
    bool m_open;
    bool m_open_buffer;
    uint32_t m_probe_type;
    uint32_t m_uC_index;
    std::string m_probe_name;
    std::unordered_map<std::string, std::vector<std::pair<std::string, uint32_t>>> m_probe_expand;
    int m_position;
    std::unordered_map<std::string, boost::property_tree::ptree> m_maps;
    std::string m_write_buffer;
    std::unordered_map<std::string, std::vector<uint32_t>> m_buffer_map;
    std::vector<int> get_list(const std::string& token) const;
    std::pair<int, int> lookup_control_code_location(const std::string& probe_name) const;
    void expand_profile(uint32_t probe_type, const std::string& probe_name);
    void expand_jprobe(uint32_t probe_type, const std::string& probe_name);
    void expand_tracepoint(uint32_t probe_type, const std::string& probe_name);
    void expand_write_buffer(const std::string& write_buffer);
    void probe_add_action(uint32_t probe_type, const std::string& probe_name, 
        const std::string& action);
    std::shared_ptr<dtrace::action::action> create_action(
        const std::string& action_string, uint32_t probe_type, 
        const std::string& probe_name, uint32_t uC_index);

public:
    parser(const std::string& map_data);
    std::unordered_map<uint32_t, std::map<std::string, std::shared_ptr<dtrace::probe::probe>>> m_probes;
    std::unordered_map<uint32_t, std::vector<std::string>> m_probe_order;
    std::unordered_map<uint32_t, std::vector<std::shared_ptr<dtrace::action::action>>> m_actions;
    std::set<uint32_t> m_uC_indices;
    void parse_line(const std::string& parse_line);
    std::unordered_map<uint32_t, uint64_t> m_mem_host_addr_map; 
};

} // namespace dtrace::parser
#endif // PARSER_H

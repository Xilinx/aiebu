// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

// This file contains the implementation of the parser class. The parser class is responsible for
// parsing the input script file and generating the corresponding probe and action.
#include "dtrace/parser/parser.h"
#include "dtrace/action/action_control.h"
#include "dtrace/probe/probe_control.h"

#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace dtrace::parser
{

//-------------------------Parser Constructor-------------------------//
/**
 * parser() - Constructor for the parser class.
 * 
 * @param map_data 
 *  Map data with details of control code.
 *
 * Initializes the parser object with the provided map data and memory host address map.
 * It sets the initial state to `probe` and initializes parser member variables. 
 * Map data is parsed from a JSON string and stored in the `m_maps` member variable.
 */
parser::
parser(const std::string& map_data)
    : m_state(state_type::probe)
    , m_begin_exist(false)
    , m_end_exist(false)
    , m_open(false)
    , m_open_buffer(false)
    , m_probe_type(0)
    , m_uC_index(0)
    , m_position(0)
{
    if (map_data.empty()) 
        DTRACE_ERROR("DTRACE_PARSER_MAP_DATA_EMPTY", "Failed to open map data for reading.");

    std::istringstream data(map_data);
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(data, pt);
    for (const auto& item : pt.get_child("debug"))
    {
        // Extract file name
        std::filesystem::path file_path(item.second.get<std::string>("file"));
        std::string map_file_name = file_path.filename().string();

        // Process line-based entries
        if (item.second.get_child_optional("line"))
        {
            std::string line_key = "jprobe:" + map_file_name + 
                ":uc" + item.second.get<std::string>("column") +
                ":line" + item.second.get<std::string>("line");
            boost::property_tree::ptree line_value;
            line_value.put("operation",     item.second.get<std::string>("operation"));
            line_value.put("page_index",    item.second.get<std::string>("page_index"));
            line_value.put("page_offset",   item.second.get<std::string>("page_offset"));
            m_maps[line_key] = line_value;

            // Process annotation-based entries if available
            if (item.second.get_child_optional("annotation"))
            {
                auto annotation = item.second.get_child("annotation");
                std::string annotation_key = "jprobe:" + map_file_name + 
                    ":uc" + item.second.get<std::string>("column") +
                    ":annotation" + annotation.get<std::string>("id");
                boost::property_tree::ptree annotation_value;
                annotation_value.put("operation",   item.second.get<std::string>("operation"));
                annotation_value.put("page_index",  item.second.get<std::string>("page_index"));
                annotation_value.put("page_offset", item.second.get<std::string>("page_offset"));
                m_maps[annotation_key] = annotation_value;
            }
        }
    }
}

//-------------------------jprobe::lookup_control_code_location-------------------------//
/**
 * lookup_control_code_location() - 
 *  Retrieves the control code location from the control code map.
 * 
 * @return 
 *  std::pair<int, int> pair containing the page and offset of the control code location.
 * 
 * This function iterates through the control code map to find the matching file 
 * and line number specified in the probe fields. If a match is found, 
 * it calculates the page and offset for the control code location.
 */
std::pair<int, int>
parser::
lookup_control_code_location(const std::string& probe_name) const
{
    int page = -1;
    int offset = -1;
    if (m_maps.find(probe_name) != m_maps.end())
    {
        const auto& value = m_maps.at(probe_name);
        page = std::stoi(value.get<std::string>("page_index"));
        offset = std::stoi(value.get<std::string>("page_offset")) - 
            static_cast<int>(page * dtrace::dtrace_ctrl::page_length_check);
    }
    return {page, offset};
}

//-------------------------parser::get_list-------------------------//
/**
 * get_list() - 
 *  Parses comma-separated string of integers and ranges into a list of integers.
 *
 * @return
 *  vector of integers parsed from the input string.
 *
 * This function takes a string containing comma-separated integers and ranges ("1,2,5-7")
 * and returns a vector of integers that includes all the specified numbers and ranges.
 */
std::vector<int>
parser::
get_list(const std::string& token) const
{
    std::vector<std::string> token_list;
    std::istringstream token_stream(token);
    std::string item;
    while (std::getline(token_stream, item, ','))
        token_list.push_back(item);

    std::vector<int> ret; // return value
    // Check if the token is empty
    if (token_list.empty())
    {
        DTRACE_ERROR("DTRACE_PARSER_INVALID_LINE_ID_TOKEN", 
            "Invalid token: " << token);
        return ret;
    }

    // Parse each token in the list and add the numbers to the return vector
    // If a token contains a range, add all the numbers in the range to the return vector
    for (const auto& item : token_list)
    {
        if (item.find('-') == std::string::npos)
        {
            ret.push_back(std::stoi(item));
            continue;
        }
        std::istringstream range_stream(item);
        std::string start, end;
        std::getline(range_stream, start, '-');
        std::getline(range_stream, end, '-');
        int start_int = std::stoi(start);
        int end_int = std::stoi(end);

        if (start_int > end_int) {
            DTRACE_ERROR("DTRACE_PARSER_INVALID_LINE_ID_RANGE", 
                "Invalid range: " << item);
            return ret;
        }

        for (int rg = start_int; rg <= end_int; ++rg)
            ret.push_back(rg);
    }
    return ret;
}

//-------------------------parser::expand_jprobe-------------------------//
/**
 * expand_jprobe() - Job probe processing function.
 *
 * @param probe_type
 * @param probe_name
 *
 * This function parses the job probe to extract uC and line information,
 * generates and stores expanded probe information in m_probe_expand, m_probe_order, and m_probes.
 */
void
parser::
expand_jprobe(uint32_t probe_type, const std::string& probe_name)
{
    m_probe_expand[probe_name] = {};
    std::vector<std::string> probe_fields;
    // Split the probe name into its components
    std::istringstream probe_stream(probe_name);
    std::string item;
    while (std::getline(probe_stream, item, ':'))
        probe_fields.push_back(item);

    if (probe_fields.size() < 4) 
        DTRACE_ERROR("DTRACE_PARSER_INVALID_JPROBE_PROBE_ARGUMENTS", 
            "Invalid jprobe format: '" << probe_name << 
            "' Expected 'jprobe:name_of_asm:ucX:line|annotationY'");

    auto uCs = get_list(probe_fields[2].substr(2));

    std::string label;
    std::vector<int> range;
    if (probe_fields[3].substr(0, 4) == "line") 
    {
        range = get_list(probe_fields[3].substr(4));
        label = probe_fields[3].substr(0, 4);
    } 
    else if (probe_fields[3].substr(0, 10) == "annotation") 
    {
        range = get_list(probe_fields[3].substr(10));
        label = probe_fields[3].substr(0, 10);
    }
    else 
    {
        DTRACE_ERROR("DTRACE_PARSER_INVALID_JPROBE_LABEL", 
            "Invalid jprobe label: " << probe_fields[3]);
    }
    // For each uC and line, generate probe name and store it in the probe expand map
    for (const auto& uC : uCs)
    {
        m_uC_index = uC;
        m_uC_indices.insert(m_uC_index);
        m_mem_host_addr_map.insert({m_uC_index, 0});
        for (const auto& index : range)
        {
            std::string probe = 
                probe_fields[0] + ":" + probe_fields[1] + 
                ":uc" + std::to_string(uC) + 
                ":" + label + std::to_string(index);
            // If we don't find the page/offset in map file, we just ignore the 
            // probe instead of raising exception; this is useful for declaring probes 
            // with line range where there are empty line and/or comment line in the range
            auto [page, offset] = lookup_control_code_location(probe);
            if (page == -1 || offset == -1)
            {
                DTRACE_WARNING("jprobe does not exist: " << probe);
                continue;
            }
                
            m_probe_expand[probe_name].emplace_back(probe, m_uC_index);
            m_probe_order[m_uC_index].push_back(probe);
            m_probes[m_uC_index][probe] = 
                std::make_shared<dtrace::probe::jprobe>(probe_type, probe, page, offset);
        }
    }
}

//-------------------------parser::expand_tracepoint-------------------------//
/**
 * expand_tracepoint() - Tracepoint probe processing function.
 *
 * @param probe_type
 * @param probe_name
 *
 * This function parses the tracepoint probe to extract uC and line information,
 * generates and stores expanded probe information in m_probe_expand, m_probe_order, and m_probes.
 */
void
parser::
expand_tracepoint(uint32_t probe_type, const std::string& probe_name)
{
    m_probe_expand[probe_name] = {};
    std::vector<std::string> probe_fields;
    // Split the probe name into its components
    std::istringstream probe_stream(probe_name);
    std::string item;
    while (std::getline(probe_stream, item, ':'))
        probe_fields.push_back(item);

    if (probe_fields.size() < 3)
        DTRACE_ERROR("DTRACE_PARSER_INVALID_TRACEPOINT_PROBE_ARGUMENTS", 
            "Invalid tracepoint format: '" << probe_name << "' Expected 'tracepoint:ucX:idZ'");
    
    auto uCs = get_list(probe_fields[1].substr(2));
    auto ids = get_list(probe_fields[2].substr(2));
    // For each uC and line, generate probe name and store it in the probe expand map
    for (const auto& uC : uCs)
    {
        m_uC_index = uC;
        m_uC_indices.insert(m_uC_index);
        m_mem_host_addr_map.insert({m_uC_index, 0});
        for (const auto& id : ids)
        {
            std::string probe = 
                probe_fields[0] + 
                ":uc" + std::to_string(uC) + 
                ":id" + std::to_string(id);
            m_probe_expand[probe_name].emplace_back(probe, m_uC_index);
            m_probe_order[m_uC_index].push_back(probe);
            m_probes[m_uC_index][probe] = 
                std::make_shared<dtrace::probe::tracepoint_probe>(probe_type, probe);
        }
    }
}

//-------------------------parser::expand_profile-------------------------//
/**
 * expand_profile() - Profile probe processing function.
 *
 * @param probe_type
 * @param probe_name
 *
 * This function parses the profile probe to extract uC and line information,
 * generates and stores expanded probe information in m_probe_expand, m_probe_order, and m_probes.
 */
void
parser::
expand_profile(uint32_t probe_type, const std::string& probe_name)
{
    m_probe_expand[probe_name] = {};
    std::vector<std::string> probe_fields;
    // Split the probe name into its components
    std::istringstream probe_stream(probe_name);
    std::string item;
    while (std::getline(probe_stream, item, ':'))
        probe_fields.push_back(item);

    if (probe_fields.size() < 3)
        DTRACE_ERROR("DTRACE_PARSER_INVALID_PROFILE_PROBE_ARGUMENTS", 
            "Invalid profile format: '" << probe_name << "' Expected 'profile:ucX:Whz'");

    auto uCs = get_list(probe_fields[1].substr(2));
    // For each uC and line, generate probe name and store it in the probe expand map
    for (const auto& uC : uCs)
    {
        m_uC_index = uC;
        m_uC_indices.insert(m_uC_index);
        m_mem_host_addr_map.insert({m_uC_index, 0});
        std::string probe = 
            probe_fields[0] + ":uc" + std::to_string(uC) + ":" + probe_fields[2];
        m_probe_expand[probe_name].emplace_back(probe, m_uC_index);
        m_probe_order[m_uC_index].push_back(probe);
        m_probes[m_uC_index][probe] = 
            std::make_shared<dtrace::probe::profile_probe>(probe_type, probe);
    }
}

//-------------------------parser::expand_write_buffer-------------------------//
/**
 * expand_write_buffer() - Parsers and adds buffer values to the buffer map.
 *
 * @param write_buffer
 *  String containing buffer information and values to be added.
 *
 * This function parses the buffer from the write buffer string and adds them to 
 * the buffer map based on the buffer name.
 */
void
parser::
expand_write_buffer(const std::string& write_buffer)
{
    // Regex to extract buffer name and values
    boost::regex buffer_regex(R"(^(\w+)\[(0x[a-fA-F0-9]+|\d+)\]\s*=\s*\[(.*)\]\s*$)");
    boost::smatch buffer;
    if (!boost::regex_match(write_buffer, buffer, buffer_regex)) 
    {
        DTRACE_ERROR("DTRACE_PARSER_INVALID_WRITE_BUFFER",
            "Invalid write buffer format: " << write_buffer);
        return;
    }
    std::string buffer_name = buffer[1]; 
    size_t buffer_length = std::stoul(buffer[2], nullptr, 0);
    std::string buffer_values = buffer[3];

    // Add values to the buffer
    std::vector<uint32_t> buffer_addr = {
        static_cast<uint32_t>(
            (m_mem_host_addr_map[m_uC_index] >> dtrace::dtrace_ctrl::forth_byte_shift) 
            & dtrace::dtrace_ctrl::mask_32
        ),
        static_cast<uint32_t>(
            m_mem_host_addr_map[m_uC_index] & dtrace::dtrace_ctrl::mask_32
        )
    };
    m_buffer_map[buffer_name] = std::make_pair(buffer_addr, std::vector<uint32_t>());
    
    std::vector<uint32_t>& buffer_map_values = m_buffer_map.at(buffer_name).second;
    buffer_map_values.clear();
    uint32_t value;
    std::string item;
    std::istringstream value_stream(buffer_values);
    while (std::getline(value_stream, item, ',')) 
    {
        item = dtrace::action::action::strip(item);

        // Skip empty items
        if (item.empty())
            continue;
    
        value = std::stoul(item, nullptr, 0);
        buffer_map_values.push_back(value);
        buffer_length--;
    }

    // Validate the number of values added
    if (buffer_length != 0)
        DTRACE_ERROR("DTRACE_PARSER_WRITE_BUFFER_LENGTH_MISMATCH", "Buffer " << buffer_name);

    // Update the memory host address map
    m_mem_host_addr_map[m_uC_index] += static_cast<uint64_t>(
        (buffer_map_values.size()) * dtrace::dtrace_ctrl::word_byte_size
    );

    // Reset state
    m_write_buffer.clear();
    m_state = state_type::action;
    m_open_buffer = false;
}

//-------------------------parser::expand_init_buffer-------------------------//
/**
 * expand_init_buffer() - Parsers and adds buffer values to the buffer map.
 *
 * @param init_buffer
 *  String containing buffer information and values to be added.
 *
 * This function parses the buffer from the initialize buffer string and adds them to 
 * the buffer map based on the buffer name.
 */
void
parser::
expand_init_buffer(const std::string& init_buffer)
{
    // Regex to extract buffer name and length
    boost::regex buffer_init_regex(R"(^(\w+)\[(0x[a-fA-F0-9]+|\d+)\]\s*=\s*\{0\}\s*$)");
    boost::smatch buffer;
    if (!boost::regex_match(init_buffer, buffer, buffer_init_regex)) 
    {
        DTRACE_ERROR("DTRACE_PARSER_INVALID_INITIALIZE_BUFFER",
            "Invalid initialize buffer format: " << init_buffer);
        return;
    }

    std::string buffer_name = buffer[1];
    size_t buffer_length = std::stoul(buffer[2], nullptr, 0);
    if (buffer_length < 2)
        DTRACE_ERROR("DTRACE_PARSER_INVALID_BUFFER_LENGTH",
            "Invalid buffer length for buffer " << buffer_name << ": " << buffer_length);

    // Initialize the buffer with zeros
    std::vector<uint32_t> buffer_values(buffer_length, 0);
    // Set the memory host address for the buffer in the buffer map vector
    std::vector<uint32_t> buffer_addr = {
        static_cast<uint32_t>(
            (m_mem_host_addr_map[m_uC_index] >> dtrace::dtrace_ctrl::forth_byte_shift) 
            & dtrace::dtrace_ctrl::mask_32
        ),
        static_cast<uint32_t>(
            m_mem_host_addr_map[m_uC_index] & dtrace::dtrace_ctrl::mask_32
        )
    };
    m_buffer_map[buffer_name] = std::make_pair(buffer_addr, buffer_values);
        
    // Update the memory host address map
    m_mem_host_addr_map[m_uC_index] += static_cast<uint64_t>(
        buffer_length * dtrace::dtrace_ctrl::word_byte_size
    );
}

//-------------------------parser::probe_add_action-------------------------//
/**
 * probe_add_action() - Adds an action to a probe.
 *
 * @param probe_type
 * @param probe_name
 * @param action
 *
 * This function adds an action to a probe based on the provided probe type, probe name, 
 * and action string. If the probe name exists in the m_probe_expand map, 
 * it iterates through the associated probes and creates an action for each probe. 
 * Otherwise, it creates an action for the given probe name and uC index.
 */
void
parser::
probe_add_action(uint32_t probe_type, const std::string& probe_name, const std::string& action)
{
    if (m_probe_expand.find(probe_name) != m_probe_expand.end())
    {
        for (const auto& probe : m_probe_expand[probe_name])
        {
            auto action_ptr = create_action(action, probe_type, probe.first, probe.second);
            m_probes[probe.second][probe.first]->add_action(action_ptr);
        }
    }
    else
    {
        auto action_ptr = create_action(action, probe_type, probe_name, m_uC_index);
        m_probes[m_uC_index][probe_name]->add_action(action_ptr);
    }
}

//-------------------------parser::create_action-------------------------//
/**
 * create_action() - Creates an action based on the provided action string.
 *
 * @param action_string 
 *  String representing the action to be created.
 * @param probe_type
 * @param probe_name
 * @param uC_index 
 *  Index of the uC where the action is to be stored.
 * @return 
 *  shared pointer to the created action.
 * @throws std::runtime_error If the action string does not match any known action type.
 *
 * This function takes an action string, probe type, probe name, and uC index,
 * and creates a corresponding action object. The created action is then stored
 * in m_actions based on the action type.
 */
std::shared_ptr<dtrace::action::action>
parser::
create_action(const std::string& action_string, uint32_t probe_type, 
    const std::string& probe_name, uint32_t uC_index)
{
    std::shared_ptr<dtrace::action::action> action;
    if (m_state == state_type::operation_block)
    {   // Python operation action
        action = std::make_shared<dtrace::action::operation_action>(
            action_string, probe_type, probe_name
        );
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::read_reg_regex))
    {   // Read register action
        action = std::make_shared<dtrace::action::read_reg_action>(
            action_string, probe_type, probe_name
        );
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::mask_write_reg_regex))
    {   // Mask write register action
        action = std::make_shared<dtrace::action::mask_write_reg_action>(
            action_string, probe_type, probe_name, m_buffer_map
        );
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::write_reg_regex))
    {   // Write register action
        action = std::make_shared<dtrace::action::write_reg_action>(
            action_string, probe_type, probe_name
        );
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::timestamp_regex))
    {   // Timestamp action
        action = std::make_shared<dtrace::action::timestamp_action>(
            action_string, probe_type, probe_name
        );
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::timestamp32_regex))
    {   // Timestamp32 action
        action = std::make_shared<dtrace::action::timestamp32_action>(
            action_string, probe_type, probe_name
        );
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::profile_regex))
    {   // Profile action
        action = std::make_shared<dtrace::action::profile_action>(
            action_string, probe_type, probe_name
        );
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::print_regex))
    {   // Print action
        action = std::make_shared<dtrace::action::print_action>(
            action_string, probe_type, probe_name, m_maps
        );
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::printa_regex))
    {   // Profile print action
        action = std::make_shared<dtrace::action::printa_action>(
            action_string, probe_type, probe_name, m_maps
        );
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::read_mem_regex))
    {   // Read memory action
        action = std::make_shared<dtrace::action::read_mem_action>(
            action_string, probe_type, probe_name, m_mem_host_addr_map[uC_index]
        );
        m_mem_host_addr_map[uC_index] = action->get_mem_host_addr();
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::write_mem_regex))
    {   // Write memory action
        action = std::make_shared<dtrace::action::write_mem_action>(
            action_string, probe_type, probe_name, m_buffer_map
        );
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::break_regex))
    {   // Break action
        action = std::make_shared<dtrace::action::break_action>(
            action_string, probe_type, probe_name
        );
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::timestamps_regex))
    {   // Timestamps action
        action = std::make_shared<dtrace::action::timestamps_action>(
            action_string, probe_type, probe_name
        );
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::timestamps32_regex))
    {   // Timestamps32 action
        action = std::make_shared<dtrace::action::timestamps32_action>(
            action_string, probe_type, probe_name
        );
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::read_handshake_regex))
    {   // Read handshake action
        action = std::make_shared<dtrace::action::read_handshake_action>(
            action_string, probe_type, probe_name
        );
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::write_handshake_regex))
    {   // Write handshake action
        action = std::make_shared<dtrace::action::write_handshake_action>(
            action_string, probe_type, probe_name
        );
    }
    else if (boost::regex_search(action_string, dtrace::action::action_name::operation_regex))
    {   // Operation action
        action = std::make_shared<dtrace::action::operation_action>(
            action_string, probe_type, probe_name
        );
    }
    else
    {
        DTRACE_ERROR("DTRACE_PARSER_INVALID_ACTION_LINE", 
            "Invalid action line: '" << action_string << "' at position " << m_position);
    }
    return action;
}

//-------------------------parser::parse_line-------------------------//
/**
 * parse_line() - 
 *  Parses input script file line and updates the parser state accordingly.
 *
 * @param parse_line 
 *  Input script file line to be parsed.
 *
 * This function processes a input script file line and determines its type based on
 * predefined regular expressions for probe and action. It updates the parser 
 * state and parser data structures based on the type of line. 
 */
void
parser::
parse_line(const std::string& parse_line)
{
    std::string line = dtrace::action::action::strip(parse_line);
    m_position++;

    // Log the line being parsed
    DTRACE_INFO("DTRACE_PARSER_LINE " << m_position << ": " << line);

    // Skip empty lines
    if (line.empty())
        return;

    // Start operation block
    if (line == "@blockopen") {
        m_state = state_type::operation_block;
        return;
    }

    // End operation block
    if (line == "@blockclose") {
        m_state = state_type::action;
        return;
    }

    if (m_state == state_type::operation_block) {
        // Create operation action for each line
        probe_add_action(m_probe_type, m_probe_name, parse_line);
        return;
    }

    // Skip comments
    boost::regex comment_regex(R"(^#(.*)$)");
    if (boost::regex_match(line, comment_regex))
        return;

    // Parse the line for begin probe
    boost::regex begin_regex(R"(^begin\s*\{?$)");
    if (boost::regex_match(line, begin_regex))
    {
        if (m_state != state_type::probe || m_begin_exist)
            DTRACE_ERROR("DTRACE_PARSER_INVALID_LINE", "Invalid line " << m_position << ": " << line);
    
        m_uC_index = 0;
        m_begin_exist = true;
        m_state = state_type::action_open;
        m_probe_type = dtrace::probe::probe_type::begin;
        m_probe_name = line;
        m_probe_order[m_uC_index].push_back(m_probe_name);
        m_probes[m_uC_index][m_probe_name] = 
            std::make_shared<dtrace::probe::begin_probe>(m_probe_type, m_probe_name);
        m_uC_indices.insert(m_uC_index);
        m_mem_host_addr_map.insert({m_uC_index, 0});
        if (line.back() == '{')
        {
            m_state = state_type::action;
            m_open = true;
        }
        return;
    }

    // Parse the line for end probe
    boost::regex end_regex(R"(^end\s*\{?$)");
    if (boost::regex_match(line, end_regex))
    {
        if (m_state != state_type::probe || m_end_exist)
            DTRACE_ERROR("DTRACE_PARSER_INVALID_LINE", "Invalid line " << m_position << ": " << line);
        
        m_uC_index = 0;
        m_end_exist = true;
        m_state = state_type::action_open;
        m_probe_type = dtrace::probe::probe_type::end;
        m_probe_name = line;
        m_probe_order[m_uC_index].push_back(m_probe_name);
        m_probes[m_uC_index][m_probe_name] = 
            std::make_shared<dtrace::probe::end_probe>(m_probe_type, m_probe_name);
        m_uC_indices.insert(m_uC_index);
        m_mem_host_addr_map.insert({m_uC_index, 0});
        if (line.back() == '{')
        {
            m_state = state_type::action;
            m_open = true;
        }
        return;
    }

    // Parse the line for probe open
    if (line == "{")
    {
        if (m_state != state_type::action_open)
            DTRACE_ERROR("DTRACE_PARSER_INVALID_LINE", "Invalid line " << m_position << ": " << line);
        
        m_state = state_type::action;
        m_open = true;
        return;
    }

    // Parse the line for probe close
    if (line == "}")
    {
        if (!m_open || m_state != state_type::action)
            DTRACE_ERROR("DTRACE_PARSER_INVALID_LINE", "Invalid line " << m_position << ": " << line);
        
        m_state = state_type::probe;
        m_open = false;
        return;
    }

    // Parse the line for jprobe
    boost::regex jprobe_regex(R"(^jprobe:([a-zA-Z0-9_\.\/]*)\:(uc[0-9,-]+)\:((line|annotation)[0-9,-]+)\s*\{?$)");
    if (boost::regex_match(line, jprobe_regex))
    {
        if (m_state != state_type::probe)
            DTRACE_ERROR("DTRACE_PARSER_INVALID_LINE", "Invalid line " << m_position << ": " << line);
        
        m_state = state_type::action_open;
        m_probe_type = dtrace::probe::probe_type::jprobe;
        m_probe_name = line;
        expand_jprobe(m_probe_type, m_probe_name);
        if (line.back() == '{')
        {
            m_state = state_type::action;
            m_open = true;
        }
        return;
    }
 
    // Parse the line for tracepoint probe
    boost::regex tracepoint_regex(R"(^tracepoint:(uc[0-9,-]+)\:(id[0-9,-]+)\s*\{?$)");
    if (boost::regex_match(line, tracepoint_regex))
    {
        if (m_state != state_type::probe)
            DTRACE_ERROR("DTRACE_PARSER_INVALID_LINE", "Invalid line " << m_position << ": " << line);
        
        m_state = state_type::action_open;
        m_probe_type = dtrace::probe::probe_type::tracepoint;
        m_probe_name = line;
        expand_tracepoint(m_probe_type, m_probe_name);
        if (line.back() == '{')
        {
            m_state = state_type::action;
            m_open = true;
        }
        return;
    }

    // Parse the line for profile probe
    boost::regex profile_regex(R"(^profile:(uc[0-9,-]+)\:([0-9]+)hz\s*\{?$)");
    if (boost::regex_match(line, profile_regex))
    {
        if (m_state != state_type::probe)
            DTRACE_ERROR("DTRACE_PARSER_INVALID_LINE", "Invalid line " << m_position << ": " << line);
        
        m_state = state_type::action_open;
        m_probe_type = dtrace::probe::probe_type::profile;
        m_probe_name = line;
        expand_profile(m_probe_type, m_probe_name);
        if (line.back() == '{')
        {
            m_state = state_type::action;
            m_open = true;
        }
        return;
    }

    // Parse the line for action and operation
    boost::regex action_regex(R"(^(.+\(.*\).*\)?)$)");
    boost::regex operation_regex(R"(^(\w+)\s*=\s*(.+)$)");
    if ((boost::regex_match(line, action_regex) || boost::regex_match(line, operation_regex))
        && m_state == state_type::action)
    {
        m_state = state_type::action;
        std::istringstream line_stream(line);
        std::string token;
        while (std::getline(line_stream, token, ';'))
        {
            std::string item = dtrace::action::action::strip(token);
            if (!item.empty())
                probe_add_action(m_probe_type, m_probe_name, item);
        }
        return;
    }

    // Parse the line for buffer initialization
    boost::regex buffer_init_regex(R"(^(\w+)\[(0x[a-fA-F0-9]+|\d+)\]\s*=\s*\{0\}\s*$)");
    if (boost::regex_match(line, buffer_init_regex) && m_state == state_type::action)
    {
        expand_init_buffer(line);
        return;
    }

    // Parse the line for write buffer for memory action
    boost::regex buffer_open_regex(R"(^(\w+\[(0x[a-fA-F0-9]+|\d+)\])(?:\s*=\s*(\[.*)?)?$)");
    boost::regex buffer_regex(R"(^(\w+)\[(0x[a-fA-F0-9]+|\d+)\]\s*=\s*\[(.*)\]\s*$)");
    if (boost::regex_match(line, buffer_open_regex) && m_state == state_type::action)
    {  
        m_state = state_type::buffer_open;
        m_open_buffer = true;
        m_write_buffer += line;
        // single line buffer
        if (boost::regex_match(m_write_buffer, buffer_regex))
            expand_write_buffer(m_write_buffer);
            
        return;
    }

    // Parse the line for open buffer
    if (m_open_buffer && m_state == state_type::buffer_open)
    {
        m_write_buffer += line;
        // multi line write buffer ends, expand buffer
        if (line.back() == ']')
            expand_write_buffer(m_write_buffer);

        return;
    }
    DTRACE_ERROR("DTRACE_PARSER_INVALID_LINE", "Invalid line " << m_position << ": " << line);
}

} // namespace dtrace::parser

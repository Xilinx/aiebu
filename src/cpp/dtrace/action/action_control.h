// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef ACTION_CONTROL_H
#define ACTION_CONTROL_H

// This file contains the declaration of the action control class and action classes.
#include "dtrace/utils.h"
#ifdef CERT_TRACE_CONTROL_H
#include "trace_control.h"
#endif

#include <boost/property_tree/ptree.hpp>
#include "common/regex_wrapper.h"

#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace dtrace::action
{
//-------------------------Action Types-------------------------//
/**
 * @class action_type
 *
 * @brief
 * dtrace::action::action_type defines action types with corresponding integer values.
 *
 * @details
 * This class provides a set of static constexpr uint32_t constants representing
 * different action types. The values of these constants depend on whether the
 * CERT_TRACE_CONTROL_H macro is defined.
 *
 * When CERT_TRACE_CONTROL_H is defined, the constants are assigned values from
 * corresponding ACTION_* macros from CERT/api/trace_control.h. 
 * Otherwise, they are assigned default integer values.
 *
 * action types-
 * - reg_read:       Register read action.
 * - reg_write:      Register write action.
 * - timestamp:      Timestamp action.
 * - profile:        Profile action.
 * - print:          Print action.
 * - printa:         Print action for profile probe.
 * - timestamp32:    Timestamp action 32-bit.
 * - mem_read:       Memory read action.
 * - mem_write:      Memory write action.
 * - break_action:   Break action.
 * - timestamps:     Multiple Timestamp action.
 * - timestamps32:   Multiple Timestamps32 action.
 * - reg_mask_write: Register mask write action.
 * - handshake_read: Handshake region read action.
 * - handshake_write:Handshake region write action.
 */
class action_type
{
public:
#ifdef CERT_TRACE_CONTROL_H
    static constexpr uint32_t reg_read = ACTION_REG_READ;   
    static constexpr uint32_t reg_write = ACTION_REG_WRITE;
    static constexpr uint32_t timestamp = ACTION_TIMESTAMP;
    static constexpr uint32_t profile = ACTION_PROFILE;
    static constexpr uint32_t print = ACTION_PRINT;
    static constexpr uint32_t printa = ACTION_PRINTA;
    static constexpr uint32_t timestamp = ACTION_TIMESTAMP32;
    static constexpr uint32_t mem_read = ACTION_MEM_READ;
    static constexpr uint32_t mem_write = ACTION_MEM_WRITE;
    static constexpr uint32_t break_action = ACTION_BREAK;
    static constexpr uint32_t timestamps = ACTION_TIMESTAMPS;
    static constexpr uint32_t timestamps32 = ACTION_TIMESTAMPS32;
    static constexpr uint32_t reg_mask_write = ACTION_REG_MASK_WRITE;
    static constexpr uint32_t handshake_read = ACTION_HS_READ;
    static constexpr uint32_t handshake_write = ACTION_HS_WRITE;
#else
    static constexpr uint32_t reg_read = 0;
    static constexpr uint32_t reg_write = 1;
    static constexpr uint32_t timestamp = 2;
    static constexpr uint32_t profile = 3;
    static constexpr uint32_t print = 4;
    static constexpr uint32_t printa = 5;
    static constexpr uint32_t timestamp32 = 6;
    static constexpr uint32_t mem_read = 7;
    static constexpr uint32_t mem_write = 8;
    static constexpr uint32_t break_action = 9;
    static constexpr uint32_t timestamps = 10;
    static constexpr uint32_t timestamps32 = 11;
    static constexpr uint32_t reg_mask_write = 12;
    static constexpr uint32_t handshake_read = 13;
    static constexpr uint32_t handshake_write = 14;
#endif
};

//-------------------------Action Names-------------------------//
/**
 * @class action_name
 *
 * @brief
 * dtrace::action::action_name contains static regular expressions for action patterns.
 *
 * @details
 * This class provides a set of static inline regular expressions that can be used
 * to match specific action patterns in script file.
 *
 * @note All regex patterns are hardcoded and guaranteed valid at compile time.
 * NOLINT comments suppress warnings about potential exceptions during static initialization.
 */
class action_name
{
public:
    static inline const aiebu::regex timestamp_regex = aiebu::regex(R"(timestamp\()");              // NOLINT
    static inline const aiebu::regex timestamp32_regex = aiebu::regex(R"(timestamp32\()");          // NOLINT
    static inline const aiebu::regex read_reg_regex = aiebu::regex(R"(read_reg\()");                // NOLINT
    static inline const aiebu::regex write_reg_regex = aiebu::regex(R"(\bwrite_reg\()");            // NOLINT
    static inline const aiebu::regex mask_write_reg_regex = aiebu::regex(R"(\bmask_write_reg\()");  // NOLINT
    static inline const aiebu::regex profile_regex = aiebu::regex(R"(opcode\(\))");                 // NOLINT
    static inline const aiebu::regex print_regex = aiebu::regex(R"(print\()");                      // NOLINT
    static inline const aiebu::regex printa_regex = aiebu::regex(R"(printa\()");                    // NOLINT
    static inline const aiebu::regex read_mem_regex = aiebu::regex(R"(read_mem\()");                // NOLINT
    static inline const aiebu::regex write_mem_regex = aiebu::regex(R"(write_mem\()");              // NOLINT
    static inline const aiebu::regex break_regex = aiebu::regex(R"(break\()");                      // NOLINT
    static inline const aiebu::regex timestamps_regex = aiebu::regex(R"(timestamps\()");            // NOLINT
    static inline const aiebu::regex timestamps32_regex = aiebu::regex(R"(timestamps32\()");        // NOLINT
    static inline const aiebu::regex read_handshake_regex = aiebu::regex(R"(read_handshake\()");    // NOLINT
    static inline const aiebu::regex write_handshake_regex = aiebu::regex(R"(write_handshake\()");  // NOLINT
    static inline const aiebu::regex operation_regex = aiebu::regex(R"(^(\w+)\s*=\s*(.+)$)");       // NOLINT
    static inline const aiebu::regex action_regex = aiebu::regex(R"((\w+)\((.*)\))");               // NOLINT
};

//-------------------------Action Control-------------------------//
/**
 * @class action_ctrl
 *
 * @brief
 * dtrace::action::action_ctrl defines constants used to create control block and action control.
 *
 * @details
 * The class provides a set of static constexpr values that are used for different
 * purposes such as page size, byte shifts, action sizes, and  masks in dtrace compiler.
 * The trace_page_size is conditionally defined based on the CERT_TRACE_CONTROL_H macro.
 */
class action_ctrl
{
public:
    static constexpr uint32_t break_action_size = 1;                                        // Size of a break action
    static constexpr uint32_t timestamp32_action_size = 2;                                  // Size of a 32-bit timestamp action
    static constexpr uint32_t timestamps_action_size = 2;                                   // Size of a multiple timestamp action
    static constexpr uint32_t timestamps_value = 2;                                         // Low and high value for timestamp
    static constexpr uint32_t reg_rw_action_size = 3;                                       // Size of a register read/write action
    static constexpr uint32_t reg_mask_w_action_size = 4;                                   // Size of a register read/write action
    static constexpr uint32_t mem_rw_action_size = 5;                                       // Size of a memory read/write action
};

//-------------------------Action class-------------------------//
/**
 * @class action
 *
 * @brief
 * dtrace::action::action defines base class representing an action with various 
 * attributes and methods to manipulate and serialize it.
 *
 * @details
 * This class provides a framework for defining actions with specific probe types, 
 * names, and arguments. It includes methods for creating string representations, 
 * stripping tokens, and serializing results. Derived classes must implement the 
 * actionize and serialize methods.
 */
class action
{
protected:
    uint32_t m_probe_type;
    std::string m_probe_name;
    std::string m_action_name;
    std::vector<std::string> m_arguments;
    uint32_t m_control_location;
    uint32_t m_mem_location;
    std::string m_result;
    void set_location(const std::vector<uint32_t>& buffer, bool is_mem_buffer);

public:
    action(uint32_t probe_type, std::string probe_name);
    std::string create_string() const;
    static std::string strip(const std::string& token);
    virtual ~action() = default;
    virtual void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) = 0;
    virtual std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const = 0;
    uint32_t get_location(bool is_mem_buffer) const;
    virtual uint64_t get_mem_host_addr() const { return 0; }
};

//-------------------------Read register-------------------------//
/**
 * @class read_reg_action
 *
 * @brief
 * dtrace::action::read_reg_action represents an action to read a register.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * for read register action in the control block and serialize the result.
 */
class read_reg_action : public action
{
public:
    read_reg_action(std::string token, uint32_t probe_type, const std::string& probe_name);
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
};

//-------------------------Write register-------------------------//
/**
 * @class write_reg_action
 *
 * @brief
 * dtrace::action::write_reg_action represents an action to write a register.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * for write register action in the control block and serialize the result.
 */
class write_reg_action : public action
{
public:
    write_reg_action(std::string token, uint32_t probe_type, const std::string& probe_name);
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
};

//-------------------------Write mask register-------------------------//
/**
 * @class mask_write_reg_action
 *
 * @brief
 * dtrace::action::mask_write_reg_action represents an action to mask write a register.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * for mask write register action in the control block and serialize the result.
 */
class mask_write_reg_action : public action
{
private:
    // Value argument 0 = Normal, 1 = HIGH, 2 = LOW
    uint32_t m_mode;
    std::vector<uint32_t> m_write_buffer_values;

public:
    mask_write_reg_action(
        std::string token, uint32_t probe_type, const std::string& probe_name,
        const std::unordered_map<std::string, std::pair<std::vector<uint32_t>, std::vector<uint32_t>>>& buffer_map
    );
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
    uint32_t get_mode() const { return m_mode; }
};

//-------------------------Timestamp-------------------------//
/**
 * @class timestamp_action
 *
 * @brief
 * dtrace::action::timestamp_action represents an action for timestamp.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * for timestamp action in the control block and serialize the result.
 */
class timestamp_action : public action
{
public:
    timestamp_action(std::string token, uint32_t probe_type, const std::string& probe_name);
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
};

//-------------------------Timestamp32-------------------------//
/**
 * @class timestamp32_action
 *
 * @brief
 * dtrace::action::timestamp32_action represents an action for 32-bit timestamp.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * for 32-bit timestamp action in the control block and serialize the result.
 */
class timestamp32_action : public action
{
public:
    timestamp32_action(std::string token, uint32_t probe_type, const std::string& probe_name);
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
};

//-------------------------Print-------------------------//
/**
 * @class print_action
 *
 * @brief
 * dtrace::action::print_action represents an action for print result.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * for print action in the control block and serialize the result.
 */
class print_action : public action
{
private:
    std::map<std::string, std::string> m_built_ins;
    std::string m_token;

public:
    print_action(
        std::string token, uint32_t probe_type, const std::string& probe_name, 
        const std::unordered_map<std::string, boost::property_tree::ptree>& maps
    );
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
};

//-------------------------Printa-------------------------//
/**
 * @class printa_action
 *
 * @brief
 * dtrace::action::printa_action represents an action for print profile probe result.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * for profile probe print action in the control block and serialize the result.
 */
class printa_action : public action
{
private:
    std::unordered_map<std::string, boost::property_tree::ptree> m_maps;

public:
    printa_action(
        std::string token, uint32_t probe_type, const std::string& probe_name, 
        std::unordered_map<std::string, boost::property_tree::ptree> maps
    );
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::pair<std::string, uint32_t> get_opcode(const uint32_t& value)  const;
    std::string format(const std::vector<uint32_t>& result_buffer) const;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
};

//-------------------------Profile-------------------------//
/**
 * @class profile_action
 *
 * @brief
 * dtrace::action::profile_action represents an action for profiling.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * for profile action in the control block and serialize the result.
 */
class profile_action : public action
{
private:
    std::string m_token;
    
public:
    profile_action(std::string token, uint32_t probe_type, const std::string& probe_name);
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
};

//-------------------------Read memory-------------------------//
/**
 * @class read_mem_action
 *
 * @brief
 * dtrace::action::read_mem_action represents an action to read a block of memory.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * for read memory action in the control block and serialize the result.
 */
class read_mem_action : public action
{
private:
    uint32_t m_length;
    uint64_t m_mem_host_addr;

public:
    read_mem_action(
        std::string token, uint32_t probe_type, const std::string& probe_name, 
        uint64_t mem_host_addr
    );
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
    uint64_t get_mem_host_addr() const override;
};

//-------------------------Write memory-------------------------//
/**
 * @class write_mem_action
 *
 * @brief
 * dtrace::action::write_mem_action represents an action to write a block of memory.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * for write memory action in the control block and serialize the result.
 */
class write_mem_action : public action
{
private:
    uint32_t m_length;
    std::vector<uint32_t> m_write_buffer_addr;
    std::vector<uint32_t> m_write_buffer_values;

public:
    write_mem_action(
        std::string token, uint32_t probe_type, const std::string& probe_name,
        const std::unordered_map<std::string, std::pair<std::vector<uint32_t>, std::vector<uint32_t>>>& buffer_map
    );
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
};

//-------------------------Break-------------------------//
/**
 * @class break_action
 *
 * @brief
 * dtrace::action::break_action represents an action to break the execution in aie debugger.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * for break action in the control block and serialize the result.
 */
class break_action : public action
{
private:
    std::string m_token;
    
public:
    break_action(std::string token, uint32_t probe_type, const std::string& probe_name);
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
};

//-------------------------Timestamps-------------------------//
/**
 * @class timestamps_action
 *
 * @brief
 * dtrace::action::timestamps_action represents an action for multiple timestamp.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * formultiple  timestamp action in the control block and serialize the result.
 */
class timestamps_action : public action
{
private:
    uint32_t m_length;
    
public:
    timestamps_action(std::string token, uint32_t probe_type, const std::string& probe_name);
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
};

//-------------------------Timestamps32-------------------------//
/**
 * @class timestamps32_action
 *
 * @brief
 * dtrace::action::timestamps32_action represents an action for 32-bit multiple timestamp.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * for 32-bit multiple timestamp action in the control block and serialize the result.
 */
class timestamps32_action : public action
{
private:
    uint32_t m_length;

public:
    timestamps32_action(std::string token, uint32_t probe_type, const std::string& probe_name);
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
};

//-------------------------Operation-------------------------//
/**
 * @class operation_action
 *
 * @brief
 * dtrace::action::operation_action represents an action for print result.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * for operations in the control block and serialize the result.
 */
class operation_action : public action
{
private:
    std::string m_token;

public:
    operation_action(std::string token, uint32_t probe_type, const std::string& probe_name);
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
};

//-------------------------Read handshake-------------------------//
/**
 * @class read_handshake_action
 *
 * @brief
 * dtrace::action::read_handshake_action represents an action to read handshake register.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * for read handshake action in the control block and serialize the result.
 */
class read_handshake_action : public action
{
public:
    read_handshake_action(std::string token, uint32_t probe_type, const std::string& probe_name);
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
};

//-------------------------Write handshake-------------------------//
/**
 * @class write_handshake_action
 *
 * @brief
 * dtrace::action::write_handshake_action represents an action to write handshake register.
 *
 * @details
 * This class inherits from the base class `action` and provides functionality
 * for write handshake action in the control block and serialize the result.
 */
class write_handshake_action : public action
{
public:
    write_handshake_action(std::string token, uint32_t probe_type, const std::string& probe_name);
    void actionize(
        uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer
    ) override;
    std::string serialize(
        const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
        const std::unordered_map<uint32_t, uint32_t>& mapping
    ) const override;
};

} // namespace dtrace::action
#endif // ACTION_CONTROL_H

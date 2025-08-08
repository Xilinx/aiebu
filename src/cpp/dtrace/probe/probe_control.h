// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef PROBE_CONTROL_H
#define PROBE_CONTROL_H

// This file contains the declaration of the probe control class and probe classes
#include "dtrace/action/action_control.h"
#include "dtrace/utils.h"
#ifdef CERT_TRACE_CONTROL_H
#include "trace_control.h"
#endif

#include <boost/property_tree/ptree.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace dtrace::probe
{

//-------------------------Probe Types-------------------------//
/**
 * @class probe_type
 *
 * @brief
 * dtrace::probe::probe_type defines types of probes used for tracing and profiling.
 *
 * @details
 * The probe_type class provides static constants representing different types of probes.
 * Depending on whether CERT_TRACE_CONTROL_H is defined, the values of these constants
 * will either be set to predefined macros or default integer values.
 *
 * When CERT_TRACE_CONTROL_H is defined, the constants from CERT/api/trace_control.h 
 * are set to the values of the corresponding macros 
 * (PROBE_BEGIN, PROBE_END, PROBE_JPROBE, PROBE_TRACEPOINT, PROBE_PROFILING).
 *
 * probe types-
 * - begin:         Represents the begin probe.
 * - end:           Represents the end probe.
 * - jprobe:        Represents a job probe.
 * - tracepoint:    Represents a tracepoint probe.
 * - profile:       Represents a profiling probe.
 */
class probe_type
{
public:
#ifdef CERT_TRACE_CONTROL_H
    static constexpr uint32_t begin = PROBE_BEGIN;
    static constexpr uint32_t end = PROBE_END;
    static constexpr uint32_t jprobe = PROBE_JPROBE;
    static constexpr uint32_t tracepoint = PROBE_TRACEPOINT;
    static constexpr uint32_t profile = PROBE_PROFILING;
#else
    static constexpr uint32_t begin = 0;
    static constexpr uint32_t end = 1;
    static constexpr uint32_t jprobe = 2;
    static constexpr uint32_t tracepoint = 3;
    static constexpr uint32_t profile = 4;
#endif
};

//-------------------------Probe Control-------------------------//
/**
 * @class probe_ctrl
 *
 * @brief
 * dtrace::action::probe_ctrl provides control mechanisms for probe operations.
 *
 * @details
 * The probe_ctrl class contains a static constant `link_end` which is used to  
 * signify the end of probe in control block and `tracepoint_split` which is used to 
 * determine the threshold value for tracepoint probes.
 *
 * The value of constants is determined by the presence of the `CERT_TRACE_CONTROL_H` macro.
 */
class probe_ctrl
{
public:
#ifdef CERT_TRACE_CONTROL_H
    static constexpr uint16_t link_end = LINK_END;
    static constexpr uint16_t tracepoint_split = TP_SPLIT;
#else
    static constexpr uint16_t link_end = 0xFFFF;
    static constexpr uint16_t tracepoint_split = 12;
#endif
    static constexpr uint16_t probe_type = 0xFF00;
};

//-------------------------Probe Class-------------------------//
/**
 * @class probe
 *
 * @brief
 * dtrace::action::probe represent a probe with specific actions and controls.
 *
 * @details
 * This class provides an interface for creating and managing probes, which
 * include various actions and controls. It is designed to be inherited by
 * other classes that implement specific probe functionalities.
 */
class probe
{
protected:
    uint32_t m_probe_type;
    std::string m_probe_name;
    std::vector<std::shared_ptr<dtrace::action::action>> m_control_actions;
    void filter_action();
    std::vector<uint32_t> actionize(std::vector<uint32_t>& control_buffer, 
        std::vector<uint32_t>& mem_buffer);
    
public:
    probe(uint32_t probe_type, std::string probe_name);
    std::vector<std::shared_ptr<dtrace::action::action>> m_actions;
    void add_action(std::shared_ptr<dtrace::action::action> action);
    std::string create_string() const;
    virtual std::vector<uint32_t> enable(std::vector<uint32_t>& control_buffer, 
        std::vector<uint32_t>& mem_buffer) = 0;
    virtual ~probe() = default;
};

//-------------------------Begin probe-------------------------//
/**
 * @class begin_probe
 *
 * @brief
 * dtrace::action::begin_probe represents begin probe.
 *
 * @details
 * This class inherits from the `probe` class and is used to enable begin probe
 * in the control block. It fires after partition is created
 */
class begin_probe : public probe
{
public:
    begin_probe(uint32_t probe_type, const std::string& probe_name);
    std::vector<uint32_t> enable(std::vector<uint32_t>& control_buffer, 
        std::vector<uint32_t>& mem_buffer) override;
};

//-------------------------End probe-------------------------//
/**
 * @class end_probe
 *
 * @brief
 * dtrace::action::end_probe represents end probe.
 *
 * @details
 * This class inherits from the `probe` class and is used to enable end probe
 * in the control block. It fires after the application completes
 */
class end_probe : public probe
{
public:
    end_probe(uint32_t probe_type, const std::string& probe_name);
    std::vector<uint32_t> enable(std::vector<uint32_t>& control_buffer, 
        std::vector<uint32_t>& mem_buffer) override;
};

//-------------------------Jprobe-------------------------//
/**
 * @class jprobe
 *
 * @brief
 * dtrace::action::jprobe represents dynamic job probe.
 *
 * @details
 * This class inherits from the `probe` class and is used to enable job probe
 * in the control block and handling probe fields and maps. Once it is placed at
 * location within control code on an opcode, the probe fires when the opcode is executed
 */
class jprobe : public probe
{
private:
    std::vector<std::string> m_probe_fields;
    uint32_t m_page;
    uint32_t m_offset;

public:
    jprobe(uint32_t probe_type, const std::string& probe_name, 
        uint32_t probe_page, uint32_t probe_offset);
    std::vector<uint32_t> enable(std::vector<uint32_t>& control_buffer, 
        std::vector<uint32_t>& mem_buffer) override;
};

//-------------------------Tracepoint probe-------------------------//
/**
 * @class tracepoint_probe
 *
 * @brief
 * dtrace::action::tracepoint_probe represents tracepoint probe.
 *
 * @details
 * This class inherits from the `probe` class and is used to enable tracepoint probe
 * in the control block. It is static probe in cert firmware at predefined location. 
 * Once placed, the probe fires when cert firmware runs to that location
 */
class tracepoint_probe : public probe
{
private:
    uint32_t m_uC;
    uint32_t m_index;

public:
    tracepoint_probe(uint32_t probe_type, const std::string& probe_name);
    std::vector<uint32_t> enable(std::vector<uint32_t>& control_buffer, 
        std::vector<uint32_t>& mem_buffer) override;
};

//-------------------------Profile probe-------------------------//
/**
 * @class profile_probe
 *
 * @brief
 * dtrace::action::profile_probe represents profile probe.
 *
 * @details
 * This class inherits from the `probe` class and is used to enable profile probe
 * in the control block and handle profiling.
 */
class profile_probe : public probe
{
private:
    uint32_t m_uC;
    uint32_t m_frequency;

public:
    profile_probe(uint32_t probe_type, const std::string& probe_name);
    std::vector<uint32_t> enable(std::vector<uint32_t>& control_buffer, 
        std::vector<uint32_t>& mem_buffer) override;
};

} // namespace dtrace::probe
#endif // PROBE_CONTROL_H

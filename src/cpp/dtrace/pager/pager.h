// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef PAGER_H
#define PAGER_H

// This file contains the declaration of the pager class, which is responsible 
// for managing paging operations on control buffers.
#include "dtrace/probe/probe_control.h"
#include "dtrace/action/action_control.h"
#include "dtrace/utils.h"

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace dtrace::pager
{

//-------------------------Pager Ctrl-------------------------//
/**
 * @class pager_ctrl
 *
 * @brief
 * dtrace::pager::pager_ctrl provides control masks and thresholds for pager operations
 *
 * @details
 * The pager_ctrl class defines several static constants used for masking and 
 * controlling various aspects of pager operations.
 */
class pager_ctrl
{   
public:
    static constexpr uint32_t mask_last_page_flag = 0xFFFFFFFE; // Mask to identify the last page flag.
    static constexpr uint32_t mask_probe_type = 0xFF;           // Mask to identify the probe type.
    static constexpr uint32_t mask_probe_count = 0x7F;          // Mask to identify the probe count.
    static constexpr uint32_t mask_probe_ctrl = 0xFFFF;         // Mask to control probe operations.
};

//-------------------------Pager-------------------------//
/**
 * @class pager
 *
 * @brief
 * dtrace::pager::pager class is responsible for managing paging operations on buffers.
 *
 * @details
 * This class provides functionalities to handle paging of data buffers, including
 * expanding pages, resetting state, and managing action location mappings.
 */
class pager
{
public:
    pager(bool restricted_order = false);
    std::vector<uint32_t> m_primary_buffer;
    std::vector<uint32_t> m_secondary_buffer;
    std::unordered_map<uint32_t, std::unordered_map<uint32_t, uint32_t>> m_primary_action_location_mapping;
    std::unordered_map<uint32_t, uint32_t> m_secondary_action_location_mapping;
    std::vector<uint32_t> paging(const std::vector<uint32_t>& buffer, uint32_t uC_index);
    const std::unordered_map<uint32_t, uint32_t>& get_action_location_mapping(uint32_t uC_index) const;

private:
    uint32_t m_page_size;
    uint32_t m_page_index;
    bool m_probe_in_order;
    uint32_t m_probes_in_page;
    void reset_state();
    bool expand_page(std::vector<uint32_t>& buffer, uint32_t size, bool force = false);
    void clear_last_page_flag(std::vector<uint32_t>& buffer);
    void update_page_header(std::vector<uint32_t>& buffer);
    void add_page_header(std::vector<uint32_t>& buffer);
    void get_begin_probe(const std::vector<uint32_t>& source, 
        std::vector<uint32_t>& destination, uint32_t uC_index);
    void get_end_probe(const std::vector<uint32_t>& source, 
        std::vector<uint32_t>& destination, uint32_t uC_index);
    void get_half_tracepoint_probe(const std::vector<uint32_t>& source, 
        std::vector<uint32_t>& destination, bool is_first_half, uint32_t uC_index);
    void get_jprobe_probe(const std::vector<uint32_t>& source, 
        std::vector<uint32_t>& destination, uint32_t uC_index);
    uint32_t get_action_size(const std::vector<uint32_t>& source, uint32_t action_offset);
    void copy_actions(const std::vector<uint32_t>& source, 
        uint32_t offset, std::vector<uint32_t>& destination, uint32_t size, uint32_t uC_index);
};

} // namespace dtrace::pager
#endif // PAGER_H

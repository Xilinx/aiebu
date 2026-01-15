// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

// This file contains the implementation of the pager class, which is responsible 
// for managing paging operations on control buffers.
#include "dtrace/pager/pager.h"
#include <stdexcept>

namespace dtrace::pager
{

//-------------------------Pager Constructor-------------------------//
/**
 * pager() - Constructor with restricted order.
 */
pager::
pager(bool restricted_order)
    : m_page_size(dtrace::dtrace_ctrl::trace_page_size >> 2)
    , m_page_index(0)
    , m_probe_in_order(restricted_order)
    , m_probes_in_page(0)
{
}

//-------------------------pager::reset_state-------------------------//
/**
 * reset_state() - Resets the state of the pager.
 * 
 * This function clears the primary and secondary buffers, 
 * clears the secondary action location mapping, and resets 
 * the page index and the number of probes in the current page 
 * to their initial values.
 */
void 
pager::
reset_state()
{
    m_primary_buffer.clear();
    m_secondary_buffer.clear();
    m_secondary_action_location_mapping.clear();
    m_page_index = 0;
    m_probes_in_page = 0;
}

//-------------------------pager::paging-------------------------//
/**
 * paging() Processes a control block buffer and generates a paged output.
 *
 * @param buffer 
 *  Control block buffer to be processed.
 * @param uC_index 
 *  Index of the uC to be used while paging.
 * @return 
 *  Vector (uint32_t) containing the processed paged control block buffer output.
 *
 * This function takes an input control block buffer and a uC index, processes 
 * the buffer, and returns a vector containing the paged control block buffer output.
 */
std::vector<uint32_t> 
pager::
paging(const std::vector<uint32_t>& buffer, uint32_t uC_index)
{
    try
    {
        // Check if the buffer is a profile buffer, single page support for profiling.
        if (((buffer[0] & dtrace::probe::probe_ctrl::probe_type) >> 
            dtrace::dtrace_ctrl::first_byte_shift) == 
            dtrace::probe::probe_type::profile)
        {
            m_primary_action_location_mapping[uC_index] = {};
            return buffer;
        }

        // Reset the state of the pager for each uC.
        reset_state();
        // Add the page header to the primary and secondary buffers.
        add_page_header(m_primary_buffer);
        add_page_header(m_secondary_buffer);
        // Add the begin probe to the primary buffer and the end probe to the secondary buffer.
        get_begin_probe(buffer, m_primary_buffer, uC_index);
        get_end_probe(buffer, m_secondary_buffer, uC_index);
        // Add the first half tracepoint probe to the primary buffer and second half 
        // tracepoint probe to the secondary buffer.
        get_half_tracepoint_probe(buffer, m_primary_buffer, true, uC_index);
        get_half_tracepoint_probe(buffer, m_secondary_buffer, false, uC_index);
        // Add the jprobe probe to the primary buffer.
        get_jprobe_probe(buffer, m_primary_buffer, uC_index);
        // Add the second half tracepoint probe to the primary buffer from secondary buffer.
        get_half_tracepoint_probe(m_secondary_buffer, m_primary_buffer, false, uC_index);
        // Add the end probe to the primary buffer from secondary buffer.
        get_end_probe(m_secondary_buffer, m_primary_buffer, uC_index);
        // Update the page header of the primary buffer with the number of probes 
        // in the page, page length and page index.
        update_page_header(m_primary_buffer);
        return m_primary_buffer;
    }
    catch (const std::exception& e)
    {
        DTRACE_ERROR("DTRACE_PAGER_ERROR", "Error in pager: " << e.what());
        return m_primary_buffer;
    }
}

//-------------------------pager::expand_page-------------------------//
/**
 * expand_page() - 
 *  Expands buffer to add new page when page size exceeds the m_page_size
 *
 * @param buffer
 *  Control block buffer to be expanded.
 * @param size 
 *  Size of buffer to be added in the new page.
 * @return 
 *  true if the buffer was expanded, false otherwise.
 *
 * This function expands the buffer to add a new page when the size of the buffer 
 * exceeds m_page_size. It clears the last page flag and updates the page header 
 * for current page, and adds the page header for the new page. 
 * It increments the page index and returns true.
 */
bool 
pager::
expand_page(std::vector<uint32_t>& buffer, uint32_t size, bool force)
{
    uint32_t space = (m_page_index + 1) * m_page_size - static_cast<uint32_t>(buffer.size());
    if (size <= space && !force)
        return false;
    
    // Clear the last page flag and update the page header for the current page.
    clear_last_page_flag(buffer);
    update_page_header(buffer);
    // Add the empty space to the buffer and add the page header for the new page.
    buffer.insert(buffer.end(), space, 0);
    add_page_header(buffer);
    // Increment the page index.
    m_page_index++;
    return true;
}

//-------------------------pager::clear_last_page_flag-------------------------//
/**
 * clear_last_page_flag() - Clears the last page flag after adding a new page.
 *
 * This function modifies the buffer to clear the last page flag at the position
 * calculated using the page index and page size after adding a new page.
 */
void 
pager::
clear_last_page_flag(std::vector<uint32_t>& buffer)
{
    buffer[m_page_index * m_page_size + dtrace::probe::probe_type::begin] &= 
        pager_ctrl::mask_last_page_flag;
}

//-------------------------pager::update_page_header-------------------------//
/**
 * update_page_header() - 
 *  Updates the page header after adding a new page and after completing the paging.
 *
 * This function modifies the buffer to update the page header with the current 
 * probe information. It updates the page header with appropriate values in the buffer 
 * based on the current page index, page size, and the number of probes in the page.
 */
void 
pager::
update_page_header(std::vector<uint32_t>& buffer)
{
    buffer[m_page_index * m_page_size + dtrace::probe::probe_type::begin] |= (
        ((m_probes_in_page >> dtrace::dtrace_ctrl::first_byte_shift) & 
        pager_ctrl::mask_probe_count) << 1
    );
    buffer[m_page_index * m_page_size + dtrace::probe::probe_type::end] |= (
        m_probes_in_page & pager_ctrl::mask_probe_type
    );
    buffer[m_page_index * m_page_size + dtrace::probe::probe_type::jprobe] |= (
        (static_cast<uint32_t>(buffer.size()) - m_page_index * m_page_size) >> 
        dtrace::dtrace_ctrl::first_byte_shift
    );
    buffer[m_page_index * m_page_size + dtrace::probe::probe_type::tracepoint] |= (
        (static_cast<uint32_t>(buffer.size()) - m_page_index * m_page_size) & 
        pager_ctrl::mask_probe_type
    );
}

//-------------------------pager::add_page_header-------------------------//
/**
 * add_page_header() - Adds a page header to the provided buffer.
 *
 * This function appends a series of predefined headers to the given buffer.
 * Each header is constructed using constants defined in the dtrace::probe and dtrace::action namespaces.
 */
void 
pager::
add_page_header(std::vector<uint32_t>& buffer)
{
    buffer.push_back(
        (dtrace::probe::probe_ctrl::link_end << dtrace::dtrace_ctrl::second_byte_shift) | 
        (dtrace::probe::probe_type::begin << dtrace::dtrace_ctrl::first_byte_shift) | 1
    );
    buffer.push_back(
        (dtrace::probe::probe_ctrl::link_end << dtrace::dtrace_ctrl::second_byte_shift) | 
        (dtrace::probe::probe_type::end << dtrace::dtrace_ctrl::first_byte_shift)
    );
    buffer.push_back(
        (dtrace::probe::probe_ctrl::link_end << dtrace::dtrace_ctrl::second_byte_shift) | 
        (dtrace::probe::probe_type::jprobe << dtrace::dtrace_ctrl::first_byte_shift)
    );
    buffer.push_back(
        (dtrace::probe::probe_ctrl::link_end << dtrace::dtrace_ctrl::second_byte_shift) | 
        (dtrace::probe::probe_type::tracepoint << dtrace::dtrace_ctrl::first_byte_shift)
    );
    m_probes_in_page = 0;
}

//-------------------------pager::get_begin_probe-------------------------//
/**
 * get_begin_probe() - Copies the begin probe and its actions from control block 
 *                     buffer to primary buffer.
 *
 * @param source 
 *  Source control block buffer vector containing the begin probe.
 * @param destination 
 *  Destination primary buffer vector where the begin probe and actions will be copied.
 * @param uC_index 
 *  Index of the uC to be used while paging.
 *
 * This function checks if the next at the begin location in the source control 
 * block buffer vector is not link end. If it is not link end, it calculates the 
 * action size and copies the actions from the source vector to the destination vector, 
 * updating the destination vector with the appropriate probe type and action size.
 *
 */
void 
pager::
get_begin_probe(const std::vector<uint32_t>& source, std::vector<uint32_t>& destination, 
    uint32_t uC_index)
{
    // begin probe can have only print action, 
    // In that case action_location_mapping should have uC_index
    m_primary_action_location_mapping[uC_index] = {};

    // Get the location of the begin probe.
    uint32_t location = dtrace::probe::probe_type::begin;
    if ((source[location] >> dtrace::dtrace_ctrl::second_byte_shift) != 
        dtrace::probe::probe_ctrl::link_end)
    {
        uint32_t action_size = get_action_size(
            source, source[location] >> dtrace::dtrace_ctrl::second_byte_shift
        );
        destination[location] = 
            (destination.size() << dtrace::dtrace_ctrl::second_byte_shift) | 
            (dtrace::probe::probe_type::begin << dtrace::dtrace_ctrl::first_byte_shift) | 
            (destination[location] & pager_ctrl::mask_probe_type);
        copy_actions(
            source, source[location] >> dtrace::dtrace_ctrl::second_byte_shift, 
            destination, action_size, uC_index
        );
    }
}

//-------------------------pager::get_end_probe-------------------------//
/**
 * get_end_probe() - 
 *  Copies the end probe and its actions from control block buffer to secondary buffer.
 *
 * @param source 
 *  Source control block buffer vector containing the end probe.
 * @param destination 
 *  Destination secondary buffer vector where the end probe and actions will be copied.
 * @param uC_index 
 *  Index of the uC to be used while paging.
 *
 * This function checks if the next at the end location in the source control block buffer 
 * vector is not link end. If it is not link end, it calculates the action size and 
 * copies the actions from the source vector to the destination vector, updating the 
 * destination vector with the appropriate probe type and action size.
 *
 */
void 
pager::
get_end_probe(const std::vector<uint32_t>& source, std::vector<uint32_t>& destination, uint32_t uC_index)
{
    // Get the location of the end probe.
    uint32_t location = dtrace::probe::probe_type::end;
    if ((source[location] >> dtrace::dtrace_ctrl::second_byte_shift) != 
        dtrace::probe::probe_ctrl::link_end)
    {
        uint32_t action_size = get_action_size(
            source, source[location] >> dtrace::dtrace_ctrl::second_byte_shift
        );
        expand_page(destination, action_size + 1);
        destination[m_page_index * m_page_size + location] = 
            ((static_cast<uint32_t>(destination.size()) - m_page_index * m_page_size) << 
            dtrace::dtrace_ctrl::second_byte_shift) | 
            (dtrace::probe::probe_type::end << dtrace::dtrace_ctrl::first_byte_shift);
        copy_actions(
            source, source[location] >> dtrace::dtrace_ctrl::second_byte_shift, 
            destination, action_size, uC_index
        );
    }
}

//-------------------------pager::get_half_tracepoint_probe-------------------------//
/**
 * get_half_tracepoint_probe() - Extracts and copies half of the tracepoint probes 
 * from the source control block buffer vector to the destination vector.
 *
 * @param source 
 *  Source control block buffer vector containing the tracepoint probes to be copied.
 * @param destination 
 *  Destination buffer vector where the selected tracepoint probes will be copied.
 * @param is_first_half 
 *  Boolean flag indicating first half (true) or the second half (false) of the tracepoint probes.
 * @param uC_index
 *  Index of the uC to be used while paging.
 *
 * This function iterates through the tracepoint probes in the source vector and 
 * selectively copies them to the destination vector based on the specified half (first or second). 
 * It ensures that the destination vector is expanded as needed to accommodate the 
 * new probes and their associated actions.
 */
void 
pager::
get_half_tracepoint_probe(const std::vector<uint32_t>& source, std::vector<uint32_t>& destination, 
    bool is_first_half, uint32_t uC_index)
{
    uint32_t link = 
        destination[m_page_index * m_page_size + dtrace::probe::probe_type::tracepoint] >> 
        dtrace::dtrace_ctrl::second_byte_shift;
    uint32_t destination_location = m_page_index * m_page_size + dtrace::probe::probe_type::tracepoint;
    while (link != dtrace::probe::probe_ctrl::link_end)
    {
        destination_location = link;
        link = destination[link] >> dtrace::dtrace_ctrl::second_byte_shift;
    }

    uint32_t source_probe_offset = source[dtrace::probe::probe_type::tracepoint] >> 
        dtrace::dtrace_ctrl::second_byte_shift;
    while (true)
    {
        if (source_probe_offset == dtrace::probe::probe_ctrl::link_end)
            break;
        
        uint32_t probe = source[source_probe_offset + 1];
        if (((probe >> dtrace::dtrace_ctrl::second_byte_shift) < 
            dtrace::probe::probe_ctrl::tracepoint_split) == is_first_half)
        {
            uint32_t action_size = get_action_size(source, source_probe_offset + 2);
            if (expand_page(destination, action_size + 2))
                destination_location = m_page_index * m_page_size + dtrace::probe::probe_type::tracepoint;
            
            destination[destination_location] = 
                ((static_cast<uint32_t>(destination.size()) - m_page_index * m_page_size) << 
                dtrace::dtrace_ctrl::second_byte_shift) | 
                (dtrace::probe::probe_type::tracepoint << dtrace::dtrace_ctrl::first_byte_shift);
            destination_location = (m_page_index * m_page_size) + 
                (destination[destination_location] >> dtrace::dtrace_ctrl::second_byte_shift);
            destination.push_back(
                (dtrace::probe::probe_ctrl::link_end << dtrace::dtrace_ctrl::second_byte_shift) | 
                (dtrace::probe::probe_type::tracepoint << dtrace::dtrace_ctrl::first_byte_shift));
            destination.push_back(probe);
            copy_actions(source, source_probe_offset + 2, destination, action_size, uC_index);
        }
        source_probe_offset = source[source_probe_offset] >> dtrace::dtrace_ctrl::second_byte_shift;
    }
}

//-------------------------pager::get_jprobe_probe-------------------------//
/**
 * get_jprobe_probe() - Copies jprobe probes from the source control block buffer 
 * vector to the destination primary buffer vector.
 *
 * @param source 
 *  Source control block buffer vector containing the jprobe probes to be copied.
 * @param destination 
 *  Destination primary buffer vector where the selected tracepoint probes will be copied.
 * @param uC_index
 *  Index of the uC to be used while paging.
 *
 * This function iterates through the jprobe probes in the source control block 
 * buffer vector and copies them to the destination primary vector. 
 * It handles the necessary paging and ensures that the probes are copied in the correct order.
 */
void 
pager::
get_jprobe_probe(const std::vector<uint32_t>& source, std::vector<uint32_t>& destination, uint32_t uC_index)
{
    uint32_t link = 
        destination[m_page_index * m_page_size + dtrace::probe::probe_type::jprobe] >> 
        dtrace::dtrace_ctrl::second_byte_shift;
    uint32_t destination_location = m_page_index * m_page_size + dtrace::probe::probe_type::jprobe;
    uint32_t destination_control_code_page_index = pager_ctrl::mask_probe_ctrl;
    while (link != dtrace::probe::probe_ctrl::link_end)
    {
        destination_location = link;
        link = destination[link] >> dtrace::dtrace_ctrl::second_byte_shift;
        destination_control_code_page_index = destination[link + 1] & pager_ctrl::mask_probe_ctrl;
    }

    uint32_t source_probe_offset = 
        source[dtrace::probe::probe_type::jprobe] >> dtrace::dtrace_ctrl::second_byte_shift;
    while (true)
    {
        if (source_probe_offset == dtrace::probe::probe_ctrl::link_end)
            break;
        
        uint32_t probe = source[source_probe_offset + 1];
        uint32_t action_size = get_action_size(source, source_probe_offset + 3);
        bool forcePaging = (!m_probe_in_order) && 
            (destination_control_code_page_index != pager_ctrl::mask_probe_ctrl) && 
            (destination_control_code_page_index != (probe & pager_ctrl::mask_probe_ctrl));
        if (expand_page(destination, action_size + 3, forcePaging))
            destination_location = m_page_index * m_page_size + dtrace::probe::probe_type::jprobe;
        
        destination[destination_location] = 
            ((static_cast<uint32_t>(destination.size()) - m_page_index * m_page_size) << 
            dtrace::dtrace_ctrl::second_byte_shift) | 
            (dtrace::probe::probe_type::jprobe << dtrace::dtrace_ctrl::first_byte_shift);
        destination_location = m_page_index * m_page_size + 
            (destination[destination_location] >> dtrace::dtrace_ctrl::second_byte_shift);
        destination.push_back(
            (dtrace::probe::probe_ctrl::link_end << dtrace::dtrace_ctrl::second_byte_shift) | 
            (dtrace::probe::probe_type::jprobe << dtrace::dtrace_ctrl::first_byte_shift)
        );
        destination.push_back(probe);
        destination.push_back(source[source_probe_offset + 2]);
        copy_actions(source, source_probe_offset + 3, destination, action_size, uC_index);
        m_probes_in_page++;
        source_probe_offset = source[source_probe_offset] >> 
            dtrace::dtrace_ctrl::second_byte_shift;
    }
}

//-------------------------pager::get_action_size-------------------------//
/**
 * get_action_size() - 
 *  Calculates the total size of an action sequence starting from a given offset.
 *
 * @param source 
 *  Source control block buffer vector containing the jprobe probes to be copied.
 * @param action_offset 
 *  Starting offset in the source control block buffer vector from which to begin 
 *  calculating the action size.
 * @return 
 *  Total size of the action sequence starting from the given offset for probe.
 * 
 * This function iterates through a sequence of probe actions in the provided source vector,
 * starting from the specified action_offset. It calculates the total size of the 
 * action sequence for particularcprobe by summing the sizes of individual actions 
 * until it encounters the last action in the sequence.
 */
uint32_t 
pager::
get_action_size(const std::vector<uint32_t>& source, uint32_t action_offset)
{
    uint32_t action_size = 0;
    uint32_t action = source[action_offset];
    while (true)
    {
        bool last = action >> dtrace::dtrace_ctrl::second_byte_shift;
        // uint32_t action_type = action & dtrace::dtrace_ctrl::mask_16;
        uint32_t action_type = action & dtrace::dtrace_ctrl::mask_8;
        if (action_type == dtrace::action::action_type::reg_read ||
            action_type == dtrace::action::action_type::reg_write ||
            action_type == dtrace::action::action_type::handshake_read ||
            action_type == dtrace::action::action_type::handshake_write ||
            action_type == dtrace::action::action_type::host_timestamp ||
            action_type == dtrace::action::action_type::timestamp)
        {   // read register action, write register action, timestamp, host_timestamp action
            action_size += dtrace::action::action_ctrl::reg_rw_action_size;
        }
        else if (action_type == dtrace::action::action_type::timestamp32 ||
            action_type == dtrace::action::action_type::sleep ||
            action_type == dtrace::action::action_type::count)
        {   // timestamp32, sleep, count action
            action_size += dtrace::action::action_ctrl::timestamp32_action_size;
        }
        else if (action_type == dtrace::action::action_type::mem_read || 
            action_type == dtrace::action::action_type::mem_write)
        {   // read memory action, write memory action
            action_size += dtrace::action::action_ctrl::mem_rw_action_size;
        }
        else if (action_type == dtrace::action::action_type::break_action)
        {   // break action
            action_size += dtrace::action::action_ctrl::break_action_size;
        }
        else if (action_type == dtrace::action::action_type::timestamps)
        {   // timestamps action
            uint32_t action_length = source[action_offset + 1];
            action_size += (
                dtrace::action::action_ctrl::timestamps_action_size + 
                (action_length * dtrace::action::action_ctrl::timestamps_value)
            );
        }
        else if (action_type == dtrace::action::action_type::timestamps32)
        {   // timestamps32 action
            uint32_t action_length = source[action_offset + 1];
            action_size += dtrace::action::action_ctrl::timestamps_action_size + action_length;
        }
        else if (action_type == dtrace::action::action_type::reg_mask_write)
        {   // mask write register action
            action_size += dtrace::action::action_ctrl::reg_mask_w_action_size;
        }
        else
        {
            throw std::runtime_error("Illegal action type");
        }
        if (last)
        {
            break;
        }
        action = source[action_offset + action_size];
    }
    return action_size;
}

//-------------------------pager::copy_actions-------------------------//
/**
 * copy_actions() - Copies range of actions from source vector to destination vector.
 *
 * @param source 
 *  Vector from which actions are copied.
 * @param offset 
 *  Starting index in the source vector from which to begin copying.
 * @param destination 
 *  Vector to which actions are copied.
 * @param size 
 *  Number of words to copy from the source vector.
 * @param uC_index
 *  Index of the uC to be used while paging.
 *
 * This function copies a specified number of words from the source vector, 
 * starting at a given offset, to the destination vector. It also updates action 
 * location mappings based on whether the source or destination is the secondary buffer.
 */
void 
pager::
copy_actions(const std::vector<uint32_t>& source, uint32_t offset, 
    std::vector<uint32_t>& destination, uint32_t size, uint32_t uC_index)
{
    for (uint32_t i = 0; i < size; ++i)
    {
        auto destination_size = static_cast<uint32_t>(destination.size());
        if (&destination == &m_secondary_buffer)
        {   // secondary buffer
            m_secondary_action_location_mapping[destination_size] = offset + i;
        }
        else if (&source == &m_secondary_buffer)
        {   // secondary buffer
            m_primary_action_location_mapping[uC_index]
                [m_secondary_action_location_mapping[offset + i]] = destination_size;
        }
        else
        {   // primary buffer
            m_primary_action_location_mapping[uC_index]
                [offset + i] = destination_size;
        }
        destination.push_back(source[offset + i]);
    }
}

//-------------------------pager::get_action_location_mapping-------------------------//
/**
 * get_action_location_mapping() - 
 *  Retrieves the action location mapping for a specified uC index.
 * 
 * @param uC_index 
 *  Index of the uC for which the action location mapping is requested.
 * @return 
 *  Map where the keys are action identifiers and the values are their corresponding locations.
 * 
 * This function returns an unordered map that maps action identifiers to their
 * corresponding locations for the given uC index.
 */
std::unordered_map<uint32_t, uint32_t> 
pager::
get_action_location_mapping(uint32_t uC_index) const
{
    return m_primary_action_location_mapping.at(uC_index);
}

} // namespace dtrace::pager

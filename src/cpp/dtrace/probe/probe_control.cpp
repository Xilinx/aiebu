// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/probe/probe_control.h"
#include <sstream>

namespace dtrace::probe
{

//-------------------------probe::probe-------------------------//
/**
 * probe() - Constructor with probe type and probe name.
 *
 * @param probe_type
 * @param probe_name
 */
probe::
probe(uint32_t probe_type, std::string probe_name)
    : m_probe_type(probe_type)
    , m_probe_name(std::move(probe_name))
{
}

//-------------------------probe::add_action-------------------------//
/**
 * add_action() - Adds an action to the probe.
 *
 * @param action 
 *  Shared pointer to the action to be added.
 *
 * This function appends the given action to the list of actions
 * associated with the probe.
 */
void
probe::
add_action(std::shared_ptr<dtrace::action::action> action)
{
    m_actions.push_back(action);
}

//-------------------------probe::filter_action-------------------------//
/**
 * filter_action() - Filters actions and stores control actions.
 *
 * This function iterates through the list of actions (`m_actions`) and filters out
 * print and operation actions. The remaining actions are considered control actions 
 * and are stored in the `m_control_actions` list.
 */
void
probe::
filter_action()
{
    for (const auto& action : m_actions)
    {
        if (dynamic_cast<dtrace::action::print_action*>(action.get()) == nullptr &&
            dynamic_cast<dtrace::action::operation_action*>(action.get()) == nullptr)
            m_control_actions.push_back(action);
    }
}

//-------------------------probe::actionize-------------------------//
/**
 * actionize() - Executes series of control actions for control block buffers.
 *
 * @param control_buffer 
 *  Reference to control block buffer that will be modified by the actions.
 * @param mem_buffer 
 *  Reference to the memory buffer that will be modified by the actions.
 * @return
 *  Memory action locations for host address patching. 
 *
 * This function iterates through the list of control actions and applies each 
 * action to the provided control and memory buffers. The last action in the 
 * list is marked as the final action.
 */
std::vector<uint32_t>
probe::
actionize(std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer)
{
    std::vector<uint32_t> mem_action_locations;
    for (size_t i = 0; i < m_control_actions.size(); i++)
    {
        bool is_last = (i == m_control_actions.size() - 1);
        try 
        {
            m_control_actions[i]->actionize(is_last, control_buffer, mem_buffer);
            DTRACE_INFO("Actionized control action " << m_control_actions[i]->create_string());
        } 
        catch (const std::exception& e) 
        {
            DTRACE_ERROR("DTRACE_ACTION_ACTIONIZE_FAILED", "Failed to actionize control action " 
                << m_control_actions[i]->create_string() << ". Exception: " << e.what());
        }

        // For action type read_mem_action and write_mem_action, store location
        // in the mem_action_locations vector for mem_host_address patching
        if (dynamic_cast<dtrace::action::read_mem_action*>(m_control_actions[i].get()) != nullptr ||
            dynamic_cast<dtrace::action::write_mem_action*>(m_control_actions[i].get()) != nullptr)
        {
            // adding 2 for mem_host_addr high location
            mem_action_locations.push_back(
                m_control_actions[i]->get_location(false) + 2 
            );
        }
    }
    return mem_action_locations;
}

//-------------------------probe::create_string-------------------------//
/**
 * create_string() - Creates a concatenated string representation of all actions.
 *
 * @return 
 *  std::string containing the concatenated string representation of all actions.
 *
 * This function iterates through the list of actions and calls the create_string()
 * method on each action to generate a concatenated string representation of all actions.
 */
std::string
probe::
create_string() const
{
    std::ostringstream output_probe;
    output_probe << "probe_name: " << m_probe_name;
    return output_probe.str();
}

} // namespace dtrace::probe

// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef TRACE_H
#define TRACE_H

// This header file contains the public APIs for creating control buffer, memory buffer, and result file
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef _WIN32
  #define DTRACE_EXPORT __declspec(dllexport)
#else
  #define DTRACE_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------dtrace c---------------------------
//---------------------------multiple uC dtrace---------------------------
/*!
 * get_dtrace_col_numbers() - Retrieves the buffer sizes required for dynamic tracing.
 *
 * @script_file:    Script file containing the probe and action details.
 * @map_data:       Map data containing details of control code.
 * @buffers_length: Number of uC details in the buffers array.
 * Return:          Status of the operation. 0 on success and 1 on failure.
 *
 * This function calculates and returns the length of uC for dynamic tracing based
 * on the provided script file and map data. It returns 0 on success and 1 on failure.
 * Does not include the log level parameter, log_level is set to error by default.
 */
DTRACE_EXPORT
uint32_t 
get_dtrace_col_numbers(const char* script_file, const char* map_data, 
    uint32_t* buffers_length);

/*!
 * get_dtrace_col_numbers_with_log() - Retrieves the buffer sizes required for dynamic tracing.
 *
 * @script_file:    Script file containing the probe and action details.
 * @map_data:       Map data containing details of control code.
 * @buffers_length: Number of uC details in the buffers array.
 * @log_level:      Log level for debugging.
 * Return:          Status of the operation. 0 on success and 1 on failure.
 *
 * This function calculates and returns the length of uC for dynamic tracing based
 * on the provided script file and map data. It returns 0 on success and 1 on failure.
 */
DTRACE_EXPORT
uint32_t 
get_dtrace_col_numbers_with_log(const char* script_file, const char* map_data, 
    uint32_t* buffers_length, uint32_t log_level);

/*!
 * get_dtrace_buffer_size() - Retrieves the buffer sizes required for dynamic tracing.
 *
 * @buffers:        Array of uC details, where each index contains a uint64_t values
 *                  with high 32-bit as length and low 32-bit for respective uC.
 * Return:          Status of the operation. 0 on success and 1 on failure.
 *
 * This function calculates and returns the length of the control and memory buffers 
 * and uC index needed for dynamic tracing based on the provided script file and map data. 
 * It returns 0 on success and 1 on failure.
 */
DTRACE_EXPORT
void
get_dtrace_buffer_size(uint64_t* buffers);

/*!
 * populate_dtrace_buffer() - Creates a dynamic tracing buffers.
 *
 * @control_buffer:         Address for control buffer and memory buffer containing 
 *                          probe and action details and mem action details for multiple uC.
 * @dtrace_buffer_dma:      Physical address of the buffer, used to patch mem action host address
 *
 * This function initializes and allocates dynamic tracing buffers for each uC index. 
 * Each element in the control buffer represents a probe or its respective action.
 */
DTRACE_EXPORT
void 
populate_dtrace_buffer(uint32_t* dtrace_buffer, uint64_t dtrace_buffer_dma);

/*!
 * get_dtrace_result_file() - Creates a result file for dynamic tracing.
 *
 * @result_file:  Output file where the readable result will be written.
 *
 * This function creates a result file by processing the result buffer and mem buffer, 
 * and writes the output to the specified result file.
 */
DTRACE_EXPORT
void 
get_dtrace_result_file(const char* result_file);

#ifdef __cplusplus
}
#endif

//---------------------------dtrace cpp---------------------------
namespace aiebu::utilities
{

/**
 * dtrace_validate() - Validates the dynamic tracing compiler functionality.
 */
void 
dtrace_validate()
{
    std::cout << "[DTRACE] working...";
}

/**
 * create_control_buffer() - Creates dynamic tracing buffers.
 *
 * @param script_file
 *  Script file containing the probe and action details.
 * @param map_data 
 *  Map data containing details of control code.
 * @param mem_host_addr_map 
 *  Memory buffer address to be used for control data.
 * @return 
 *  Pair of control buffer and memory buffer.
 *
 * This function processes the provided script file and map data to generate
 * two vectors for control buffer and mem buffer of uint32_t values. 
 */
std::pair<std::vector<uint32_t>, std::vector<uint32_t>> 
create_control_buffer(const std::string& script_file, const std::string& map_data, 
    std::unordered_map<uint32_t, uint64_t>& mem_host_addr_map);

/**
 * create_result_file() - Creates a result file based on the provided script and map data.
 *
 * @param script_file
 *  Script file containing the probe and action details.
 * @param map_data 
 *  Map data containing details of control code.
 * @param result_buffers
 *  Result buffer containing probe and action details after running the control code.
 * @param mem_buffers
 *  Memory buffer containing read/write mem action details after running the control code.
 * @param result_file
 *  Output file where the readable result will be written.
 *
 * This function takes a script file, map data, result buffer, and result file.
 * It creates a result file by processing the control buffer and map data, and
 * writes the output to the specified result file.
 */
void
create_result_file(const std::string& script_file, const std::string& map_data, 
    const std::unordered_map<uint32_t, std::vector<uint32_t>>& result_buffers, 
    const std::unordered_map<uint32_t, std::vector<uint32_t>>& mem_buffers, 
    const std::string& result_file);

} // namespace aiebu::utilities

#endif // TRACE_H

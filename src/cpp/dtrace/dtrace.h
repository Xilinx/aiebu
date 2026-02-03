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
/*!
 * handle to a dtrace context.
 * Each handle represents an independent dtrace instance for a command.
 */
typedef void* dtrace_handle_t;

/*!
 * get_dtrace_col_numbers() - Retrieves the buffer sizes required for dynamic tracing.
 *
 * @script_file:    Script file containing the probe and action details.
 * @map_data:       Map data containing details of control code.
 * @buffers_length: Number of uC details in the buffers array.
 * Return:          Handle to the dtrace context, or NULL on failure.
 *
 * This function calculates and returns the length of uC for dynamic tracing based
 * on the provided script file and map data. It returns a handle to the dtrace context.
 * Does not include the log level parameter, log_level is set to error by default.
 */
DTRACE_EXPORT
dtrace_handle_t 
get_dtrace_col_numbers(const char* script_file, const char* map_data, 
    uint32_t* buffers_length);

/*!
 * get_dtrace_col_numbers_with_log() - Retrieves the buffer sizes required for dynamic tracing.
 *
 * @script_file:    Script file containing the probe and action details.
 * @map_data:       Map data containing details of control code.
 * @buffers_length: Number of uC details in the buffers array.
 * @log_level:      Log level for debugging.
 * Return:          Handle to the dtrace context, or NULL on failure.
 *
 * This function calculates and returns the length of uC for dynamic tracing based
 * on the provided script file and map data. It returns a handle to the dtrace context.
 */
DTRACE_EXPORT
dtrace_handle_t 
get_dtrace_col_numbers_with_log(const char* script_file, const char* map_data, 
    uint32_t* buffers_length, uint32_t log_level);

/*!
 * get_dtrace_buffer_size() - Retrieves the buffer sizes required for dynamic tracing.
 *
 * @dtrace_handle:  Handle to the dtrace context.
 * @buffers:        Array of uC details, where each index contains a uint64_t values
 *                  with high 32-bit as length and low 32-bit for respective uC.
 *
 * This function calculates and returns the length of the control and memory buffers 
 * and uC index needed for dynamic tracing based on the provided script file and map data. 
 */
DTRACE_EXPORT
void
get_dtrace_buffer_size(dtrace_handle_t dtrace_handle, uint64_t* buffers);

/*!
 * populate_dtrace_buffer() - Creates a dynamic tracing buffers.
 *
 * @dtrace_handle:          Handle to the dtrace context.
 * @control_buffer:         Address for control buffer and memory buffer containing 
 *                          probe and action details and mem action details for multiple uC.
 * @dtrace_buffer_dma:      Physical address of the buffer, used to patch mem action host address
 *
 * This function initializes and allocates dynamic tracing buffers for each uC index. 
 * Each element in the control buffer represents a probe or its respective action.
 */
DTRACE_EXPORT
void 
populate_dtrace_buffer(dtrace_handle_t dtrace_handle, uint32_t* dtrace_buffer, 
    uint64_t dtrace_buffer_dma);

/*!
 * get_dtrace_result_file() - Creates a result file for dynamic tracing.
 *
 * @dtrace_handle:    Handle to the dtrace context.
 * @result_file:      Output file where the readable result will be written.
 *
 * This function creates a result file by processing the result buffer and mem buffer, 
 * and writes the output to the specified result file.
 */
DTRACE_EXPORT
void 
get_dtrace_result_file(dtrace_handle_t dtrace_handle, const char* result_file);

#ifdef __cplusplus
}
#endif

#endif // TRACE_H

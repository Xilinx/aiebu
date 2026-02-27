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
 * handle to a dynamic tracing context.
 * Each handle represents an independent dynamic tracing instance for a command.
 */
typedef void* dtrace_handle_t;

/*!
 * create_dtrace_handle() - Creates a handle to the dynamic tracing context.
 *
 * @script_file:    Script file containing the probe and action details.
 * @map_data:       Map data containing details of control code.
 * @log_level:      Log level for debugging.
 *
 * @return Opaque raw handle to the dynamic tracing context owned by the caller, or NULL on failure.
 * @note The caller must release this handle by calling destroy_dtrace_handle().
 */
DTRACE_EXPORT
dtrace_handle_t
create_dtrace_handle(const char* script_file, const char* map_data, uint32_t log_level);

/*!
 * get_dtrace_col_numbers() - Retrieves the buffer sizes required for dynamic tracing.
 *
 * @dtrace_handle:  Handle to the dynamic tracing context.
 * @buffers_length: Number of uC details in the buffers array.
 *
 * This function calculates and returns the length of uC for dynamic tracing based
 * on the provided script file and map data.
 */
DTRACE_EXPORT
void
get_dtrace_col_numbers(dtrace_handle_t dtrace_handle, uint32_t* buffers_length);

/*!
 * get_dtrace_buffer_size() - Retrieves the buffer sizes required for dynamic tracing.
 *
 * @dtrace_handle:  Handle to the dynamic tracing context.
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
 * @dtrace_handle:          Handle to the dynamic tracing context.
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
 * @dtrace_handle:    Handle to the dynamic tracing context.
 * @result_file:      Output file where the readable result will be written.
 *
 * This function creates a result file by processing the result buffer and mem buffer, 
 * and writes the output to the specified result file.
 */
DTRACE_EXPORT
void 
get_dtrace_result_file(dtrace_handle_t dtrace_handle, const char* result_file);

/*!
* destroy_dtrace_handle() - Destroys a dynamic tracing context.
*
* @dtrace_handle:  Handle to the dynamic tracing context.
*
* This function releases all resources associated with the dynamic tracing handle. 
* After this call, handle must not be used again.
*/
DTRACE_EXPORT
void
destroy_dtrace_handle(dtrace_handle_t dtrace_handle);

#ifdef __cplusplus
}
#endif

#endif // TRACE_H

// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026 Advanced Micro Devices, Inc. All rights reserved.

#ifndef TRACE_H
#define TRACE_H

//The header file contains the APIs for creating dtrace control buffer, memory buffer, and result file
#include <cstdint>
#include <string>

namespace ELFIO {
class elfio;
}

/*!
 * handle to a dynamic tracing context.
 * Each handle represents an independent dynamic tracing instance for a command.
 */
using dtrace_handle_t = void*;  // NOLINT

/*!
 * create_dtrace_handle() - Creates a handle to the dynamic tracing context.
 *
 * @script_file:    Path to script file containing probe and action details.
 * @map_data:       Optional debug information JSON from the ELF. Required when
 *                  the control script uses jprobe or profile; may be empty for
 *                  begin, end, and tracepoint probes.
 * @log_level:      Log level for debugging.
 * @output_fmt:     Output format for result file.
 *
 * @return Opaque raw handle to the dynamic tracing context owned by the caller, or NULL on failure.
 * @note The caller must release this handle by calling destroy_dtrace_handle().
 */
dtrace_handle_t
create_dtrace_handle(const std::string& script_file, const std::string& map_data, uint32_t log_level,
    uint32_t output_fmt);

/*!
 * create_dtrace_handle_elf() - Creates a handle to the dynamic tracing from an ELF.
 *
 * @script_file:       Path to script file containing probe and action details.
 * @elf:               Pre-parsed ELFIO object used to extract the debug information.
 * @kernel_instance:   Kernel instance in "kernel:instance" format.
 *                     Required for full ELFs; debug information is extracted
 *                     for that kernel instance. Must be empty for partial ELFs.
 * @log_level:         Log level for debugging.
 * @output_fmt:        Output format for result file.
 *
 * @return Opaque raw handle to the dynamic tracing context owned by the caller, or NULL on failure.
 * @note The caller must release this handle by calling destroy_dtrace_handle().
 */
dtrace_handle_t
create_dtrace_handle_elf(const std::string& script_file, const ELFIO::elfio& elf,
    const std::string& kernel_instance, uint32_t log_level, uint32_t output_fmt);

/*!
 * get_dtrace_col_numbers() - Retrieves the buffer sizes required for dynamic tracing.
 *
 * @dtrace_handle:  Handle to the dynamic tracing context.
 * @buffers_length: Number of uC details in the buffers array.
 *
 * This function calculates and returns the length of uC for dynamic tracing based
 * on the provided script file and map data.
 */
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
void
get_dtrace_buffer_size(dtrace_handle_t dtrace_handle, uint64_t* buffers);

/*!
 * populate_dtrace_buffer() - Creates a dynamic tracing buffers.
 *
 * @dtrace_handle:          Handle to the dynamic tracing context.
 * @dtrace_buffer:          Address for control buffer and memory buffer containing 
 *                          probe and action details and mem action details for multiple uC.
 * @dtrace_buffer_dma:      Physical address of the buffer, used to patch mem action host address
 *
 * This function initializes and allocates dynamic tracing buffers for each uC index. 
 * Each element in the control buffer represents a probe or its respective action.
 */
void 
populate_dtrace_buffer(dtrace_handle_t dtrace_handle, uint32_t* dtrace_buffer, 
    uint64_t dtrace_buffer_dma);

/*!
 * get_dtrace_result_file() - Creates a result file for dynamic tracing.
 *
 * @dtrace_handle:    Handle to the dynamic tracing context.
 * @result_file:      Output file name where the readable result will be written.
 *
 * This function creates a result file by processing the result buffer and mem buffer, 
 * and writes the output to the specified result file.
 */
void
get_dtrace_result_file(dtrace_handle_t dtrace_handle, const std::string& result_file);

/*!
 * get_dtrace_result_buffer() - Return dtrace result as JSON string.
 *
 * @dtrace_handle:    Handle to the dynamic tracing context.
 * @return            JSON serialized result as string.
 *
 * This function extracts results by processing the result buffer and mem buffer,
 * and returns the result as JSON string.
 */
std::string
get_dtrace_result_buffer(dtrace_handle_t dtrace_handle);

/*!
* destroy_dtrace_handle() - Destroys a dynamic tracing context.
*
* @dtrace_handle:  Handle to the dynamic tracing context.
*
* This function releases all resources associated with the dynamic tracing handle. 
* After this call, handle must not be used again.
*/
void
destroy_dtrace_handle(dtrace_handle_t dtrace_handle);

#endif // TRACE_H

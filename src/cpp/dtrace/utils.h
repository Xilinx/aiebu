// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef DTRACE_UTILS_H
#define DTRACE_UTILS_H

// This file contains the declaration of the log level and expressions for dtrace.
#ifdef CERT_TRACE_CONTROL_H
#include "trace_control.h"
#endif

#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

// Log Levels Enum
enum class dtrace_log_level {
    dtrace_error = 1,
    dtrace_warning = 2,
    dtrace_info = 3
};

// Logging Prefix
#define DTRACE_PREFIX "[DTRACE] "

// Error Logging
#ifndef DTRACE_ERROR
#define DTRACE_ERROR(rc, fmt) \
    do { /* NOLINT(cppcoreguidelines-avoid-do-while) */ \
        std::ostringstream output; \
        output << DTRACE_PREFIX "[ERROR] " << __func__ << ":" << __LINE__ << ": " << fmt << "\n"; \
        std::cerr << output.str(); \
        throw std::runtime_error(DTRACE_PREFIX "[ERROR] " + std::string(rc) + "\n"); \
    } while (0)
#endif

// Warning Logging
#ifndef DTRACE_WARNING
#define DTRACE_WARNING(fmt) \
    do { /* NOLINT(cppcoreguidelines-avoid-do-while) */ \
        if (dtrace::get_current_log_level() >= dtrace_log_level::dtrace_warning) { \
            std::ostringstream output; \
            output << DTRACE_PREFIX "[WARNING] " << __func__ << ":" << __LINE__ << ": " << fmt << "\n"; \
            std::cout << output.str(); \
        } \
    } while (0)
#endif

// Info Logging
#ifndef DTRACE_INFO
#define DTRACE_INFO(fmt) \
    do { /* NOLINT(cppcoreguidelines-avoid-do-while) */ \
        if (dtrace::get_current_log_level() >= dtrace_log_level::dtrace_info) { \
            std::ostringstream output; \
            output << DTRACE_PREFIX "[INFO] " << __func__ << ":" << __LINE__ << ": " << fmt << "\n"; \
            std::cout << output.str(); \
        } \
    } while (0)
#endif

namespace dtrace
{
//-------------------------Utils-------------------------//
//-------------------------Log Level-------------------------//
void set_log_level(uint32_t log_level);
dtrace_log_level get_current_log_level();

//-------------------------Control-------------------------//
/**
 * @class dtrace_ctrl
 *
 * @brief
 * dtrace::dtrace_ctrl defines constants used to create control block and action control.
 *
 * @details
 * The class provides a set of static constexpr values that are used for different
 * purposes such as page size, byte shifts, action sizes, and  masks in dtrace compiler.
 * The trace_page_size is conditionally defined based on the CERT_TRACE_CONTROL_H macro.
 */
class dtrace_ctrl
{
public:
#ifdef CERT_TRACE_CONTROL_H
    static constexpr uint32_t trace_page_size = PROFILE_PAGE_SIZE_TEST;                     // Page size for trace buffer when CERT_TRACE_CONTROL_H is defined  
#else   
    static constexpr uint32_t trace_page_size = 4096;                                       // Default page size for trace buffer
#endif
    static constexpr uint32_t first_byte_shift = 8;                                         // First byte for word
    static constexpr uint32_t second_byte_shift = 16;                                       // Second byte for word
    static constexpr uint32_t third_byte_shift = 24;                                        // Third byte for word
    static constexpr uint32_t forth_byte_shift = 32;                                        // Fourth byte for word
    static constexpr uint32_t word_byte_size = 4;                                           // Size of a word in bytes
    static constexpr uint32_t page_length_check = 0x2000;                                   // Value used to check the page length
    static constexpr uint32_t placeholder_value = 0x12345678;                               // Placeholder value
    static constexpr uint32_t mask_high_bit = 0x80000000;                                   // Mask for the high bit
    static constexpr uint32_t mask_8 = 0xFF;                                                // Mask for 8-bit unsigned integers    
    static constexpr uint32_t mask_16 = 0xFFFF;                                             // Mask for 16-bit unsigned integers    
    static constexpr uint32_t mask_32 = 0xFFFFFFFF;                                         // Mask for 32-bit unsigned integers    
    static constexpr uint32_t empty_buffer_check = 0xFFFFFFFF;                              // Check for empty buffer
    static constexpr uint32_t handshake_overflow = 0xFBADBEEF;                              // Value used to check handshake overflow
    static constexpr uint32_t decimal_base = 10;                                            // Base for decimal numbers
    static constexpr uint32_t hexadecimal_base = 16;                                        // Base for hexadecimal numbers
    static constexpr uint32_t decimal_hexadecimal_base = 0;                                 // Base for both decimal and hexadecimal numbers
    static constexpr uint32_t label_annotation_length = 10;                                 // Length of label annotation
    static constexpr uint32_t label_line_length = 4;                                        // Length of label line                                                                             
};

} // namespace dtrace
#endif // DTRACE_UTILS_H

// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef DTRACE_UTILS_H
#define DTRACE_UTILS_H

// This file contains the declaration of the log levels, output format enums, and 
// logging utilities for dtrace.
#ifdef CERT_TRACE_CONTROL_H
#include "trace_control.h"
#endif

#include <elfio/elfio.hpp>

#include <cstdint>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>

namespace dtrace
{
//-------------------------Output Result Format-------------------------//
// Output result format Enum
enum class dtrace_output_format {
    python = 0,  // Python output format (default)
    json = 1     // JSON output format
};

// variable to store the current output format
extern dtrace_output_format g_current_output_format; // NOLINT

// Function to set the output format
inline dtrace_output_format get_output_format()
{
    return g_current_output_format;
}

inline void set_output_format(uint32_t output_format)
{
    g_current_output_format = static_cast<dtrace_output_format>(output_format);
}

//-------------------------Log Level-------------------------//
// Log Levels Enum
enum class dtrace_log_level {
    dtrace_error = 0,
    dtrace_warning = 1,
    dtrace_info = 2
};

// variable to store the current log level
extern dtrace_log_level g_current_log_level; // NOLINT

// Function to set the log level
inline dtrace_log_level get_current_log_level() 
{
    return g_current_log_level;
}

inline void set_log_level(uint32_t log_level) 
{
    g_current_log_level = static_cast<dtrace_log_level>(log_level);
}

//-------------------------Control Constants-------------------------//
/**
 * @class dtrace_ctrl
 *
 * @brief
 * dtrace::dtrace_ctrl defines constants used to create control block and action control.
 *
 * @details
 * The class provides a set of static constexpr values that are used for different
 * purposes such as page size, byte shifts, action sizes, and masks in dtrace compiler.
 * The trace_page_size is conditionally defined based on the CERT_TRACE_CONTROL_H macro.
 */
class dtrace_ctrl
{
public:
#ifdef CERT_TRACE_CONTROL_H
    static constexpr uint32_t trace_page_size = PROFILE_PAGE_SIZE_TEST; // Page size for trace buffer when CERT_TRACE_CONTROL_H is defined
#else
    static constexpr uint32_t trace_page_size = 4096;                   // Default page size for trace buffer
#endif
    static constexpr uint32_t first_byte_shift = 8;                     // First byte for word
    static constexpr uint32_t second_byte_shift = 16;                   // Second byte for word
    static constexpr uint32_t third_byte_shift = 24;                    // Third byte for word
    static constexpr uint32_t forth_byte_shift = 32;                    // Fourth byte for word
    static constexpr uint32_t word_byte_size = 4;                       // Size of a word in bytes
    static constexpr uint32_t page_length_check = 0x2000;               // Value used to check the page length
    static constexpr uint32_t placeholder_value = 0x12345678;           // Placeholder value
    static constexpr uint32_t mask_high_bit = 0x80000000;               // Mask for the high bit
    static constexpr uint32_t mask_8 = 0xFF;                            // Mask for 8-bit unsigned integers
    static constexpr uint32_t mask_16 = 0xFFFF;                         // Mask for 16-bit unsigned integers
    static constexpr uint32_t mask_32 = 0xFFFFFFFF;                     // Mask for 32-bit unsigned integers
    static constexpr uint32_t empty_buffer_check = 0xFFFFFFFF;          // Check for empty buffer
    static constexpr uint32_t handshake_overflow = 0xFBADBEEF;          // Value used to check handshake overflow
    static constexpr uint32_t result_value_init = 0xFBADCAFE;           // Initial value for action result
    static constexpr uint32_t decimal_base = 10;                        // Base for decimal numbers
    static constexpr uint32_t hexadecimal_base = 16;                    // Base for hexadecimal numbers
    static constexpr uint32_t decimal_hexadecimal_base = 0;             // Base for both decimal and hexadecimal numbers
    static constexpr uint32_t label_annotation_length = 10;             // Length of label annotation
    static constexpr uint32_t label_line_length = 4;                    // Length of label line
};

/**
 * @class elf_dump_map
 *
 * @brief Extracts .dump section JSON from ELF binaries for dtrace.
 *
 * @details
 * The elf_dump_map class provides methods to extract the .dump section from ELF binaries.
 * It supports both non-config and config ELF binaries, allowing retrieval of the .dump section
 * in JSON format. The class uses the ELFIO library to parse the ELF files and extract
 * the relevant sections based on the provided kernel instance filter.
 */
class elf_dump_map {
public:
  explicit elf_dump_map(const ELFIO::elfio& elf);

  // Non-config ELF: first ".dump" PROGBITS section.
  std::string get_dump_section_json() const;

  // Config ELF: ".dump" section for kernel:instance (group-filtered).
  std::string get_dump_section_json(const std::string& kernel_instance) const;

private:
  const ELFIO::elfio& m_elf; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

  static std::string extract_kernel_name_from_mangled(const std::string& symbol_name);

  std::set<ELFIO::Elf_Half>
  get_filtered_section_indices(const std::string& kernel_instance_filter) const;
};

} // namespace dtrace

// -------------------------Logging Macros-------------------------//
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
        if (dtrace::get_current_log_level() >= dtrace::dtrace_log_level::dtrace_warning) { \
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
        if (dtrace::get_current_log_level() >= dtrace::dtrace_log_level::dtrace_info) { \
            std::ostringstream output; \
            output << DTRACE_PREFIX "[INFO] " << __func__ << ":" << __LINE__ << ": " << fmt << "\n"; \
            std::cout << output.str(); \
        } \
    } while (0)
#endif

#endif // DTRACE_UTILS_H

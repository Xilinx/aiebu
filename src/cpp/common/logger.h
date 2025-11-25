// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_COMMON_LOGGER_H
#define AIEBU_COMMON_LOGGER_H

#include <iostream>
#include <sstream>

namespace aiebu {

// Log Levels
enum class log_level {
  error = 0,   // Always shown
  warn = 1,    // Warnings
  info = 2,    // Informational messages (shown with verbose)
  debug = 3    // Debug messages (shown with verbose)
};

// Global log level (default: errors and warnings)
inline log_level g_log_level = log_level::warn;

// Set log level
inline void set_log_level(log_level level) {
  g_log_level = level;
}

// Get log level
inline log_level get_log_level() {
  return g_log_level;
}

// Enable verbose mode (shows info and debug messages)
inline void enable_verbose_logging() {
  g_log_level = log_level::debug;
}

// Disable verbose mode (back to default: errors and warnings)
inline void disable_verbose_logging() {
  g_log_level = log_level::warn;
}

} // namespace aiebu

// Logging macros
#define LOG_ERROR(msg) \
  do { \
    std::cerr << "[ERROR] " << msg << std::endl; \
  } while (0)

#define LOG_WARN(msg) \
  do { \
    if (aiebu::g_log_level >= aiebu::log_level::warn) { \
      std::cout << "[WARN] " << msg << std::endl; \
    } \
  } while (0)

#define LOG_INFO(msg) \
  do { \
    if (aiebu::g_log_level >= aiebu::log_level::info) { \
      std::cout << msg << std::endl; \
    } \
  } while (0)

#define LOG_DEBUG(msg) \
  do { \
    if (aiebu::g_log_level >= aiebu::log_level::debug) { \
      std::cout << "[DEBUG] " << msg << std::endl; \
    } \
  } while (0)

#endif // AIEBU_COMMON_LOGGER_H


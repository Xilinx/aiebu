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

// Internal function to access log level (thread-safe singleton pattern)
inline log_level& get_log_level_ref() {
  static log_level level = log_level::warn;
  return level;
}

// Set log level
inline void set_log_level(log_level level) {
  get_log_level_ref() = level;
}

// Get log level
inline log_level get_log_level() {
  return get_log_level_ref();
}

// Enable verbose mode (shows info and debug messages)
inline void enable_verbose_logging() {
  get_log_level_ref() = log_level::debug;
}

// Disable verbose mode (back to default: errors and warnings)
inline void disable_verbose_logging() {
  get_log_level_ref() = log_level::warn;
}

} // namespace aiebu

// NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
#define LOG_ERROR(msg) \
  do { \
    std::cerr << "[ERROR] " << msg << std::endl; \
  } while (0)

#define LOG_WARN(msg) \
  do { \
    if (aiebu::get_log_level_ref() >= aiebu::log_level::warn) { \
      std::cout << "[WARN] " << msg << std::endl; \
    } \
  } while (0)

#define LOG_INFO(msg) \
  do { \
    if (aiebu::get_log_level_ref() >= aiebu::log_level::info) { \
      std::cout << "[INFO]" << msg << std::endl; \
    } \
  } while (0)

#define LOG_DEBUG(msg) \
  do { \
    if (aiebu::get_log_level_ref() >= aiebu::log_level::debug) { \
      std::cout << "[DEBUG] " << msg << std::endl; \
    } \
  } while (0)
// NOLINTEND(cppcoreguidelines-avoid-do-while)

#endif // AIEBU_COMMON_LOGGER_H


// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_COMMON_LOGGER_H
#define AIEBU_COMMON_LOGGER_H

#include <iostream>
#include <sstream>
#include <string>

namespace aiebu {

enum class log_level {
    error = 0,
    warn = 1,
    info = 2,
    debug = 3
};

inline log_level& get_log_level_ref() {
    static log_level level = log_level::warn;  // default
  return level;
}

inline void set_log_level(log_level level) {
  get_log_level_ref() = level;
}

inline log_level get_log_level() {
  return get_log_level_ref();
}

// functions for verbose mode if needed for future use
inline void enable_verbose_logging() {
    set_log_level(log_level::debug);
}

inline void disable_verbose_logging() {
    set_log_level(log_level::warn);
}

// check if level is enabled
inline bool is_enabled(log_level level) noexcept {
    return static_cast<int>(level) <= static_cast<int>(get_log_level_ref());
}

// Logger stream class that supports << operator
class log_stream {
private:
    log_level level;
    std::ostringstream oss;
    bool enabled;

    void output() {
        if (!enabled) return;

        std::string msg = oss.str();
        switch (level) {
            case log_level::error:
                std::cerr << "[ERROR] " << msg << std::endl;
                break;
            case log_level::warn:
                std::cout << "[WARN ] " << msg << std::endl;
                break;
            case log_level::info:
                std::cout << "[INFO ] " << msg << std::endl;
                break;
            case log_level::debug:
                std::cout << "[DEBUG] " << msg << std::endl;
                break;
        }
    }

public:
    log_stream(log_level lvl)
        : level(lvl), enabled(is_enabled(lvl)) {}

    // Destructor outputs the message
    ~log_stream() {
        output();
    }

    // Overload << operator to support streaming
    template<typename T>
    log_stream& operator<<(const T& value) {
        if (enabled) {
            oss << value;
        }
        return *this;
    }

    // Special handling for std::endl and other manipulators
    log_stream& operator<<(std::ostream& (*manip)(std::ostream&)) {
        if (enabled) {
            oss << manip;
        }
        return *this;
    }
};

inline log_stream LOG_ERROR() {
    return log_stream(log_level::error);
}

inline log_stream LOG_WARN() {
    return log_stream(log_level::warn);
}

inline log_stream LOG_INFO() {
    return log_stream(log_level::info);
}

inline log_stream LOG_DEBUG() {
    return log_stream(log_level::debug);
}

} // namespace aiebu

#endif // AIEBU_COMMON_LOGGER_H

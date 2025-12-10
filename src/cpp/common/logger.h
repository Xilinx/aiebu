// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_COMMON_LOGGER_H
#define AIEBU_COMMON_LOGGER_H

#include <iostream>
#include <streambuf>
#include <memory>
#include <array>

namespace aiebu {

enum class log_level {
    error = 0,
    warn = 1,
    info = 2,
    debug = 3
};

// constexpr helpers
constexpr int to_int(log_level lvl) noexcept {
    return static_cast<int>(lvl);
}

constexpr std::size_t to_index(log_level lvl) noexcept {
    return static_cast<std::size_t>(lvl);
}

// Global minimum log level (singleton pattern)
inline log_level& get_log_level_ref() noexcept {
    static log_level min_level = log_level::warn;
    return min_level;
}

inline bool is_enabled(log_level lvl) noexcept {
    return to_int(lvl) <= to_int(get_log_level_ref());
}

// Null buffer that discards all output for disabled log levels
class null_buffer : public std::streambuf {
protected:
    int_type overflow(int_type c) override {
        return traits_type::not_eof(c);
    }
};

// Optimized log_stream with nested buffer
class log_stream : public std::ostream {
public:
    log_stream(std::ostream& out, log_level lvl)
        : std::ostream(&buf), buf(out, lvl) {}

    void set_level(log_level lvl) { buf.set_level(lvl); }

private:
    class buf_type : public std::streambuf {
    public:
        buf_type(std::ostream& out, log_level lvl)
            : sink(out), level(lvl) {}

        void set_level(log_level lvl) { level = lvl; }

    protected:
        int_type overflow(int_type ch) override {
            if (!begun) {
                sink << '[' << to_string(level) << "] ";
                begun = true;
            }
            if (!traits_type::eq_int_type(ch, traits_type::eof())) {
                sink.put(static_cast<char>(ch));
            }
            return traits_type::not_eof(ch);
        }

        int sync() override {
            sink.flush();
            begun = false;  // New message next time
            return 0;
        }

    private:
        static constexpr const char* to_string(log_level l) noexcept {
            switch (l) {
                case log_level::error: return "ERROR";
                case log_level::warn:  return "WARN ";
                case log_level::info:  return "INFO ";
                case log_level::debug: return "DEBUG";
            }
            return "?";
        }

        std::ostream& sink;
        log_level level;
        bool begun{false};
    };

    buf_type buf;
};

// Optimized logger using array-based storage
class logger {
public:
    logger() {
        // Create one log_stream per level (error uses cerr, others use cout)
        streams[to_index(log_level::error)] = std::make_unique<log_stream>(std::cerr, log_level::error);
        streams[to_index(log_level::warn)]  = std::make_unique<log_stream>(std::cout, log_level::warn);
        streams[to_index(log_level::info)]  = std::make_unique<log_stream>(std::cout, log_level::info);
        streams[to_index(log_level::debug)] = std::make_unique<log_stream>(std::cout, log_level::debug);
    }

    inline std::ostream& operator()(log_level lvl) noexcept {
        if (is_enabled(lvl)) {
            return *streams[to_index(lvl)];
        }
        // Lazily init null stream for disabled levels
        if (!null_stream) {
            null_stream = std::make_unique<std::ostream>(&null_buf);
        }
        return *null_stream;
    }

    // Convenience method (same as operator())
    inline std::ostream& get_stream(log_level lvl) noexcept {
        return (*this)(lvl);
    }

private:
    std::array<std::unique_ptr<log_stream>, 4> streams;
    null_buffer null_buf;
    std::unique_ptr<std::ostream> null_stream;
};

// Global logger instance
inline std::ostream& get_logger(log_level lvl) {
    static logger instance;
    return instance(lvl);
}

// Public API functions
inline void set_log_level(log_level lvl) noexcept {
    get_log_level_ref() = lvl;
}

inline log_level get_log_level() noexcept {
    return get_log_level_ref();
}

inline void enable_verbose_logging() noexcept {
    set_log_level(log_level::debug);
}

inline void disable_verbose_logging() noexcept {
    set_log_level(log_level::warn);
}

// Log functions
inline std::ostream& log_error() noexcept {
    return get_logger(log_level::error);
}

inline std::ostream& log_warn() noexcept {
    return get_logger(log_level::warn);
}

inline std::ostream& log_info() noexcept {
    return get_logger(log_level::info);
}

inline std::ostream& log_debug() noexcept {
    return get_logger(log_level::debug);
}

} // namespace aiebu

#endif // AIEBU_COMMON_LOGGER_H


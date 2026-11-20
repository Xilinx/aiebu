// SPDX-License-Identifier: MIT
// Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.
//
// dtrace_profile — per-stage timing harness for the dtrace workflow.
//
// Runs the full create_dtrace_handle_elf → get_dtrace_buffer_size →
// populate_dtrace_buffer → get_dtrace_result_file → destroy_dtrace_handle
// pipeline N times, timing each stage separately and reporting a table
// matching the style of the profiling reports in dwarf_profile_*.png.
//
// Usage:
//   dtrace_profile <elf_file> <script_file> <kernel_instance> [iterations]
//
// Example:
//   dtrace_profile single_col_dwarf.elf script.ct DPU:dpu 100
//   dtrace_profile multi-graph-dwarf.elf script.ct DPU:subgraph_0 10

#include "dtrace/dtrace.h"
#include "dtrace/utils.h"
#include "dtrace/control/control.h"
#include <elfio/elfio.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

// dtrace API declarations not in dtrace.h
void     destroy_dtrace_handle(dtrace_handle_t h);
void     get_dtrace_col_numbers(dtrace_handle_t h, uint32_t* out);
void     get_dtrace_buffer_size(dtrace_handle_t h, uint64_t* out);
void     populate_dtrace_buffer(dtrace_handle_t h, uint32_t* buf, uint64_t base);
void     get_dtrace_result_file(dtrace_handle_t h, const std::string& path);

static constexpr uint64_t k_ctrl_code_base = 0x200000;
static constexpr uint32_t k_word_shift     = 32;
static constexpr int      k_table_width    = 70;   ///< separator line width in characters
static constexpr double   k_us_per_sec     = 1e6;  ///< microseconds per second
static constexpr double   k_us_per_ms      = 1e3;  ///< microseconds per millisecond

// ── Timing accumulator ──────────────────────────────────────────────────────
struct Stage {
    std::string          name;
    std::vector<double>  samples_us;  // one sample per iteration

    explicit Stage(std::string n) : name(std::move(n)) {}

    void record(double us) { samples_us.push_back(us); }

    double total_us() const {
        return std::accumulate(samples_us.begin(), samples_us.end(), 0.0);
    }
    double mean_us() const {
        return samples_us.empty() ? 0.0
            : total_us() / static_cast<double>(samples_us.size());
    }
};

static double now_us()
{
    return std::chrono::duration<double, std::micro>(
        std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

// Time one lambda invocation and add the sample to stage.
template<typename Fn>
static auto timed(Stage& stage, Fn&& fn) -> decltype(fn())
{
    const double t0 = now_us();
    auto result = fn();
    stage.record(now_us() - t0);
    return result;
}

// Void specialisation
template<typename Fn>
static void timed_void(Stage& stage, Fn&& fn)
{
    const double t0 = now_us();
    fn();
    stage.record(now_us() - t0);
}

// ── Report ──────────────────────────────────────────────────────────────────
static void print_report(const std::vector<Stage>& stages, int iterations,
                         const std::string& col3 = "Stage", bool include_total = true)
{
    double grand_total_us = 0;
    for (const auto& s : stages)
        grand_total_us += s.total_us();

    constexpr int W_SEC   = 10;
    constexpr int W_SHARE = 10;

    std::cout << "\n"
              << std::string(k_table_width, '-') << "\n"
              << std::setw(W_SEC) << "Seconds"
              << std::setw(W_SHARE) << "Share"
              << "  " << col3 << "\n"
              << std::string(k_table_width, '-') << "\n";

    std::cout << std::fixed << std::setprecision(2);
    for (const auto& s : stages) {
        const double secs  = s.total_us() / k_us_per_sec;
        const double share = grand_total_us > 0
            ? 100.0 * s.total_us() / grand_total_us : 0.0;
        std::cout << std::setw(W_SEC)  << secs
                  << std::setw(W_SHARE - 1) << share << "%"
                  << "  " << s.name << "\n";
    }

    if (include_total) {
        std::cout << std::string(k_table_width, '-') << "\n"
                  << std::setw(W_SEC) << grand_total_us / k_us_per_sec
                  << std::setw(W_SHARE - 1) << "100.00" << "%"
                  << "  Total (" << iterations << " iteration"
                  << (iterations != 1 ? "s" : "") << ")\n"
                  << std::string(k_table_width, '-') << "\n";
    }
    std::cout << "\nMean per iteration: "
              << grand_total_us / iterations / k_us_per_ms << " ms\n";
}

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cerr << "Usage: dtrace_profile <elf_file> <script_file>"
                     " <kernel_instance> [iterations] [report_dir]\n"
                  << "  kernel_instance  e.g. DPU:dpu or DPU:subgraph_0\n"
                  << "  iterations       number of full workflow runs (default 10)\n"
                  << "  report_dir       optional; writes dwarf_profile_{1,2}.json\n";
        return 1;
    }

    const std::string elf_path        = argv[1];
    const std::string script_file     = argv[2];
    const std::string kernel_instance = argv[3];
    const int         iterations      = (argc > 4) ? std::stoi(argv[4]) : 10;

    // ── Load and parse ELF once — matches "main: load the ELF" in profile ────
    Stage s_load("main: load the ELF and parse arguments");
    ELFIO::elfio elf;
    {
        const double t0 = now_us();
        if (!elf.load(elf_path)) {
            std::cerr << "ERROR: failed to load ELF: " << elf_path << "\n";
            return 1;
        }
        s_load.record(now_us() - t0);
    }

    std::cout << "ELF:             " << elf_path         << "\n"
              << "Script:          " << script_file      << "\n"
              << "Kernel instance: " << kernel_instance  << "\n"
              << "Iterations:      " << iterations       << "\n";

    // ── Stage accumulators (match profile 2 rows) ─────────────────────────────
    Stage s_create  ("create_dtrace_handle_elf");
    Stage s_bufsize ("get_dtrace_buffer_size");
    Stage s_populate("allocate the buffer and populate_dtrace_buffer");
    Stage s_result  ("get_dtrace_result_file");
    Stage s_destroy ("destroy_dtrace_handle");

    // ── Warm-up ───────────────────────────────────────────────────────────────
    {
        auto* h = create_dtrace_handle_elf(script_file, elf, kernel_instance, 0, 0);
        if (!h) {
            std::cerr << "ERROR: create_dtrace_handle_elf failed\n"
                      << "  Verify script_file exists and kernel_instance is correct\n";
            return 1;
        }
        destroy_dtrace_handle(h);
    }

    // ── Timed iterations ──────────────────────────────────────────────────────
    for (int i = 0; i < iterations; ++i) {

        // 1. create_dtrace_handle_elf
        dtrace_handle_t h = nullptr;
        timed_void(s_create, [&] {
            h = create_dtrace_handle_elf(script_file, elf, kernel_instance, 0, 0);
        });
        if (!h) {
            std::cerr << "ERROR: iteration " << i << " failed\n";
            return 1;
        }

        // 2. get_dtrace_buffer_size
        uint32_t buffers_length = 0;
        uint64_t dtrace_buf_len = 0;
        timed_void(s_bufsize, [&] {
            get_dtrace_col_numbers(h, &buffers_length);
            std::vector<uint64_t> bufsizes(buffers_length);
            get_dtrace_buffer_size(h, bufsizes.data());
            for (uint32_t j = 0; j < buffers_length; ++j)
                dtrace_buf_len += static_cast<uint32_t>(bufsizes[j] >> k_word_shift);
        });

        // 3. allocate + populate_dtrace_buffer
        std::vector<uint32_t> dtrace_buf;
        timed_void(s_populate, [&] {
            dtrace_buf.assign(dtrace_buf_len, 0);
            populate_dtrace_buffer(h, dtrace_buf.data(), k_ctrl_code_base);
        });

        // 4. get_dtrace_result_file (write to /dev/null to avoid I/O skew)
        timed_void(s_result, [&] {
            get_dtrace_result_file(h, "/dev/null");
        });

        // 5. destroy_dtrace_handle
        timed_void(s_destroy, [&] {
            destroy_dtrace_handle(h);
        });
    }

    // ── Report (API workflow) ─────────────────────────
    const std::vector<Stage> api_stages =
        {s_load, s_create, s_bufsize, s_populate, s_result, s_destroy};
    std::cout << "\nAPI workflow breakdown:\n";
    print_report(api_stages, iterations, "API", true);

    // ── Sub-stage breakdown of create_dtrace_handle_elf ──────────────────────
    std::cout << "\nSub-stage breakdown of create_dtrace_handle_elf:\n";

    Stage s_debug_map   ("get_debug_map (filter + release_rows + decode)");
    Stage s_control_ctor("control::control (parser + compile + clear_maps)");
    Stage s_control_dtor("control destructor (after clear_maps)");

    const dtrace::elf_debug_map elf_map(elf);
    const bool group_elf = dtrace::elf_debug_map::is_group_elf(elf);

    dtrace::set_log_level(0);
    dtrace::set_output_format(0);

    // Warm-up
    {
        const dtrace::debug_map_t dm = group_elf
            ? elf_map.get_debug_map(kernel_instance) : elf_map.get_debug_map();
        dtrace::control ctrl(script_file, dm);
    }

    for (int i = 0; i < iterations; ++i) {
        dtrace::debug_map_t dm;
        timed_void(s_debug_map, [&] {
            dm = group_elf
                ? elf_map.get_debug_map(kernel_instance)
                : elf_map.get_debug_map();
        });

        // Construction only (exclude destructor).
        std::unique_ptr<dtrace::control> ctrl;
        const double t_ctrl0 = now_us();
        ctrl = std::make_unique<dtrace::control>(script_file, dm);
        s_control_ctor.record(now_us() - t_ctrl0);

        timed_void(s_control_dtor, [&] {
            ctrl.reset();
        });
    }

    // Detail report: create internals + create wall time as summary row share of API total
    std::vector<Stage> detail_stages = {s_debug_map, s_control_ctor, s_control_dtor};
    print_report(detail_stages, iterations, "Step", true);

    return 0;
}

# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.

# Helper CMake script for launching an executable at build time and capturing
# its output in execuatable.txt file. Primarily used with ctest
# You can manually run this script using the following synopsys:
# cmake -P run.cmake -o <logfile> -x <executable> -- <exe_args...>
# Manual command lines for reference are following--
# cmake -P cmake/run.cmake -o dump.txt -x build/Debug/aiebu/bin/aiebu-dump -- -D build/Debug/test/aie2-ctrlcode/pmctrlpkt/mc_code_txn.elf

if (${CMAKE_ARGC} LESS 6)
  message(FATAL_ERROR "-- Invalid usage, synopsys: cmake -P run.cmake -o <logfile> -x <executable> -- <args...>")
endif()

message("-- ${CMAKE_ARGV3}")

if (NOT ("${CMAKE_ARGV3}" STREQUAL "-o"))
  message(FATAL_ERROR "-- Invalid usage, synopsys: cmake -P run.cmake -o <logfile> -x <executable> -- <args...>")
endif()

if (NOT ("${CMAKE_ARGV5}" STREQUAL "-x"))
  message(FATAL_ERROR "-- Invalid usage, synopsys: cmake -P run.cmake -o <logfile> -x <executable> -- <args...>")
endif()

set(ofile ${CMAKE_ARGV4})
set(exe ${CMAKE_ARGV6})

foreach(INDEX RANGE 8 ${CMAKE_ARGC})
  list(APPEND ARGS "${CMAKE_ARGV${INDEX}}")
endforeach()

message("-- Running ${exe} with provided arguments, ${ARGS}, capturing the output in ${ofile}")

execute_process(COMMAND ${exe} ${ARGS} OUTPUT_FILE ${ofile})

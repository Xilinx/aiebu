# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

# DMA_compiler: <model>/config.json (asm cwd = model dir).
# X3: <model>/Work/ps/elf/aiebu_config.json (asm cwd = Work/ps/elf).
# Shared prebuilt asm: test/aie4-ctrlcode/real_models/DMA_compiler/prebuilt.
# If missing, link from build/real_test/prebuilt (see copy_models.sh).

set(aie4_real_models
  DMA_compiler/test_1
)

# No elf_to_legacy / control.txt gold (output too large for CI).
set(aie4_skip_elftolegacy
  X3/test_10
)

# Use a function (not a macro): return() only exits this function, not the foreach caller.
function(add_aie4_real_model_test model skip_elftolegacy_var)
  set(model_dir "${CMAKE_CURRENT_SOURCE_DIR}/${model}")
  set(config_path "${model_dir}/config.json")
  set(asm_work_dir "${model_dir}")

  if(NOT EXISTS "${config_path}")
    set(config_path "${model_dir}/Work/ps/elf/aiebu_config.json")
    set(asm_work_dir "${model_dir}/Work/ps/elf")
  endif()

  if(NOT EXISTS "${config_path}")
    message(STATUS "aie4 real_models: skipping ${model} (no config.json or Work/ps/elf/aiebu_config.json)")
    return()
  endif()

  string(REPLACE "/" "_" model_id "${model}")
  set(test_bin "${CMAKE_CURRENT_BINARY_DIR}/${model_id}")
  file(MAKE_DIRECTORY "${test_bin}")

  set(test_prefix "aie4_real_${model_id}")
  set(elf_name "control.elf")
  set(elf_path "${test_bin}/${elf_name}")

  add_test(NAME "${test_prefix}_asm"
    COMMAND aiebu-asm -t aie4_config -j "${config_path}" -o "${elf_path}" -f disabledump
    WORKING_DIRECTORY "${asm_work_dir}")

  add_test(NAME "${test_prefix}_md5sum"
    COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
            "${elf_path}" "${model_dir}/gold.md5"
    WORKING_DIRECTORY "${test_bin}")

  set_tests_properties("${test_prefix}_md5sum" PROPERTIES DEPENDS "${test_prefix}_asm")
  set_tests_properties("${test_prefix}_asm" PROPERTIES LABELS "benchmark")

  if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(skip_elftolegacy "${${skip_elftolegacy_var}}")
    list(FIND skip_elftolegacy "${model}" skip_elftolegacy_idx)
    if(skip_elftolegacy_idx EQUAL -1)
      add_test(NAME "${test_prefix}_elftolegacy"
        COMMAND "${AIEBU_SOURCE_DIR}/src/python/aiebu/elf_to_legacy.py"
                -o "${test_bin}/control.out.txt" -i "${elf_path}"
        WORKING_DIRECTORY "${test_bin}")

      add_test(NAME "${test_prefix}_elftolegacy_compare"
        COMMAND ${CMAKE_COMMAND} -E compare_files
                "${test_bin}/control.out.txt" "${model_dir}/control.txt"
        WORKING_DIRECTORY "${test_bin}")

      set_tests_properties("${test_prefix}_elftolegacy"
        PROPERTIES DEPENDS "${test_prefix}_asm"
        ENVIRONMENT "PYTHONPATH=${AIEBU_SOURCE_DIR}/src/python")
      set_tests_properties("${test_prefix}_elftolegacy_compare"
        PROPERTIES DEPENDS "${test_prefix}_elftolegacy")
    endif()

    add_test(NAME "${test_prefix}_readelf"
      COMMAND "${AIEBU_SOURCE_DIR}/test/aie2ps-ctrlcode/run-readelf.sh"
              "${elf_path}" "${test_bin}/control.elf.txt"
      WORKING_DIRECTORY "${test_bin}")

    add_test(NAME "${test_prefix}_readelf_compare"
      COMMAND ${CMAKE_COMMAND} -E compare_files
              "${test_bin}/control.elf.txt" "${model_dir}/control.gold"
      WORKING_DIRECTORY "${test_bin}")

    set_tests_properties("${test_prefix}_readelf" PROPERTIES DEPENDS "${test_prefix}_asm")
    set_tests_properties("${test_prefix}_readelf_compare" PROPERTIES DEPENDS "${test_prefix}_readelf")
  endif()
endfunction()

foreach(model IN LISTS aie4_real_models)
  add_aie4_real_model_test("${model}" "aie4_skip_elftolegacy")
endforeach()

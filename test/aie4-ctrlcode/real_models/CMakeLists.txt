# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

# DMA_compiler: <model>/config.json (asm cwd = model dir).
# X3: <model>/Work/ps/elf/aiebu_config.json (asm cwd = Work/ps/elf).
# Shared prebuilt asm: test/aie4-ctrlcode/real_models/prebuilt (via DMA_compiler/prebuilt).
# If missing, link from build/real_test/prebuilt (see copy_models.sh).

set(_aie4_prebuilt_dir "${CMAKE_CURRENT_SOURCE_DIR}/prebuilt")

set(_aie4_dma_prebuilt "${CMAKE_CURRENT_SOURCE_DIR}/DMA_compiler/prebuilt")
if(EXISTS "${_aie4_prebuilt_dir}" AND NOT EXISTS "${_aie4_dma_prebuilt}")
  file(CREATE_LINK "../prebuilt" "${_aie4_dma_prebuilt}" SYMBOLIC)
endif()

set(_aie4_real_models
  DMA_compiler/test_1
)

# No elf_to_legacy / control.txt gold (output too large for CI).
set(_aie4_skip_elftolegacy
  X3/test_10
)

# Use a function (not a macro): return() only exits this function, not the foreach caller.
function(add_aie4_real_model_test MODEL SKIP_ELFTOLEGACY_VAR)
  set(_model_dir "${CMAKE_CURRENT_SOURCE_DIR}/${MODEL}")
  set(_config "${_model_dir}/config.json")
  set(_asm_work_dir "${_model_dir}")

  if(NOT EXISTS "${_config}")
    set(_config "${_model_dir}/Work/ps/elf/aiebu_config.json")
    set(_asm_work_dir "${_model_dir}/Work/ps/elf")
  endif()

  if(NOT EXISTS "${_config}")
    message(STATUS "aie4 real_models: skipping ${MODEL} (no config.json or Work/ps/elf/aiebu_config.json)")
    return()
  endif()

  string(REPLACE "/" "_" _model_id "${MODEL}")
  set(_test_bin "${CMAKE_CURRENT_BINARY_DIR}/${_model_id}")
  file(MAKE_DIRECTORY "${_test_bin}")

  set(_test_prefix "aie4_real_${_model_id}")
  set(_elf_name "control.elf")
  set(_elf "${_test_bin}/${_elf_name}")

  add_test(NAME "${_test_prefix}_asm"
    COMMAND aiebu-asm -t aie4_config -j "${_config}" -o "${_elf}" -f disabledump
    WORKING_DIRECTORY "${_asm_work_dir}")

  add_test(NAME "${_test_prefix}_md5sum"
    COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
            "${_elf}" "${_model_dir}/gold.md5"
    WORKING_DIRECTORY "${_test_bin}")

  set_tests_properties("${_test_prefix}_md5sum" PROPERTIES DEPENDS "${_test_prefix}_asm")
  set_tests_properties("${_test_prefix}_asm" PROPERTIES LABELS "benchmark")

  if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(_skip_elftolegacy "${${SKIP_ELFTOLEGACY_VAR}}")
    list(FIND _skip_elftolegacy "${MODEL}" _aie4_skip_elftolegacy_idx)
    if(_aie4_skip_elftolegacy_idx EQUAL -1)
      add_test(NAME "${_test_prefix}_elftolegacy"
        COMMAND "${AIEBU_SOURCE_DIR}/src/python/aiebu/elf_to_legacy.py"
                -o "${_test_bin}/control.out.txt" -i "${_elf}"
        WORKING_DIRECTORY "${_test_bin}")

      add_test(NAME "${_test_prefix}_elftolegacy_compare"
        COMMAND ${CMAKE_COMMAND} -E compare_files
                "${_test_bin}/control.out.txt" "${_model_dir}/control.txt"
        WORKING_DIRECTORY "${_test_bin}")

      set_tests_properties("${_test_prefix}_elftolegacy"
        PROPERTIES DEPENDS "${_test_prefix}_asm"
        ENVIRONMENT "PYTHONPATH=${AIEBU_SOURCE_DIR}/src/python")
      set_tests_properties("${_test_prefix}_elftolegacy_compare"
        PROPERTIES DEPENDS "${_test_prefix}_elftolegacy")
    endif()

    add_test(NAME "${_test_prefix}_readelf"
      COMMAND "${AIEBU_SOURCE_DIR}/test/aie2ps-ctrlcode/run-readelf.sh"
              "${_elf}" "${_test_bin}/control.elf.txt"
      WORKING_DIRECTORY "${_test_bin}")

    add_test(NAME "${_test_prefix}_readelf_compare"
      COMMAND ${CMAKE_COMMAND} -E compare_files
              "${_test_bin}/control.elf.txt" "${_model_dir}/control.gold"
      WORKING_DIRECTORY "${_test_bin}")

    set_tests_properties("${_test_prefix}_readelf" PROPERTIES DEPENDS "${_test_prefix}_asm")
    set_tests_properties("${_test_prefix}_readelf_compare" PROPERTIES DEPENDS "${_test_prefix}_readelf")
  endif()
endfunction()

foreach(_model IN LISTS _aie4_real_models)
  add_aie4_real_model_test("${_model}" _aie4_skip_elftolegacy)
endforeach()

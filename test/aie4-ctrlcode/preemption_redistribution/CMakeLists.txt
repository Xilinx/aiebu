# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

find_package(Python3 COMPONENTS Interpreter REQUIRED)

macro(add_preemption_redistribution_read_bd_tests TEST_NAME ELF_NAME TEST_BIN TEST_DIR)
  add_test(NAME "${TEST_NAME}_read_bd"
    COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/run.cmake" -o read_bd_output.txt
            -x ${Python3_EXECUTABLE} --
            "${AIEBU_SOURCE_DIR}/src/python/aiebu/read_bd.py"
            "${ELF_NAME}"
    WORKING_DIRECTORY "${TEST_BIN}")

  add_test(NAME "${TEST_NAME}_read_bd_compare"
    COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
            "${TEST_BIN}/read_bd_output.txt"
            "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DIR}/gold_read_bd.txt"
    WORKING_DIRECTORY "${TEST_BIN}")

  set_tests_properties("${TEST_NAME}_read_bd" PROPERTIES DEPENDS "${TEST_NAME}_asm")
  set_tests_properties("${TEST_NAME}_read_bd_compare" PROPERTIES DEPENDS "${TEST_NAME}_read_bd")
endmacro()

macro(add_preemption_redistribution_pos_test TEST_DIR TEST_NAME)
  set(TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DIR}/test.asm")
  set(TEST_BIN "${CMAKE_CURRENT_BINARY_DIR}/${TEST_DIR}")
  set(ELF_NAME "${TEST_NAME}.elf")
  file(MAKE_DIRECTORY "${TEST_BIN}")

  add_test(NAME "${TEST_NAME}_asm"
    COMMAND aiebu-asm -t aie4 -c "${TEST_SRC}" -L "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DIR}"
            -o "${ELF_NAME}" -f disabledump
    WORKING_DIRECTORY "${TEST_BIN}")

  add_test(NAME "${TEST_NAME}_md5sum"
    COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
            "${TEST_BIN}/${ELF_NAME}"
            "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DIR}/gold.md5"
    WORKING_DIRECTORY "${TEST_BIN}")

  set_tests_properties("${TEST_NAME}_md5sum" PROPERTIES DEPENDS "${TEST_NAME}_asm")
  set_tests_properties("${TEST_NAME}_asm" PROPERTIES LABELS memcheck)

  add_preemption_redistribution_read_bd_tests("${TEST_NAME}" "${ELF_NAME}" "${TEST_BIN}" "${TEST_DIR}")
endmacro()

macro(add_preemption_redistribution_config_pos_test TEST_DIR TEST_NAME ELF_NAME CONFIG_FILE)
  set(TEST_BIN "${CMAKE_CURRENT_BINARY_DIR}/${TEST_DIR}")
  file(MAKE_DIRECTORY "${TEST_BIN}")

  add_test(NAME "${TEST_NAME}_asm"
    COMMAND aiebu-asm -t aie4_config -j "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DIR}/${CONFIG_FILE}"
            -o "${ELF_NAME}" -f disabledump
    WORKING_DIRECTORY "${TEST_BIN}")

  add_test(NAME "${TEST_NAME}_md5sum"
    COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
            "${TEST_BIN}/${ELF_NAME}"
            "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DIR}/gold.md5"
    WORKING_DIRECTORY "${TEST_BIN}")

  set_tests_properties("${TEST_NAME}_md5sum" PROPERTIES DEPENDS "${TEST_NAME}_asm")
  set_tests_properties("${TEST_NAME}_asm" PROPERTIES LABELS memcheck)

  add_preemption_redistribution_read_bd_tests("${TEST_NAME}" "${ELF_NAME}" "${TEST_BIN}" "${TEST_DIR}")
endmacro()

macro(add_preemption_redistribution_neg_test TEST_DIR TEST_NAME)
  set(TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DIR}/test.asm")
  set(TEST_BIN "${CMAKE_CURRENT_BINARY_DIR}/${TEST_DIR}")
  file(MAKE_DIRECTORY "${TEST_BIN}")

  add_test(NAME "${TEST_NAME}_asm"
    COMMAND aiebu-asm -t aie4 -c "${TEST_SRC}" -L "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DIR}"
            -o "${TEST_NAME}.elf" -f disabledump
    WORKING_DIRECTORY "${TEST_BIN}")

  set_tests_properties("${TEST_NAME}_asm" PROPERTIES WILL_FAIL TRUE)
endmacro()

add_preemption_redistribution_config_pos_test(
  test1 aie4_preemption_redistribution_test1 preemption-redistribution-test1.elf config.json)

add_preemption_redistribution_pos_test(test3_disjoint_direct aie4_preempt_redist_test3_disjoint_direct)
add_preemption_redistribution_neg_test(test4_set_bit_overlap aie4_preempt_redist_test4_set_bit_overlap)
add_preemption_redistribution_pos_test(test5_zero_hintmap_absorb aie4_preempt_redist_test5_zero_hintmap_absorb)
add_preemption_redistribution_pos_test(test6_cross_word_hole aie4_preempt_redist_test6_cross_word_hole)
add_preemption_redistribution_pos_test(test7_bit_at_143 aie4_preempt_redist_test7_bit_at_143)
add_preemption_redistribution_pos_test(test8_interleaved_three_col aie4_preempt_redist_test8_interleaved_three_col)
add_preemption_redistribution_pos_test(test9_all_no_hintmap aie4_preempt_redist_test9_all_no_hintmap)
add_preemption_redistribution_pos_test(test10_multi_pt_mixed aie4_preempt_redist_test10_multi_pt_mixed)
add_preemption_redistribution_pos_test(test13 aie4_preempt_redist_test13)
add_preemption_redistribution_neg_test(test11_bit_144_error aie4_preempt_redist_test11_bit_144_error)
add_preemption_redistribution_neg_test(test12_fixed_window_set_overlap aie4_preempt_redist_test12_fixed_window_set_overlap)

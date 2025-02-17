# SPDX-License-Identifier: MIT
# Copyright (C) 2023 Advanced Micro Devices, Inc.

file(COPY "${AIEBU_SOURCE_DIR}/.pylintrc" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB tfiles "${CMAKE_CURRENT_SOURCE_DIR}/*.asm")

set(yamlfile ${AIEBU_SOURCE_DIR}/specification/isa-spec.yaml)

foreach(tfile ${tfiles})

  cmake_path(GET tfile FILENAME sampleasm)
  cmake_path(GET tfile STEM sample)
  set(sampletxt "${sample}.txt")
  set(sampleelf "${sample}.elf")
  set(samplecppelf "${sample}_cpp.elf")
  set(sampleelftext "${sample}.elf.txt")
  set(sampleouttxt "${sample}.out.txt")
  set(samplecppouttxt "${sample}_cpp.out.txt")
  set(samplegold "${sample}.gold")
  set(sampledisasm "${sample}.dis.asm")

  add_test(NAME ${sample}
    COMMAND "${AIEBU_SOURCE_DIR}/src/python/aiebu/control_asm_disasm.py" --disable_dump_map "${tfile}" -o "${sampleelf}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  add_test(NAME ${sample}_cpp
    COMMAND "${AIEBU_BINARY_DIR}/opt/xilinx/aiebu/bin/aiebu-asm"  -t aie2ps -c "${tfile}" -o "${samplecppelf}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  add_test(NAME ${sample}_elftolegacy
    COMMAND "${AIEBU_SOURCE_DIR}/src/python/aiebu/elf_to_legacy.py" "-o" "${sampleouttxt}" "-i" "${sampleelf}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  add_test(NAME ${sample}_elftolegacy_cpp
    COMMAND "${AIEBU_SOURCE_DIR}/src/python/aiebu/elf_to_legacy.py" "-o" "${samplecppouttxt}" "-i" "${samplecppelf}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  add_test(NAME "${sample}_compare"
    COMMAND ${CMAKE_COMMAND} -E compare_files "${sampleouttxt}" "${CMAKE_CURRENT_SOURCE_DIR}/${sampletxt}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  add_test(NAME "${sample}_compare_cpp"
    COMMAND ${CMAKE_COMMAND} -E compare_files "${samplecppouttxt}" "${CMAKE_CURRENT_SOURCE_DIR}/${sampletxt}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  add_test(NAME "${sample}_readelf"
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/run-readelf.sh" "${sampleelf}" "${sampleelftext}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  add_test(NAME "${sample}_compareelf"
    COMMAND ${CMAKE_COMMAND} -E compare_files "${sampleelftext}" "${CMAKE_CURRENT_SOURCE_DIR}/${samplegold}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  add_test(NAME "${sample}_disassemble"
    COMMAND "${AIEBU_SOURCE_DIR}/src/python/aiebu/control_asm_disasm.py" "-d" "${sampleelf}" "-o" "${sampledisasm}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  set_tests_properties(${sample} "${sample}_elftolegacy" "${sample}_disassemble" PROPERTIES ENVIRONMENT
    "PYTHONPATH=${AIEBU_SOURCE_DIR}")

endforeach()

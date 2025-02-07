# SPDX-License-Identifier: MIT
# Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.
add_library(aiebu_library_objects OBJECT
  analyzer/reporter.cpp
  analyzer/transaction.cpp
  assembler/aiebu_assembler.cpp
  assembler/assembler.cpp
  common/aiebu_error.cpp
  common/writer.cpp
  common/assembler_state.cpp
  elf/elfwriter.cpp
  preprocessor/aie2/aie2_blob_preprocessor_input.cpp
  preprocessor/aie2/aie2_asm_preprocessor_input.cpp
  preprocessor/asm/asm_parser.cpp
  ops/ops.cpp
  )

target_include_directories(aiebu_library_objects
  PRIVATE
  ${AIEBU_AIE_RT_HEADER_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${AIEBU_BINARY_DIR}/lib
  ${AIEBU_SOURCE_DIR}/src/cpp/ELFIO

  # The following should be spelled out in code
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/preprocessor
  ${CMAKE_CURRENT_SOURCE_DIR}/preprocessor/aie2
  ${CMAKE_CURRENT_SOURCE_DIR}/encoder
  ${CMAKE_CURRENT_SOURCE_DIR}/encoder/aie2
  ${CMAKE_CURRENT_SOURCE_DIR}/common
  ${CMAKE_CURRENT_SOURCE_DIR}/ops
  ${CMAKE_CURRENT_SOURCE_DIR}/analyzer
  ${CMAKE_CURRENT_SOURCE_DIR}/assembler
  ${CMAKE_CURRENT_SOURCE_DIR}/elf
  ${CMAKE_CURRENT_SOURCE_DIR}/elf/aie2
  ${AIEBU_BINARY_DIR}/lib/gen
  )

set_target_properties(aiebu_library_objects PROPERTIES
  CXX_VISIBILITY_PRESET hidden
  POSITION_INDEPENDENT_CODE ON
  )

add_dependencies(aiebu_library_objects ctrlcodelib)

if (AIEBU_FULL STREQUAL "ON")
  target_sources(aiebu_library_objects
    PRIVATE
    encoder/aie2ps/aie2ps_encoder.cpp
    preprocessor/aie2ps/aie2ps_preprocessor_input.cpp
    preprocessor/asm/pager.cpp
    )
  target_include_directories(aiebu_library_objects
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/preprocessor/aie2ps
    ${CMAKE_CURRENT_SOURCE_DIR}/encoder/aie2ps
    ${CMAKE_CURRENT_SOURCE_DIR}/elf/aie2ps
    ${CMAKE_CURRENT_BINARY_DIR}/../../../python/
    )
  add_dependencies(aiebu_library_objects cpp-assembler-stubs)
endif()

add_library(aiebu SHARED
  $<TARGET_OBJECTS:aiebu_library_objects>
  )

add_library(aiebu_static STATIC
  $<TARGET_OBJECTS:aiebu_library_objects>
  )

target_link_libraries(aiebu xaiengine)

if (MSVC)
  target_link_libraries(aiebu advapi32)
  target_link_libraries(aiebu_static advapi32)
endif()

install (TARGETS aiebu
  LIBRARY DESTINATION ${AIEBU_INSTALL_LIB_DIR}
  RUNTIME DESTINATION ${AIEBU_INSTALL_BIN_DIR}
)

install (TARGETS aiebu aiebu_static
  ARCHIVE DESTINATION ${AIEBU_INSTALL_LIB_DIR}
  LIBRARY DESTINATION ${AIEBU_INSTALL_LIB_DIR}
)

install(FILES
  include/aiebu.h
  include/aiebu_assembler.h
  include/aiebu_error.h
  DESTINATION ${AIEBU_INSTALL_INCLUDE_DIR}
  CONFIGURATIONS Debug Release COMPONENT Runtime
)

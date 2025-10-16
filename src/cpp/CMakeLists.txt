# SPDX-License-Identifier: MIT
# Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

# In arm compiler we dont have static libs so not building utility
if (AIEBU_NATIVE_BUILD)
  add_subdirectory(utils)
endif()

add_library(aiebu_library_objects OBJECT
  analyzer/reporter.cpp
  analyzer/transaction.cpp
  analyzer/packets.cpp
  analyzer/passmanager.cpp
  analyzer/aie2p_passes.cpp
  assembler/aiebu_assembler.cpp
  assembler/assembler.cpp
  common/aiebu_error.cpp
  common/writer.cpp
  common/assembler_state.cpp
  common/utils.cpp
  common/file_utils.cpp
  elf/elfwriter.cpp
  preprocessor/aie2/aie2_blob_preprocessor_input.cpp
  preprocessor/aie2/aie2_asm_preprocessor_input.cpp
  preprocessor/asm/asm_parser.cpp
  ops/ops.cpp
  disassembler/disassembler.cpp
  )

# Remove globle includes in case of submodule XRT as there are conflicts
# in version.h and utils.h
set_target_properties(aiebu_library_objects PROPERTIES
  INCLUDE_DIRECTORIES ""  # Clear inherited dirs
  )

target_include_directories(aiebu_library_objects
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${AIEBU_SOURCE_DIR}
  ${AIEBU_ELFIO_SRC_DIR}
  ${AIEBU_AIE-RT_HEADER_DIR}
  ${AIEBU_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
  ${Boost_INCLUDE_DIRS}

  # The following should be spelled out in code
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
  ${CMAKE_CURRENT_SOURCE_DIR}/disassembler
  ${AIEBU_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/gen
  )

target_compile_definitions(aiebu_library_objects
  PRIVATE
  -DAIEBU_EXPORTS
  )

if (AIEBU_NATIVE_BUILD)
  target_compile_definitions(aiebu_library_objects
    PRIVATE
    AIEBU_NATIVE_BUILD=1
    )

  add_dependencies(aiebu_library_objects ctrlcodelib)
endif()

set_target_properties(aiebu_library_objects PROPERTIES
  CXX_VISIBILITY_PRESET hidden
  POSITION_INDEPENDENT_CODE ON
  )

target_sources(aiebu_library_objects
  PRIVATE
  encoder/aie2ps/aie2ps_encoder.cpp
  encoder/aie2ps/report.cpp
  preprocessor/aie2ps/aie2ps_preprocessor_input.cpp
  preprocessor/asm/pager.cpp
  disassembler/disassembler.cpp
  )
target_include_directories(aiebu_library_objects
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/preprocessor/aie2ps
  ${CMAKE_CURRENT_SOURCE_DIR}/encoder/aie2ps
  ${CMAKE_CURRENT_SOURCE_DIR}/elf/aie2ps
  ${CMAKE_CURRENT_SOURCE_DIR}/disassembler
  ${CMAKE_CURRENT_BINARY_DIR}
  )

add_library(aiebu_static STATIC
  $<TARGET_OBJECTS:aiebu_library_objects>
  )

if (MSVC)
  target_link_libraries(aiebu_static advapi32)
else()
  # On linux use xrt_coreutil.a
  set_target_properties(aiebu_static PROPERTIES OUTPUT_NAME "aiebu")
endif()

install (TARGETS aiebu_static
  EXPORT aiebu-targets
  LIBRARY DESTINATION ${AIEBU_INSTALL_LIB_DIR} COMPONENT ${AIEBU_COMPONENT} NAMELINK_COMPONENT ${AIEBU_DEV_COMPONENT}
  ARCHIVE DESTINATION ${AIEBU_INSTALL_LIB_DIR} COMPONENT ${AIEBU_DEV_COMPONENT}
)

install(FILES
  include/aiebu/aiebu.h
  include/aiebu/aiebu_assembler.h
  include/aiebu/aiebu_error.h
  DESTINATION ${AIEBU_INSTALL_INCLUDE_DIR}/aiebu
  CONFIGURATIONS Debug Release COMPONENT ${AIEBU_DEV_COMPONENT}
)

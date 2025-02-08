# SPDX-License-Identifier: MIT
# Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.
add_library(aiebu_asm_objects OBJECT
  asm.cpp
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/target/target.cpp
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/common/utils.cpp
  )

target_include_directories(aiebu_asm_objects
  PRIVATE
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src
  ${Boost_INCLUDE_DIRS}

  # these should be spelled out in source code
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/assembler
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/common
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/target
  ${AIEBU_SOURCE_DIR}/src/cpp/cxxopts/include
  )

add_executable(aiebu-asm $<TARGET_OBJECTS:aiebu_asm_objects>)
target_link_libraries(aiebu-asm PRIVATE aiebu_static)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_link_options(aiebu-asm PRIVATE "-static")
  set_target_properties(aiebu-asm PROPERTIES INSTALL_RPATH "" BUILD_RPATH "")

  # Create a dynamically linked executable. aiebu-asm-dyn, on Linux for running
  # valgrind, etc. This binary is not released for deployment but only used for
  # internal testing. Note that valgrind known to report many glibc related
  # errors when running with fully statically linked executables, hence the
  # dynamically linked binary for testing
  add_executable(aiebu-asm-dyn $<TARGET_OBJECTS:aiebu_asm_objects>)
  target_link_libraries(aiebu-asm-dyn PRIVATE aiebu_static)
endif()

# This custom target fails if aiebu-asm has any dynamic dependencies
add_custom_target(check_dynamic_deps ALL
  COMMAND ${CMAKE_COMMAND} -E echo "Checking for dynamic dependencies ..."
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/depends.cmake" $<TARGET_FILE:aiebu-asm> aiebu-asm_depends.txt
  DEPENDS aiebu-asm
  )

install(TARGETS aiebu-asm
  RUNTIME DESTINATION ${AIEBU_INSTALL_BIN_DIR}
  CONFIGURATIONS Debug Release COMPONENT Runtime
)

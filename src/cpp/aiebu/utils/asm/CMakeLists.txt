# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

set(GENERATOR_OUT_DIR "${AIEBU_BINARY_DIR}/lib/gen")
set(AIEBU_INSTALL_BINARY_DIR "${AIEBU_INSTALL_DIR}/bin/")
set(ASM_TGT "aiebu-asm")
list(APPEND TGT_LIST ${ASM_TGT})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  # Create a dynamically linked executable. aiebu-asm-dyn, on Linux for running
  # valgrind, etc. This binary is not released for deployment but only used for
  # internal testing. Note that valgrind known to report many glibc related errors
  # when running with fully statically linked executables, hence the dynamically
  # linked binary for testing
  set(ASM_TGT_DYN "aiebu-asm-dyn")
  list(APPEND TGT_LIST ${ASM_TGT_DYN})
endif()

set(TARGET_DEPENDS_LIST_FILE "${ASM_TGT}_depends.txt")

foreach(TGT ${TGT_LIST})
  add_executable(${TGT}
    asm.cpp
    ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/target/target.cpp
    ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/common/utils.cpp
  )

  if ((${CMAKE_SYSTEM_NAME} STREQUAL "Linux") AND (${TGT} STREQUAL "aiebu-asm"))
    # Static linking on Linux
    target_link_options(${TGT} PRIVATE "-static")
    set_target_properties(${ASM_TGT} PROPERTIES INSTALL_RPATH "" BUILD_RPATH "")
  endif()

  if (MSVC AND (NOT (AIEBU_MSVC_LEGACY_LINKING)))
    # Hybrid linking on Windows
    target_compile_options(${TGT}
      PRIVATE
      /MT$<$<CONFIG:Debug>:d>  # static linking with the CRT
      )
    target_link_options(${TGT}
      PRIVATE
      /NODEFAULTLIB:libucrt$<$<CONFIG:Debug>:d>.lib  # Hybrid CRT
      /DEFAULTLIB:ucrt$<$<CONFIG:Debug>:d>.lib       # Hybrid CRT
      )
  endif()

  target_link_libraries(${TGT}
    PRIVATE
    aiebu_static
  )

  target_include_directories(${TGT} PRIVATE
    ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include
    ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/assembler/
    ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/common/
    ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/target/
    ${AIEBU_SOURCE_DIR}/src/cpp/cxxopts/include/
    ${Boost_INCLUDE_DIRS}
  )
endforeach()

add_custom_command(
  OUTPUT "${TARGET_DEPENDS_LIST_FILE}"
  COMMAND "${CMAKE_COMMAND}" -P "${AIEBU_SOURCE_DIR}/cmake/depends.cmake" $<TARGET_FILE:${ASM_TGT}> "${TARGET_DEPENDS_LIST_FILE}"
  DEPENDS ${ASM_TGT}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(
  dependencylist ALL
  DEPENDS ${TARGET_DEPENDS_LIST_FILE}
)

install(TARGETS ${ASM_TGT}
  RUNTIME DESTINATION ${AIEBU_INSTALL_BINARY_DIR}
  CONFIGURATIONS Debug Release COMPONENT Runtime
)

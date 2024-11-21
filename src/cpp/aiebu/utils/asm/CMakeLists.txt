# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

set(GENERATOR_OUT_DIR "${AIEBU_BINARY_DIR}/lib/gen")
set(AIEBU_INSTALL_BINARY_DIR "${AIEBU_INSTALL_DIR}/bin/")
set(ASM_TGT "aiebu-asm")

set(TARGET_DEPENDS_LIST_FILE "${ASM_TGT}_depends.txt")

add_executable(${ASM_TGT}
  asm.cpp
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/target/target.cpp
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/common/utils.cpp
  )

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  # Static linking on Linux
  target_link_options(${ASM_TGT} PRIVATE "-static")
endif()

if (MSVC)
  # Hybrid linking on Windows
  target_compile_options(${ASM_TGT}
    PRIVATE
    $<$<CONFIG:Debug>:/MTd>
    $<$<CONFIG:Release>:/MT>)
  add_link_options($<$<CONFIG:Debug>:/NODEFAULTLIB:libucrtd.lib>
                   $<$<CONFIG:Debug>:/DEFAULTLIB:ucrtd.lib>
                   $<$<CONFIG:Release>:/NODEFAULTLIB:libucrt.lib>
                   $<$<CONFIG:Release>:/DEFAULTLIB:ucrt.lib>)
endif()

target_link_libraries(${ASM_TGT}
  PRIVATE
  aiebu_static
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_link_libraries(${ASM_TGT}
    PRIVATE
    dl
    z
  )
endif()

target_include_directories(${ASM_TGT} PRIVATE
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/assembler/
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/common/
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/target/
  ${AIEBU_SOURCE_DIR}/src/cpp/cxxopts/include/
  ${Boost_INCLUDE_DIRS}
  )

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

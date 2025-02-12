###############################################################################
# Copyright (C) 2024, Advanced Micro Devices, Inc. All Rights Reserved.
# SPDX-License-Identifier: MIT
###############################################################################

set(GENERATOR_EXE1 "preemption")
set(GENERATOR_OUT_DIR "${AIEBU_BINARY_DIR}/lib/gen")
set(AIEBU_INSTALL_AIE_LIB_DIR "${AIEBU_INSTALL_DIR}/lib/aie2")
set(CHECKSUMS_FILE "${GENERATOR_OUT_DIR}/checksums.txt")

file(MAKE_DIRECTORY ${GENERATOR_OUT_DIR})

add_executable(${GENERATOR_EXE1} gen-preemption.cpp)

# Argh, many folks don't know the difference between {0} and {}
if (NOT MSVC)
  target_compile_options(${GENERATOR_EXE1} PRIVATE -Wno-missing-field-initializers)
endif()

target_include_directories(
  ${GENERATOR_EXE1}
  PRIVATE
  ${AIEBU_AIE_RT_HEADER_DIR}
)

target_link_libraries(${GENERATOR_EXE1} xaiengine)

# Copy libxaiengine.dll to the working directory for preemption.exe to find the library on Windows
# when it runs.
# The copy is not required for Linux but I am copying nevertheless as it does not harm
# When aie-rt is available as static library, we will switch to using static linking
add_custom_command(
  OUTPUT ${GENERATOR_OUT_DIR}/preempt_save_phx_4x1.bin
         ${GENERATOR_OUT_DIR}/preempt_save_phx_4x2.bin
         ${GENERATOR_OUT_DIR}/preempt_save_phx_4x4.bin
         ${GENERATOR_OUT_DIR}/preempt_save_stx_4x1.bin
         ${GENERATOR_OUT_DIR}/preempt_save_stx_4x2.bin
         ${GENERATOR_OUT_DIR}/preempt_save_stx_4x4.bin
         ${GENERATOR_OUT_DIR}/preempt_save_stx_4x8.bin
         ${GENERATOR_OUT_DIR}/preempt_restore_phx_4x1.bin
         ${GENERATOR_OUT_DIR}/preempt_restore_phx_4x2.bin
         ${GENERATOR_OUT_DIR}/preempt_restore_phx_4x4.bin
         ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x1.bin
         ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x2.bin
         ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x4.bin
         ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x8.bin
         ${GENERATOR_OUT_DIR}/stx_save_restore_map.h
  COMMAND ${GENERATOR_EXE1}
  DEPENDS ${GENERATOR_EXE1}
  COMMENT "Generating save/restore ctrlcode stubs in directory ${GENERATOR_OUT_DIR}"
  VERBATIM
  WORKING_DIRECTORY ${GENERATOR_OUT_DIR}
)

add_custom_target(
  ctrlcodelib ALL
  DEPENDS ${GENERATOR_OUT_DIR}/preempt_save_stx_4x1.bin
          ${GENERATOR_OUT_DIR}/preempt_save_stx_4x2.bin
          ${GENERATOR_OUT_DIR}/preempt_save_stx_4x4.bin
          ${GENERATOR_OUT_DIR}/preempt_save_stx_4x8.bin
          ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x1.bin
          ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x2.bin
          ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x4.bin
          ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x8.bin
          ${GENERATOR_OUT_DIR}/preempt_save_phx_4x1.bin
          ${GENERATOR_OUT_DIR}/preempt_save_phx_4x2.bin
          ${GENERATOR_OUT_DIR}/preempt_save_phx_4x4.bin
          ${GENERATOR_OUT_DIR}/preempt_restore_phx_4x1.bin
          ${GENERATOR_OUT_DIR}/preempt_restore_phx_4x2.bin
          ${GENERATOR_OUT_DIR}/preempt_restore_phx_4x4.bin
          ${GENERATOR_OUT_DIR}/stx_save_restore_map.h
)

install(FILES ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x1.bin
              ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x2.bin
              ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x4.bin
              ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x8.bin
              ${GENERATOR_OUT_DIR}/preempt_save_stx_4x1.bin
              ${GENERATOR_OUT_DIR}/preempt_save_stx_4x2.bin
              ${GENERATOR_OUT_DIR}/preempt_save_stx_4x4.bin
              ${GENERATOR_OUT_DIR}/preempt_save_stx_4x8.bin
              ${GENERATOR_OUT_DIR}/preempt_save_phx_4x1.bin
              ${GENERATOR_OUT_DIR}/preempt_save_phx_4x2.bin
              ${GENERATOR_OUT_DIR}/preempt_save_phx_4x4.bin
              ${GENERATOR_OUT_DIR}/preempt_restore_phx_4x1.bin
              ${GENERATOR_OUT_DIR}/preempt_restore_phx_4x2.bin
              ${GENERATOR_OUT_DIR}/preempt_restore_phx_4x4.bin
              ${GENERATOR_OUT_DIR}/stx_save_restore_map.h
  DESTINATION ${AIEBU_INSTALL_AIE_LIB_DIR}
  CONFIGURATIONS Debug Release COMPONENT Runtime
)

# For Copy transaction commands and ELF Generation
set(GENERATOR_EXE2 "copy")

add_executable(${GENERATOR_EXE2} gen-copy.cpp)

# Argh, many folks don't know the difference between {0} and {}
if (NOT MSVC)
  target_compile_options(${GENERATOR_EXE2} PRIVATE -Wno-missing-field-initializers)
endif()

target_include_directories(
  ${GENERATOR_EXE2}
  PRIVATE
  ${AIEBU_AIE_RT_HEADER_DIR}
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include
)

target_link_libraries(${GENERATOR_EXE2} xaiengine aiebu_static)

# Copy libxaiengine.dll to the working directory for preemption.exe to find the library on Windows
# when it runs.
# The copy is not required for Linux but I am copying nevertheless as it does not harm
# When aie-rt is available as static library, we will switch to using static linking
add_custom_command(
  OUTPUT ${GENERATOR_OUT_DIR}/copy_Mem_Tile_to_DDR.bin ${GENERATOR_OUT_DIR}/copy_DDR_to_Mem_Tile.bin
  COMMAND ${CMAKE_COMMAND} -E echo "Copying $<TARGET_FILE:aiebu> to ${GENERATOR_OUT_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:aiebu>" "${GENERATOR_OUT_DIR}"
  COMMAND ${GENERATOR_EXE2}
  DEPENDS ${GENERATOR_EXE2}
  COMMENT "Generating copy ctrlcode stubs in directory ${GENERATOR_OUT_DIR}"
  VERBATIM
  WORKING_DIRECTORY ${GENERATOR_OUT_DIR}
)

add_custom_target(
  copyctrlcodelib ALL
  DEPENDS ${GENERATOR_OUT_DIR}/copy_Mem_Tile_to_DDR.bin
          ${GENERATOR_OUT_DIR}/copy_DDR_to_Mem_Tile.bin
)

add_custom_command(
  OUTPUT "${CHECKSUMS_FILE}"
  COMMAND "${CMAKE_COMMAND}" -P "${AIEBU_SOURCE_DIR}/cmake/md5sum.cmake" "${GENERATOR_OUT_DIR}" "${CHECKSUMS_FILE}"
  DEPENDS ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x1.bin
          ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x2.bin
          ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x4.bin
          ${GENERATOR_OUT_DIR}/preempt_restore_stx_4x8.bin
          ${GENERATOR_OUT_DIR}/preempt_save_stx_4x1.bin
          ${GENERATOR_OUT_DIR}/preempt_save_stx_4x2.bin
          ${GENERATOR_OUT_DIR}/preempt_save_stx_4x4.bin
          ${GENERATOR_OUT_DIR}/preempt_save_stx_4x8.bin
          ${GENERATOR_OUT_DIR}/preempt_save_phx_4x1.bin
          ${GENERATOR_OUT_DIR}/preempt_save_phx_4x2.bin
          ${GENERATOR_OUT_DIR}/preempt_save_phx_4x4.bin
          ${GENERATOR_OUT_DIR}/preempt_restore_phx_4x1.bin
          ${GENERATOR_OUT_DIR}/preempt_restore_phx_4x2.bin
          ${GENERATOR_OUT_DIR}/preempt_restore_phx_4x4.bin
          ${GENERATOR_OUT_DIR}/copy_Mem_Tile_to_DDR.bin
          ${GENERATOR_OUT_DIR}/copy_DDR_to_Mem_Tile.bin
          ctrlcodelib
          copyctrlcodelib
)

add_custom_target(
  ctrlcodemd5sum ALL
  DEPENDS ${CHECKSUMS_FILE}
)

install(FILES ${GENERATOR_OUT_DIR}/copy_Mem_Tile_to_DDR.bin ${GENERATOR_OUT_DIR}/copy_DDR_to_Mem_Tile.bin
  DESTINATION ${AIEBU_INSTALL_AIE_LIB_DIR}
  CONFIGURATIONS Debug Release COMPONENT Runtime
)

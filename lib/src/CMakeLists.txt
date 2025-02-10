# SPDX-License-Identifier: MIT
# Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All Rights Reserved.
set(AIEBU_INSTALL_AIE_LIB_DIR "${AIEBU_INSTALL_LIB_DIR}/aie2")

set(AIEBU_PREEMPT_GEN_FILES
  ${AIEBU_GEN_DIR}/preempt_restore_phx_4x1.bin
  ${AIEBU_GEN_DIR}/preempt_restore_phx_4x2.bin
  ${AIEBU_GEN_DIR}/preempt_restore_phx_4x4.bin
  ${AIEBU_GEN_DIR}/preempt_restore_stx_4x1.bin
  ${AIEBU_GEN_DIR}/preempt_restore_stx_4x2.bin
  ${AIEBU_GEN_DIR}/preempt_restore_stx_4x4.bin
  ${AIEBU_GEN_DIR}/preempt_restore_stx_4x8.bin
  ${AIEBU_GEN_DIR}/preempt_save_phx_4x1.bin
  ${AIEBU_GEN_DIR}/preempt_save_phx_4x2.bin
  ${AIEBU_GEN_DIR}/preempt_save_phx_4x4.bin
  ${AIEBU_GEN_DIR}/preempt_save_stx_4x1.bin
  ${AIEBU_GEN_DIR}/preempt_save_stx_4x2.bin
  ${AIEBU_GEN_DIR}/preempt_save_stx_4x4.bin
  ${AIEBU_GEN_DIR}/preempt_save_stx_4x8.bin
  ${AIEBU_GEN_DIR}/stx_save_restore_map.h
  )

set(AIEBU_COPY_GEN_FILES
  ${AIEBU_GEN_DIR}/copy_Mem_Tile_to_DDR.bin
  ${AIEBU_GEN_DIR}/copy_DDR_to_Mem_Tile.bin
  )
  
  
set(AIEBU_PREEMPTION_EXE "preemption")
add_executable(${AIEBU_PREEMPTION_EXE} gen-preemption.cpp)

# Argh, many folks don't know the difference between {0} and {}
if (NOT MSVC)
  target_compile_options(${AIEBU_PREEMPTION_EXE} PRIVATE -Wno-missing-field-initializers)
endif()

target_include_directories(${AIEBU_PREEMPTION_EXE} PRIVATE ${AIEBU_AIE_RT_HEADER_DIR})
target_link_libraries(${AIEBU_PREEMPTION_EXE} xaiengine)

# Create the working directory for custom command generated files
file(MAKE_DIRECTORY ${AIEBU_GEN_DIR})

add_custom_command(
  OUTPUT ${AIEBU_PREEMPT_GEN_FILES}
  COMMAND ${AIEBU_PREEMPTION_EXE}
  DEPENDS ${AIEBU_PREEMPTION_EXE}
  COMMENT "Generating save/restore ctrlcode stubs in directory ${AIEBU_GEN_DIR}"
  VERBATIM
  WORKING_DIRECTORY ${AIEBU_GEN_DIR}
)
add_custom_target(ctrlcodelib DEPENDS ${AIEBU_PREEMPT_GEN_FILES})

install(
  FILES ${AIEBU_PREEMPT_GEN_FILES}
  DESTINATION ${AIEBU_INSTALL_AIE_LIB_DIR}
  CONFIGURATIONS Debug Release
  COMPONENT Runtime
)

# For Copy transaction commands and ELF Generation
set(AIEBU_COPY_EXE "copy")
add_executable(${AIEBU_COPY_EXE} gen-copy.cpp)

# Argh, many folks don't know the difference between {0} and {}
if (NOT MSVC)
  target_compile_options(${AIEBU_COPY_EXE} PRIVATE -Wno-missing-field-initializers)
endif()

target_include_directories(${AIEBU_COPY_EXE}
  PRIVATE
  ${AIEBU_AIE_RT_HEADER_DIR}
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include
)

target_link_libraries(${AIEBU_COPY_EXE} xaiengine aiebu_static)

add_custom_command(
  OUTPUT ${AIEBU_COPY_GEN_FILES}
  COMMAND ${AIEBU_COPY_EXE}
  DEPENDS ${AIEBU_COPY_EXE}
  COMMENT "Generating copy ctrlcode stubs in directory ${AIEBU_GEN_DIR}"
  VERBATIM
  WORKING_DIRECTORY ${AIEBU_GEN_DIR}
)

add_custom_target(copyctrlcodelib DEPENDS ${AIEBU_COPY_GEN_FILES})

install(FILES ${AIEBU_GEN_DIR}/copy_Mem_Tile_to_DDR.bin ${AIEBU_GEN_DIR}/copy_DDR_to_Mem_Tile.bin
  DESTINATION ${AIEBU_INSTALL_AIE_LIB_DIR}
  CONFIGURATIONS Debug Release COMPONENT Runtime
)

set(AIEBU_CHECKSUMS_FILE "${AIEBU_GEN_DIR}/checksums.txt")
add_custom_command(
  OUTPUT ${AIEBU_CHECKSUMS_FILE}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${AIEBU_GEN_DIR}
  COMMAND "${CMAKE_COMMAND}" -P "${AIEBU_SOURCE_DIR}/cmake/md5sum.cmake" "${AIEBU_GEN_DIR}" "${AIEBU_CHECKSUMS_FILE}"
  DEPENDS ${AIEBU_PREEMPT_GEN_FILES} ${AIEBU_COPY_GEN_FILES} ctrlcodelib copyctrlcodelib
)

add_custom_target(ctrlcodemd5sum ALL DEPENDS ${AIEBU_CHECKSUMS_FILE})



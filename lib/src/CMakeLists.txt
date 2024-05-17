###############################################################################
# Copyright (C) 2024, Advanced Micro Devices, Inc. All Rights Reserved.
# SPDX-License-Identifier: MIT
###############################################################################

set(GENERATOR_EXE "preemption")
set(GENERATOR_OUT_DIR "${AIEBU_BINARY_DIR}/lib/gen")
set(AIEBU_INSTALL_AIE_LIB_DIR "${AIEBU_INSTALL_DIR}/lib/aie")
set(CHECKSUMS_FILE "${GENERATOR_OUT_DIR}/checksums.txt")

file(MAKE_DIRECTORY ${GENERATOR_OUT_DIR})

add_executable(${GENERATOR_EXE} preemption.cpp)

target_include_directories(
  ${GENERATOR_EXE}
  PRIVATE
  ${AIEBU_BINARY_DIR}/lib/aie-rt/driver/driver-src/include
  ${AIEBU_SOURCE_DIR}/lib/aie_controller/include/ps
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include
)

target_link_libraries(${GENERATOR_EXE} xaiengine aiebu)

# Copy libxaiengine.dll to the working directory for preemption.exe to find the library on Windows
# when it runs.
# The copy is not required for Linux but I am copying nevertheless as it does not harm
# When aie-rt is available as static library, we will switch to using static linking
add_custom_command(
  OUTPUT ${GENERATOR_OUT_DIR}/preempt_restore_1col.bin ${GENERATOR_OUT_DIR}/preempt_save_1col.bin
  COMMAND ${CMAKE_COMMAND} -E echo "Copying $<TARGET_FILE:xaiengine> to ${GENERATOR_OUT_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:xaiengine>" "${GENERATOR_OUT_DIR}"
  COMMAND ${CMAKE_COMMAND} -E echo "Copying $<TARGET_FILE:aiebu> to ${GENERATOR_OUT_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:aiebu>" "${GENERATOR_OUT_DIR}"
  COMMAND ${GENERATOR_EXE}
  DEPENDS ${GENERATOR_EXE}
  COMMENT "Generating save/restore ctrlcode stubs in directory ${GENERATOR_OUT_DIR}"
  VERBATIM
  WORKING_DIRECTORY ${GENERATOR_OUT_DIR}
)

add_custom_target(
  ctrlcodelib ALL
  DEPENDS ${GENERATOR_OUT_DIR}/preempt_restore_1col.bin
          ${GENERATOR_OUT_DIR}/preempt_save_1col.bin
)

add_custom_command(
  OUTPUT "${CHECKSUMS_FILE}"
  COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_SOURCE_DIR}/md5sum.cmake" "${GENERATOR_OUT_DIR}" "${CHECKSUMS_FILE}"
  DEPENDS ${GENERATOR_OUT_DIR}/preempt_restore_1col.bin
          ${GENERATOR_OUT_DIR}/preempt_save_1col.bin
          ctrlcodelib
)

add_custom_target(
  ctrlcodemd5sum ALL
  DEPENDS ${CHECKSUMS_FILE}
)

install(FILES ${GENERATOR_OUT_DIR}/preempt_restore_1col.bin ${GENERATOR_OUT_DIR}/preempt_save_1col.bin
  DESTINATION ${AIEBU_INSTALL_AIE_LIB_DIR}
  CONFIGURATIONS Debug Release COMPONENT Runtime
)

###############################################################################
# Copyright (C) 2024, Advanced Micro Devices, Inc. All Rights Reserved.
# SPDX-License-Identifier: MIT
###############################################################################

set(GENERATOR_EXE "preemption")
set(GENERATOR_OUT_DIR "${AIEBU_BINARY_DIR}/lib/gen")
set(AIEBU_INSTALL_AIE_LIB_DIR "${AIEBU_INSTALL_DIR}/lib/aie")

file(MAKE_DIRECTORY ${GENERATOR_OUT_DIR})

add_executable(${GENERATOR_EXE} preemption.cpp)

target_include_directories(
  ${GENERATOR_EXE}
  PRIVATE
  ${AIEBU_BINARY_DIR}/lib/aie-rt/driver/driver-src/include
  ${AIEBU_SOURCE_DIR}/lib/aie_controller/include/ps
  ${AIEBU_SOURCE_DIR}/lib/aie_controller/include
)

target_link_libraries(${GENERATOR_EXE} xaiengine)

add_custom_command(
  OUTPUT ${GENERATOR_OUT_DIR}/preempt_restore_1col.bin ${GENERATOR_OUT_DIR}/preempt_save_1col.bin
  COMMAND ${GENERATOR_EXE}
  DEPENDS ${GENERATOR_EXE}
  COMMENT "Generating save/restore ctrlcode stubs in ${GENERATOR_OUT_DIR}"
  VERBATIM
  WORKING_DIRECTORY ${GENERATOR_OUT_DIR}
)

add_custom_target(
  ctrlcodelib ALL
  DEPENDS
  ${GENERATOR_OUT_DIR}/preempt_restore_1col.bin
  ${GENERATOR_OUT_DIR}/preempt_save_1col.bin
)

install(FILES ${GENERATOR_OUT_DIR}/preempt_restore_1col.bin ${GENERATOR_OUT_DIR}/preempt_save_1col.bin
  DESTINATION ${AIEBU_INSTALL_AIE_LIB_DIR}
  CONFIGURATIONS Debug Release COMPONENT Runtime
)

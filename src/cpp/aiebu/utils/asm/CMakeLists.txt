# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

set(AIEBU_INSTALL_BIN_DIR       "${AIEBU_INSTALL_DIR}/bin/")
set(GENERATOR_OUT_DIR "${AIEBU_BINARY_DIR}/lib/gen")
set(AIEBU_INSTALL_AIE_LIB_DIR "${AIEBU_INSTALL_DIR}/lib/aie")

set(TARGET "aiebu-asm")

add_executable(${TARGET}
  asm.cpp
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/target/target.cpp
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/common/utils.cpp
  )

target_link_libraries(${TARGET}
  PRIVATE
  aiebu_static
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  )

target_include_directories(${TARGET} PRIVATE
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/assembler/
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/common/
  ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/utils/target/
  )

install(TARGETS ${TARGET}
  RUNTIME DESTINATION ${AIEBU_INSTALL_BIN_DIR}
  CONFIGURATIONS Debug Release COMPONENT Runtime
)

# SPDX-License-Identifier: MIT
# Copyright (C) 2023 Advanced Micro Devices, Inc.

file(COPY "${AIEBU_SOURCE_DIR}/.pylintrc" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME pylibelf
  COMMAND ${PYLINT} -E "${AIEBU_SOURCE_DIR}/src/python/aiebu/pylibelf"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME ctrlcode
  COMMAND ${PYLINT} -E "${AIEBU_SOURCE_DIR}/src/python/aiebu/ctrlcode"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(
  lint
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${AIEBU_SOURCE_DIR}/src/python" ${PYLINT} --exit-zero "pylibelf"
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${AIEBU_SOURCE_DIR}/src/python" ${PYLINT} --exit-zero "ctrlcode"
  COMMENT "Running ${PYLINT} on ${AIEBU_SOURCE_DIR}/src/python/aiebu/pylibelf and ${AIEBU_SOURCE_DIR}/src/python/aiebu/ctrlcode"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

set_tests_properties(pylibelf ctrlcode PROPERTIES ENVIRONMENT
    "PYTHONPATH=${AIEBU_SOURCE_DIR}/src/python/aiebu")

set(AIEBU_PYTHON_INSTALL_DIR  "${AIEBU_INSTALL_DIR}/lib/python3")

message("-- AIEBU_PYTHON_INSTALL_DIR=${AIEBU_PYTHON_INSTALL_DIR}")

install(DIRECTORY aiebu/ctrlcode aiebu/pylibelf DESTINATION ${AIEBU_PYTHON_INSTALL_DIR} CONFIGURATIONS Debug COMPONENT Runtime PATTERN __pycache__ EXCLUDE )

install(PROGRAMS aiebu/control_asm_disasm.py aiebu/elf_to_legacy.py aiebu/spec_tool.py DESTINATION ${AIEBU_PYTHON_INSTALL_DIR} CONFIGURATIONS Debug COMPONENT Runtime)

install(FILES aiebu/__init__.py DESTINATION ${AIEBU_PYTHON_INSTALL_DIR} CONFIGURATIONS Debug COMPONENT Runtime)

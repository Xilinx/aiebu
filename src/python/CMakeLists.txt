# SPDX-License-Identifier: MIT
# Copyright (C) 2023 Advanced Micro Devices, Inc.

file(COPY "${AIEBU_SOURCE_DIR}/.pylintrc" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  add_test(NAME pylibelf
    COMMAND ${PYLINT} -E "${AIEBU_SOURCE_DIR}/src/python/aiebu/pylibelf"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  # Disable this test on Windows as it is failing in CI pipeline; it passes on my Windows laptop though
  add_test(NAME ctrlcode
    COMMAND ${PYLINT} -E "${AIEBU_SOURCE_DIR}/src/python/aiebu/ctrlcode"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_tests_properties(pylibelf ctrlcode PROPERTIES ENVIRONMENT
    "PYTHONPATH=${AIEBU_SOURCE_DIR}/src/python/aiebu")
endif()

add_custom_target(
  lint
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${AIEBU_SOURCE_DIR}/src/python" ${PYLINT} --exit-zero "pylibelf"
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${AIEBU_SOURCE_DIR}/src/python" ${PYLINT} --exit-zero "ctrlcode"
  COMMENT "Running ${PYLINT} on ${AIEBU_SOURCE_DIR}/src/python/aiebu/pylibelf and ${AIEBU_SOURCE_DIR}/src/python/aiebu/ctrlcode"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

if(NOT SPEC_TOOL_DEPS_DOWNLOADED)
  if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    execute_process(
      COMMAND wget -q -O markdown_graphviz_svg.py https://raw.githubusercontent.com/Tanami/markdown-graphviz-svg/master/markdown_graphviz_svg.py
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )
  else()
    execute_process(
      COMMAND powershell wget -O markdown_graphviz_svg.py https://raw.githubusercontent.com/Tanami/markdown-graphviz-svg/master/markdown_graphviz_svg.py
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )
  endif()
  set(SPEC_TOOL_DEPS_DOWNLOADED TRUE CACHE BOOL "spec_tool.py dependencies downloaded" FORCE)
endif()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/aie2ps)

add_custom_target(py-stubs
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${CMAKE_CURRENT_BINARY_DIR}"
          ${Python3_EXECUTABLE}
          ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
          ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
          ${AIEBU_SOURCE_DIR}/templates/aie2ps generate_py > ${CMAKE_CURRENT_BINARY_DIR}/aie2ps/isa.py
  BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/aie2ps/isa.py
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
          ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
)

set(AIEBU_PYTHON_INSTALL_DIR  "${AIEBU_INSTALL_DIR}/lib/python3")

message("-- AIEBU_PYTHON_INSTALL_DIR=${AIEBU_PYTHON_INSTALL_DIR}")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  install(DIRECTORY aiebu/ctrlcode aiebu/scripts aiebu/pylibelf DESTINATION ${AIEBU_PYTHON_INSTALL_DIR} CONFIGURATIONS Debug Release COMPONENT Runtime PATTERN __pycache__ EXCLUDE )

  install(PROGRAMS aiebu/control_asm_disasm.py DESTINATION ${AIEBU_PYTHON_INSTALL_DIR} CONFIGURATIONS Debug Release COMPONENT Runtime)

  install(FILES aiebu/__init__.py DESTINATION ${AIEBU_PYTHON_INSTALL_DIR} CONFIGURATIONS Debug Release COMPONENT Runtime)
endif()

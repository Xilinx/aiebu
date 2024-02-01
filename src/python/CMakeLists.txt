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

if(NOT SPEC_TOOL_DEPS_DOWNLOADED)
  execute_process(
    COMMAND wget -q -O markdown_graphviz_svg.py https://raw.githubusercontent.com/Tanami/markdown-graphviz-svg/master/markdown_graphviz_svg.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  set(SPEC_TOOL_DEPS_DOWNLOADED TRUE CACHE BOOL "spec_tool.py dependencies downloaded" FORCE)
endif()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/aie2ps)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/aie2)

add_custom_target(docs
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${CMAKE_CURRENT_BINARY_DIR}"
          ${AIEBU_SOURCE_DIR}/src/python/aiebu/spec_tool.py
          ${CMAKE_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
          ${CMAKE_SOURCE_DIR}/templates/aie2ps generate_docs > ${CMAKE_CURRENT_BINARY_DIR}/aie2ps/isa-spec.md
  BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/aie2ps/isa-spec.md
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ${AIEBU_SOURCE_DIR}/src/python/aiebu/spec_tool.py
          ${CMAKE_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
)

add_custom_target(html-docs
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${CMAKE_CURRENT_BINARY_DIR}"
          ${AIEBU_SOURCE_DIR}/src/python/aiebu/spec_tool.py
          ${CMAKE_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
          ${CMAKE_SOURCE_DIR}/templates/aie2ps generate_html_docs > ${CMAKE_CURRENT_BINARY_DIR}/aie2ps/isa-spec.html
  BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/aie2ps/isa-spec.html
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ${AIEBU_SOURCE_DIR}/src/python/aiebu/spec_tool.py
          ${CMAKE_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
)

add_custom_target(c-stubs
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${CMAKE_CURRENT_BINARY_DIR}"
          ${AIEBU_SOURCE_DIR}/src/python/aiebu/spec_tool.py
          ${CMAKE_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
          ${CMAKE_SOURCE_DIR}/templates/aie2ps generate_c_stubs > ${CMAKE_CURRENT_BINARY_DIR}/aie2ps/isa_stubs.h
  BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/aie2ps/isa_stubs.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ${AIEBU_SOURCE_DIR}/src/python/aiebu/spec_tool.py
          ${CMAKE_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
)

add_custom_target(py-stubs
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${CMAKE_CURRENT_BINARY_DIR}"
          ${AIEBU_SOURCE_DIR}/src/python/aiebu/spec_tool.py
          ${CMAKE_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
          ${CMAKE_SOURCE_DIR}/templates/aie2ps generate_py > ${CMAKE_CURRENT_BINARY_DIR}/aie2ps/isa.py
  BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/aie2ps/isa.py
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ${AIEBU_SOURCE_DIR}/src/python/aiebu/spec_tool.py
          ${CMAKE_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
)

add_custom_target(docs-aie2
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${CMAKE_CURRENT_BINARY_DIR}"
          ${AIEBU_SOURCE_DIR}/src/python/aiebu/spec_tool.py
          ${CMAKE_SOURCE_DIR}/specification/aie2/isa-spec.yaml
          ${CMAKE_SOURCE_DIR}/templates/aie2 generate_docs > ${CMAKE_CURRENT_BINARY_DIR}/aie2/isa-spec.md
  BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/aie2/isa-spec.md
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ${AIEBU_SOURCE_DIR}/src/python/aiebu/spec_tool.py
          ${CMAKE_SOURCE_DIR}/specification/aie2/isa-spec.yaml
)

add_custom_target(html-docs-aie2
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${CMAKE_CURRENT_BINARY_DIR}"
          ${AIEBU_SOURCE_DIR}/src/python/aiebu/spec_tool.py
          ${CMAKE_SOURCE_DIR}/specification/aie2/isa-spec.yaml
          ${CMAKE_SOURCE_DIR}/templates/aie2 generate_html_docs > ${CMAKE_CURRENT_BINARY_DIR}/aie2/isa-spec.html
  BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/aie2/isa-spec.html
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ${AIEBU_SOURCE_DIR}/src/python/aiebu/spec_tool.py
          ${CMAKE_SOURCE_DIR}/specification/aie2/isa-spec.yaml
)

add_custom_target(isa-spec ALL)
add_dependencies(isa-spec docs html-docs c-stubs py-stubs docs-aie2 html-docs-aie2)

set(AIEBU_PYTHON_INSTALL_DIR  "${AIEBU_INSTALL_DIR}/lib/python3")

message("-- AIEBU_PYTHON_INSTALL_DIR=${AIEBU_PYTHON_INSTALL_DIR}")

install(DIRECTORY aiebu/ctrlcode aiebu/pylibelf DESTINATION ${AIEBU_PYTHON_INSTALL_DIR} CONFIGURATIONS Debug Release COMPONENT Runtime PATTERN __pycache__ EXCLUDE )

install(PROGRAMS aiebu/control_asm_disasm.py DESTINATION ${AIEBU_PYTHON_INSTALL_DIR} CONFIGURATIONS Debug Release COMPONENT Runtime)

install(FILES aiebu/__init__.py DESTINATION ${AIEBU_PYTHON_INSTALL_DIR} CONFIGURATIONS Debug Release COMPONENT Runtime)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/aie2ps/isa.py
              ${CMAKE_CURRENT_BINARY_DIR}/aie2ps/isa-spec.html
              ${CMAKE_CURRENT_BINARY_DIR}/aie2ps/isa-spec.md
              ${CMAKE_CURRENT_BINARY_DIR}/aie2ps/isa_stubs.h
       DESTINATION ${AIEBU_SPECIFICATION_INSTALL_DIR}/aie2ps
       CONFIGURATIONS Debug Release COMPONENT Runtime
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/aie2/isa-spec.html
              ${CMAKE_CURRENT_BINARY_DIR}/aie2/isa-spec.md
       DESTINATION ${AIEBU_SPECIFICATION_INSTALL_DIR}/aie2
       CONFIGURATIONS Debug Release COMPONENT Runtime
)

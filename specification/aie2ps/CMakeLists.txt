# SPDX-License-Identifier: MIT
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
add_custom_target(cpp-assembler-stubs
  DEPENDS spec-tool-deps
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${SPEC_PYTHON_PATH}"
  ${Python3_EXECUTABLE}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  ${AIEBU_SOURCE_DIR}/templates/aie2ps generate_cpp_assembler_stubs
  > ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa.h
  BYPRODUCTS ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  )

add_custom_target(docs
  DEPENDS spec-tool-deps
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${SPEC_PYTHON_PATH}"
  ${Python3_EXECUTABLE}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  ${AIEBU_SOURCE_DIR}/templates/aie2ps generate_docs
  > ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.md
  BYPRODUCTS ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.md
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  )

add_custom_target(html-docs
  DEPENDS spec-tool-deps
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${SPEC_PYTHON_PATH}"
  ${Python3_EXECUTABLE}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  ${AIEBU_SOURCE_DIR}/templates/aie2ps generate_html_docs
  > ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.html
  BYPRODUCTS ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.html
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  )

add_custom_target(c-stubs
  DEPENDS spec-tool-deps
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${SPEC_PYTHON_PATH}"
  ${Python3_EXECUTABLE}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  ${AIEBU_SOURCE_DIR}/templates/aie2ps generate_c_stubs
  > ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa_stubs.h
  BYPRODUCTS ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa_stubs.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  )

add_custom_target(c-defines
  DEPENDS spec-tool-deps
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${SPEC_PYTHON_PATH}"
  ${Python3_EXECUTABLE}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  ${AIEBU_SOURCE_DIR}/templates/aie2ps generate_c_defines
  > ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa_defines.h
  BYPRODUCTS ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa_defines.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  )

add_dependencies(isa-spec-gen cpp-assembler-stubs docs html-docs c-stubs c-defines)

install(FILES isa-spec.html isa-spec.md isa_stubs.h isa_defines.h isa.h
  DESTINATION ${AIEBU_SPECIFICATION_INSTALL_DIR}/aie2ps
  CONFIGURATIONS Debug Release COMPONENT Runtime
  )

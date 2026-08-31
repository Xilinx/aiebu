# SPDX-License-Identifier: MIT
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
set(AIE2PS_TEMPLATES_DIR ${AIEBU_SOURCE_DIR}/templates/aie2ps)
set(AIE2PS_ASSEMBLER_STUB_TEMPLATE ${AIE2PS_TEMPLATES_DIR}/assembler_stub.h)
set(AIE2PS_C_DEFINES_TEMPLATE ${AIE2PS_TEMPLATES_DIR}/defines.h)
set(AIE2PS_C_STUBS_TEMPLATE ${AIE2PS_TEMPLATES_DIR}/stubs.h)
set(AIE2PS_DOCS_HTML_TEMPLATE ${AIE2PS_TEMPLATES_DIR}/docs.html)
set(AIE2PS_DOCS_MARKDOWN_TEMPLATE ${AIE2PS_TEMPLATES_DIR}/docs.md)

add_custom_command(
  OUTPUT ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa.h
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${SPEC_PYTHON_PATH}"
  ${Python3_EXECUTABLE}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  ${AIE2PS_TEMPLATES_DIR} generate_cpp_assembler_stubs
  > ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa.h
  DEPENDS ${SPEC_MARKDOWN_GRAPHVIZ_SVG}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  ${AIE2PS_ASSEMBLER_STUB_TEMPLATE}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
add_custom_target(cpp-assembler-stubs
  DEPENDS ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa.h
  )

add_custom_command(
  OUTPUT ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.md
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${SPEC_PYTHON_PATH}"
  ${Python3_EXECUTABLE}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  ${AIE2PS_TEMPLATES_DIR} generate_docs
  > ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.md
  DEPENDS ${SPEC_MARKDOWN_GRAPHVIZ_SVG}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  ${AIE2PS_DOCS_MARKDOWN_TEMPLATE}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
add_custom_target(docs
  DEPENDS ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.md
  )

add_custom_command(
  OUTPUT ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.html
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${SPEC_PYTHON_PATH}"
  ${Python3_EXECUTABLE}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  ${AIE2PS_TEMPLATES_DIR} generate_html_docs
  > ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.html
  DEPENDS ${SPEC_MARKDOWN_GRAPHVIZ_SVG}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  ${AIE2PS_DOCS_HTML_TEMPLATE}
  ${AIE2PS_DOCS_MARKDOWN_TEMPLATE}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
add_custom_target(html-docs
  DEPENDS ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.html
  )

add_custom_command(
  OUTPUT ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa_stubs.h
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${SPEC_PYTHON_PATH}"
  ${Python3_EXECUTABLE}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  ${AIE2PS_TEMPLATES_DIR} generate_c_stubs
  > ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa_stubs.h
  DEPENDS ${SPEC_MARKDOWN_GRAPHVIZ_SVG}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  ${AIE2PS_C_STUBS_TEMPLATE}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
add_custom_target(c-stubs
  DEPENDS ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa_stubs.h
  )

add_custom_command(
  OUTPUT ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa_defines.h
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${SPEC_PYTHON_PATH}"
  ${Python3_EXECUTABLE}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  ${AIE2PS_TEMPLATES_DIR} generate_c_defines
  > ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa_defines.h
  DEPENDS ${SPEC_MARKDOWN_GRAPHVIZ_SVG}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa-spec.yaml
  ${AIE2PS_C_DEFINES_TEMPLATE}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
add_custom_target(c-defines
  DEPENDS ${AIEBU_SOURCE_DIR}/specification/aie2ps/isa_defines.h
  )

add_dependencies(isa-spec-gen cpp-assembler-stubs docs html-docs c-stubs c-defines)

# install(FILES isa-spec.html isa-spec.md isa_stubs.h isa_defines.h isa.h
#   DESTINATION ${AIEBU_SPECIFICATION_INSTALL_DIR}/aie2ps
#   CONFIGURATIONS Debug Release
#   COMPONENT ${AIEBU_COMPONENT}
#   )

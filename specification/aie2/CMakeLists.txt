# SPDX-License-Identifier: MIT
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
add_custom_target(docs-aie2
  DEPENDS spec-tool-deps
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${SPEC_PYTHON_PATH}"
  ${Python3_EXECUTABLE}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2/isa-spec.yaml
  ${AIEBU_SOURCE_DIR}/templates/aie2 generate_docs > ${AIEBU_SOURCE_DIR}/specification/aie2/isa-spec.md
  BYPRODUCTS ${AIEBU_SOURCE_DIR}/specification/aie2/isa-spec.md
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2/isa-spec.yaml
  )

add_custom_target(html-docs-aie2
  DEPENDS spec-tool-deps
  COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH="${SPEC_PYTHON_PATH}"
  ${Python3_EXECUTABLE}
  ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2/isa-spec.yaml
  ${AIEBU_SOURCE_DIR}/templates/aie2 generate_html_docs > ${AIEBU_SOURCE_DIR}/specification/aie2/isa-spec.html
  BYPRODUCTS ${AIEBU_SOURCE_DIR}/specification/aie2/isa-spec.html
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ${AIEBU_SOURCE_DIR}/specification/spec_tool.py
  ${AIEBU_SOURCE_DIR}/specification/aie2/isa-spec.yaml
  )

add_dependencies(isa-spec-gen docs-aie2 html-docs-aie2)

install(FILES isa-spec.html isa-spec.md
  DESTINATION ${AIEBU_SPECIFICATION_INSTALL_DIR}/aie2
  CONFIGURATIONS Debug Release COMPONENT Runtime
  )

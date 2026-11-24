# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.

# tree-sitter requires tree-sitter.json next to the generated output;
# copy it from the source directory as part of CMake config
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tree-sitter.json
  ${CMAKE_CURRENT_BINARY_DIR}/tree-sitter.json COPYONLY)

set(TS_GRAMMAR ${CMAKE_CURRENT_SOURCE_DIR}/grammar.js)
set(TS_PARSER ${CMAKE_CURRENT_BINARY_DIR}/src/parser.c)

add_custom_command(
  OUTPUT ${TS_PARSER}
  COMMAND ${TREE_SITTER} generate ${TS_GRAMMAR}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${TS_GRAMMAR} ${CMAKE_CURRENT_BINARY_DIR}/tree-sitter.json
  COMMENT "Generating tree-sitter parser from ${TS_GRAMMAR}"
  VERBATIM
)

add_custom_target(tree-sitter-aie2p-ctrlcode ALL
  DEPENDS ${TS_PARSER}
)

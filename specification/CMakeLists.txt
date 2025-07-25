# SPDX-License-Identifier: MIT
# Copyright (C) 2023 Advanced Micro Devices, Inc.
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  add_custom_target(spec-tool-deps
    COMMAND wget -q -O markdown_graphviz_svg.py
    https://raw.githubusercontent.com/Tanami/markdown-graphviz-svg/master/src/markdown_graphviz_svg/markdown_graphviz_svg.py
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/markdown_graphviz_svg.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
else()
  add_custom_target(spec-tool-deps
    COMMAND powershell wget -O markdown_graphviz_svg.py
    https://raw.githubusercontent.com/Tanami/markdown-graphviz-svg/master/src/markdown_graphviz_svg/markdown_graphviz_svg.py
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/markdown_graphviz_svg.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endif()

set(SPEC_PYTHON_PATH ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(isa-spec-gen)
add_subdirectory(aie2ps)

# install(DIRECTORY aie2ps
#   DESTINATION ${AIEBU_SPECIFICATION_INSTALL_DIR}
#   CONFIGURATIONS Debug Release
#   COMPONENT ${AIEBU_COMPONENT}
#   PATTERN __pycache__ EXCLUDE)

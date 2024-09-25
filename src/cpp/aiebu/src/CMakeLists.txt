# SPDX-License-Identifier: MIT
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

set(AIEBU_INSTALL_LIB_DIR       "${AIEBU_INSTALL_DIR}/lib")
set(AIEBU_INSTALL_INCLUDE_DIR       "${AIEBU_INSTALL_DIR}/include")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE AIEBU_CPP_FILES "*.cpp")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  list(FILTER AIEBU_CPP_FILES EXCLUDE REGEX "windows/*")
endif()

message("-- AIEBU_CPP_FILES: ${AIEBU_CPP_FILES}")

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(aiebu_library_objects OBJECT
  ${AIEBU_CPP_FILES}
  )

target_include_directories(aiebu_library_objects PRIVATE
  ${AIEBU_AIE_RT_HEADER_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/preprocessor
  ${CMAKE_CURRENT_SOURCE_DIR}/preprocessor/aie2
  ${CMAKE_CURRENT_SOURCE_DIR}/encoder
  ${CMAKE_CURRENT_SOURCE_DIR}/encoder/aie2
  ${CMAKE_CURRENT_SOURCE_DIR}/common
  ${CMAKE_CURRENT_SOURCE_DIR}/analyzer
  ${CMAKE_CURRENT_SOURCE_DIR}/assembler
  ${CMAKE_CURRENT_SOURCE_DIR}/elf
  ${CMAKE_CURRENT_SOURCE_DIR}/elf/aie2
  ${AIEBU_ELFIO_SRC_DIR}
  ${Boost_INCLUDE_DIRS}
  )

add_library(aiebu SHARED
  $<TARGET_OBJECTS:aiebu_library_objects>
  )

add_library(aiebu_static STATIC
  $<TARGET_OBJECTS:aiebu_library_objects>
  )

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_link_libraries(aiebu ${OPENSSL_LIBRARIES})
  target_link_libraries(aiebu_static ${OPENSSL_LIBRARIES})
else()
  target_link_libraries(aiebu advapi32)
endif()

target_link_libraries(aiebu xaiengine)
#target_link_libraries(aiebu_static xaiengine)

install (TARGETS aiebu
  LIBRARY DESTINATION ${AIEBU_INSTALL_LIB_DIR}
  RUNTIME DESTINATION ${AIEBU_INSTALL_BIN_DIR}
)

install (TARGETS aiebu aiebu_static
  ARCHIVE DESTINATION ${AIEBU_INSTALL_LIB_DIR}
  LIBRARY DESTINATION ${AIEBU_INSTALL_LIB_DIR}
)

install(FILES include/aiebu.h include/aiebu_assembler.h include/aiebu_error.h
  DESTINATION ${AIEBU_INSTALL_INCLUDE_DIR}
  CONFIGURATIONS Debug Release COMPONENT Runtime
)

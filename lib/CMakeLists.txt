# SPDX-License-Identifier: MIT
# Copyright (C) 2024-2026 Advanced Micro Devices, Inc.

# Clone aie-rt from GitHub in the build directory if AIEBU_AIE-RT_SRC_DIR is not defined or doesn't exist
if (NOT DEFINED AIEBU_AIE-RT_SRC_DIR)
  set(AIEBU_AIE-RT_SRC_DIR ${CMAKE_BINARY_DIR}/lib/aie-rt)
endif()

set(AIE_RT_COMMIT_HASH "c60f889558ed9a82dd650125f813fb40017b1333")

if (NOT EXISTS ${AIEBU_AIE-RT_SRC_DIR})
  message(STATUS "Cloning aie-rt from https://github.com/Xilinx/aie-rt.git to ${AIEBU_AIE-RT_SRC_DIR}")
  find_package(Git REQUIRED)
  # Ensure the parent directory exists
  get_filename_component(AIE_RT_PARENT_DIR ${AIEBU_AIE-RT_SRC_DIR} DIRECTORY)
  file(MAKE_DIRECTORY ${AIE_RT_PARENT_DIR})
  execute_process(
    COMMAND ${GIT_EXECUTABLE} clone https://github.com/Xilinx/aie-rt.git ${AIEBU_AIE-RT_SRC_DIR}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    RESULT_VARIABLE GIT_CLONE_RESULT
    ERROR_VARIABLE GIT_CLONE_ERROR
  )
  if (NOT GIT_CLONE_RESULT EQUAL "0")
    message(FATAL_ERROR "Failed to clone aie-rt repository: ${GIT_CLONE_ERROR}")
  endif()
  message(STATUS "Checking out commit ${AIE_RT_COMMIT_HASH}")
  execute_process(
    COMMAND ${GIT_EXECUTABLE} checkout ${AIE_RT_COMMIT_HASH}
    WORKING_DIRECTORY ${AIEBU_AIE-RT_SRC_DIR}
    RESULT_VARIABLE GIT_CHECKOUT_RESULT
    ERROR_VARIABLE GIT_CHECKOUT_ERROR
  )
  if (NOT GIT_CHECKOUT_RESULT EQUAL "0")
    message(FATAL_ERROR "Failed to checkout commit ${AIE_RT_COMMIT_HASH}: ${GIT_CHECKOUT_ERROR}")
  endif()
  message(STATUS "Successfully cloned aie-rt repository at commit ${AIE_RT_COMMIT_HASH}")
else()
  # Verify that the existing repository is at the correct commit
  find_package(Git REQUIRED)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    WORKING_DIRECTORY ${AIEBU_AIE-RT_SRC_DIR}
    OUTPUT_VARIABLE CURRENT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )
  if (NOT CURRENT_COMMIT STREQUAL "${AIE_RT_COMMIT_HASH}")
    message(STATUS "aie-rt repository exists but is at different commit. Checking out ${AIE_RT_COMMIT_HASH}")
    execute_process(
      COMMAND ${GIT_EXECUTABLE} checkout ${AIE_RT_COMMIT_HASH}
      WORKING_DIRECTORY ${AIEBU_AIE-RT_SRC_DIR}
      RESULT_VARIABLE GIT_CHECKOUT_RESULT
      ERROR_VARIABLE GIT_CHECKOUT_ERROR
    )
    if (NOT GIT_CHECKOUT_RESULT EQUAL "0")
      message(FATAL_ERROR "Failed to checkout commit ${AIE_RT_COMMIT_HASH}: ${GIT_CHECKOUT_ERROR}")
    endif()
    message(STATUS "Successfully checked out commit ${AIE_RT_COMMIT_HASH}")
  endif()
endif()

if (DEFINED AIEBU_AIE-RT_SRC_DIR AND EXISTS ${AIEBU_AIE-RT_SRC_DIR})
  message(STATUS "Building aie-rt from ${AIEBU_AIE-RT_SRC_DIR}")
  set(XAIENGINE_BUILD_SHARED OFF CACHE BOOL "Force static build of xaiengine library" FORCE)
  if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_compile_options("-Wno-error=unused-parameter")
  endif()
  # Build aie-rt driver as part of the main build
  aiebu_add_subdirectory_disable_install_target(${AIEBU_AIE-RT_SRC_DIR}/driver ${AIEBU_BINARY_DIR}/lib/aie-rt/driver)
  message(STATUS "aie-rt driver will be built from ${AIEBU_AIE-RT_SRC_DIR}/driver")
  set(AIEBU_AIE-RT_HEADER_DIR ${AIEBU_AIE-RT_SRC_DIR}/driver/include)
endif()

if (AIEBU_NATIVE_BUILD)
  add_subdirectory(src)
endif()

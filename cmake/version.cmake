# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

# AMD promotion build works from copied sources with no git
# repository.  The build cannot query git for git metadata.  The
# promotion build has explicitly overwritten config/version.h.in and
# config/version.json.in with pre-generated ones.
if (DEFINED ENV{DK_ROOT})

message("-- Skipping Git metadata")

else (DEFINED ENV{DK_ROOT})

# Get the branch
execute_process(
  COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  OUTPUT_VARIABLE AIEBU_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND ${GIT_EXECUTABLE} rev-parse --verify HEAD
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  OUTPUT_VARIABLE AIEBU_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get number of commits for HEAD
execute_process(
  COMMAND ${GIT_EXECUTABLE} rev-list --count HEAD
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  OUTPUT_VARIABLE AIEBU_HEAD_COMMITS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

#Set AIEBU_HEAD_COMMITS to default value if above command is not executed
if (NOT AIEBU_HEAD_COMMITS)
set (AIEBU_HEAD_COMMITS -1)
endif()

# Get number of commits between HEAD and master
execute_process(
  COMMAND ${GIT_EXECUTABLE} rev-list --count HEAD ^origin/main-ge
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  OUTPUT_VARIABLE AIEBU_BRANCH_COMMITS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

#Set AIEBU_BRANCH_COMMITS to default value if above command is not executed
if (NOT AIEBU_BRANCH_COMMITS)
set (AIEBU_BRANCH_COMMITS -1)
endif()

# Get the latest abbreviated commit hash date of the working branch
execute_process(
  COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%cD
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  OUTPUT_VARIABLE AIEBU_HASH_DATE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get all of the modified files in the current git environment
execute_process(
  COMMAND ${GIT_EXECUTABLE} status --porcelain -u no
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  OUTPUT_VARIABLE AIEBU_MODIFIED_FILES
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REPLACE "\n" "," AIEBU_MODIFIED_FILES "${AIEBU_MODIFIED_FILES}")

endif(DEFINED ENV{DK_ROOT})

# Get the build date RFC format
execute_process(
  COMMAND date -R
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  OUTPUT_VARIABLE AIEBU_DATE_RFC
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(TIMESTAMP AIEBU_DATE "%Y-%m-%d %H:%M:%S")

configure_file(
  ${AIEBU_SOURCE_DIR}/cmake/config/version.h.in
  ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/gen/version.h
)

configure_file(
  ${AIEBU_SOURCE_DIR}/cmake/config/version.json.in
  ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/gen/version.json
)

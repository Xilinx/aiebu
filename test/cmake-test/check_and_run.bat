@echo off

REM SPDX-License-Identifier: Apache-2.0
REM Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

set STAGING_DIR=%1
shift

if not exist "%STAGING_DIR%" (
    echo Skipping test because staging dir "%STAGING_DIR%" does not exist
    exit /B 77
)

rem Run the rest of the command
%*

@echo off

REM SPDX-License-Identifier: Apache-2.0
REM Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

REM Usage: check_and_run.bat <dir> <cmd> <args...>

set STAGING_DIR=%1
shift
set CMD=%*

if not exist "%STAGING_DIR%" (
    echo Skipping test because staging dir "%STAGING_DIR%" does not exist
    exit /B 77
)

rem Extract the actual command and its arguments after the first two
for /f "tokens=1* delims= " %%A in ("%CMD%") do (
    set "CMD=%%B"
)

rem Run the rest of the command
%CMD%

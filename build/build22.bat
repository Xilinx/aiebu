
REM SPDX-License-Identifier: MIT
REM Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

set SCRIPTDIR=%~dp0
set BUILDDIR=%SCRIPTDIR%
set NOCMAKE=0

set CMAKEFLAGS=-DMSVC_PARALLEL_JOBS=%LOCAL_MSVC_PARALLEL_JOBS% -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
ECHO CMAKEFLAGS=%CMAKEFLAGS%


@ECHO OFF

set SCRIPTDIR=%~dp0
set BUILDDIR=%SCRIPTDIR%

set DEBUG=1
set RELEASE=1
set CREATE_PACKAGE=0
set CMAKEFLAGS=
set NOCMAKE=0
set NOCTEST=0
set BOOST=C:\Xilinx\XRT\ext

IF DEFINED MSVC_PARALLEL_JOBS ( SET LOCAL_MSVC_PARALLEL_JOBS=%MSVC_PARALLEL_JOBS%) ELSE ( SET LOCAL_MSVC_PARALLEL_JOBS=3 )

:parseArgs
  if [%1] == [] (
    goto argsParsed
  ) else (
  if [%1] == [-clean] (
    goto Clean
  ) else (
  if [%1] == [-help] (
    goto Help
  ) else (
  if [%1] == [-dbg] (
    set RELEASE=0
  ) else (
  if [%1] == [-opt] (
    set DEBUG=0
  ) else (
  if [%1] == [-boost] (
    set BOOST=%2
    shift
  ) else (
  if [%1] == [-pkg] (
    set CREATE_PACKAGE=1
  ) else (
  if [%1] == [-nocmake] (
    set NOCMAKE=1
  ) else (
    echo Unknown option: %1
    goto Help
  ))))))))
  shift
  goto parseArgs

:argsParsed

set CMAKEFLAGS=-DMSVC_PARALLEL_JOBS=%LOCAL_MSVC_PARALLEL_JOBS% -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DBOOST_ROOT=%BOOST%
ECHO CMAKEFLAGS=%CMAKEFLAGS%

if [%DEBUG%] == [1] (
   call :DebugBuild
   if errorlevel 1 (exit /B %errorlevel%)
)

if [%RELEASE%] == [1] (
   call :ReleaseBuild
   if errorlevel 1 (exit /B %errorlevel%)
)

goto :EOF

REM --------------------------------------------------------------------------
:Help
ECHO.
ECHO Usage: build22.bat [options]
ECHO.
ECHO [-help]                    - List this help
ECHO [-clean]                   - Remove build directories
ECHO [-dbg]                     - Creates a debug build
ECHO [-opt]                     - Creates a release build
ECHO [-package]                 - Packages the release build to a MSI archive
ECHO [-boost]                   - Specify the path to the boost install
ECHO                              Note: Depends on the WIX application.
GOTO:EOF

REM --------------------------------------------------------------------------
:Clean
PUSHD %BUILDDIR%
IF EXIST WDebug (
  ECHO Removing 'WDebug' directory...
  rmdir /S /Q WDebug
)
IF EXIST WRelease (
  ECHO Removing 'WRelease' directory...
  rmdir /S /Q WRelease
)
POPD
GOTO:EOF

REM --------------------------------------------------------------------------
:DebugBuild
echo ====================== Windows Debug Build ============================
set CMAKEFLAGS=%CMAKEFLAGS% -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%BUILDDIR%/WDebug/xilinx
ECHO CMAKEFLAGS=%CMAKEFLAGS%

MKDIR %BUILDDIR%\WDebug
PUSHD %BUILDDIR%\WDebug

if [%NOCMAKE%] == [0] (
   cmake -G "Visual Studio 17 2022" %CMAKEFLAGS% ../../
   IF errorlevel 1 (POPD & exit /B %errorlevel%)
)

cmake --build . --verbose --config Debug
IF errorlevel 1 (POPD & exit /B %errorlevel%)

cmake --build . --verbose --config Debug --target run_tests
IF errorlevel 1 (POPD & exit /B %errorlevel%)

cmake --build . --verbose --config Debug --target install
IF errorlevel 1 (POPD & exit /B %errorlevel%)

POPD
GOTO:EOF

REM --------------------------------------------------------------------------
:ReleaseBuild
ECHO ====================== Windows Release Build ============================
set CMAKEFLAGS=%CMAKEFLAGS% -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%BUILDDIR%/WRelease/xilinx
ECHO CMAKEFLAGS=%CMAKEFLAGS%

MKDIR %BUILDDIR%\WRelease
PUSHD %BUILDDIR%\WRelease

if [%NOCMAKE%] == [0] (
   cmake -G "Visual Studio 17 2022" %CMAKEFLAGS% ../../
   IF errorlevel 1 (POPD & exit /B %errorlevel%)
)

cmake --build . --verbose --config Release
IF errorlevel 1 (POPD & exit /B %errorlevel%)

cmake --build . --verbose --config Release --target run_tests
IF errorlevel 1 (POPD & exit /B %errorlevel%)

cmake --build . --verbose --config Release --target install
IF errorlevel 1 (POPD & exit /B %errorlevel%)

ECHO ====================== Zipping up Installation Build ============================
cpack -G ZIP -C Release

if [%CREATE_PACKAGE%]  == [1] (
  ECHO ====================== Creating MSI Archive ============================
  cpack -G WIX -C Release
)

popd
GOTO:EOF

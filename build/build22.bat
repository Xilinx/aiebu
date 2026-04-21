@ECHO OFF

REM SPDX-License-Identifier: MIT
REM Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.
setlocal enabledelayedexpansion
set SCRIPTDIR=%~dp0
set SCRIPTDIR=%SCRIPTDIR:~0,-1%
set BUILDDIR=%SCRIPTDIR%

set DEBUG=1
set RELEASE=1
set CREATE_PACKAGE=0
set CMAKEFLAGS=
set NOCMAKE=0
set NOCTEST=0
set AIEBU_BUILD=""
set BOOST=C:\Xilinx\XRT\ext.new
set GENERATOR="Visual Studio 17 2022"
set PLATFORM=WBuild

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
  if [%1] == [-r] (
    set AIEBU_BUILD="aie2"
  ) else (
  if [%1] == [-p] (
    set AIEBU_BUILD="python"
    set CMAKEFLAGS=%CMAKEFLAGS% -DAIEBU_PYTHON=ON
  ) else (
  if [%1] == [-arm64] (
    set CMAKEFLAGS=%CMAKEFLAGS% -A ARM64 -T host=x64 -DCMAKE_TOOLCHAIN_FILE=%BUILDDIR%/../cmake/arm64.cmake
  ) else (
  if [%1] == [-pkg] (
    set CREATE_PACKAGE=1
  ) else (
  if [%1] == [-nocmake] (
    set NOCMAKE=1
  ) else (
  if [%1] == [-noctest] (
    set NOCTEST=1
  ) else (
    echo Unknown option: %1
    goto Help
  ))))))))))))
  shift
  goto parseArgs

:argsParsed

REM configure
if [%NOCMAKE%] == [0] (
   echo Configuring CMake project

   set CMAKEFLAGS=%CMAKEFLAGS%^
   -DMSVC_PARALLEL_JOBS=%LOCAL_MSVC_PARALLEL_JOBS%^
   -DBOOST_ROOT=%BOOST%^
   -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

   echo cmake -B %BUILDDIR%\%PLATFORM% -G %GENERATOR% !CMAKEFLAGS! %BUILDDIR%\..
   cmake -B %BUILDDIR%\%PLATFORM% -G %GENERATOR% !CMAKEFLAGS! %BUILDDIR%\..
   IF errorlevel 1 (exit /B %errorlevel%)
)

if [%DEBUG%] == [1] (
   echo cmake --build %BUILDDIR%\%PLATFORM% --config Debug --verbose
   cmake --build %BUILDDIR%\%PLATFORM% --config Debug --verbose
   if errorlevel 1 (exit /B %errorlevel%)

   echo cmake --install %BUILDDIR%\%PLATFORM% --config Debug --prefix %BUILDDIR%\%PLATFORM%\Debug\xilinx\aiebu --verbose
   cmake --install %BUILDDIR%\%PLATFORM% --config Debug --prefix %BUILDDIR%\%PLATFORM%\Debug\xilinx\aiebu --verbose
   if errorlevel 1 (exit /B %errorlevel%)

   if [%NOCTEST%] == [0] (
      echo ctest --test-dir %BUILDDIR%\%PLATFORM% -C Debug -j %LOCAL_MSVC_PARALLEL_JOBS%
      ctest --test-dir %BUILDDIR%\%PLATFORM% -C Debug -j %LOCAL_MSVC_PARALLEL_JOBS%
      if errorlevel 1 (exit /B %errorlevel%)
   )
)

if [%RELEASE%] == [1] (
   echo cmake --build %BUILDDIR%\%PLATFORM% --config Release --verbose
   cmake --build %BUILDDIR%\%PLATFORM% --config Release --verbose
   if errorlevel 1 (exit /B %errorlevel%)

   echo cmake --install %BUILDDIR%\%PLATFORM% --config Release --prefix %BUILDDIR%\%PLATFORM%\Release\xilinx\aiebu --verbose
   cmake --install %BUILDDIR%\%PLATFORM% --config Release --prefix %BUILDDIR%\%PLATFORM%\Release\xilinx\aiebu --verbose
   if errorlevel 1 (exit /B %errorlevel%)

   if [%NOCTEST%] == [0] (
      echo ctest --test-dir %BUILDDIR%\%PLATFORM% -C Release -j %LOCAL_MSVC_PARALLEL_JOBS%
      ctest --test-dir %BUILDDIR%\%PLATFORM% -C Release -j %LOCAL_MSVC_PARALLEL_JOBS%
      if errorlevel 1 (exit /B %errorlevel%)
   )

   ECHO ====================== Create SDK ZIP archive ============================
   echo cpack -G ZIP -B %BUILDDIR%\%PLATFORM% -C Release --config %BUILDDIR%\%PLATFORM%\CPackConfig.cmake
   cpack -G ZIP -B %BUILDDIR%\%PLATFORM% -C Release --config %BUILDDIR%\%PLATFORM%\CPackConfig.cmake
   if errorlevel 1 (exit /B %errorlevel%)

   if [%CREATE_PACKAGE%]  == [1] (
      ECHO ====================== Creating MSI Archive ============================
      echo cpack -G WIX -B %BUILDDIR%\%PLATFORM% -C Release --config %BUILDDIR%\%PLATFORM%\CPackConfig.cmake
      cpack -G WIX -B %BUILDDIR%\%PLATFORM% -C Release --config %BUILDDIR%\%PLATFORM%\CPackConfig.cmake
      if errorlevel 1 (exit /B %errorlevel%)
   )
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
ECHO [-package]                 - Packages the release build to a MSI archive.
ECHO [-r]                       - build only aie2
ECHO [-p]                       - build python assembler also
ECHO [-noctest]                 - run no test during build
ECHO                              Note: Depends on the WIX application.
GOTO:EOF

REM --------------------------------------------------------------------------
:Clean
IF EXIST %BUILDDIR%\%PLATFORM% (
  ECHO Removing '%BUILDDIR%\%PLATFORM%' directory...
  rmdir /S /Q %BUILDDIR%\%PLATFORM%
)

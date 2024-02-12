.. _README.rst:

..
    comment:: SPDX-License-Identifier: MIT
    comment:: Copyright (C) 2024 Advanced Micro Devices, Inc.

============================
AIE Binary Utilities (AIEBU)
============================

AIE Binary Utilities for assembler, disassembler, ELF, etc.

Init worksapce, including submodules
====================================

git submodule update --init --recursive


Build dependencies
==================

Compiling requires having installed:
 * cmake >= version 3.18
 * c++17

Build Instruction
=================
works for centos and ubuntu

 * cd build
 * ./build.sh

Test
----
"test/cpp_test" contain sample code to show usage exposed apis(c/cpp).

Test binaries location: opt/xilinx/aiebu/bin/cpp_api and opt/xilinx/aiebu/bin/c_api

Exposed headerfiles/Library
---------------------------
Headerfiles: opt/xilinx/aiebu/include
 * aiebu.h
 * aiebu_assembler.h
 * aiebu_error.h

Libary: opt/xilinx/aiebu/lib
 * libaiebu.so

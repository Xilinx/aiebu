.. _README.rst:

..
    comment:: SPDX-License-Identifier: MIT
    comment:: Copyright (C) 2024 Advanced Micro Devices, Inc.

============================
AIE Binary Utilities (AIEBU)
============================

This repository contains library and utilities to work with AIE *ctrlcode*

Init workspace, including submodules
====================================

::

   git submodule update --init --recursive

Build Dependencies
==================

 * cmake 3.21 or above
 * c++17 compiler
 * Boost
 * Openssl (linux only)
 * ELFIO (included as submodule)
 * aie-rt (included as submodule)

Build Instruction
=================
Linux
-----

::

   cd build
   ./build.sh

Windows
-------

::

   cd build
   ./build22.bat


Test
----
Directory ``test/cpp_test`` contains sample code to show usage of public C/C++ APIs.

Compiled test binaries location:
 * ``opt/xilinx/aiebu/bin/cpp_api``
 * ``opt/xilinx/aiebu/bin/c_api``


Public Header Files
-------------------

Directory ``opt/xilinx/aiebu/include``
 * aiebu.h
 * aiebu_assembler.h
 * aiebu_error.h

Compiled Libraries
------------------

Directory ``opt/xilinx/aiebu/lib``
 * libaiebu.so
 * libaiebu_static.a

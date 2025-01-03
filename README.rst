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

 * cmake 3.18 or above
 * c++17 compiler
 * Boost (header only)
 * cxxopts (included as submodule)
 * ELFIO (included as submodule)
 * AMD aie-rt (included as submodule)

Python Dependencies
-------------------

 * pylint
 * markdown
 * pyyaml
 * Jinja2

Build Instruction
=================
Linux
-----

::

   cd build
   ./build.sh

Windows
-------

aiebu uses *hybrid* linking on Windows which involves static linking with C++ but
dynamic linking with Universal C Runtime. There is no dependency on MS VCRT.

::

   cd build
   ./build22.bat


Test
----
Directories ``test/cpp_test`` and ``test/cmake-test/sample`` contain sample code to show usage of public C/C++ APIs.


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

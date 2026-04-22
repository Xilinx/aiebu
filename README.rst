.. _README.rst:

..
    comment:: SPDX-License-Identifier: MIT
    comment:: Copyright (C) 2024-2026 Advanced Micro Devices, Inc.

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

 * cmake 3.18 or above (3.24 on windows)
 * c++17 compiler
 * Boost (header only) minimum version supported is 1.76
 * cxxopts (included as submodule)
 * ELFIO (included as submodule)

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
Ubuntu minimum supported version is 24.04

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

ELF Format for AIE Architectures
================================

AIEBU generates ELF files for AIE control code. The ELF header contains OS/ABI
and ABI version fields that identify the target architecture and ELF format version.

OS/ABI Values
-------------

The OS/ABI field (``e_ident[EI_OSABI]``) identifies the target AIE architecture.
Values are chosen with high Hamming distance for robustness against bit flips.

.. list-table:: OS/ABI Values
   :header-rows: 1
   :widths: 25 15 60

   * - Architecture
     - Value (Hex)
     - Description
   * - aie2ps_group
     - 0x46 (70)
     - Legacy group ELF for AIE2PS/AIE4 family
   * - aie2p
     - 0x45 (69)
     - AIE2P architecture
   * - aie2ps
     - 0x40 (64)
     - AIE2PS specific (config with ``.target``)
   * - aie4
     - 0x4B (75)
     - AIE4 architecture
   * - aie4a
     - 0x56 (86)
     - AIE4A architecture variant
   * - aie4z
     - 0x69 (105)
     - AIE4Z architecture variant

ELF ABI Version
---------------

The ABI version field (``e_ident[EI_ABIVERSION]``) uses major.minor encoding:
upper nibble = major version, lower nibble = minor version.

.. list-table:: ELF ABI Version Values
   :header-rows: 1
   :widths: 25 15 60

   * - Version
     - Value (Hex)
     - Description
   * - Non-config
     - 0x02 (0.2)
     - Non-config ELF (all architectures)
   * - Legacy config (AIE2PS/AIE4)
     - 0x03 (0.3)
     - Config ELF without ``.target`` directive
   * - AIE2P config (legacy)
     - 0x10 (1.0)
     - Legacy AIE2P config ELF (read-only, no longer written)
   * - Config with .target
     - 0x20 (2.0)
     - Config ELF with ``.target`` directive or AIE2/AIE2P config

Behavior by Target Option
-------------------------

The following table shows how the ``-t`` option and ``.target`` directive
affect ELF version and OS/ABI:

.. list-table:: ELF Output by Target
   :header-rows: 1
   :widths: 30 20 15 35

   * - -t Option
     - .target in ASM
     - ELF Version
     - OS/ABI
   * - aie2ps/aie4 (non-config)
     - Any (ignored)
     - 0x02
     - aie2ps_group (0x46)
   * - aie2ps_config/aie4_config
     - Not present
     - 0x03
     - aie2ps_group (0x46)
   * - aie2ps_config/aie4_config
     - Present
     - 0x20
     - Based on .target value
   * - aie2_config/aie2p_config
     - N/A (binary input)
     - 0x20
     - aie2p (0x45)
   * - aie2/aie2p (non-config)
     - N/A (binary input)
     - 0x02
     - aie2p (0x45)

**Key points:**

* Non-config ELFs always use version 0x02, regardless of ``.target`` directive
* Config ELFs use version 0x20 only when ``.target`` directive is present
* Version 0x20 enables architecture-specific OS/ABI values

Version and OS/ABI Compatibility
--------------------------------

ELF readers use the ABI version to determine valid OS/ABI values:

* **Version 0x02 (non-config)**: OS/ABI must be ``aie2ps_group`` (0x46) or
  ``aie2p`` (0x45). Used for all non-config ELFs.

* **Version 0x03 (legacy config)**: OS/ABI must be ``aie2ps_group`` (0x46).
  Used for AIE2PS/AIE4 config ELFs without ``.target`` directive.

* **Version 0x10 (AIE2P config - legacy)**: OS/ABI must be ``aie2p`` (0x45).
  Legacy config for AIE2P. No longer written by assembler; supported for
  reading existing ELF files only.

* **Version 0x20 (config with .target)**: All OS/ABI values are valid
  (``aie2ps``, ``aie2p``, ``aie2ps_group``, ``aie4``, ``aie4a``, ``aie4z``).
  Used for config ELFs with ``.target`` directive or AIE2/AIE2P config.

The ``.target`` directive in assembly source determines whether the new config
format (version 0x20) is used. For config ELFs, when ``.target`` is present,
the assembler validates it against the ``-t`` command-line option and sets
the specific OS/ABI value. Non-config ELFs always use legacy values regardless
of ``.target`` directive.

Constants Definition
--------------------

All OS/ABI and ELF version constants are defined in a single header file:

``src/cpp/elf/aie_elf_constants.h``

This file contains:

* ``osabi_aie2ps_group`` (0x46) - Legacy group ELF
* ``osabi_aie2p`` (0x45) - AIE2P
* ``osabi_aie2ps`` (0x40) - AIE2PS specific
* ``osabi_aie4`` (0x4B) - AIE4
* ``osabi_aie4a`` (0x56) - AIE4A
* ``osabi_aie4z`` (0x69) - AIE4Z
* ``elf_version_legacy`` (0x02) - Non-config
* ``elf_version_legacy_config`` (0x03) - Legacy config
* ``elf_version_aie2p_config`` (0x10) - Legacy AIE2P config (read-only)
* ``elf_version_config`` (0x20) - Config with .target

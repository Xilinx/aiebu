.. _AIEBU_USER_MANUAL.rst:

..
    comment:: SPDX-License-Identifier: MIT
    comment:: Copyright (C) 2024 Advanced Micro Devices, Inc.


AIE Binary Utilities (AIEBU)
============================

This repository contains library and utilities to work with AIE
*ctrlcode.*

AIEBU-ASM command line utility
------------------------------

**NAME**

-  aiebu-asm - Assembles AIE control code binary files and associated
   control packet and pm control packet binaries into ELF files.

**LOCATION**

-  cd <workspace>/aiebu/build/Debug/opt/xilinx/aiebu/bin

**SYNOPSIS**
::

-  aiebu-asm [OPTIONS]
-  aiebu-asm [-t \| --target]
-            [-h \| --help]

**DESCRIPTION**

-  aiebu-asm is a command-line utility used to assemble AI Engine TXN
   control code binaries or assembly files into Executable Loadable
   Format (ELF) files. It parses control code, control packet and
   patching JSON file and assembles them into various loadable ELF
   segments and dynamic sections of the resultant ELF file generated.
   The control code, control packet, pm control packet, save/restore
   control code are added to the loadable ELF segments and the patching
   information is added to the dynamic ELF sections. For control code
   supporting preemption, AIEBU also generates the control code binaries
   needed for save and restore operations. The ELF files are then loaded
   by XRT (Xilinx Runtime) and executed by the NPU firmware. The ELF
   loadable sections are patched in run time by XRT and loaded onto the
   NPU to run.

**OPTIONS**
::

-  -t, --target TARGET
-  Specify the target for the assembler. Supported targets are
-  aie2ps/aie2asm/aie2txn

- Once we select on of the 3 targets we can proceed with the other options in the utility.



Target: aie2txn
---------------

- For AIE2 and AIE2P architectures below command options of aiebu-asm are provided.


**SYNOPSIS**
::

-  aiebu-asm -t aie2txn [OPTIONS]
-  
-  aiebu-asm -t aie2txn [-c \|-- control code ]
-  [-p \| -- control packet ]
-  [-j \| -- json ]
-  [-m \| -- pm control packet ]
-  [-o \| -- output ]
-  [-r \| -- report]
-  [-h \| --help]


**OPTIONS**
::

-  -t, --target TARGET
-  Specify the target for the assembler. Supported targets are
-  aie2ps/aie2asm/aie2txn
-  
-  -c, --controlcode FILE
-  Provide TXN control code binary file with its path.
-  
-  -p, --controlpkt FILE
-  Provide the control packet binary file with its path. Use this option if the control packet is provided.
-  
-  -j, --json FILE
-  Provide the patching JSON file with its path. This file has has patching information for the control packet.
-  This option is always accompanies with -p option above
-  
-  -m, --pmctrlpkt ID:FILE
-  Specify a program memory control packet in the format
-  \`<id>:<file>`. Where <id> is the pm control packet ID and <file> is the pm control packet binary with its path.
-  
-  -o, --outputelf FILE
-  Provide name of the ELF output file with its path that will be generated as a result of running the aiebu-asm utility.
-  
-  -r, --report
-  Standalone option, no value to be provided with this option.
-  This option generate the control code assembly file (.asm) at the same location where the output elf is generated using the -o option.
-  This option needs to be used with -c and -o options to provide the control_code.bin and output.elf path respectively.
-  Also generates a report for the assembled output on the console.
-  
-  -h, --help
-  Show this help message and exit.


**EXAMPLE**

-  TXN Control Code only

   -  ``./aiebu-asm -t aie2txn -c <path_to_control_code.bin>/control_code.bin -o <path_to_output.elf>/output.elf``

-  TXN Control Code + Control Packet

   - ``./aiebu-asm -t aie2txn -c <path_to_control_code.bin>/control_code.bin -p <path_to_control_packet.bin>/control_packet.bin -j <path_to_external_buffer_id.json>/external_buffer_id.json -o <path_to_output.elf>/output.elf``



**ELF SECTIONS**

-  ELF is an executable loadable format file used to package the transaction control code binaries, control packet binaries, pm control packet binaries and provide necessary patching information. `readelf(1) - Linux manual page <https://www.man7.org/linux/man-pages/man1/readelf.1.html>`__ is a utility which dumps the elf file in readable format.



+------------------+--------------------------------------------------+
| **Sections**     | **Description**                                  |
+==================+==================================================+
| .ctrltext        | This corresponds to the transaction control code |
|                  | binary, used for running run time DMA like       |
|                  | operations on the                                |
+------------------+--------------------------------------------------+
| .ctrldata        | This corresponds to control control-packet       |
|                  | binary used when running inferences, used by     |
|                  | shim DMA program and also to configure AIE       |
|                  | registers.                                       |
+------------------+--------------------------------------------------+
| .preempt_save    | control code to do save operation during         |
|                  | preemption                                       |
+------------------+--------------------------------------------------+
| .preempt_restore | control code to do restore operation during      |
|                  | preemption                                       |
+------------------+--------------------------------------------------+
| .ctrlpkt.pm.n    | AIE pm in control packet format with ID n        |
+------------------+--------------------------------------------------+
| .dynsym          | a set of external symbols of fixed length.       |
+------------------+--------------------------------------------------+
| .dynstr          | strings of symbols required                      |
+------------------+--------------------------------------------------+
| .dynamic         | pointer to the relocation section and size       |
+------------------+--------------------------------------------------+
| .reldyn          | dynamic relocation contains the address of the   |
|                  | symbols need to be patched at run time           |
+------------------+--------------------------------------------------+



AIEBU-DUMP Command line utility
===============================

**NAME**

-  aiebu-dump

**LOCATION**

-  cd <workspace>/aiebu/build/Debug/aiebu/bin

**SYNOPSIS**
::

-  aiebu-dump [OPTION...] positional parameters
-  
-  aiebu-dump [-a \| --archive-headers ] <filename>
-  [-f \| --file-headers ]
-  [-x \| --all-headers ]
-  [-d \| --disassemble ]
-  [-H \| --help ]
-  [-m \| --architecture arg ]
-  [-D \| --disassemble-all ]
-  [-t \| --syms ]
-  [-r \| --reloc ]

**DESCRIPTION**

-  aiebu-dump is an aiebu dumping utility for AIE binaries and ELF
   files. This is used in displaying various sections of the binary
   files and ELF files like dumping their headers, displaying specific
   sections of the ELF files like symbol tables, relocation entries,
   etc.

**OPTIONS**
::

-  -a, --archive-headers Display archive header information
-  
-  -f, --file-headers Display the contents of the overall file header
-  
-  -x, --all-headers Display contents of all elf headers plus frequency of each opcode
-  in .ctrltext, .prempt_save and .preempt_restore sections of the elf.
-  
-  -d, --disassemble Display assembler contents of ctrltext section if ELF is given as input
-  Displays the control packet assembly format if control packet binary is given as input
-  
-  -H, --help show help message and exit
-  
-  -m, --architecture arg Specify the target architecture as MACHINE (aie2ps/aie2asm/aie2txn/aie2dpu) (default: unspecified)
-  -D, --disassemble-all Display assembler contents of all sections
-  
-  -t, --syms Display contents of the symbols table(s)
-  
-  -r, --reloc Display relocation entries in the file


**EXAMPLE**

- Display all ELF Headers and Opcode frequency

  - If the user wants to display ELF Header, Program Header and Section Headers of an ELF file along with the frequency of each opcode present in the .ctrltext, .preempt_save and .preempt_restore sections then use -x options as given below. The ELF file is given as input and the output is displayed on the console.

  - ``./aiebu-dump <path_to_filename.elf>/<filename.elf> -x``


- Display Assembly format of .ctrltext section

  - If the user wants to display the contents of the control code assembly in text format then use option -d. This essentially displays the assembler contents of only the .ctrltext section. The ELF file is given as input and the output is displayed on the console.

  - ``./aiebu-dump <path_to_filename.elf>/<filename.elf> -d``


- Display Assembly code of all ELF sections

  - If the user wants to display the contents of the control code assembly, premption_save control code assembly and preemption_restore control code assembly, the control packet unpacking etc in text format then use option -D. This essentially displays the assembler contents of all sections of the ELF. This is not implemented yet. The ELF file is given as input and the output is displayed on the console.

  - ``./aiebu-dump <path_to_filename.elf>/<filename.elf> -D``

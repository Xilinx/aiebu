..
    comment:: SPDX-License-Identifier: MIT
    comment:: Copyright (C) 2024 Advanced Micro Devices, Inc.

===================
BASIC ctrlcode test
===================

Input
=====

1. ml_txn.bin -- TXN ctrlcode binary. Checked in as *base64* encoded ml_txn.b64 ascii file which is decoded into binary before use with the help of **b64.cmake**
2. ctrl_pkt0.bin -- Control Packet. Checked in as *base64* encoded ctrl_pkt0.b64 which is decoded into binary before use with the help of **b64.cmake**
3. external_buffer_id.json -- Buffer metadata with address patch information

Output
======

basic.elf -- ELF file for loading by XRT and execution by NPU FW.

Command
=======

``ctest`` in this directory runs the following commands::

   cmake -P b64.cmake -d ml_txn.b64 ml_txn.bin
   cmake -P b64.cmake -d ctrl_pkt0.b64 ctrl_pkt0.bin
   aiebu-asm -r -t aie2txn -c ml_txn.bin -p ctrl_pkt0.bin -j external_buffer_id.json -o basic.elf

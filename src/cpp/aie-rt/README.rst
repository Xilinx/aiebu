.. _README.rst:

..
    comment:: SPDX-License-Identifier: MIT
    comment:: Copyright (C) 2025-2026 Advanced Micro Devices, Inc.

=====================
AIE-RT Usage in AIEBU
=====================

aiebu uses a local copy of the aie-rt *header files*, which are taken directly
from the aie-rt repository and checked into this directory.

Note that aiebu does **not** use the aie-rt source code and therefore does not
link against the aie-rt library. Only the aie2p opcode definitions are consumed
from aie-rt. The aie2ps and aie4 opcode definitions are implemented directly
within aiebu.

===========================
Notes for AIEBU Maintainers
===========================

When aie2p opcode definitions are updated in aie-rt, please copy the updated
aie-rt header files into this directory and update the HASH information listed
below to reflect the new version.

AIE-RT Snapshot
===============

URL https://github.com/Xilinx/aie-rt.git
HASH b50df92dd8ba190d3dc5318a0546504d5b7d3ad5

.. _README.rst:

..
    comment:: SPDX-License-Identifier: MIT
    comment:: Copyright (C) 2024 Advanced Micro Devices, Inc.

============================
AIE Binary Utilities Library
============================

Source code to generate save/restore transaction control-code (1 column for strix only for now)

Step to generate the transaction buffer
======================================
.. code-block:: sh

  EXPORT AIEBU=<PATH_TO_AIEBU>

  git clone <AIEBU>
  git submodule update --init --recursive
  cd $AIEBU/lib/aie-rt/driver/
  mkdir build
  cd build
  cmake ../
  make

  cd $AIEBU/lib/src
  make
  export LD_LIBRARY_PATH=$AIEBU/lib/aie-rt/driver/build/driver-src:$LD_LIBRARY_PATH
  ./preemption.exe

Generated transaction binary will be located under src/

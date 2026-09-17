#!/bin/bash

# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2025 Advanced Micro Devices, Inc.

# Strip out the following from readelf output if present. Newer readelf
# include this message which skews the gold file comparison
# Key to Flags:
#   W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
#   L (link order), N (extra OS processing required), G (group), T (TLS),
#   C (compressed), O (ordered), R (GNU retain), E (exclude)
#
# Normalize ELF header Machine: to match gold files (col_0.gold et al.),
# which expect WE32100. Ancient versions of eu-readelf report "m32" which
# skews the gold comparison.
#
# elfutils 0.190 (RHEL 8.10) prints unknown e_machine values as "<unknown>"
# without the hex suffix. Other versions print "<unknown>: 0x10d". Normalize
# the bare "<unknown>" form to "<unknown>: 0x10d" as AIE Ctrlcode has been
# officially assigned machine code 269 (0x10d)

eu-readelf -a $1 \
  | sed "/Key to Flags/,+3d" \
  | sed '/^[[:space:]]*Machine:/s/m32/WE32100/' \
  | sed '/^[[:space:]]*Machine:.*<unknown>$/s/<unknown>/<unknown>: 0x10d/' \
  > "$2"

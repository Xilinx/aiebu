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

eu-readelf -a $1 | sed "/Key to Flags/,+3d" | sed '/^[[:space:]]*Machine:/s/m32/WE32100/' > "$2"

# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2024 Advanced Micro Devices, Inc.

class Decoder:
    def __init__(self, symbols):
        # list all shim tile BD registers DDR address need to be processed
        self.symbols = symbols
        self.symbolmap = {}

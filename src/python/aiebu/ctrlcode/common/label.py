# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

class Label:
    def __init__(self, name):
        self.name = name

    def __str__(self):
        return f"{self.name}:"


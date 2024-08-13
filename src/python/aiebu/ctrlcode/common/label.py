# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

class Label:
    def __init__(self, name, filename):
        self.name = name
        self.filename = filename

    def __str__(self):
        return f"{self.name}:"


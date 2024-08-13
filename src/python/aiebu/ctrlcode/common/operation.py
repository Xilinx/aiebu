# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

class Operation:
    def __init__(self, name, args, filename):
        self.name = name.lower()
        self.filename = filename
        self.args = [x.strip() for x in args.strip().split(',')] if args is not None else []

    def __str__(self):
        return f"{self.name} {self.args}"


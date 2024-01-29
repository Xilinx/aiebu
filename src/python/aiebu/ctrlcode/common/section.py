# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2023 Advanced Micro Devices, Inc.

from enum import Enum

class Section(Enum):
    """
    Represents a user symbol (variable refernce) encountered in assembly code.
    """
    TEXT = 1
    DATA = 2
    UNKNOWN = 3

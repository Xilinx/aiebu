# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2024 Advanced Micro Devices, Inc.


class ELF_Section:
  """ ELF_Section class to hold section data of elf """
  def __init__(self, name, section):
    self.name = name
    self.section = section
    self.bytearray = bytearray()

  def write_bytes(self, buf):
    self.bytearray = self.bytearray + buf

  def tell(self):
    return len(self.bytearray)

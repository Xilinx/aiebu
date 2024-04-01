# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2024 Advanced Micro Devices, Inc.


class ELF_Section:

  def __init__(self, name, section, paired_elf_section=None):
    self.name = name
    self.section_index = -1
    self.section = section
    self.bytearray = bytearray()
    self.paired_elf_section = paired_elf_section

  def set_section_index(self, index):
    self.section_index = index

  def write_bytes(self, buf):
    self.bytearray = self.bytearray + buf

  def tell(self):
    return len(self.bytearray)

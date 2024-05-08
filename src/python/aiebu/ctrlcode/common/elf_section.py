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

  def write_byte(self, buf):
    self.bytearray.append(buf)

  def tell(self):
    return len(self.bytearray)

  def _words_to_bytes(self, words):
    result = []
    for word in words:
      word_data = []
      for i in range(0, 4):
        byte = (word >> ((3-i) * 8)) & 0xFF
        word_data.append(byte)
      result += reversed(word_data)
    return result

  def write_words(self, words):
    data = self._words_to_bytes(words)
    for byte in data:
      self.write_byte(byte)

# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc.

from ctrlcode.common.util import words_to_bytes

class ELF_Section:
  """ ELF_Section class to hold section data of elf """

  def __init__(self, name, section):
    self.name = name
    self.section_index = -1
    self.section = section
    self.bytearray = bytearray()

  def set_section_index(self, index):
    self.section_index = index

  def write_bytes(self, buf):
    self.bytearray = self.bytearray + buf

  def write_byte(self, buf):
    self.bytearray.append(buf)

  def read_word(self, offset):
    return int.from_bytes(self.bytearray[offset:offset+4], byteorder='little')

  def write_word_at(self, offset, word):
    bytes = self._words_to_bytes([word])
    for index, byte in enumerate(bytes):
        self.bytearray[offset + index] = byte

  def tell(self):
    return len(self.bytearray)

  def _words_to_bytes(self, words):
    return words_to_bytes(words)

  def write_words(self, words):
    data = self._words_to_bytes(words)
    for byte in data:
      self.write_byte(byte)

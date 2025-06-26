// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#include "disassembler/disassembler.h"
#include <fstream>
#include <stdexcept>

#include <iostream>
namespace aiebu {

asm_disassembler::asm_disassembler(const std::string& filename, const std::string& lfile)
  : writer(lfile){
  if (!reader.load(filename)) {
    throw std::runtime_error("Failed to load ELF: " + filename);
  }
  aiebu::isa_disassembler i;
  isa_ops = i.get_isamap();
}

void asm_disassembler::run() {
  dump();
}

void asm_disassembler::dump() {
  std::shared_ptr<disassembler_state> state = std::make_shared<disassembler_state>();
  for (const auto& sec_ptr : reader.sections) {
    const ELFIO::section* sec = sec_ptr.get();
    std::string name = sec->get_name();
    std::cout << "SECTION: " << name << "\n";
    if (sec->get_type() != ELFIO::SHT_PROGBITS)
      continue;
    dump_section_info(sec);
    if (is_text_section(name)) {
      dump_text_section(sec, state);
    } else if (is_data_section(name)) {
      dump_data_section(sec, state);
      state->reset();
    }
  }
}

void asm_disassembler::dump_section_info(const ELFIO::section* sec) {
  std::string mode;
  if (sec->get_flags() & ELFIO::SHF_ALLOC) mode += "a";
  if (sec->get_flags() & ELFIO::SHF_WRITE) mode += "w";
  if (sec->get_flags() & ELFIO::SHF_EXECINSTR) mode += "x";
  writer.write_directive("");
  writer.write_directive(".section " + sec->get_name() + ",\"" + mode + "\"");
  writer.write_directive(".align " + std::to_string(sec->get_addr_align()));
}

void asm_disassembler::dump_text_section(const ELFIO::section* sec, std::shared_ptr<disassembler_state> state) {
  const char* data = sec->get_data();
  size_t size = sec->get_size();
  size_t pos = 16; // skip ELF-specific header padding

  //std::cout << "text size:" << size <<"\n";
  while (pos < size) {
    //std::cout << "test pos:" << pos << " spos:" << state->pos <<"\n";
    uint8_t opcode = *reinterpret_cast<const uint8_t*>(data + pos);
    auto it = isa_ops->find(opcode);
    if (it != isa_ops->end()) {
      pos += it->second->deserializer()->deserialize(writer, state, data + pos); // implement your reader logic
    } else {
      std::cerr << "Unknown opcode " << opcode << " at position " << pos << "\n";
    }
  }
}

void asm_disassembler::dump_data_section(const ELFIO::section* sec, std::shared_ptr<disassembler_state> state) {
  const char* data = sec->get_data();
  size_t size = sec->get_size();
  size_t pos = 0;
  auto dummy_isa = std::make_shared<isa_op>("dummy", 0, std::vector<opArg>{});
  while (pos < size) {
    uint8_t opcode = *reinterpret_cast<const uint8_t*>(data + pos);
    auto state_labels = state->get_labels();
    auto state_local_ptrs = state->get_local_ptrs();
    if (state_labels.find(state->address()) != state_labels.end()) {
      ucDmaBd_op_deserializer deserializer(dummy_isa);
      pos += deserializer.deserialize(writer, state, data + pos);
    }
    else if (state_local_ptrs.find(state->address()) != state_local_ptrs.end()) {
      writer.write_directive(".align 4");
      long_op_deserializer deserializer(dummy_isa);
      pos += deserializer.deserialize(writer, state, data + pos);
    }
    else if (opcode == align) {
      state->increment_address(1);
      pos++;
    }
    else if (opcode == 0) {
      // Do nothing, these are padding bytes
      state->increment_address(1);
      pos++;
    }
    else {
      throw std::runtime_error("Illegal state at position " + std::to_string(opcode));
    }
  }
}

}

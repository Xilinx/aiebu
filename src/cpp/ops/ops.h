// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _ADSM_OPS_ISA_OP_H_
#define _ADSM_OPS_ISA_OP_H_

#include <memory>
#include "utils.h"
#include "oparg.h"
#include "assembler_state.h"
#include "common/disassembler_state.h"
#include "writer.h"

namespace aiebu {

class isa_op;
class op_serializer;
class isa_op_serializer;
class assembler_state;

class op_deserializer;
class isa_op_deserializer;
class align_op_deserializer;
class ucDmaBd_op_deserializer;
class long_op_deserializer;

class op_serializer
{
protected:
  constexpr static uint8_t pad = 0x00;
  constexpr static uint8_t width_8 = 8;
  constexpr static uint8_t width_16 = 16;
  constexpr static uint8_t width_32 = 32;
  std::shared_ptr<isa_op> m_opcode;
  std::vector<std::string> m_args;

public:
  op_serializer(std::shared_ptr<isa_op> opcode, std::vector<std::string> args):m_opcode(opcode) {
    for (auto a : args)
      m_args.emplace_back(a);
  }
  virtual ~op_serializer() = default;

  const std::vector<std::string>& get_args() const { return m_args; }

  virtual offset_type size(assembler_state& ) { return 0;}
  virtual offset_type align() {return 0;}
  virtual std::vector<uint8_t> serialize(std::shared_ptr<assembler_state> /*state*/, std::vector<symbol>& /*symbols*/, uint32_t /*colnum*/, pageid_type /*pagenum*/)
  { std::vector<uint8_t> v; return v;}
};


class isa_op_serializer: public op_serializer
{
public:
  isa_op_serializer(std::shared_ptr<isa_op> opcode, std::vector<std::string> args):op_serializer(opcode, args) {}

  offset_type size(assembler_state& state) override;

  offset_type align() override { return 0; }
  std::vector<uint8_t> serialize(std::shared_ptr<assembler_state> state, std::vector<symbol>& symbols, uint32_t colnum, pageid_type pagenum) override;
};

class long_op_serializer: public op_serializer
{
public:
  long_op_serializer(std::shared_ptr<isa_op> opcode, std::vector<std::string> args):op_serializer(opcode, args) {}

  offset_type size(assembler_state& /*state*/) override { return 4; }

  offset_type align() override { return 4; }
  std::vector<uint8_t> serialize(std::shared_ptr<assembler_state> state, std::vector<symbol>& symbols, uint32_t colnum, pageid_type pagenum) override;
};

class align_op_serializer: public op_serializer
{
public:
  align_op_serializer(std::shared_ptr<isa_op> opcode, std::vector<std::string> args):op_serializer(opcode, args) {}

  offset_type size(assembler_state& state) override;

  offset_type align() override { return 0; }
  std::vector<uint8_t> serialize(std::shared_ptr<assembler_state> state, std::vector<symbol>& symbols, uint32_t colnum, pageid_type pagenum) override;
};

class ucDmaBd_op_serializer: public op_serializer
{
public:
  ucDmaBd_op_serializer(std::shared_ptr<isa_op> opcode, std::vector<std::string> args):op_serializer(opcode, args) {}

  offset_type size(assembler_state& /*state*/) override { return 16; }

  offset_type align() override { return 16; }
  std::vector<uint8_t> serialize(std::shared_ptr<assembler_state> state, std::vector<symbol>& symbols, uint32_t colnum, pageid_type pagenum) override;
};


class isa_op_disasm
{
protected:
  std::string m_opname;
  uint8_t m_code;
  std::vector<opArg> m_args;
public:
  const std::vector<opArg>& get_args() const { return m_args; }
  uint8_t get_code() const { return m_code; }
  const std::string& get_code_name() const { return m_opname; }
  std::unique_ptr<op_deserializer> create_deserializer() const;

  isa_op_disasm(std::string opname, uint8_t code, std::vector<opArg> args):m_opname(std::move(opname)), m_code(code) {
    for (auto a : args)
      m_args.emplace_back(a);
  }
};

class op_deserializer
{
protected:
  static constexpr unsigned int field_width = 8;
  static constexpr unsigned int byte_shift_8 = 8;
  static constexpr unsigned int byte_shift_16 = 16;
  static constexpr unsigned int byte_shift_24 = 24;
  static constexpr uint32_t len_8 = 1;
  static constexpr uint32_t len_16 = 2;
  static constexpr uint32_t len_32 = 4;
  static constexpr uint32_t align_4 = 4;
  static constexpr uint32_t align_16 = 16;
  static constexpr uint32_t ucDmaBd_size = 16;
  static uint32_t numlabel;
  std::string label = "@label";
  const isa_op_disasm* m_opcode;


public:
  uint8_t read_uint8(const char* data) {
    return static_cast<uint8_t>(*data);
  }

  uint16_t read_uint16(const char* data) {
    return static_cast<uint16_t>(static_cast<uint8_t>(data[0])) | 
           (static_cast<uint16_t>(static_cast<uint8_t>(data[1])) << byte_shift_8);
  }

  uint32_t read_uint32(const char* data) {
    return static_cast<uint32_t>(static_cast<uint8_t>(data[0])) | 
           (static_cast<uint32_t>(static_cast<uint8_t>(data[1])) << byte_shift_8) | 
           (static_cast<uint32_t>(static_cast<uint8_t>(data[2])) << byte_shift_16) | 
           (static_cast<uint32_t>(static_cast<uint8_t>(data[3])) << byte_shift_24);
  }

  uint32_t get_arg_val(const char* data, uint32_t len) {
    if (len == len_8) {
      return read_uint8(data);
    } else if (len == len_16) {
      return read_uint16(data);
    } else if (len == len_32) {
      return read_uint32(data);
    } else {
      throw error(error::error_code::invalid_asm, "Invalid length for reading data: " + std::to_string(len) + "\n");
    }
  }

  std::string get_label() {
    std::string ret = label + std::to_string(numlabel++);
    return ret;
  }

  explicit op_deserializer(const isa_op_disasm* opcode): m_opcode(opcode) {}

  virtual ~op_deserializer() = default;

  // Copy constructor
  op_deserializer(const op_deserializer& other) = default;

  // Copy assignment operator
  op_deserializer& operator=(const op_deserializer& other) = default;

  // Move constructor
  op_deserializer(op_deserializer&& other) noexcept = default;

  // Move assignment operator
  op_deserializer& operator=(op_deserializer&& other) noexcept = default;

  virtual offset_type size(disassembler_state& ) { return 0;}
  virtual offset_type align() { return align_4; }
  virtual uint32_t deserialize(asm_writer& writer, std::shared_ptr<disassembler_state> state, const char* data) = 0;
};


class align_op_deserializer: public op_deserializer
{
public:
  explicit align_op_deserializer(const isa_op_disasm* opcode):op_deserializer(opcode) {}
  offset_type size(disassembler_state& /*state*/) override { return len_8; }
  offset_type align() override { return align_4; }
  uint32_t deserialize(asm_writer& writer, std::shared_ptr<disassembler_state> state, const char* data) override;
};


class long_op_deserializer: public op_deserializer
{
public:
  explicit long_op_deserializer(const isa_op_disasm* opcode):op_deserializer(opcode) {}
  offset_type size(disassembler_state& /*state*/) override { return len_32; }

  offset_type align() override { return align_4; }
  uint32_t deserialize(asm_writer& writer, std::shared_ptr<disassembler_state> state, const char* data) override;
};

class ucDmaBd_op_deserializer: public op_deserializer
{
public:
  explicit ucDmaBd_op_deserializer(const isa_op_disasm* opcode):op_deserializer(opcode) {}
  offset_type size(disassembler_state& /*state*/) override { return ucDmaBd_size; }
  offset_type align() override { return align_16; }
  uint32_t deserialize(asm_writer& writer, std::shared_ptr<disassembler_state> state, const char* data) override;
};

class isa_op_deserializer: public op_deserializer
{
private:
  std::string handle_tile_id_arg(uint32_t val, uint32_t& tile, std::shared_ptr<disassembler_state> state);
  std::string handle_actor_id_arg(uint32_t val, uint32_t tile, std::shared_ptr<disassembler_state> state);
  std::string handle_descriptor_ptr_arg(uint32_t val, std::shared_ptr<disassembler_state> state);
  std::string handle_table_ptr_arg(uint32_t val, std::shared_ptr<disassembler_state> state);
  std::string handle_generic_const_arg(const opArg& arg, uint32_t val);
  std::string handle_register_arg(uint32_t val);
  std::string handle_barrier_arg(uint32_t val);
  std::string handle_page_id_arg(uint32_t val, std::shared_ptr<disassembler_state> state);

  // Inline helper functions for argument type checking
  inline bool is_tile_id(const opArg& arg) { return arg.get_name() == "tile_id"; }
  inline bool is_actor_id(const opArg& arg) { return arg.get_name() == "actor_id"; }
  inline bool is_descriptor_ptr(const opArg& arg) { return arg.get_name() == "descriptor_ptr"; }
  inline bool is_table_ptr(const opArg& arg) { return arg.get_name() == "table_ptr"; }
  inline bool is_offset(const opArg& arg) { return arg.get_name() == "offset"; }

public:
  explicit isa_op_deserializer(const isa_op_disasm* opcode):op_deserializer(opcode) {}
  offset_type size(disassembler_state& state) override;
  offset_type align() override { return align_4; }
  uint32_t deserialize(asm_writer& writer, std::shared_ptr<disassembler_state> state, const char* data) override;
};


class isa_op : public std::enable_shared_from_this<isa_op>
{
protected:
  std::string m_opname;
  uint8_t m_code;
  std::vector<opArg> m_args;
public:
  const std::vector<opArg>& get_args() const { return m_args; }
  uint8_t get_code() const { return m_code; }
  const std::string& get_code_name() const { return m_opname; }

  isa_op(std::string opname, uint8_t code, std::vector<opArg> args):m_opname(opname), m_code(code) {
    for (auto a : args)
      m_args.emplace_back(a);
  }

  std::shared_ptr<isa_op>
  get_shared_ptr()
  {
    return shared_from_this();
  }

  std::shared_ptr<op_serializer> serializer(std::vector<std::string> args)
  {
    if (!m_opname.compare(".long"))
      return std::make_shared<long_op_serializer>(get_shared_ptr(), args);
    else if (!m_opname.compare(".align"))
      return std::make_shared<align_op_serializer>(get_shared_ptr(), args);
    else if (!m_opname.compare("uc_dma_bd"))
      return std::make_shared<ucDmaBd_op_serializer>(get_shared_ptr(), args);
    else
      return std::make_shared<isa_op_serializer>(get_shared_ptr(), args);
  }
};

  inline std::unique_ptr<op_deserializer> isa_op_disasm::create_deserializer() const
  {
    const std::string& opname = get_code_name();

    if (!opname.compare(".long"))
      return std::unique_ptr<op_deserializer>(new long_op_deserializer(this));
    else if (!opname.compare(".align"))
      return std::unique_ptr<op_deserializer>(new align_op_deserializer(this));
    else if (!opname.compare("uc_dma_bd") || !opname.compare("uc_dma_bd_shim"))
      return std::unique_ptr<op_deserializer>(new ucDmaBd_op_deserializer(this));
    else
      return std::unique_ptr<op_deserializer>(new isa_op_deserializer(this));
  }

}
#endif //_ADSM_OPS_ISA_OP_H_

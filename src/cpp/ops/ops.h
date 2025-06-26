// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _ADSM_OPS_ISA_OP_H_
#define _ADSM_OPS_ISA_OP_H_

#include <memory>
#include "writer.h"
#include "utils.h"
#include "oparg.h"
#include "assembler_state.h"
#include "common/disassembler_state.h"

namespace aiebu {

class isa_op;
class op_serializer;
class isa_op_serializer;
class assembler_state;

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

class op_deserializer;
class isa_op_deserializer;
class align_op_deserializer;
class ucDmaBd_op_deserializer;
class long_op_deserializer;

class op_deserializer
{
protected:
  static constexpr unsigned int field_width = 8;
  static uint32_t numlabel;
  std::string label = "@label";
  std::shared_ptr<isa_op> m_opcode;

uint8_t read_uint8(const char* data) {
    return static_cast<uint8_t>(*data);
}

uint16_t read_uint16_le(const char* data) {
    uint8_t b0 = read_uint8(data);
    uint8_t b1 = read_uint8(data+1);
    return static_cast<uint16_t>(b0 | (b1 << 8));
}

uint32_t read_uint32_le(const char* data) {
    uint8_t b0 = read_uint8(data);
    uint8_t b1 = read_uint8(data+1);
    uint8_t b2 = read_uint8(data+2);
    uint8_t b3 = read_uint8(data+3);
    return static_cast<uint32_t>(b0 | (b1 << 8) | (b2 << 16) | (b3 << 24));
}

uint32_t read_len_le(const char* data, uint32_t len) {
  if (len == 1)
    return read_uint8(data);
  else if (len == 2)
    return read_uint16_le(data);
  else if (len == 4)
    return read_uint32_le(data);
  throw std::runtime_error("Unsupported read_len_le for len:" + std::to_string(len));
}

  std::string get_label() { return label + std::to_string(numlabel++); }
public:
  op_deserializer(std::shared_ptr<isa_op> opcode):m_opcode(opcode) {}
  //op_deserializer() {}
  virtual ~op_deserializer() = default;

  virtual offset_type size(disassembler_state& ) { return 0;}
  virtual offset_type align() {return 4;}
  virtual uint32_t deserialize(ctrl_writer& writer, std::shared_ptr<disassembler_state> state, const char* data) = 0;
};

class align_op_deserializer: public op_deserializer
{
public:
  align_op_deserializer(std::shared_ptr<isa_op> opcode):op_deserializer(opcode) {}

  offset_type size(disassembler_state& /*state*/) override { return 1; }

  offset_type align() override { return 4; }
  uint32_t deserialize(ctrl_writer& writer, std::shared_ptr<disassembler_state> state, const char* data) override;
};

class long_op_deserializer: public op_deserializer
{
public:
  long_op_deserializer(std::shared_ptr<isa_op> opcode):op_deserializer(opcode) {}
  //long_op_deserializer():op_deserializer() {}

  offset_type size(disassembler_state& /*state*/) override { return 4; }

  offset_type align() override { return 4; }
  uint32_t deserialize(ctrl_writer& writer, std::shared_ptr<disassembler_state> state, const char* data) override;
};

class ucDmaBd_op_deserializer: public op_deserializer
{
public:
  ucDmaBd_op_deserializer(std::shared_ptr<isa_op> opcode):op_deserializer(opcode) {}
  //ucDmaBd_op_deserializer():op_deserializer() {}

  offset_type size(disassembler_state& /*state*/) override { return 16; }

  offset_type align() override { return 16; }
  uint32_t deserialize(ctrl_writer& writer, std::shared_ptr<disassembler_state> state, const char* data) override;
};

class isa_op_deserializer: public op_deserializer
{

public:
  isa_op_deserializer(std::shared_ptr<isa_op> opcode):op_deserializer(opcode) {}

  offset_type size(disassembler_state& state) override;

  offset_type align() override { return 4; }
  uint32_t deserialize(ctrl_writer& writer, std::shared_ptr<disassembler_state> state, const char* data) override;
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

  std::shared_ptr<op_deserializer> deserializer()
  {
    if (!m_opname.compare(".long"))
      return std::make_shared<long_op_deserializer>(get_shared_ptr());
    else if (!m_opname.compare(".align"))
      return std::make_shared<align_op_deserializer>(get_shared_ptr());
    else if (!m_opname.compare("uc_dma_bd"))
      return std::make_shared<ucDmaBd_op_deserializer>(get_shared_ptr());
    else
      return std::make_shared<isa_op_deserializer>(get_shared_ptr());
  }
};

}
#endif //_ADSM_OPS_ISA_OP_H_

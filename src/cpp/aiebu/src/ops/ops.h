// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _ADSM_OPS_ISA_OP_H_
#define _ADSM_OPS_ISA_OP_H_

#include "utils.h"
#include "oparg.h"
#include "assembler_state.h"

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

  const std::vector<std::string>& get_args() const { return m_args; }

  virtual offset_type size(assembler_state& ) { return 0;}
  virtual offset_type align() {return 0;}
  virtual std::vector<uint8_t> serialize(assembler_state& /*state*/, std::vector<symbol>& /*symbols*/, uint32_t /*colnum*/, pageid_type /*pagenum*/)
  { std::vector<uint8_t> v; return v;}
};


class isa_op_serializer: public op_serializer
{
public:
  isa_op_serializer(std::shared_ptr<isa_op> opcode, std::vector<std::string> args):op_serializer(opcode, args) {}

  offset_type size(assembler_state& state) override;

  offset_type align() override { return 0; }
  std::vector<uint8_t> serialize(assembler_state& state, std::vector<symbol>& symbols, uint32_t colnum, pageid_type pagenum) override;
};

class long_op_serializer: public op_serializer
{
public:
  long_op_serializer(std::shared_ptr<isa_op> opcode, std::vector<std::string> args):op_serializer(opcode, args) {}

  offset_type size(assembler_state& /*state*/) override { return 4; }

  offset_type align() override { return 4; }
  std::vector<uint8_t> serialize(assembler_state& state, std::vector<symbol>& symbols, uint32_t colnum, pageid_type pagenum) override;
};

class align_op_serializer: public op_serializer
{
public:
  align_op_serializer(std::shared_ptr<isa_op> opcode, std::vector<std::string> args):op_serializer(opcode, args) {}

  offset_type size(assembler_state& state) override;

  offset_type align() override { return 0; }
  std::vector<uint8_t> serialize(assembler_state& state, std::vector<symbol>& symbols, uint32_t colnum, pageid_type pagenum) override;
};

class ucDmaBd_op_serializer: public op_serializer
{
public:
  ucDmaBd_op_serializer(std::shared_ptr<isa_op> opcode, std::vector<std::string> args):op_serializer(opcode, args) {}

  offset_type size(assembler_state& /*state*/) override { return 16; }

  offset_type align() override { return 16; }
  std::vector<uint8_t> serialize(assembler_state& state, std::vector<symbol>& symbols, uint32_t colnum, pageid_type pagenum) override;
};

class ucDmaShimBd_op_serializer: public op_serializer
{
public:
  ucDmaShimBd_op_serializer(std::shared_ptr<isa_op> opcode, std::vector<std::string> args):op_serializer(opcode, args) {}

  offset_type size(assembler_state& /*state*/) override { return 16; }

  offset_type align() override { return 16; }
  std::vector<uint8_t> serialize(assembler_state& state, std::vector<symbol>& symbols, uint32_t colnum, pageid_type pagenum) override;
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
    else if (!m_opname.compare("uc_dma_bd_shim"))
      return std::make_shared<ucDmaShimBd_op_serializer>(get_shared_ptr(), args);
    else
      return std::make_shared<isa_op_serializer>(get_shared_ptr(), args);
  }

};

}
#endif //_ADSM_OPS_ISA_OP_H_

// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSOR_INPUT_H_
#define _AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSOR_INPUT_H_

#include <map>
#include "symbol.h"
#include "utils.h"
#include "file_utils.h"
#include "preprocessor_input.h"
#include "asm/asm_parser.h"
#include "logger.h"
#include <boost/format.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace aiebu {

class aie2_isa_op_factory_base;
class aie2_isa_op;
class operation;

class aie2_blob_preprocessor_input : public preprocessor_input
{
protected:
  const std::string ctrlText = ".ctrltext";
  const std::string ctrl_data = ".ctrldata";
  const std::string preempt_save = ".preempt_save";
  const std::string preempt_restore = ".preempt_restore";
  const std::string preempt_lib = "preempt";
  const std::string scratch_pad = "scratch-pad-mem";
  const std::string ctrlpkt_pm = "ctrlpkt-pm-";
  const std::string scratch_pad_ctrl = "scratch-pad-ctrl";
  const std::string legacydpuxclbin = "legacydpuxclbin";

  constexpr static uint32_t SHIM_DMA_BD0_0 = 0x0001D000;
  constexpr static uint32_t SHIM_DMA_BD_NUM = 16;
  constexpr static uint32_t SHIM_DMA_BD_SIZE = 0x20; // 8*4bytes

  constexpr static uint32_t MEM_DMA_BD0_0 = 0x000A0000;
  constexpr static uint32_t MEM_DMA_BD_NUM = 48;
  constexpr static uint32_t MEM_DMA_BD_SIZE = 0x20; // 8*4bytes
  constexpr static uint32_t byte_in_word = 4;
  constexpr static uint32_t MAX_ARG_INDEX = 512; // approximated value 512 to limit the number of arguments in XRT kernel call

  constexpr static uint64_t RANGE_32BIT = 0xFFFFFFFF; // Max value supported in 32bit elf supported

  // For transaction buffer flow. In Xclbin kernel argument, actual argument start from 3,
  // 0th is opcode, 1st is instruct buffer, 2nd is instruct buffer size.
  uint32_t arg_offset = 3;

  enum class offset_type {
    CONTROL_PACKET,
    COALESED_BUFFER
  };

  enum class register_id {
    MEM_BUFFER_LENGTH,
    MEM_BASE_ADDRESS,
    SHIM_BUFFER_LENGTH
  };

  std::map<register_id, uint32_t> register_mask = {
    { register_id::MEM_BUFFER_LENGTH, 0x1FFFF},
    { register_id::MEM_BASE_ADDRESS, 0x7FFFF},
    { register_id::SHIM_BUFFER_LENGTH, 0xFFFFFFFF}
  };

  std::map<uint32_t, std::string> xrt_id_map;
  std::vector<uint32_t> pm_id_list;
  std::vector<std::string> pdi_id_list;
  bool haspreempt = false;
  std::shared_ptr<partition_info> m_partition;
  virtual uint32_t extractSymbolFromBuffer(std::vector<char>& mc_code, const std::string& section_name, const std::string& argname) = 0;
  void aiecompiler_json_parser(const boost::property_tree::ptree& pt);
  void dmacompiler_json_parser(const boost::property_tree::ptree& pt);
  void readmetajson(std::istream& patch_json);
  void extract_control_packet_patch(const std::string& name, uint32_t arg_index,
                                    const boost::property_tree::ptree& _pt);
  void extract_coalesed_buffers(const std::string& name, const boost::property_tree::ptree& _pt);
  void clear_shimBD_address_bits(std::vector<char>& mc_code, uint32_t offset) const;
  void validate_json(uint32_t offset, uint32_t size, uint32_t arg_index, offset_type type) const;
  uint32_t get_32_bit_property(const boost::property_tree::ptree& pt, const std::string& property, bool defaultvalue = false) const;
  void add_preemption_code(uint32_t col);
  std::string get_pdi_name(uint16_t pdi_id)
  {
    return ".pdi." + std::to_string(pdi_id);
  }

  void set_numcolumn(uint32_t col) { m_partition->set_numcolumn(col); }
public:
  aie2_blob_preprocessor_input()
  {
    m_partition = std::make_shared<partition_info>(DEFAULT_COLUMN, 0);
  }

  std::shared_ptr<const partition_info> get_partition_info() const { return std::const_pointer_cast<const partition_info>(m_partition); }

  virtual void set_args(const std::vector<char>& mc_code,
                        const std::vector<char>& patch_json,
                        const std::vector<char>& control_packet,
                        const std::vector<std::string>& libs,
                        const std::vector<std::string>& /*libpaths*/,
                        const std::map<uint32_t, std::vector<char> >& ctrlpkt) override
  {
    const std::string loglevel_prefix = "loglevel_";
    for (const auto& lib: libs)
    {
      if (lib == legacydpuxclbin)
        arg_offset = 1;
      else if (lib.find(loglevel_prefix) == 0) {
        // Process log level for library API users
        std::string log_level_str = lib.substr(loglevel_prefix.size());
        if (log_level_str == "error")
          set_log_level(log_level::error);
        else if (log_level_str == "warn")
          set_log_level(log_level::warn);
        else if (log_level_str == "info")
          set_log_level(log_level::info);
        else if (log_level_str == "debug")
          set_log_level(log_level::debug);
        else
          log_warn() << "Invalid log level flag: " << lib << ", ignored";
      }
      else
        log_warn() << "Invalid flag: " << lib << ", ignored";
    }

    m_data[".ctrltext"] = mc_code;

    if(control_packet.size())
      m_data[".ctrldata"] = control_packet;

    for (auto& pm_ctrl : ctrlpkt)
    {
      m_data[".ctrlpkt.pm." + std::to_string(pm_ctrl.first)] = pm_ctrl.second;
      pm_id_list.push_back(pm_ctrl.first);
    }

    if (patch_json.size() !=0 )
    {
      vector_streambuf vsb(patch_json);
      std::istream elf_stream(&vsb);
      readmetajson(elf_stream);
    }

    auto col = extractSymbolFromBuffer(m_data[".ctrltext"], ctrlText, "");
    set_numcolumn(col);

    if (haspreempt)
      add_preemption_code(col);
  }

  virtual void set_args(const std::vector<char>& mc_code,
                       const std::vector<char>& patch_json,
                       const std::vector<char>& control_packet,
                       const std::vector<std::string>& /*flags*/,
                       const std::vector<std::string>& /*libpaths*/,
                       const std::vector<uint32_t>& pmid_list,
                       const std::vector<std::string>& pdi_list)
  {
    arg_offset = 0;
    m_data[".ctrltext"] = mc_code;

    if(control_packet.size())
      m_data[".ctrldata"] = control_packet;

    for (auto id : pmid_list)
      pm_id_list.push_back(id);

    for (auto& pdi_name : pdi_list)
      pdi_id_list.push_back(pdi_name);

    if (patch_json.size() !=0 )
    {
      vector_streambuf vsb(patch_json);
      std::istream elf_stream(&vsb);
      readmetajson(elf_stream);
    }

    auto col = extractSymbolFromBuffer(m_data[".ctrltext"], ctrlText, "");
    set_numcolumn(col);

    if (haspreempt)
      add_preemption_code(col);
  }
};

class aie2_blob_transaction_preprocessor_input : public aie2_blob_preprocessor_input
{
protected:
  virtual uint32_t extractSymbolFromBuffer(std::vector<char>& mc_code, const std::string& section_name, const std::string& argname) override;

  struct patch_helper_input {
    const std::string& section_name;
    const std::string& argname;
    uint32_t reg;
    uint32_t argidx;
    uint32_t offset;
    uint64_t buffer_length_in_bytes;
    uint64_t addend;
  };
  void patch_helper(std::vector<char>& mc_code, const patch_helper_input& input);
  uint32_t process_txn(const char *ptr, std::vector<char>& mc_code, const std::string& section_name, const std::string& argname);
  uint32_t process_txn_opt(const char *ptr, std::vector<char>& mc_code, const std::string& section_name, const std::string& argname);
  void resize_scratchpad(const std::string& section_name)
  {
    std::vector<symbol> &syms = get_symbols();
    uint64_t size = 0;
    for (auto& sym : syms)
    {
      if (section_name.compare(sym.get_section_name()))
        continue;

      auto ssize = sym.get_size();
      auto saddend = sym.get_addend();
      size = ssize + saddend > size ? ssize + saddend : size;
    }

    for (auto& sym : syms)
    {
      if (section_name.compare(sym.get_section_name()))
        continue;

      sym.set_size(size);
    }
  }
public:
  virtual void set_args(const std::vector<char>& mc_code,
                        const std::vector<char>& patch_json,
                        const std::vector<char>& control_packet,
                        const std::vector<std::string>& libs,
                        const std::vector<std::string>& libpaths,
                        const std::map<uint32_t, std::vector<char> >& ctrlpkt) override
  {
    aie2_blob_preprocessor_input::set_args(mc_code, patch_json, control_packet, libs, libpaths, ctrlpkt);
    resize_scratchpad(preempt_save);
    resize_scratchpad(preempt_restore);
  }

  void set_args(const std::vector<char>& mc_code,
                const std::vector<char>& patch_json,
                const std::vector<char>& control_packet,
                const std::vector<std::string>& flags,
                const std::vector<std::string>& libpaths,
                const std::vector<uint32_t>& pmid_list,
                const std::vector<std::string>& pdi_list) override
  {
    aie2_blob_preprocessor_input::set_args(mc_code, patch_json, control_packet, flags, libpaths, pmid_list, pdi_list);
    resize_scratchpad(preempt_save);
    resize_scratchpad(preempt_restore);
  }
};

class aie2_blob_dpu_preprocessor_input : public aie2_blob_preprocessor_input
{
  constexpr static uint32_t OP_NOOP = 0;
  constexpr static uint32_t OP_NOOP_SIZE = 1;

  constexpr static uint32_t OP_WRITEBD = 1;
  //OP_WRITEBD_SIZE depend on row (9 for 0/1 and 7 for rest)
  constexpr static uint32_t OP_WRITEBD_SIZE_9  = 9;
  constexpr static uint32_t OP_WRITEBD_SIZE_7  = 7;

  constexpr static uint32_t OP_WRITE32 = 2;
  constexpr static uint32_t OP_WRITE32_SIZE = 3;

  constexpr static uint32_t OP_SYNC = 3;
  constexpr static uint32_t OP_SYNC_SIZE = 2;

  constexpr static uint32_t OP_WRITEBD_EXTEND_AIETILE = 4;
  constexpr static uint32_t OP_WRITEBD_EXTEND_AIETILE_SIZE = 8;

  constexpr static uint32_t OP_WRITE32_EXTEND_GENERAL = 5;
  constexpr static uint32_t OP_WRITE32_EXTEND_GENERAL_SIZE = 3;

  constexpr static uint32_t OP_WRITEBD_EXTEND_SHIMTILE = 6;
  constexpr static uint32_t OP_WRITEBD_EXTEND_SHIMTILE_SIZE = 10;

  constexpr static uint32_t OP_WRITEBD_EXTEND_MEMTILE = 7;
  constexpr static uint32_t OP_WRITEBD_EXTEND_MEMTILE_SIZE = 11;

  constexpr static uint32_t OP_WRITE32_EXTEND_DIFFBD = 8;
  constexpr static uint32_t OP_WRITE32_EXTEND_DIFFBD_SIZE = 4;

  constexpr static uint32_t OP_WRITEBD_EXTEND_SAMEBD_MEMTILE = 9;
  constexpr static uint32_t OP_WRITEBD_EXTEND_SAMEBD_MEMTILE_SIZE = 9;

  constexpr static uint32_t OP_DUMPDDR = 10;
  constexpr static uint32_t OP_DUMPDDR_SIZE = 44;

  constexpr static uint32_t OP_WRITESHIMBD = 11;
  constexpr static uint32_t OP_WRITESHIMBD_SIZE = 9;

  constexpr static uint32_t OP_WRITEMEMBD = 12;
  constexpr static uint32_t OP_WRITEMEMBD_SIZE = 9;

  constexpr static uint32_t OP_WRITE32_RTP = 13;
  constexpr static uint32_t OP_WRITE32_RTP_SIZE = 3;

  constexpr static uint32_t OP_READ32 = 14;
  constexpr static uint32_t OP_READ32_SIZE = 2;

  constexpr static uint32_t OP_READ32_POLL = 15;
  constexpr static uint32_t OP_READ32_POLL_SIZE = 4;

  constexpr static uint32_t OP_RECORD_TIMESTAMP = 16;
  constexpr static uint32_t OP_RECORD_TIMESTAMP_SIZE = 1;

  constexpr static uint32_t OP_MERGESYNC = 17;
  constexpr static uint32_t OP_MERGESYNC_SIZE = 1;

  constexpr static uint32_t OP_DUMP_REGISTER = 18;
  // OP_DUMP_REGISTER_SIZE is calculated runtime

protected:
  void patch_shimbd(const uint32_t* ins_buffer, size_t pc, const std::string& section_name);
  virtual uint32_t extractSymbolFromBuffer(std::vector<char>& mc_code, const std::string& section_name, const std::string& argname) override;
};

/*
 * aie2_isa_op_factory definition is required here for the m_mnemonic_table ctor/dtor to be
 * visible to the compiler in places where only this header file is included and an instance
 * of aie2_asm_preprocessor_input is created.
 */
class aie2_isa_op_factory_base {
public:
  aie2_isa_op_factory_base() = default;
  [[nodiscard]] virtual std::unique_ptr<aie2_isa_op> create_aie2_isa_op(const std::vector<std::string>& args) const = 0;
  virtual ~aie2_isa_op_factory_base() = default;

  aie2_isa_op_factory_base(aie2_isa_op_factory_base&& o) = delete;
  aie2_isa_op_factory_base(const aie2_isa_op_factory_base& temp_obj) = delete;
  aie2_isa_op_factory_base& operator=(const aie2_isa_op_factory_base& temp_obj) = delete;
  aie2_isa_op_factory_base& operator=(const aie2_isa_op_factory_base&& temp_obj) = delete;
};

/*
 * This class encodes the ASM version of aie2 ctlrcode into binary
 */
class aie2_asm_preprocessor_input : public aie2_blob_transaction_preprocessor_input
{
private:
  std::vector<char> encode(const std::vector<char>& mc_asm_code);
  std::map<std::string, std::unique_ptr<aie2_isa_op_factory_base>> m_mnemonic_table;

protected:
  std::unique_ptr<aie2_isa_op> assemble_operation(std::shared_ptr<operation> op);

public:
  aie2_asm_preprocessor_input();
  void set_args(const std::vector<char>& mc_asm_code,
                const std::vector<char>& patch_json,
                const std::vector<char>& control_packet,
                const std::vector<std::string>& libs,
                const std::vector<std::string>& libpaths,
                const std::map<uint32_t, std::vector<char> >& ctrlpkt) override
  {
    const std::vector<char> mc_code = encode(mc_asm_code);
    aie2_blob_transaction_preprocessor_input::set_args(mc_code, patch_json, control_packet, libs, libpaths, ctrlpkt);
  }
};

class instance_input
{
  std::map<std::string, std::vector<char>> m_data_common;
  std::map<std::string, std::shared_ptr<aie2_blob_transaction_preprocessor_input>> m_instances;
  std::vector<uint32_t> pm_id_list;
  std::vector<std::string> pdi_id_list;
public:

  const std::map<std::string, std::shared_ptr<aie2_blob_transaction_preprocessor_input>>& get_instance_map() const
  {
    return m_instances;
  }

  const std::map<std::string, std::vector<char>>& get_common() const
  {
    return m_data_common;
  }

  void add_instance(const std::string& instance, std::shared_ptr<aie2_blob_transaction_preprocessor_input> val)
  {
    m_instances[instance] = val;
  }

  void add_common_data(const std::string& dname, std::vector<char> val)
  {
    m_data_common[dname] = std::move(val);
  }

  const std::vector<uint32_t>& get_pm_id_list() const { return pm_id_list; }

  void add_pm_id(uint32_t val) { pm_id_list.push_back(val); }

  const std::vector<std::string>& get_pdi_id_list() const { return pdi_id_list; }

  void add_pdi_id(std::string val) { pdi_id_list.emplace_back(std::move(val)); }
};

class aie2_config_preprocessor_input : public aie2_blob_transaction_preprocessor_input
{
  static constexpr const char* pm_ctrlpkt_type = "pmctrlpkt";
  std::map<std::string, instance_input> kernel_map;
protected:
  void readconfigjson(std::istream& patch_json, const std::vector<std::string>& paths);
  void add_pdi(const std::string& kernel, const boost::property_tree::ptree& pinstance, const std::vector<std::string>& paths);
  void add_instance(const std::string& kernel, const boost::property_tree::ptree& pinstance,const std::vector<std::string>& paths);

  std::string get_pdi_name(uint32_t pdi_id)
  {
    return ".pdi." + std::to_string(pdi_id);
  }

  std::string get_pmctrlpkt_name(uint32_t pdi_id)
  {
    return ".ctrlpkt.pm." + std::to_string(pdi_id);
  }
public:
  void set_args(const std::vector<char>& /*mc_code*/,
                const std::vector<char>& patch_json,
                const std::vector<char>& /*control_packet*/,
                const std::vector<std::string>& /*libs*/,
                const std::vector<std::string>& libpaths,
                const std::map<uint32_t, std::vector<char> >& /*ctrlpkt*/) override
  {
    arg_offset = 0;
    if (patch_json.size() !=0)
    {
      vector_streambuf vsb(patch_json);
      std::istream elf_stream(&vsb);
      readconfigjson(elf_stream, libpaths);
    }
  }

  const std::map<std::string, instance_input>&
  get_kernel_map() const { return kernel_map; }
};

}
#endif //_AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSOR_INPUT_H_

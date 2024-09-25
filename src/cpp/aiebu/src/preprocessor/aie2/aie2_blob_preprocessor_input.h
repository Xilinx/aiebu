// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSOR_INPUT_H_
#define _AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSOR_INPUT_H_

#include <map>
#include "symbol.h"
#include "utils.h"
#include "aiebu_assembler.h"
#include "preprocessor_input.h"
#include <boost/format.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace aiebu {

class aie2_blob_preprocessor_input : public preprocessor_input
{
protected:
  const std::string ctrlText = ".ctrltext";
  const std::string ctrlData = ".ctrldata";
  const std::string preempt_save = ".preempt_save";
  const std::string preempt_restore = ".preempt_restore";
  const std::string preempt_lib = "preempt";
  const std::string scratch_pad = "scratch-pad-mem";

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
  virtual uint32_t extractSymbolFromBuffer(std::vector<char>& mc_code, const std::string& section_name, const std::string& argname) = 0;
  void readmetajson(std::stringstream& patch_json);
  void extract_control_packet_patch(const std::string& name, const boost::property_tree::ptree& _pt);
  void extract_coalesed_buffers(const std::string& name, const boost::property_tree::ptree& _pt);
  void clear_shimBD_address_bits(std::vector<char>& mc_code, uint32_t offset) const;
public:
  aie2_blob_preprocessor_input() {}
  virtual void set_args(const std::vector<char>& mc_code,
                        const std::vector<char>& patch_json,
                        const std::vector<char>& control_packet,
                        const std::vector<std::string>& libs,
                        const std::vector<std::string>& libpaths) override
  {
    m_data[".ctrltext"] = mc_code;
    m_data[".ctrldata"] = control_packet;

    if (patch_json.size() !=0 )
    {
      std::stringstream elf_stream(patch_json.data());
      readmetajson(elf_stream);
    }

    auto col = extractSymbolFromBuffer(m_data[".ctrltext"], ctrlText, control_packet.size() ? "control-packet" : "");

    for (const auto& lib: libs)
    {
      if (lib == preempt_lib)
      {
        m_data[preempt_save] = readfile(findFilePath("preempt_save_stx_4x" + std::to_string(col) + ".bin", libpaths));
        m_data[preempt_restore] = readfile(findFilePath("preempt_restore_stx_4x" + std::to_string(col) + ".bin", libpaths));
        extractSymbolFromBuffer(m_data[preempt_save], preempt_save, scratch_pad);
        extractSymbolFromBuffer(m_data[preempt_restore], preempt_restore, scratch_pad);
      }
      else
        std::cout << "Invalid flag: " << lib << ", ignored !!!" << std::endl;
    }
  }
};

class aie2_blob_transaction_preprocessor_input : public aie2_blob_preprocessor_input
{
protected:
  virtual uint32_t extractSymbolFromBuffer(std::vector<char>& mc_code, const std::string& section_name, const std::string& argname) override;
  void patch_helper(std::vector<char>& mc_code, const std::string& section_name, const std::string& argname,
                    uint32_t reg, uint32_t argidx, uint32_t offset, uint64_t buffer_length_in_bytes, uint32_t addend);
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
                        const std::vector<std::string>& libpaths) override
  {
    aie2_blob_preprocessor_input::set_args(mc_code, patch_json, control_packet, libs, libpaths);
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

}
#endif //_AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSOR_INPUT_H_

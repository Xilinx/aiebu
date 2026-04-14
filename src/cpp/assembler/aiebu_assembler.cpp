// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.
#include "assembler.h"
#include "elfwriter.h"
#include "encoder.h"
#include "preprocessor.h"
#include "preprocessor_input.h"
#include "reporter.h"
#include "symbol.h"
#include "utils.h"
#include "file_utils.h"
#include "transform_manager.h"
#include "logger.h"
#include "aiebu/aiebu.h"
#include "aiebu/aiebu_assembler.h"
#include "aiebu/aiebu_error.h"
#include "json/nlohmann/json.hpp"

#include <algorithm>
#include <cctype>
#include <map>
#include <string>
#include <limits>
#include <iterator>
#include <sstream>

namespace aiebu {

aiebu_assembler::
aiebu_assembler(buffer_type type,
                const std::vector<char>& buffer,
                const std::vector<std::string>& libs,
                const std::vector<std::string>& libpaths,
                const std::vector<char>& patch_json)
                : aiebu_assembler(type, buffer, {}, patch_json, libs, libpaths, {})
{ }

aiebu_assembler::
aiebu_assembler(buffer_type type,
                const std::vector<char>& buffer1,
                const std::vector<char>& buffer2,
                const std::vector<char>& patch_json,
                const std::vector<std::string>& libs,
                const std::vector<std::string>& libpaths,
                const std::map<uint32_t, std::vector<char> >& ctrlpkt) : m_type(type), artifacts()
{
  if (m_type == buffer_type::blob_instr_dpu)
  {
    aiebu::assembler a(assembler::elf_type::aie2_dpu_blob);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, buffer2);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie2;
  }
  else if (m_type == buffer_type::blob_instr_transaction)
  {
    aiebu::assembler a(assembler::elf_type::aie2_transaction_blob);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, buffer2, ctrlpkt);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie2;
  }
  else if (m_type == buffer_type::asm_aie2)
  {
    aiebu::assembler a(assembler::elf_type::aie2_asm);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, buffer2, ctrlpkt);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie2;
  }
  else if (m_type == buffer_type::asm_aie2ps)
  {
    aiebu::assembler a(assembler::elf_type::aie2ps_asm);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, {}, {}, &artifacts);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie2ps;
  }
  else if (m_type == buffer_type::aie2_config)
  {
    aiebu::assembler a(assembler::elf_type::aie2_config);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, buffer2, {}, &artifacts);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie2_config;
  }
  else if (m_type == buffer_type::asm_aie4 ||
           m_type == buffer_type::asm_aie4a ||
           m_type == buffer_type::asm_aie4z)
  {
    // All aie4 family uses same elf_type - specific OSABI determined from .target directive
    aiebu::assembler a(assembler::elf_type::aie4_asm);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, {}, {}, &artifacts);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie4;
  }
  else if (m_type == buffer_type::aie2ps_config)
  {
    aiebu::assembler a(assembler::elf_type::aie2ps_config);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, buffer2, {}, &artifacts);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie2ps_config;
  }
  else if (m_type == buffer_type::aie4_config ||
           m_type == buffer_type::aie4a_config ||
           m_type == buffer_type::aie4z_config)
  {
    // All aie4 family config uses same elf_type - specific OSABI determined from .target directive
    aiebu::assembler a(assembler::elf_type::aie4_config);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, buffer2, {}, &artifacts);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie4_config;
  }
  else {
    throw error(error::error_code::invalid_buffer_type, "Buffer_type not supported !!!");
  }
}

aiebu_assembler::
aiebu_assembler(buffer_type type,
                const std::vector<char>& config_json_buffer,
                const file_artifact& artifacts,
                const std::vector<std::string>& flags)
{
  if (type == buffer_type::aie2_config)
  {
    aiebu::assembler a(assembler::elf_type::aie2_config);
    elf_data = a.process({}, flags, {}, config_json_buffer, {}, {},&artifacts);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie2_config;
  }
  else if (type == buffer_type::aie2ps_config)
  {
    aiebu::assembler a(assembler::elf_type::aie2ps_config);
    elf_data = a.process({}, flags, {}, config_json_buffer, {}, {},&artifacts);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie2ps_config;
  }
  else if (type == buffer_type::aie4_config)
  {
    aiebu::assembler a(assembler::elf_type::aie4_config);
    elf_data = a.process({}, flags, {}, config_json_buffer, {}, {},&artifacts);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie4_config;
  }
  else {
    throw error(error::error_code::invalid_buffer_type, "Buffer_type not supported !!!");
  }
}

std::vector<char>
aiebu_assembler::
get_elf() const
{
  return elf_data;
}

void
aiebu_assembler::
get_report(std::ostream &stream) const
{
  if (!stream) {
    throw error(error::error_code::invalid_input,
                "The given stream is not writable or has failed.");
  }
  reporter rep(m_output_type, elf_data);
  rep.elf_summary(stream);
  rep.ctrlcode_summary(stream);
}

void
aiebu_assembler::
disassemble(const std::filesystem::path &root) const
{
  reporter rep(m_output_type, elf_data);
  rep.disassemble(root, true);
}


// ---------------------------------------------------------------------------
// op_tbl_impl  (PIMPL, defined in .cpp like argtbl_impl)
// ---------------------------------------------------------------------------
class aiebu_assembler::op_tbl_impl
{
  std::vector<op_loc> m_tbl;

public:
  explicit op_tbl_impl(std::vector<op_loc> tbl)
      : m_tbl(std::move(tbl)) {}

  const std::vector<op_loc>&
  get() const
  {
    return m_tbl;
  }
};

// ---------------------------------------------------------------------------
// op_tbl  (public inner class methods)
// ---------------------------------------------------------------------------
aiebu_assembler::op_tbl::
op_tbl(std::shared_ptr<op_tbl_impl> in_impl)
  : handle(std::move(in_impl))
{}

std::vector<aiebu_assembler::op_loc>
aiebu_assembler::op_tbl::
get_line_info() const
{
  return handle->get();
}

// ---------------------------------------------------------------------------
// aiebu_assembler::get_op_locations
// ---------------------------------------------------------------------------

// Look up the ISA name for a raw opcode value using the isa_disassembler map.
// The returned string_view points into the stable isa_disassembler instance.
static std::string_view
opcode_to_name(uint8_t opcode)
{
  static const isa_disassembler s_isa;
  const auto* isa_map = s_isa.get_isa_map();

  auto it = isa_map->find(opcode);
  if (it == isa_map->end())
    throw error(error::error_code::invalid_input,
                "Unknown opcode: 0x" + ELFIO::to_hex_string(opcode) + "\n");

  return it->second.get_code_name();
}

// Parse one .dump JSON blob and append grouped lineinfo entries into loc,
// keeping only entries whose "operation" field starts with the given prefix.
static void
parse_dump_json(const std::string& dump_json, std::string_view op_prefix,
                aiebu_assembler::op_loc& loc)
{
  std::map<uint32_t, size_t> col_index;

  auto jdoc = nlohmann::json::parse(dump_json);
  for (const auto& entry : jdoc.at("debug")) {
    std::string op = entry.value("operation", std::string{});
    std::transform(op.begin(), op.end(), op.begin(), ::tolower);
    if (op.size() < op_prefix.size() ||
        op.compare(0, op_prefix.size(), op_prefix) != 0)
      continue;

    uint32_t col = entry.at("column").get<uint32_t>();
    uint32_t linenum = entry.at("line").get<uint32_t>();
    std::string filename = entry.value("file", std::string{});

    auto it = col_index.find(col);
    if (it == col_index.end()) {
      col_index[col] = loc.line_info.size();
      loc.line_info.push_back({col, {{linenum, std::move(filename)}}});
    } else {
      loc.line_info[it->second].entries.emplace_back(linenum, std::move(filename));
    }
  }
}

aiebu_assembler::op_tbl
aiebu_assembler::
get_op_locations(uint8_t opcode, const std::string& kernel_name) const
{
  transform_manager trans(elf_data);
  std::vector<op_loc> results;
  const auto prefix = opcode_to_name(opcode);

  if (kernel_name.empty()) {
    // Non-config ELF: single .dump section, no group filtering needed.
    trans.check_aie2ps_aie4_elf();
    std::string dump_json = trans.get_dump_section_json();
    if (!dump_json.empty()) {
      op_loc loc;
      parse_dump_json(dump_json, prefix, loc);
      if (!loc.line_info.empty())
        results.push_back(std::move(loc));
    }
  } else {
    // Config ELF: one .dump section per kernel instance, filtered by group.
    trans.check_aie2ps_aie4_fullelf();
    for (const auto& inst_name : trans.get_kernel_instances(kernel_name)) {
      op_loc loc;
      loc.inst_name = inst_name;

      std::string dump_json = trans.get_dump_section_json(kernel_name + ":" + inst_name);
      if (dump_json.empty())
        continue;

      parse_dump_json(dump_json, prefix, loc);
      if (!loc.line_info.empty())
        results.push_back(std::move(loc));
    }
  }

  return op_tbl(std::make_shared<op_tbl_impl>(std::move(results)));
}

aiebu_assembler::op_tbl
aiebu_assembler::
get_op_locations(uint8_t opcode) const
{
  return get_op_locations(opcode, "");
}

class aiebu_assembler::argtbl_impl
{
  std::vector<instinfo> m_tbl;
  std::string m_kernel_name;
  std::string m_new_kernel_name;

  public:
  explicit argtbl_impl(std::vector<instinfo> in_tbl, std::string kernel_name)
      : m_tbl(std::move(in_tbl)), m_kernel_name(std::move(kernel_name)), m_new_kernel_name(m_kernel_name) { }

    std::vector<instinfo>&
    get()
    {
      return m_tbl;
    }

    void
    set_name(const std::string& name)
    {
      // Update new kernel name (ELF update happens in flush_argtbl)
      m_new_kernel_name = name;
    }

    void
    set_kernel_name(const std::string& name)
    {
      // Update new kernel name (ELF update happens in flush_argtbl)
      m_kernel_name = name;
    }

    const std::string&
    get_new_name() const
    {
      return m_new_kernel_name;
    }

    const std::string&
    get_kernel_name() const
    {
      return m_kernel_name;
    }
};

aiebu_assembler::argtbl::
argtbl(std::shared_ptr<argtbl_impl> in_impl)
  : handle(std::move(in_impl))
{}

std::vector<instinfo>&
aiebu_assembler::argtbl::
get()
{
  return handle->get();
}

void
aiebu_assembler::argtbl::
set_name(const std::string& name)
{
  handle->set_name(name);
}

aiebu_assembler::argtbl
aiebu_assembler::
get_argtbl(const std::string& kernel_name)
{
  std::vector<instinfo> tbl;

  transform_manager trans(elf_data);

  // Get all instances for this kernel from the ELF
  // For each instance, extract its arginfo table
  auto instances = trans.get_kernel_instances(kernel_name);

  for (const auto& inst_name : instances) {
    instinfo info;
    info.inst_name = inst_name;

    // Extract relocation info for this kernel:instance
    std::string filter = kernel_name + ":" + inst_name;
    auto args = trans.extract_rela_sections(filter);

    // Convert arginfo to instinfo::arginfo
    for (const auto& arg : args) {
      info.inst_arginfo.push_back({arg.xrt_idx, arg.bd_offset});
    }

    tbl.push_back(std::move(info));
  }

  return argtbl{std::make_shared<argtbl_impl>(std::move(tbl), kernel_name)};
}

void
aiebu_assembler::
flush_argtbl(const argtbl& arg_table)
{
  const auto& impl = arg_table.get_handle();
  const auto& table = impl->get();
  const auto& orig_kernel = impl->get_kernel_name();
  const auto& new_kernel = impl->get_new_name();

  // Update kernel name in ELF if it changed
  if (!orig_kernel.empty() && orig_kernel != new_kernel) {
    transform_manager trans(elf_data);
    elf_data = trans.update_kernel_name(orig_kernel, new_kernel);
    impl->set_kernel_name(new_kernel);
  }

  // Update relocation sections for each instance
  for (const auto& inst : table) {
    std::string filter = new_kernel + ":" + inst.inst_name;

    // Convert instinfo::arginfo to arginfo for transform_manager
    std::vector<arginfo> args;
    for (const auto& arg : inst.inst_arginfo) {
      args.push_back({arg.xrt_idx, arg.bd_offset});
    }

    transform_manager trans(elf_data);
    elf_data = trans.update_rela_sections(args, filter);
  }
}

aiebu_assembler::
aiebu_assembler(const std::string& elf_fnm)
{
  elf_data = readfile(elf_fnm);
}

aiebu_assembler::
aiebu_assembler(const std::vector<char>& buffer): elf_data(buffer) { }

aiebu_assembler::
aiebu_assembler(ELFIO::elfio* elf)
{
  if (!elf)
    throw error(error::error_code::invalid_input, "elf pointer is null");

  std::stringstream stream;
  stream << std::noskipws;
  elf->save(stream);
  std::copy(std::istream_iterator<char>(stream),
            std::istream_iterator<char>(),
            std::back_inserter(elf_data));
}

aiebu_assembler::
aiebu_assembler(const ELFIO::elfio* elf)
  : aiebu_assembler(const_cast<ELFIO::elfio*>(elf))
{ }

class file_artifact_impl
{
  public:
    void add_vfile(const std::string& name, const std::vector<char>& buffer)
    {
      m_artifacts[name] = buffer;
    }

    void add_vfile(std::string& name, std::vector<char>&& buffer)
    {
      m_artifacts[name] = std::move(buffer);
    }

    const std::vector<char>& get(const std::string& name) const
    {
      auto it = m_artifacts.find(name);
      if (it != m_artifacts.end())
        return it->second;
      throw error(error::error_code::invalid_input, "entry not found in artifacts"); 
    }

    std::vector<char> get(const std::string& name, const std::vector<std::string>& paths) const
    {
      auto it = m_artifacts.find(name);
      if (it != m_artifacts.end()) 
        return it->second;
      return readfile(name, paths);
    }

  private:
    std::unordered_map<std::string, std::vector<char>> m_artifacts;
};

file_artifact::
file_artifact() : pimpl(std::make_unique<file_artifact_impl>())
{}

file_artifact::~file_artifact() = default;

void
file_artifact::
add_vfile(const std::string& name, const std::vector<char>& buffer)
{
  pimpl->add_vfile(name, buffer);
}

void
file_artifact::
add_vfile(std::string& name, std::vector<char>&& buffer)
{
  pimpl->add_vfile(name, std::move(buffer));
}

const std::vector<char>&
file_artifact::
get(const std::string& name) const
{
  return pimpl->get(name);
}

std::vector<char>
file_artifact::
get(const std::string& name, const std::vector<std::string>& paths) const
{
  return pimpl->get(name, paths);
}

}

int
aiebu_assembler_get_elf(enum aiebu_assembler_buffer_type type,
                        const char* buffer1,
                        size_t buffer1_size,
                        const char* buffer2,
                        size_t buffer2_size,
                        void** elf_buf,
                        const char* patch_json,
                        size_t patch_json_size,
                        const char* libs,
                        const char* libpaths,
                        struct pm_ctrlpkt* pm_ctrlpkts,
                        size_t pm_ctrlpkt_size)
{
  int ret = 0;
  constexpr size_t MAX_ALLOC =
    static_cast<size_t>(std::numeric_limits<std::ptrdiff_t>::max());
  if (buffer1 == nullptr && buffer1_size != 0)
  {
    aiebu::log_error() << "Invalid buffer1 size\n";
    return -(static_cast<int>(aiebu::error::error_code::invalid_input));
  }

  if (buffer2 == nullptr && buffer2_size != 0)
  {
    aiebu::log_error() << "Invalid buffer2 size\n";
    return -(static_cast<int>(aiebu::error::error_code::invalid_input));
  }

  if (patch_json == nullptr && patch_json_size !=0)
  {
    aiebu::log_error() << "Invalid patch json size\n";
    return -(static_cast<int>(aiebu::error::error_code::invalid_input));
  }

  try
  {
    std::vector<char> v1, v2, v3;
    std::vector<char> velf;

    v1.assign(buffer1, buffer1+buffer1_size);
    v2.assign(buffer2, buffer2+buffer2_size);
    v3.assign(patch_json, patch_json+patch_json_size);
    std::map<uint32_t, std::vector<char> > mctrlpkt;

    std::vector<std::string> vlibs;
    if (libs)
      vlibs = aiebu::splitoption(libs);

    std::vector<std::string> vlibpaths;
    if (libpaths)
      vlibpaths = aiebu::splitoption(libpaths);

    for (auto i=0ul; i < pm_ctrlpkt_size; i++)
    {
      std::vector<char> v(pm_ctrlpkts[i].pm_buffer, pm_ctrlpkts[i].pm_buffer + pm_ctrlpkts[i].pm_buffer_size);
      mctrlpkt[pm_ctrlpkts[i].pm_id] = std::move(v);
    }

    aiebu::aiebu_assembler handler((aiebu::aiebu_assembler::buffer_type)type, v1, v2, v3, vlibs, vlibpaths, mctrlpkt);
    velf = handler.get_elf();

    if (velf.size() > MAX_ALLOC)
      throw std::bad_alloc();
    char* aelf = static_cast<char*>(std::malloc(velf.size()));
    if (!aelf)
      throw std::bad_alloc();

    std::copy(velf.begin(), velf.end(), aelf);
    *elf_buf = (void*)aelf;
    ret =  static_cast<int>(velf.size());
  }
  catch (aiebu::error &ex)
  {
    aiebu::log_error() << ex.what() << "\n";
    ret = -(ex.get_code());
  }
  catch (std::exception &ex)
  {
    aiebu::log_error() << ex.what() << "\n";
    ret = -(static_cast<int>(aiebu::error::error_code::internal_error));
  }
  return ret;
}

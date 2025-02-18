// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.
#include <cstdint>
#include <memory>
#include <string>

#include "aie2_blob_preprocessor_input.h"
#include "asm/asm_parser.h"
#include "utils.h"

#include "xaiengine.h"
#include "xaiengine/xaiegbl.h"

namespace aiebu {

class aie2_isa_op;

const std::map<std::string, XAie_Preempt_level> preempt_level_table = {
    {"NOOP",          NOOP},
    {"MEM_TILE",      MEM_TILE},
    {"AIE_TILE",      AIE_TILE},
    {"AIE_REGISTERS", AIE_REGISTERS},
    {"INVALID",       INVALID},
};

const std::map<XAie_TxnOpcode, std::string> opcode_table = {
    {XAIE_IO_WRITE,                   "XAIE_IO_WRITE"},
    {XAIE_IO_BLOCKWRITE,              "XAIE_IO_BLOCKWRITE"},
    {XAIE_IO_MASKWRITE,               "XAIE_IO_MASKWRITE"},
    {XAIE_IO_MASKPOLL,                "XAIE_IO_MASKPOLL"},
    {XAIE_IO_NOOP,                    "XAIE_IO_NOOP"},
    {XAIE_IO_PREEMPT,                 "XAIE_IO_PREEMPT"},
    {XAIE_IO_MASKPOLL_BUSY,           "XAIE_IO_MASKPOLL_BUSY"},
    {XAIE_IO_LOADPDI,                 "XAIE_IO_LOADPDI"},
    {XAIE_IO_LOAD_PM_START,           "XAIE_IO_LOAD_PM_START"},
    {XAIE_CONFIG_SHIMDMA_BD,          "XAIE_CONFIG_SHIMDMA_BD"},
    {XAIE_CONFIG_SHIMDMA_DMABUF_BD,  "XAIE_CONFIG_SHIMDMA_DMABUF_BD"},
    {XAIE_IO_CUSTOM_OP_TCT,          "XAIE_IO_CUSTOM_OP_TCT"},
    {XAIE_IO_CUSTOM_OP_DDR_PATCH,    "XAIE_IO_CUSTOM_OP_DDR_PATCH"},
    {XAIE_IO_CUSTOM_OP_READ_REGS,    "XAIE_IO_CUSTOM_OP_READ_REGS"},
    {XAIE_IO_CUSTOM_OP_RECORD_TIMER, "XAIE_IO_CUSTOM_OP_RECORD_TIMER"},
    {XAIE_IO_CUSTOM_OP_MERGE_SYNC,   "XAIE_IO_CUSTOM_OP_MERGE_SYNC"},
    {XAIE_IO_CUSTOM_OP_NEXT,         "XAIE_IO_CUSTOM_OP_NEXT"},
    {XAIE_IO_LOAD_PM_END_INTERNAL,   "XAIE_IO_LOAD_PM_END_INTERNAL"}
};

/*
 * Base class to represent a ctrlcode operation instance. Each ctrlcode operation defined
 * by the specification requires its own derived class which are defined below.
 * Derived classed encapsulate the appropriate 'specialized' version of XAie_OpHdr.
 */
class aie2_isa_op {
private:
  /*
   * Total size of this op instance in binary including extended attributes.
   * It is populated by the derived class as some operations are variable sized.
   */
  size_t m_size = 0;
  const XAie_TxnOpcode m_code;

protected:
  XAie_OpHdr *m_op =  nullptr;

protected:
  /*
   * Method to get access to the variable size portion of the operation.
   * e.g. BLOCKWRITE, TCT, etc.
   */
  template <typename RAWTYPE> [[nodiscard]] RAWTYPE *get_extended_storage() const {
    char *start = reinterpret_cast<char *>(m_op);
    start += get_op_base_size();
    assert((reinterpret_cast<std::uintptr_t>(start) % alignof(RAWTYPE)) == 0);
    return reinterpret_cast<RAWTYPE *>(start);
  }

  void initialize_OpHdr(size_t size)  {
    m_size = size;
    char *storage = new char[m_size];
    std::memset(storage, 0, m_size);
    m_op = reinterpret_cast<XAie_OpHdr *>(storage);
    m_op->Op = m_code;
  }

  void operand_count_check(const std::vector<std::string>& args, unsigned int size) const {
    if (args.size() >= size)
      return;
    std::string msg = get_mnemonic();
    msg += " requires at least " + std::to_string(size) + " operands";
    throw error(error::error_code::invalid_asm, msg);
  }

public:
  explicit aie2_isa_op(XAie_TxnOpcode code) : m_code(code) {}

  virtual ~aie2_isa_op() {
    if (m_op == nullptr)
      return;
    char *storage = reinterpret_cast<char *>(m_op);
    delete [] storage;
    m_op = nullptr;
  }

  aie2_isa_op(aie2_isa_op&& o) noexcept : m_size(o.m_size),
                                          m_code(o.m_code),
                                          m_op(o.m_op) {
    o.m_op = nullptr;
    o.m_size = 0;
  }

  aie2_isa_op(const aie2_isa_op& temp_obj) = delete;
  aie2_isa_op& operator=(const aie2_isa_op& temp_obj) = delete;
  aie2_isa_op& operator=(const aie2_isa_op&& temp_obj) = delete;

  /* Serialize the operation to binary that can be the processed by aie2 blob */
  virtual void serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char *>(m_op), m_size);
  }

  [[nodiscard]] const std::string& get_mnemonic() const {
    return opcode_table.at(m_code);
  }

  [[nodiscard]] virtual size_t get_op_base_size() const = 0;

  [[nodiscard]] size_t get_op_size() const {
    return m_size;
  }
};

class XAIE_IO_WRITE_op : public aie2_isa_op {
public:
  explicit XAIE_IO_WRITE_op(const std::vector<std::string>& args) : aie2_isa_op(XAIE_IO_WRITE)
  {
    // e.g. XAIE_IO_WRITE                  @0x801d214, 0x30005
    operand_count_check(args, 2);
    std::string regoff = args[0].substr(1);
    initialize_OpHdr(sizeof(XAie_Write32Hdr));

    auto op = reinterpret_cast<XAie_Write32Hdr *>(m_op);
    op->RegOff = to_uinteger<uint64_t>(regoff);
    op->Value = to_uinteger<uint32_t>(args[1]);
    op->Size = sizeof(XAie_Write32Hdr);
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_Write32Hdr);
  }
};


class XAIE_IO_BLOCKWRITE_op : public aie2_isa_op {
public:
  explicit XAIE_IO_BLOCKWRITE_op(const std::vector<std::string>& args) : aie2_isa_op(XAIE_IO_BLOCKWRITE)
  {
    // e.g. XAIE_IO_BLOCKWRITE             @0x801d060, [8]
    // [0] 0x3
    // [1] 0x0
    // [2] 0x0
    // [3] 0x0
    // [4] 0x80000000
    // [5] 0x2000000
    // [6] 0x100007
    // [7] 0x2000000
    operand_count_check(args, 2);
    unsigned idx = 0;
    std::string regoff = args[idx++].substr(1);
    // Determine the total size including extended storage by counting the number of writes
    initialize_OpHdr(sizeof(XAie_BlockWrite32Hdr) + sizeof(uint32_t) * (args.size() - idx));

    auto op = reinterpret_cast<XAie_BlockWrite32Hdr *>(m_op);
    op->RegOff = to_uinteger<uint64_t>(regoff);
    op->Size = get_op_size();
    // Capture the extended values
    auto values = get_extended_storage<unsigned int>();
    for (unsigned int i = 0; idx < args.size(); idx++, i++)
      values[i] = to_uinteger<uint32_t>(args[idx]);
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_BlockWrite32Hdr);
  }
};

class XAIE_IO_MASKWRITE_op : public aie2_isa_op {
public:
  explicit XAIE_IO_MASKWRITE_op(const std::vector<std::string>& args) : aie2_isa_op(XAIE_IO_MASKWRITE) {
    operand_count_check(args, 3);
    initialize_OpHdr(sizeof(XAie_MaskWrite32Hdr));
    std::string regoff = args[0].substr(1);

    auto op = reinterpret_cast<XAie_MaskWrite32Hdr *>(m_op);
    op->RegOff = to_uinteger<uint64_t>(regoff);
    op->Value = to_uinteger<uint32_t>(args[1]);
    op->Mask = to_uinteger<uint32_t>(args[2]);
    op->Size = get_op_size();
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_MaskWrite32Hdr);
  }
};

class XAIE_IO_MASKPOLL_op : public aie2_isa_op {
public:
  explicit XAIE_IO_MASKPOLL_op(const std::vector<std::string>& args) : aie2_isa_op(XAIE_IO_MASKPOLL) {
    // e.g. XAIE_IO_MASKPOLL,               @0x801d228, 0x78003c()==0x0
    operand_count_check(args, 2);
    initialize_OpHdr(sizeof(XAie_MaskPoll32Hdr));
    unsigned idx = 0;
    const std::string regoff = args[idx++].substr(1);

    auto op = reinterpret_cast<XAie_MaskPoll32Hdr *>(m_op);
    op->RegOff = to_uinteger<uint64_t>(regoff);

    const std::regex mask_poll_regex = get_regex({fragment::begin_anchor_re, fragment::hex_re, fragment::l_brack_re,
        fragment::r_brack_re, fragment::equal_re, fragment::hex_re, fragment::end_anchor_re});

//    const std::regex mask_poll_regex("^" + HEX_RE + L_BRACK_RE + R_BRACK_RE + "==" + HEX_RE + "$");
    std::smatch matches;
    if (!std::regex_match(args[idx], matches, mask_poll_regex))
        throw error(error::error_code::invalid_asm, args[idx]);

    if (matches.size() != 3)
        throw error(error::error_code::invalid_asm, args[idx]);


    op->Mask = to_uinteger<uint32_t>(matches[1]);
    op->Value = to_uinteger<uint32_t>(matches[2]);
    op->Size = get_op_size();
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_MaskPoll32Hdr);
  }
};


class XAIE_IO_NOOP_op : public aie2_isa_op {
public:
  // e.g. XAIE_IO_NOOP
  explicit XAIE_IO_NOOP_op(const std::vector<std::string>& args) : aie2_isa_op(XAIE_IO_NOOP) {
    operand_count_check(args, 0);
    initialize_OpHdr(sizeof(XAie_NoOpHdr));
  }
  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_NoOpHdr);
  }
};


class XAIE_IO_PREEMPT_op : public aie2_isa_op {
public:
  // e.g. XAIE_IO_PREEMPT MEM_TILE
  explicit XAIE_IO_PREEMPT_op(const std::vector<std::string>& args) : aie2_isa_op(XAIE_IO_PREEMPT) {
    operand_count_check(args, 1);
    initialize_OpHdr(sizeof(XAie_PreemptHdr));

    auto op = reinterpret_cast<XAie_PreemptHdr *>(m_op);
    op->Preempt_level = preempt_level_table.at(args[0]);
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_PreemptHdr);
  }
};


class XAIE_IO_LOADPDI_op : public aie2_isa_op {
public:
  explicit XAIE_IO_LOADPDI_op(const std::vector<std::string>& args) : aie2_isa_op(XAIE_IO_LOADPDI) {
    operand_count_check(args, 3);
    initialize_OpHdr(sizeof(XAie_LoadPdiHdr));

    auto op = reinterpret_cast<XAie_LoadPdiHdr *>(m_op);
    op->PdiId = to_uinteger<uint16_t>(args[0]);
    op->PdiSize = to_uinteger<uint16_t>(args[1]);
    op->PdiAddress = to_uinteger<uint64_t>(args[2]);
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_LoadPdiHdr);
  }
};


class XAIE_IO_LOAD_PM_START_op : public aie2_isa_op {
public:
  explicit XAIE_IO_LOAD_PM_START_op(const std::vector<std::string>& args) : aie2_isa_op(XAIE_IO_LOAD_PM_START) {
    operand_count_check(args, 2);
    initialize_OpHdr(sizeof(XAie_PmLoadHdr));

    auto op = reinterpret_cast<XAie_PmLoadHdr *>(m_op);
    const auto load_seq = to_uinteger<uint32_t>(args[0]);
    for (unsigned int i = 0; i < 3; i++) {
      op->LoadSequenceCount[i] = static_cast<uint8_t>((load_seq >> i) & 0xff);
    }
    op->PmLoadId = to_uinteger<uint32_t>(args[1]);
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_PmLoadHdr);
  }
};


class XAIE_IO_CUSTOM_OP_TCT_op : public aie2_isa_op {
public:
  explicit XAIE_IO_CUSTOM_OP_TCT_op(const std::vector<std::string>& args) : aie2_isa_op(XAIE_IO_CUSTOM_OP_TCT) {
    operand_count_check(args, 1);
    initialize_OpHdr(sizeof(XAie_CustomOpHdr) + sizeof(tct_op_t));

    auto op = reinterpret_cast<XAie_CustomOpHdr *>(m_op);
    op->Size = get_op_size();

    auto values = get_extended_storage<tct_op_t>();
    values->word = to_uinteger<uint32_t>(args[0]);
    values->config = to_uinteger<uint32_t>(args[1]);
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_CustomOpHdr);
  }
};


class XAIE_IO_CUSTOM_OP_DDR_PATCH_op : public aie2_isa_op {
public:
  explicit XAIE_IO_CUSTOM_OP_DDR_PATCH_op(const std::vector<std::string>& args) : aie2_isa_op(XAIE_IO_CUSTOM_OP_DDR_PATCH) {
    operand_count_check(args, 1);
    initialize_OpHdr(sizeof(XAie_CustomOpHdr) + sizeof(patch_op_t));

    auto op = reinterpret_cast<XAie_CustomOpHdr *>(m_op);
    op->Size = get_op_size();
    auto values = get_extended_storage<patch_op_t>();

    const std::string regoff = args[0].substr(1);
    values->regaddr = to_uinteger<uint64_t>(regoff);
    values->argidx = to_uinteger<uint64_t>(args[1]);
    values->argplus = to_uinteger<uint64_t>(args[2]);
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_CustomOpHdr);
  }
};

/*
 * Templatized factory based constructor for instances of classes derived from aie2_isa_op
 */
template <typename ISA_OP> class aie2_isa_op_factory : public aie2_isa_op_factory_base {
public:
  aie2_isa_op_factory() = default;
  [[nodiscard]] std::unique_ptr<aie2_isa_op> create_aie2_isa_op(const std::vector<std::string>& args) const override {
    return std::make_unique<ISA_OP>(ISA_OP(args));
  }
};

/*
 * TODO: How to initialize a global constant std::map with a std::unique_ptr as 'value'? The
 * C++ compilers do not know how to copy the std::pair with a std::unique_ptr inside which is
 * required for this mnemonic_table table's initialization :-(
 * Till then we manually initialize the table as part of aie2_asm_preprocessor_input constructor.
 *
const std::map<std::string, std::unique_ptr<aie2_isa_op_factory>> mnemonic_table = {
  {"XAIE_IO_WRITE", std::make_unique<aie2_isa_op_factory_special<XAIE_IO_WRITE_op>>()},
  {"XAIE_IO_BLOCKWRITE", std::make_unique<aie2_isa_op_factory_special<XAIE_IO_BLOCKWRITE_op>>()},
  {"XAIE_IO_MASKWRITE", std::make_unique<aie2_isa_op_factory_special<XAIE_IO_MASKWRITE_op>>()},
  {"XAIE_IO_MASKWPOLL", std::make_unique<aie2_isa_op_factory_special<XAIE_IO_MASKPOLL_op>>()},
  {"XAIE_IO_NOOP", std::make_unique<aie2_isa_op_factory_special<XAIE_IO_NOOP_op>>()},
  {"XAIE_IO_PREEMPT", std::make_unique<aie2_isa_op_factory_special<XAIE_IO_PREEMPT_op>>()},
  {"XAIE_IO_LOADPDI", std::make_unique<aie2_isa_op_factory_special<XAIE_IO_LOADPDI_op>>()},
  {"XAIE_IO_LOAD_PM_START", std::make_unique<aie2_isa_op_factory_special<XAIE_IO_LOAD_PM_START_op>>()},
};
*/

/*
 * Populate the contructor map for the templatized factory function. The map is used to bind
 * operand token to the factory function. Ideally this map should be populated as static
 * const but I ran into linker problems documented above.
 */
aie2_asm_preprocessor_input::aie2_asm_preprocessor_input() {
  m_mnemonic_table.emplace("XAIE_IO_WRITE", std::make_unique<aie2_isa_op_factory<XAIE_IO_WRITE_op>>());
  m_mnemonic_table.emplace("XAIE_IO_BLOCKWRITE", std::make_unique<aie2_isa_op_factory<XAIE_IO_BLOCKWRITE_op>>());
  m_mnemonic_table.emplace("XAIE_IO_MASKWRITE", std::make_unique<aie2_isa_op_factory<XAIE_IO_MASKWRITE_op>>());
  m_mnemonic_table.emplace("XAIE_IO_MASKPOLL", std::make_unique<aie2_isa_op_factory<XAIE_IO_MASKPOLL_op>>());
  m_mnemonic_table.emplace("XAIE_IO_NOOP", std::make_unique<aie2_isa_op_factory<XAIE_IO_NOOP_op>>());
  m_mnemonic_table.emplace("XAIE_IO_PREEMPT", std::make_unique<aie2_isa_op_factory<XAIE_IO_PREEMPT_op>>());
  m_mnemonic_table.emplace("XAIE_IO_LOADPDI", std::make_unique<aie2_isa_op_factory<XAIE_IO_LOADPDI_op>>());
  m_mnemonic_table.emplace("XAIE_IO_LOAD_PM_START", std::make_unique<aie2_isa_op_factory<XAIE_IO_LOAD_PM_START_op>>());
  m_mnemonic_table.emplace("XAIE_IO_CUSTOM_OP_TCT", std::make_unique<aie2_isa_op_factory<XAIE_IO_CUSTOM_OP_TCT_op>>());
  m_mnemonic_table.emplace("XAIE_IO_CUSTOM_OP_DDR_PATCH", std::make_unique<aie2_isa_op_factory<XAIE_IO_CUSTOM_OP_DDR_PATCH_op>>());
}

std::unique_ptr<aie2_isa_op> aie2_asm_preprocessor_input::assemble_operation(std::shared_ptr<operation> op)
{
  std::string name = op->get_name();
  std::transform(name.begin(), name.end(), name.begin(), ::toupper);
  auto iter  = m_mnemonic_table.find(name);

  if (iter == m_mnemonic_table.end())
    throw error(error::error_code::invalid_asm, "Invalid opcode " + op->get_name());

  /* Look up the matching factory and construct the aie2_isa_op */
  return iter->second->create_aie2_isa_op(op->get_args());;
}

std::vector<char>
aie2_asm_preprocessor_input::encode(const std::vector<char>& mc_asm_code) {
  std::shared_ptr<asm_parser> a(new asm_parser(mc_asm_code, {}));
  a->parse_lines();
  std::stringstream store;

  auto collist = a->get_col_list();
  XAie_TxnHeader hdr = {0, 1, 4, 6, static_cast<uint8_t>(collist.size()), 1, 0, 0, 0};
  store.write(reinterpret_cast<const char *>(&hdr), sizeof(hdr));

  std::vector<std::unique_ptr<aie2_isa_op>> isa_op_list;

  /* The ASM lines hang off the last column referred by.attach_to_group directive */
  auto coldata = a->get_col_asmdata(collist.size() - 1);
  auto labels = a->getLabelsforcol(collist.size() - 1);
  if (labels.size() != 1)
    throw error(error::error_code::invalid_asm, "aie2 ctrlcode should have only one label");
  for (auto line : coldata.get_label_asmdata(labels.front())) {
    std::unique_ptr<aie2_isa_op> isa_op = assemble_operation(line->get_operation());
    isa_op_list.push_back(std::move(isa_op));
  }

  /* Now serialize all the isa_ops */
  for (const auto& isa_op : isa_op_list) {
    isa_op->serialize(store);
  }
  store.seekp(0, std::ios_base::end);
  std::streamoff size = store.tellp();
  store.seekp(0);

  hdr.TxnSize = size;
  hdr.NumOps = isa_op_list.size();
  store.write(reinterpret_cast<const char *>(&hdr), sizeof(hdr));

  std::vector<char> result(size);
  const std::string binary = store.str();
  std::copy(binary.begin(), binary.end(), result.begin());
  return result;
}
}

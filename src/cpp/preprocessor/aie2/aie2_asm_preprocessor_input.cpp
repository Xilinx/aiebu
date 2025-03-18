// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.
#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "aie2_blob_preprocessor_input.h"
#include "asm/asm_parser.h"
#include "utils.h"

#include "xaiengine.h"
#include "xaiengine/xaiegbl.h"

namespace aiebu {

class aie2_isa_op;

const std::map<std::string, XAie_Preempt_level> preempt_level_table = { //NOLINT
    {"#noop",          NOOP},
    {"#mem_tile",      MEM_TILE},
    {"#aie_tile",      AIE_TILE},
    {"#aie_registers", AIE_REGISTERS},
    {"#invalid",       INVALID},
};

const std::map<std::string, uint8_t> dma_direction_table = { //NOLINT
    {"#dma_s2mm",      0},
    {"#dma_mm2s",      1},
};

const std::map<XAie_TxnOpcode, std::string> opcode_table = { //NOLINT
    {XAIE_IO_WRITE,                   "xaie_io_write"},
    {XAIE_IO_BLOCKWRITE,              "xaie_io_blockwrite"},
    {XAIE_IO_MASKWRITE,               "xaie_io_maskwrite"},
    {XAIE_IO_MASKPOLL,                "xaie_io_maskpoll"},
    {XAIE_IO_NOOP,                    "xaie_io_noop"},
    {XAIE_IO_PREEMPT,                 "xaie_io_preempt"},
    {XAIE_IO_MASKPOLL_BUSY,           "xaie_io_maskpoll_busy"},
    {XAIE_IO_LOADPDI,                 "xaie_io_loadpdi"},
    {XAIE_IO_LOAD_PM_START,           "xaie_io_load_pm_start"},
    {XAIE_CONFIG_SHIMDMA_BD,          "xaie_config_shimdma_bd"},
    {XAIE_CONFIG_SHIMDMA_DMABUF_BD,  "xaie_config_shimdma_dmabuf_bd"},
    {XAIE_IO_CUSTOM_OP_TCT,          "xaie_io_custom_op_tct"},
    {XAIE_IO_CUSTOM_OP_DDR_PATCH,    "xaie_io_custom_op_ddr_patch"},
    {XAIE_IO_CUSTOM_OP_READ_REGS,    "xaie_io_custom_op_read_regs"},
    {XAIE_IO_CUSTOM_OP_RECORD_TIMER, "xaie_io_custom_op_record_timer"},
    {XAIE_IO_CUSTOM_OP_MERGE_SYNC,   "xaie_io_custom_op_merge_sync"},
    {XAIE_IO_CUSTOM_OP_NEXT,         "xaie_io_custom_op_next"},
    {XAIE_IO_LOAD_PM_END_INTERNAL,   "xaie_io_load_pm_end_internal"}
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

  [[nodiscard]] virtual unsigned int outstanding_ext_op_count() const {
    return 0;
  }

  virtual void process_outstanding_ext_op(const std::shared_ptr<operation> op) {
    const std::vector<std::string>& args = op->get_args();
    throw error(error::error_code::internal_error, opcode_table.at(m_code) +
                    " does not require extended operands" + args[0]);
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
private:
  unsigned int outstanding_extended_operand_count = 0;

  [[nodiscard]] unsigned int get_extended_operand_index() const {
    size_t ex_op_size = get_op_size() - get_op_base_size();
    return ex_op_size / sizeof(uint32_t) - outstanding_extended_operand_count;
  }

public:
  explicit XAIE_IO_BLOCKWRITE_op(const std::vector<std::string>& args) : aie2_isa_op(XAIE_IO_BLOCKWRITE)
  {
    // e.g. XAIE_IO_BLOCKWRITE             @0x801d060, [8]
    //      XAIE_IO_BLOCKWRITE.0           0x6
    //      XAIE_IO_BLOCKWRITE.1           0x0
    //      XAIE_IO_BLOCKWRITE.2           0x0
    //      XAIE_IO_BLOCKWRITE.3           0x0
    //      XAIE_IO_BLOCKWRITE.4           0x80000000
    //      XAIE_IO_BLOCKWRITE.5           0x2000000
    //      XAIE_IO_BLOCKWRITE.6           0x300007
    //      XAIE_IO_BLOCKWRITE.7           0x2000000

    operand_count_check(args, 2);
    unsigned int idx = 0;
    std::string regoff = args[idx++].substr(1);
    // Determine the total size including extended storage by counting the number of writes

    static const std::regex index_regex = get_regex({fragment::index_re});

    std::smatch matches;
    if (!std::regex_match(args[idx], matches, index_regex))
        throw error(error::error_code::invalid_asm, args[idx]);

    if (matches.size() != 2)
        throw error(error::error_code::invalid_asm, args[1]);

    outstanding_extended_operand_count = to_uinteger<uint32_t>(matches[1]);
    initialize_OpHdr(sizeof(XAie_BlockWrite32Hdr) +
                     sizeof(uint32_t) * outstanding_extended_operand_count);

    auto op = reinterpret_cast<XAie_BlockWrite32Hdr *>(m_op);
    op->RegOff = to_uinteger<uint64_t>(regoff);
    op->Size = get_op_size();
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_BlockWrite32Hdr);
  }

  [[nodiscard]] unsigned int outstanding_ext_op_count() const override {
    return outstanding_extended_operand_count;
  }

  [[nodiscard]] unsigned int total_extended_operand_count() const {
    return (get_op_size() - get_op_base_size()) / sizeof(uint32_t);
  }

  void process_outstanding_ext_op(const std::shared_ptr<operation> op) override {
    if (outstanding_extended_operand_count == 0)
      throw error(error::error_code::invalid_asm, "This instance of " + get_mnemonic() +
                      " cannot have more than " + std::to_string(total_extended_operand_count()) + " operands");

    const std::vector<std::string>& args = op->get_args();
    std::string base = opcode_table.at(XAIE_IO_BLOCKWRITE);
    base += ".";
    base += std::to_string(get_extended_operand_index());

    if (base.compare(op->get_name()))
      throw error(error::error_code::invalid_asm, "Invalid operand " + op->get_name());

    auto values = get_extended_storage<unsigned int>();
    values[get_extended_operand_index()] = to_uinteger<uint32_t>(args[0]);
    outstanding_extended_operand_count--;
  }
};

class XAIE_IO_MASKWRITE_op : public aie2_isa_op {
public:
  explicit XAIE_IO_MASKWRITE_op(const std::vector<std::string>& args) : aie2_isa_op(XAIE_IO_MASKWRITE) {
    // e.g. XAIE_IO_MASKWRITE             @0x801d060, 0x30005(), 0x3
    operand_count_check(args, 3);
    initialize_OpHdr(sizeof(XAie_MaskWrite32Hdr));
    std::string regoff = args[0].substr(1);

    auto op = reinterpret_cast<XAie_MaskWrite32Hdr *>(m_op);
    op->RegOff = to_uinteger<uint64_t>(regoff);
    static const std::regex mask_regex = get_regex({fragment::begin_anchor_re, fragment::hex_re, fragment::l_brack_re,
        fragment::r_brack_re, fragment::end_anchor_re});

    std::smatch matches;
    if (!std::regex_match(args[1], matches, mask_regex))
        throw error(error::error_code::invalid_asm, args[1]);

    if (matches.size() != 2)
        throw error(error::error_code::invalid_asm, args[1]);

    op->Mask = to_uinteger<uint32_t>(matches[1]);
    op->Value = to_uinteger<uint32_t>(args[2]);
    op->Size = get_op_size();
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_MaskWrite32Hdr);
  }
};

class XAIE_IO_MASKPOLL_op : public aie2_isa_op {
public:
  explicit XAIE_IO_MASKPOLL_op(const std::vector<std::string> &args,
                               XAie_TxnOpcode code = XAIE_IO_MASKPOLL) : aie2_isa_op(code) {
    // e.g. XAIE_IO_MASKPOLL,               @0x801d228, 0x78003c()==0x0
    operand_count_check(args, 2);
    initialize_OpHdr(sizeof(XAie_MaskPoll32Hdr));
    unsigned int idx = 0;
    const std::string regoff = args[idx++].substr(1);

    auto op = reinterpret_cast<XAie_MaskPoll32Hdr *>(m_op);
    op->RegOff = to_uinteger<uint64_t>(regoff);

    static const std::regex mask_poll_regex = get_regex({fragment::begin_anchor_re, fragment::hex_re, fragment::l_brack_re,
        fragment::r_brack_re, fragment::equal_re, fragment::hex_re, fragment::end_anchor_re});

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


class XAIE_IO_MASKPOLL_BUSY_op : public XAIE_IO_MASKPOLL_op {
public:
  /* Only difference from XAIE_IO_MASKPOLL_op is the code */
  explicit XAIE_IO_MASKPOLL_BUSY_op(const std::vector<std::string> &args)
      : XAIE_IO_MASKPOLL_op(args, XAIE_IO_MASKPOLL_BUSY) {
    // e.g. XAIE_IO_MASKPOLL_BUSY               @0x801d228, 0x78003c()==0x0
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
  // e.g. XAIE_IO_PREEMPT                       #MEM_TILE
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
  // e.g. XAIE_IO_LOADPDI              #0, 0x100, 0x80004000
  explicit XAIE_IO_LOADPDI_op(const std::vector<std::string>& args) : aie2_isa_op(XAIE_IO_LOADPDI) {
    operand_count_check(args, 3);
    initialize_OpHdr(sizeof(XAie_LoadPdiHdr));

    auto op = reinterpret_cast<XAie_LoadPdiHdr *>(m_op);
    op->PdiId = to_uinteger<uint16_t>(args[0].substr(1));
    op->PdiSize = to_uinteger<uint16_t>(args[1]);
    op->PdiAddress = to_uinteger<uint64_t>(args[2]);
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_LoadPdiHdr);
  }
};


class XAIE_IO_LOAD_PM_START_op : public aie2_isa_op {
public:
  // e.g. XAIE_IO_LOAD_PM_START           0x158, #0
  explicit XAIE_IO_LOAD_PM_START_op(const std::vector<std::string> &args)
      : aie2_isa_op(XAIE_IO_LOAD_PM_START) {
    operand_count_check(args, 2);
    initialize_OpHdr(sizeof(XAie_PmLoadHdr));

    auto op = reinterpret_cast<XAie_PmLoadHdr *>(m_op);
    const auto load_seq = to_uinteger<uint32_t>(args[0]);
    for (unsigned int i = 0; i < 3; i++) {
      op->LoadSequenceCount[i] = static_cast<uint8_t>((load_seq >> i * 8) & 0xff);
    }
    op->PmLoadId = to_uinteger<uint32_t>(args[1].substr(1));
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_PmLoadHdr);
  }
};


class XAIE_IO_CUSTOM_OP_TCT_op : public aie2_isa_op {
private:
  std::pair<uint8_t, uint8_t> parse_index(const std::regex &regx, const std::string &token) const {
    std::smatch cmatches;
    if (!std::regex_match(token, cmatches, regx))
        throw error(error::error_code::invalid_asm, token);

    if (cmatches.size() !=3)
        throw error(error::error_code::invalid_asm, token);

    return std::make_pair<uint8_t, uint8_t>(to_uinteger<uint8_t>(cmatches[1]), to_uinteger<uint8_t>(cmatches[2]));
  }

public:
  explicit XAIE_IO_CUSTOM_OP_TCT_op(const std::vector<std::string>& args) : aie2_isa_op(XAIE_IO_CUSTOM_OP_TCT) {
    operand_count_check(args, 4);
    initialize_OpHdr(sizeof(XAie_CustomOpHdr) + sizeof(tct_op_t));

    auto op = reinterpret_cast<XAie_CustomOpHdr *>(m_op);
    op->Size = get_op_size();

    auto values = get_extended_storage<tct_op_t>();
    unsigned int idx = 0;
    static const std::regex row_regex = get_regex({fragment::row, fragment::add_dec_re});
    static const std::regex col_regex = get_regex({fragment::column, fragment::add_dec_re});

    std::pair<uint8_t, uint8_t> row_val = parse_index(row_regex, args[idx++]);
    std::pair<uint8_t, uint8_t> col_val = parse_index(col_regex, args[idx++]);

    uint8_t dir =  dma_direction_table.at(args[idx++]);
    uint8_t channel = to_uinteger<uint32_t>(args[idx]);

    values->word = col_val.first;
    values->word <<= 8;
    values->word |= row_val.first;
    values->word <<= 8;
    values->word |= dir;

    values->config = channel;
    values->config <<= 8;
    values->config |= col_val.second;
    values->config <<= 8;
    values->config |= row_val.second;
    values->config <<= 8;
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_CustomOpHdr);
  }
};


class XAIE_IO_CUSTOM_OP_MERGE_SYNC_op : public aie2_isa_op {
public:
  explicit XAIE_IO_CUSTOM_OP_MERGE_SYNC_op(const std::vector<std::string>& args) : aie2_isa_op(XAIE_IO_CUSTOM_OP_MERGE_SYNC) {
    operand_count_check(args, 1);
    initialize_OpHdr(sizeof(XAie_CustomOpHdr) + sizeof(tct_op_t));

    auto op = reinterpret_cast<XAie_CustomOpHdr *>(m_op);
    op->Size = get_op_size();

    auto values = get_extended_storage<tct_op_t>();

    static const std::regex regx = get_regex({fragment::column, fragment::equal_re, fragment::dec_re});
    std::smatch cmatches;
    if (!std::regex_match(args[0], cmatches, regx))
        throw error(error::error_code::invalid_asm, args[0]);

    if (cmatches.size() !=3)
        throw error(error::error_code::invalid_asm, args[0]);

    values->word = to_uinteger<uint8_t>(cmatches[1]);
    values->word <<= 8;
    values->word |= to_uinteger<uint8_t>(cmatches[2]);
    values->config = 0;
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_CustomOpHdr);
  }
};


class XAIE_IO_CUSTOM_OP_DDR_PATCH_op : public aie2_isa_op {
public:
  // e.g. XAIE_IO_CUSTOM_OP_DDR_PATCH     @0x1d004, #1, +0x1f8
  explicit XAIE_IO_CUSTOM_OP_DDR_PATCH_op(const std::vector<std::string> &args)
      : aie2_isa_op(XAIE_IO_CUSTOM_OP_DDR_PATCH) {
    operand_count_check(args, 1);
    initialize_OpHdr(sizeof(XAie_CustomOpHdr) + sizeof(patch_op_t));

    auto op = reinterpret_cast<XAie_CustomOpHdr *>(m_op);
    op->Size = get_op_size();
    auto values = get_extended_storage<patch_op_t>();

    const std::string regoff = args[0].substr(1);
    values->regaddr = to_uinteger<uint64_t>(regoff);
    values->argidx = to_uinteger<uint64_t>(args[1].substr(1));
    values->argplus = to_uinteger<uint64_t>(args[2].substr(1));
  }

  [[nodiscard]] size_t get_op_base_size() const override {
    return sizeof(XAie_CustomOpHdr);
  }
};

class XAIE_IO_CUSTOM_OP_RECORD_TIMER_op : public aie2_isa_op {
public:
  // e.g. XAIE_IO_CUSTOM_OP_RECORD_TIMER #14
  explicit XAIE_IO_CUSTOM_OP_RECORD_TIMER_op(const std::vector<std::string> &args)
      : aie2_isa_op(XAIE_IO_CUSTOM_OP_RECORD_TIMER) {
    operand_count_check(args, 1);
    initialize_OpHdr(sizeof(XAie_CustomOpHdr) + sizeof(unsigned int));

    auto op = reinterpret_cast<XAie_CustomOpHdr *>(m_op);
    op->Size = get_op_size();
    auto values = get_extended_storage<unsigned int>();

    values[0] = to_uinteger<uint64_t>(args[0].substr(1));
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
  m_mnemonic_table.emplace("xaie_io_write", std::make_unique<aie2_isa_op_factory<XAIE_IO_WRITE_op>>());
  m_mnemonic_table.emplace("xaie_io_blockwrite", std::make_unique<aie2_isa_op_factory<XAIE_IO_BLOCKWRITE_op>>());
  m_mnemonic_table.emplace("xaie_io_maskwrite", std::make_unique<aie2_isa_op_factory<XAIE_IO_MASKWRITE_op>>());
  m_mnemonic_table.emplace("xaie_io_maskpoll", std::make_unique<aie2_isa_op_factory<XAIE_IO_MASKPOLL_op>>());
  m_mnemonic_table.emplace("xaie_io_maskpoll_busy", std::make_unique<aie2_isa_op_factory<XAIE_IO_MASKPOLL_BUSY_op>>());
  m_mnemonic_table.emplace("xaie_io_noop", std::make_unique<aie2_isa_op_factory<XAIE_IO_NOOP_op>>());
  m_mnemonic_table.emplace("xaie_io_preempt", std::make_unique<aie2_isa_op_factory<XAIE_IO_PREEMPT_op>>());
  m_mnemonic_table.emplace("xaie_io_loadpdi", std::make_unique<aie2_isa_op_factory<XAIE_IO_LOADPDI_op>>());
  m_mnemonic_table.emplace("xaie_io_load_pm_start", std::make_unique<aie2_isa_op_factory<XAIE_IO_LOAD_PM_START_op>>());
  m_mnemonic_table.emplace("xaie_io_custom_op_tct", std::make_unique<aie2_isa_op_factory<XAIE_IO_CUSTOM_OP_TCT_op>>());
  m_mnemonic_table.emplace("xaie_io_custom_op_merge_sync", std::make_unique<aie2_isa_op_factory<XAIE_IO_CUSTOM_OP_MERGE_SYNC_op>>());
  m_mnemonic_table.emplace("xaie_io_custom_op_ddr_patch", std::make_unique<aie2_isa_op_factory<XAIE_IO_CUSTOM_OP_DDR_PATCH_op>>());
  m_mnemonic_table.emplace("xaie_io_custom_op_record_timer", std::make_unique<aie2_isa_op_factory<XAIE_IO_CUSTOM_OP_RECORD_TIMER_op>>());
}

std::unique_ptr<aie2_isa_op> aie2_asm_preprocessor_input::assemble_operation(std::shared_ptr<operation> op)
{
  auto iter  = m_mnemonic_table.find(op->get_name());

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
    // If the previous recorded operation is expecting extension operations continue
    // populating the previous operation.
    if (isa_op_list.size() && isa_op_list.back()->outstanding_ext_op_count()) {
      isa_op_list.back()->process_outstanding_ext_op(line->get_operation());
      continue;
    }
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

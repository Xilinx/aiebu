// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#include <iostream>
#include <ostream>
#include <map>
#include <sstream>
#include <list>

#include "basicpass.h"
#include "passmanager.h"
#include "aie2p_passes.h"
#include "code_section.h"
#include "symbol.h"
#include "aiebu/aiebu_error.h"

#include <xaiengine/xaiegbl.h>
#include <xaiengine/xaie_txn.h>

namespace aiebu {

#ifndef EM_AIECTRLCODE
constexpr ELFIO::Elf_Half EM_AIECTRLCODE = 269; // AMD / Xilinx AIEngine ctrlcode
#endif

#ifdef ELFCLASS32
#undef ELFCLASS32
#endif

#ifdef ELFDATA2LSB
#undef ELFDATA2LSB
#endif

#ifdef SHT_PROGBITS
#undef SHT_PROGBITS
#endif

#ifdef SHF_ALLOC
#undef SHF_ALLOC
#endif

#ifdef PT_LOAD
#undef PT_LOAD
#endif

#ifdef PT_PHDR
#undef PT_PHDR
#endif

inline bool is_ctrldata(const std::string& name)
{
  return !name.compare(".ctrldata");
}

inline bool is_pm_ctrlpkt(const std::string& name)
{
  return !name.substr(0,std::strlen(".ctrlpkt")).compare(".ctrlpkt");
}

inline void elf_debug_dump(const ELFIO::elfio &nbin) {
  for (auto & sec : nbin.sections) {
    std::cout << '[' << sec->get_index() << "] " << sec->get_name() << ": 0x" << std::hex << sec->get_offset() << ": 0x" << std::hex << sec->get_address() << std::dec << "\n";;
  }

  for (auto & seg : nbin.segments) {
    std::cout << '[' << seg->get_index() << "] " << std::hex << seg->get_offset() << std::dec << '(' << seg->get_sections_num() << ")\n";;
  }
}

inline void elf_layout(ELFIO::elfio &ebin) {
  std::ostringstream nullstream;
  if (!ebin.save(nullstream))
    throw error(error::error_code::internal_error, "ELF layout generation failed\n");
}

class passmanager::passmanager_impl {
private:
  // Passed by the client but updated by this transformer
  ELFIO::elfio &m_elf;
  // m_bin is a private copy of a working ELF object used by
  // upgrade_legacy_elf_assign_adddress() which is std::move'd back into
  // client's ELF object m_elf.
  // Please note that m_bin object's life cycle should really have beeen
  // limited to scope of upgrade_legacy_elf_assign_adddress() but it here
  // it has class scope life cycle. This is because of as yet unresolved
  // bug where when m_nbin was going out of scope after std::move in
  // upgrade_legacy_elf_assign_adddress(), [m_elf = std::move(m_bin)]
  // failures where seen inside ELFIO
  // when using m_elf. The bug seems to be in std::move of ELFIO header
  // object -- from m_nbin to m_elf -- which should be debugged separately.
  ELFIO::elfio m_nbin;
  boost::property_tree::ptree m_spec;
  bool m_debug;
  // Preserves the original section index to name mapping
  std::vector<std::string> m_sectable;
  std::list<basic_node<XAie_OpHdr>> m_nodes;

private:

  void itemize(const ELFIO::section *buffer) {
    const char *start = buffer->get_data();
    const char *curr = start;
    auto Hdr = reinterpret_cast<const XAie_TxnHeader *>(curr);
    const auto num_ops = Hdr->NumOps;

    if ((Hdr->Major == AIE2P_OPT_MAJOR_VER) &&
        (Hdr->Minor == AIE2P_OPT_MINOR_VER)) {
      // Only legacy transaction binary format supported today
      return;
    }

    curr += sizeof(*Hdr);
    for (auto i = 0U; i < num_ops; i++) {
      auto op_hdr = reinterpret_cast<const XAie_OpHdr *>(curr);
      size_t size = 0;
      switch (op_hdr->Op) {
      case XAIE_IO_WRITE: {
        auto w_hdr = reinterpret_cast<const XAie_Write32Hdr *>(curr);
        size = w_hdr->Size;
        break;
      }
      case XAIE_IO_BLOCKWRITE: {
        auto bw_header = reinterpret_cast<const XAie_BlockWrite32Hdr *>(curr);
        size = bw_header->Size;
        break;
      }
      case XAIE_IO_MASKWRITE: {
        auto mw_header = reinterpret_cast<const XAie_MaskWrite32Hdr *>(curr);
        size = mw_header->Size;
        break;
      }
      case XAIE_IO_MASKPOLL:
      case XAIE_IO_MASKPOLL_BUSY: {
        auto mp_header = reinterpret_cast<const XAie_MaskPoll32Hdr *>(curr);
        size = mp_header->Size;
        break;
      }
      case XAIE_IO_NOOP: {
        size = sizeof(XAie_NoOpHdr);
        break;
      }
      case XAIE_IO_PREEMPT: {
        size = sizeof(XAie_PreemptHdr);
        break;
      }
      case XAIE_IO_LOADPDI: {
        size = sizeof(XAie_LoadPdiHdr);
        break;
      }
      case XAIE_IO_LOAD_PM_START: {
        size = sizeof(XAie_PmLoadHdr);
        break;
      }
      case (XAIE_IO_CUSTOM_OP_TCT):
      case (XAIE_IO_CUSTOM_OP_DDR_PATCH):
      case (XAIE_IO_CUSTOM_OP_READ_REGS):
      case (XAIE_IO_CUSTOM_OP_RECORD_TIMER):
      case (XAIE_IO_CUSTOM_OP_MERGE_SYNC): {
        auto Hdr = reinterpret_cast<const XAie_CustomOpHdr *>(curr);
        size = Hdr->Size;
        break;
      }
      default:
        throw error(error::error_code::invalid_opcode, std::to_string(op_hdr->Op) + "\n");
        break;
      }
      m_nodes.emplace_back(op_hdr, size, curr - start);
      curr += size;
    }
  }


  void serialize_nodes(ELFIO::section *psec) {
    std::stringstream store;
    const char *ptr = psec->get_data();
    XAie_TxnHeader hdr;
    std::memcpy(&hdr, ptr, sizeof(hdr));
    hdr.NumOps = m_nodes.size();
    store.write(reinterpret_cast<const char *>(&hdr), sizeof(hdr));
    for (auto &node : m_nodes) {
      node.m_transformed_offset = store.tellp();
      store.write(reinterpret_cast<const char *>(node.m_op), static_cast<std::streamsize>(node.m_size));
    }
    store.seekp(0, std::ios_base::end);
    hdr.TxnSize = store.tellp();
    store.seekp(0);
    store.write(reinterpret_cast<const char *>(&hdr), sizeof(hdr));
    psec->free_data();
    psec->set_data(store.str());
  }


  void run_transforms(ELFIO::section *psec)
  {
    boost::property_tree::ptree items = m_spec.get_child("allpasses");
    const std::string name = psec->get_name();
    auto it = std::find_if(items.begin(), items.end(), [name](auto n) {
      return n.second.template get_optional<std::string>("section") == name;
    });
    if (it == items.end())
      return;

    itemize(psec);
    if (!m_nodes.size())
      return;

    const boost::property_tree::ptree passes = it->second.get_child("passes");
    // Now run the passes as defined by user in the JSON specification
    for (const auto &pass : passes) {
      auto passname = pass.second.get_optional<std::string>("pass");
      if (passname.get() == "null") {
        XAie_OpHdr_null nullpass(m_nodes);
        nullpass.transform();
      }
      else if (passname.get() == "loadpdi") {
        auto pdiid = pass.second.get_optional<int>("pdiid");
        auto pdisize = pass.second.get_optional<int>("pdisize");
        XAie_OpHdr_add_loadpdi loadpdi(m_nodes, pdiid.get(), pdisize.get());
        loadpdi.transform();
      }
      else if (passname.get() == "nopreempt") {
        XAie_OpHdr_drop_preempt nopreempt(m_nodes);
        nopreempt.transform();
      }
      else if (passname.get() == "print") {
        XAie_OpHdr_print printer(m_nodes, std::cout);
        printer.transform();
      } else {
        throw error(error::error_code::internal_error, "Pass " + passname.get() + " not supported\n");
      }
    }

    serialize_nodes(psec);
  }


  std::list<basic_node<XAie_OpHdr>>::const_iterator find_node(size_t offset) const {
    auto it = std::find_if(m_nodes.begin(), m_nodes.end(), [offset](auto &n) {
      return (n.m_original_offset <= offset) && (offset < (n.m_original_offset + n.m_size));
    });
    return it;
  }


  void adjust_relocations()
  {
    auto dsec = m_elf.sections[".dynsym"];
    if (!dsec)
      return;

    auto tsec= m_elf.sections[".ctrltext"];
    auto tsec_index = tsec->get_index();

    // Stores symbol to its index
    std::map<std::string, ELFIO::Elf_Xword> ctrltext_symbols;
    // Go through all symbols and identify the ones which are used in the ctrltext
    ELFIO::symbol_section_accessor symb(m_elf, dsec);
    const auto sym_num = symb.get_symbols_num();
    for (ELFIO::Elf_Xword i = 0; i < sym_num; ++i) {
      std::string name;
      ELFIO::Elf64_Addr value = 0;
      ELFIO::Elf_Xword size = 0;
      unsigned char bind = 0;
      unsigned char type = 0;
      ELFIO::Elf_Half section = 0;
      unsigned char other = 0;
      symb.get_symbol(i, name, value, size, bind, type, section, other);
      if (section != tsec_index)
        continue;
      // may require updating the address where this symbol is used in the ctrltext
      auto it = ctrltext_symbols.find(name);
      if (it != ctrltext_symbols.end())
        continue;
      ctrltext_symbols.insert(it, {name, i});
    }

    auto rsec = m_elf.sections[".rela.dyn"];
    if (!rsec)
      return;
    ELFIO::relocation_section_accessor reloc(m_elf, rsec);

    // Go through all relocations with symbols pointing to ctrltext and
    // adjust the address where these symbols will be used
    // TODO: This algorithm is O(N^2).
    // Need to rework find_node() implementation and its usage in this loop to
    // improve performance
    const auto reloc_num = reloc.get_entries_num();
    for (ELFIO::Elf_Xword i = 0; i < reloc_num; ++i) {
      ELFIO::Elf64_Addr offset = 0;
      ELFIO::Elf64_Addr symbolValue = 0;
      std::string symbolName;
      unsigned type = 0;
      ELFIO::Elf_Sxword addend = 0;
      ELFIO::Elf_Sxword calcValue = 0;
      reloc.get_entry(i, offset, symbolValue, symbolName, type, addend,
                      calcValue);
      auto it = ctrltext_symbols.find(symbolName);
      if (it == ctrltext_symbols.end())
        continue;

      // Find the node this symbol was patched in
      const auto nodei = find_node(offset);
      if ((nodei == m_nodes.end()) || (nodei->m_state != basic_node_state::original)) {
        // We should drop/invalidate this relocation as it is either orphaned or a newly added node
        reloc.set_entry(i, offset, it->second,
                        (char)symbol::patch_schema::unknown, addend);
      }
      else {
        auto new_address = nodei->m_transformed_offset + (offset - nodei->m_original_offset);
        reloc.set_entry(i, new_address, it->second, type, addend);
      }
    }
  }


  bool compare_section_layout() const
  {
    auto it = m_sectable.begin();
    for (auto &sec : m_elf.sections) {
      if (it == m_sectable.end())
        return false;
      if (*it != sec->get_name())
        return false;
      it++;
    }
    return true;
  }


  unsigned int find_section_original_index(const std::string &secname) const
  {
    unsigned i = ~0;
    for (auto name : m_sectable) {
      if (name == secname)
        return i;
    }
    return i;
  }


  bool is_legacy_elf_with_unset_address() const
  {
    for (auto &seg : m_elf.segments) {
      if (seg->get_virtual_address() != 0)
        return false;
      if (seg->get_physical_address() != 0)
        return false;
    }
    return true;
  }


  void upgrade_legacy_elf_assign_adddress()
  {
    // Upgrading address in exisiting ELF does not work. We need to create a new
    // ELF with contents from exisiting ELF and then
    // [1] update the flags
    // [2] drop segment entries for sections that are not used for execution
    // [3] update the address
    //
    m_nbin.create(ELFIO::ELFCLASS32, ELFIO::ELFDATA2LSB);
    m_nbin.set_os_abi(m_elf.get_os_abi());
    m_nbin.set_abi_version(m_elf.get_abi_version());
    m_nbin.set_type( m_elf.get_type() );
    m_nbin.set_machine( EM_AIECTRLCODE);
    m_nbin.set_flags(m_elf.get_flags());

    for (auto &sec : m_elf.sections) {
      // The following two sections are automatically added by ELFIO
      if (sec->get_name() == "")
        continue;
      if (sec->get_name() == ".shstrtab")
        continue;
      m_nbin.sections.add(sec->get_name());
    }

    // Temporary table of section to its original offset
    std::vector<std::pair<int, size_t>> offsets;
    // Populate the new ELF object with relevant sections and segments from
    // existing ELF object
    for (auto &sec : m_elf.sections) {
      auto offset = sec->get_offset();
      const std::string name = sec->get_name();
      auto it = std::find_if(m_nbin.sections.begin(), m_nbin.sections.end(), [&name](auto &n) {
        return n->get_name() == name;
      });

      // Move the section as is from the old ELF into the new ELF
      *it = std::move(sec);
      if ((*it)->get_type() != ELFIO::SHT_PROGBITS) {
        // Remove the ALLOC flags from sections except text and data
        auto flags = (*it)->get_flags();
        flags &= ~ELFIO::SHF_ALLOC;
        (*it)->set_flags(flags);
      }
      offsets.emplace_back((*it)->get_index(), offset);
    }

    // Create new segments using attributes from the old segments
    for (auto &seg : m_elf.segments) {
      auto type = seg->get_type();
      if ((type != ELFIO::PT_LOAD) && (type != ELFIO::PT_PHDR))
        continue;
      auto nseg = m_nbin.segments.add();
      size_t offset = seg->get_offset();
      nseg->set_type(seg->get_type());
      nseg->set_flags(type);
      nseg->set_align(seg->get_align());

      // Find which section was the old segment pointing to before using offset as the key
      auto it = std::find_if(offsets.begin(), offsets.end(), [offset](std::pair<int, size_t> n) {
        return n.second == offset;
      });
      if (it == offsets.end())
        continue;
      // Bind the new segment to its new section
      nseg->add_section_index(it->first, seg->get_align());
    }

    elf_layout(m_nbin);
    adjust_addresses(m_nbin);
    elf_layout(m_nbin);

    if (m_debug)
      elf_debug_dump(m_nbin);

    // Now move over the newly created ELF back to the original ELF
    m_elf = std::move(m_nbin);

    elf_layout(m_elf);

    if (m_debug)
      elf_debug_dump(m_elf);
  }


  void adjust_addresses(ELFIO::elfio &nelf) {
    // Update the address of the each section to match its offset
    for (auto &sec : nelf.sections) {
      sec->set_address(sec->get_offset());
    }

    for (auto &seg : nelf.segments) {
      if (!seg->get_sections_num())
        continue;
      if (seg->get_sections_num() != 1)
        throw error(error::error_code::invalid_elf,
                    "Segment " + std:: to_string(seg->get_index()) + " with multiple sections encountered\n");

      // Update the address of the each segment which has a section
      auto offset = nelf.sections[seg->get_section_index_at(0)]->get_offset();
      seg->set_virtual_address(offset);
      seg->set_physical_address(offset);
    }
  }


  void pretransform()
  {
    if (!is_legacy_elf_with_unset_address())
      return;

    upgrade_legacy_elf_assign_adddress();
    elf_layout(m_elf);
  }

public:
  passmanager_impl(ELFIO::elfio &elf, const boost::property_tree::ptree &spec,
                       bool debug = false) : m_elf(elf), m_spec(spec), m_debug(debug) {}

  ~passmanager_impl() = default;

  passmanager_impl(const passmanager_impl &) = delete;
  passmanager_impl(passmanager_impl &&) = delete;
  passmanager_impl &operator=(const passmanager_impl &) = delete;
  passmanager_impl& operator=(passmanager_impl &&) = delete;

  void run_transforms() {
    for (auto &sec : m_elf.sections) {
      m_sectable.push_back(sec->get_name());
    }

    pretransform();

    for (auto &sec : m_elf.sections) {
      if (sec->get_type() != ELFIO::SHT_PROGBITS)
        continue;
      if (is_pm_ctrlpkt(sec->get_name()) || is_ctrldata(sec->get_name()))
        continue;
      run_transforms(sec.get());
    }

    elf_layout(m_elf);
    adjust_addresses(m_elf);
    if (!compare_section_layout())
      throw error(error::error_code::internal_error, "Order of sections changed after transform\n");
    adjust_relocations();
  }
};

passmanager::passmanager(ELFIO::elfio &elf, const boost::property_tree::ptree &spec,
                         bool debug) : m_impl(std::make_unique<passmanager_impl>(elf, spec, debug)) {}

passmanager::~passmanager() = default;

void passmanager::run_transforms() {
  m_impl->run_transforms();
}

}

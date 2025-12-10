// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#include "elfwriter.h"
#include "logger.h"

namespace aiebu {

ELFIO::section*
elf_writer::
add_section(const elf_section& data)
{
  // add section
  ELFIO::section* sec = m_elfio.sections.add(data.get_name());
  sec->set_type(data.get_type());
  sec->set_flags(data.get_flags());
  sec->set_addr_align(data.get_align());
  const std::vector<uint8_t> buf = data.get_buffer();

  if(buf.size())
    sec->set_data(reinterpret_cast<const char*>(buf.data()), static_cast<ELFIO::Elf_Word>(buf.size()));
  //sec->set_info( data.get_info() );
  if (!data.get_link().empty())
  {
    const ELFIO::section* lsec = m_elfio.sections[data.get_link()];
    sec->set_link(lsec->get_index());
  }
  // set section address
  sec->set_address(data.get_addr());
  return sec;
}

ELFIO::segment*
elf_writer::
add_segment(const elf_segment& data)
{
  // add segment
  ELFIO::segment* seg = m_elfio.segments.add();
  seg->set_type(data.get_type());
  seg->set_virtual_address(data.get_vaddr());
  seg->set_physical_address(data.get_paddr());
  seg->set_flags(data.get_flags());
  seg->set_align(data.get_align());
  if (!data.get_link().empty())
  {
    const ELFIO::section* sec = m_elfio.sections[data.get_link()];
    seg->add_section_index(sec->get_index(),
                           sec->get_addr_align());
  }
  return seg;
}

ELFIO::string_section_accessor
elf_writer::
add_dynstr_section()
{
  ELFIO::string_section_accessor stra( dstr_sec );
  return stra;
}

void
elf_writer::
add_dynsym_section(ELFIO::string_section_accessor* stra, std::vector<symbol>& syms, const std::string&index_string)
{
  // Create symbol table writer
  ELFIO::symbol_section_accessor syma( m_elfio, dsym_sec );
  std::map<std::string, ELFIO::Elf_Word> hash;
  for (auto & sym : syms) {
    std::string key = sym.get_section_name() + index_string + "_" + sym.get_name() + "_" +
                      std::to_string(sym.get_size());
    auto it = hash.find(key);
    if (it == hash.end())
    {
      const ELFIO::section* sec = m_elfio.sections[sym.get_section_name()+ index_string];
      auto index = syma.add_symbol(*stra, sym.get_name().c_str(), 0,
                                   sym.get_size(), ELFIO::STB_GLOBAL, ELFIO::STT_OBJECT,
                                   0, sec->get_index());
      hash[key] = index;
    }
    sym.set_index(hash[key]);
  }

}

void
elf_writer::
add_reldyn_section(std::vector<symbol>& syms)
{
  // Create relocation table writer
  ELFIO::relocation_section_accessor rela( m_elfio, rel_sec );
  for (auto & sym : syms) {
      rela.add_entry(sym.get_pos(), sym.get_index(), (char)sym.get_schema(), (ELFIO::Elf_Sxword)sym.get_addend());
  }
}

void
elf_writer::
add_dynamic_section()
{
  ELFIO::dynamic_section_accessor dyn(m_elfio, dynamic_sec);
  dyn.add_entry(ELFIO::DT_RELA, rel_sec->get_index());
  dyn.add_entry(ELFIO::DT_RELASZ, rel_sec->get_size());
}

void
elf_writer::
add_note(ELFIO::Elf_Word type, const std::string& name, const std::vector<char>& dec)
{
  ELFIO::section* note_sec = m_elfio.sections.add( name.c_str() );
  note_sec->set_type( ELFIO::SHT_NOTE );
  note_sec->set_addr_align( 1 );

  ELFIO::note_section_accessor note_writer( m_elfio, note_sec );
  note_writer.add_note( type, "XRT", dec.data(), dec.size() );
}

std::vector<char>
elf_writer::
finalize()
{
  add_note(NT_XRT_UID, ".note.xrt.UID", m_uid.calculate());
  log_info() << "UID:" << m_uid.str();
  std::stringstream stream;
  stream << std::noskipws;
  //m_elfio.save( "hello_32" );
  m_elfio.save( stream );
  std::vector<char> v;
  std::copy(std::istream_iterator<char>(stream),
            std::istream_iterator<char>( ),
            std::back_inserter(v));
  return v;
}

std::vector<uint32_t>
elf_writer::
add_text_data_section(const std::vector<std::shared_ptr<writer>>& mwriter, std::vector<symbol>& syms, const std::string& index_string)
{
  std::vector<uint32_t> section_index_list;
  for(auto element : mwriter)
  {
    auto buffer = std::dynamic_pointer_cast<section_writer>(element);
    if (buffer->get_data().size() == 0)
      continue;

    m_uid.update(buffer->get_data());
    elf_section sec_data;
    sec_data.set_name(buffer->get_name()+index_string);
    sec_data.set_type(ELFIO::SHT_PROGBITS);
    if (buffer->get_type() == code_section::text)
      sec_data.set_flags(ELFIO::SHF_ALLOC | ELFIO::SHF_EXECINSTR);
    else
      sec_data.set_flags(ELFIO::SHF_ALLOC | ELFIO::SHF_WRITE);
    sec_data.set_align(align);
    sec_data.set_buffer(buffer->get_data());
    sec_data.set_link("");
    // set section address equal to segment virtual address as segment:section has 1:1 mapping
    cur_addr = get_virtual_addr(prev_virtual_addr, prev_seg_size);
    sec_data.set_addr(cur_addr);

    elf_segment seg_data;
    seg_data.set_type(ELFIO::PT_LOAD);
    if (buffer->get_type() == code_section::text)
      seg_data.set_flags(ELFIO::PF_X | ELFIO::PF_R);
    else
      seg_data.set_flags(ELFIO::PF_W | ELFIO::PF_R);
    // set segment virtual address
    seg_data.set_vaddr(cur_addr);
    seg_data.set_paddr(0x0);
    seg_data.set_link(buffer->get_name()+index_string);
    seg_data.set_align(text_align);

    section_index_list.push_back(add_section(sec_data)->get_index());
    add_segment(seg_data);
    if (buffer->hassymbols())
    {
      auto lsyms = buffer->get_symbols();
      syms.insert(syms.end(), lsyms.begin(), lsyms.end());
    }

    // Update for next iteration
    prev_virtual_addr = cur_addr;
    prev_seg_size = buffer->get_data().size();
  }
  return section_index_list;
}

void
elf_writer::
init_symtab()
{
  str_sec = m_elfio.sections.add(".strtab");
  str_sec->set_type(ELFIO::SHT_STRTAB);
  str_sec->set_entry_size(0);
  sym_sec = m_elfio.sections.add(".symtab");
  sym_sec->set_type(ELFIO::SHT_SYMTAB);
  sym_sec->set_info(1);
  sym_sec->set_addr_align(0x4);
  sym_sec->set_entry_size(m_elfio.get_default_entry_size(ELFIO::SHT_SYMTAB));
  sym_sec->set_link(str_sec->get_index());
}

ELFIO::Elf_Word
elf_writer::
add_symtab(const std::string& name)
{
  ELFIO::string_section_accessor stra(str_sec);
  // Create symbol table writer
  ELFIO::symbol_section_accessor syma( m_elfio, sym_sec );
  // Another way to add symbol
  return syma.add_symbol( stra, name.c_str(), 0x00000000, 0, ELFIO::STB_WEAK, ELFIO::STT_FUNC, 0,
                   ELFIO::SHN_UNDEF );
}

ELFIO::Elf_Word
elf_writer::
add_symtab_section(const std::string& name, ELFIO::Elf_Word index)
{
  ELFIO::string_section_accessor stra(str_sec);
  // Create symbol table writer
  ELFIO::symbol_section_accessor syma( m_elfio, sym_sec );
  // Another way to add symbol
  return syma.add_symbol( stra, name.c_str(), 0x00000000, 0, ELFIO::STB_WEAK, ELFIO::STT_OBJECT, 0,
                   index );
}

void
elf_writer::
init_dynamic_sections()
{
  std::call_once(dynamic_flag, [this] {
    dstr_sec = m_elfio.sections.add( ".dynstr" );
    dstr_sec->set_type( ELFIO::SHT_STRTAB );
    dstr_sec->set_entry_size( 0 );
    ELFIO::string_section_accessor stra( dstr_sec );

    dsym_sec = m_elfio.sections.add(".dynsym");
    dsym_sec->set_type( ELFIO::SHT_DYNSYM );
    dsym_sec->set_addr_align( phdr_align );
    dsym_sec->set_entry_size(m_elfio.get_default_entry_size(ELFIO::SHT_SYMTAB));
    dsym_sec->set_link( dstr_sec->get_index() );
    dsym_sec->set_info( 1 );


    rel_sec = m_elfio.sections.add( ".rela.dyn" );
    rel_sec->set_type( ELFIO::SHT_RELA );
    rel_sec->set_addr_align(phdr_align);
    rel_sec->set_entry_size(m_elfio.get_default_entry_size(ELFIO::SHT_RELA));
    rel_sec->set_link( dsym_sec->get_index() );

    dynamic_sec = m_elfio.sections.add( ".dynamic" );
    dynamic_sec->set_type( ELFIO::SHT_DYNAMIC );
    dynamic_sec->set_addr_align( phdr_align );
    dynamic_sec->set_link( dstr_sec->get_index() );
    dynamic_sec->set_entry_size(m_elfio.get_default_entry_size(ELFIO::SHT_DYNAMIC));
    dynamic_sec->set_info( 0 );
  });
}

std::vector<uint32_t>
elf_writer::
process_common_helper(const std::vector<std::shared_ptr<writer>>& mwriter, const std::string& index_string)
{
  // add sections
  std::vector<symbol> syms;
  auto section_index_list = add_text_data_section(mwriter, syms, index_string);
  if (syms.size())
  {
    init_dynamic_sections();
    ELFIO::string_section_accessor str = add_dynstr_section();
    add_dynsym_section(&str, syms, index_string);
    add_reldyn_section(syms);
  }
  return section_index_list;
}

std::vector<char>
elf_writer::
process(std::vector<std::shared_ptr<writer>>& mwriter)
{
  process_common_helper(mwriter, "");
  if (dstr_sec)
    add_dynamic_section();
  return finalize();
}

void
elf_writer::
add_group(const std::string& name, const std::vector<uint32_t>& member, ELFIO::Elf_Word info_index)
{
  // add section
  ELFIO::section* sec = m_elfio.sections.add(name);
  sec->set_type(ELFIO::SHT_GROUP);
  sec->set_addr_align(align);
  sec->set_info(info_index);
  sec->set_entry_size(4);

  if(member.size())
    sec->set_data(reinterpret_cast<const char*>(member.data()), static_cast<ELFIO::Elf_Word>(member.size()*4));

  const ELFIO::section* lsec = m_elfio.sections[".symtab"];
  sec->set_link(lsec->get_index());
}

// align address to 16 bytes.
uint64_t
elf_writer::align_address(uint64_t address)
{
  return (address + 15) & ~15ULL;
}

// Calcualte the next section address based on previous section address and size.
uint64_t
elf_writer::get_virtual_addr(uint64_t prev_virtual_addr, uint64_t prev_seg_size)
{
  return align_address(prev_virtual_addr + prev_seg_size);
}

}

// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "elfwriter.h"

namespace aiebu {

ELFIO::section*
elf_writer::
add_section(elf_section data)
{
  // add section
  ELFIO::section* sec = m_elfio.sections.add(data.get_name());
  sec->set_type(data.get_type());
  sec->set_flags(data.get_flags());
  sec->set_addr_align(data.get_align());
  std::vector<uint8_t> buf = data.get_buffer();

  if(buf.size())
    sec->set_data(reinterpret_cast<char*>(buf.data()), static_cast<ELFIO::Elf_Word>(buf.size()));
  //sec->set_info( data.get_info() );
  if (!data.get_link().empty())
  {
    const ELFIO::section* lsec = m_elfio.sections[data.get_link()];
    sec->set_link(lsec->get_index());
  }
  return sec;
}

ELFIO::segment*
elf_writer::
add_segment(elf_segment data)
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
  // add .dynstr section
  ELFIO::section* dstr_sec = m_elfio.sections.add( ".dynstr" );
  dstr_sec->set_type( ELFIO::SHT_STRTAB );
  dstr_sec->set_entry_size( 0 );
  ELFIO::string_section_accessor stra( dstr_sec );
  return stra;
}

void
elf_writer::
add_dynsym_section(ELFIO::string_section_accessor* stra, std::vector<symbol>& syms)
{
  // add .dynsym section
  ELFIO::section* dsym_sec = m_elfio.sections.add(".dynsym");
  dsym_sec->set_type( ELFIO::SHT_DYNSYM );
  dsym_sec->set_flags(ELFIO::SHF_ALLOC);
  dsym_sec->set_addr_align( phdr_align );
  dsym_sec->set_entry_size(m_elfio.get_default_entry_size(ELFIO::SHT_SYMTAB));
  ELFIO::section* dstr_sec = m_elfio.sections[".dynstr"];
  dsym_sec->set_link( dstr_sec->get_index() );
  dsym_sec->set_info( 1 );

  // Create symbol table writer
  ELFIO::symbol_section_accessor syma( m_elfio, dsym_sec );
  std::map<std::string, ELFIO::Elf_Word> hash;
  for (auto & sym : syms) {
    std::string key = sym.get_section_name() + "_" + sym.get_name() + "_" +
                      std::to_string(sym.get_size());
    auto it = hash.find(key);
    if (it == hash.end())
    {
      const ELFIO::section* sec = m_elfio.sections[sym.get_section_name()];
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
  // Create relocation table section
  ELFIO::section* rel_sec = m_elfio.sections.add( ".rela.dyn" );
  rel_sec->set_type( ELFIO::SHT_RELA );
  rel_sec->set_flags(ELFIO::SHF_ALLOC);
  //section* data_sec = m_elfio.sections[".data"];
  //rel_sec->set_info( data_sec->get_index());
  rel_sec->set_addr_align(phdr_align);
  rel_sec->set_entry_size(m_elfio.get_default_entry_size(ELFIO::SHT_RELA));
  ELFIO::section* dsym_sec = m_elfio.sections[".dynsym"];
  rel_sec->set_link( dsym_sec->get_index() );

  // Create relocation table writer
  ELFIO::relocation_section_accessor rela( m_elfio, rel_sec );
  for (auto & sym : syms) {
      rela.add_entry(sym.get_pos(), sym.get_index(), (unsigned char)ELFIO::R_X86_64_32,
                     (((ELFIO::Elf_Sxword)sym.get_schema() & SCHEMA_MASK) | ((sym.get_addend() << ADDEND_SHIFT) & ADDEND_MASK)));
  }
}

void
elf_writer::
add_dynamic_section_segment()
{
  // add dynamic section
  elf_section sec_data;
  sec_data.set_name(".dynamic");
  sec_data.set_type(ELFIO::SHT_DYNAMIC);
  sec_data.set_flags(ELFIO::SHF_ALLOC);
  sec_data.set_align(phdr_align);
  //sec_data.set_buffer();
  sec_data.set_link(".dynstr");

  ELFIO::section *dyn_sec = add_section(sec_data);
  dyn_sec->set_entry_size(m_elfio.get_default_entry_size(ELFIO::SHT_DYNAMIC));
  dyn_sec->set_info( 0 );

  ELFIO::dynamic_section_accessor dyn(m_elfio, dyn_sec);
  ELFIO::section* rel_sec = m_elfio.sections[".rela.dyn"];
  dyn.add_entry(ELFIO::DT_RELA, rel_sec->get_index());
  dyn.add_entry(ELFIO::DT_RELASZ, rel_sec->get_size());


  elf_segment seg_data;
  seg_data.set_type(ELFIO::PT_DYNAMIC);
  seg_data.set_flags(ELFIO::PF_W | ELFIO::PF_R);
  seg_data.set_vaddr(0x0);
  seg_data.set_paddr(0x0);
  seg_data.set_link(".dynamic");
  seg_data.set_align(phdr_align);
  add_segment(seg_data);
}

void
elf_writer::
add_note(ELFIO::Elf_Word type, std::string name, std::string dec)
{
  ELFIO::section* note_sec = m_elfio.sections.add( name.c_str() );
  note_sec->set_type( ELFIO::SHT_NOTE );
  note_sec->set_addr_align( 1 );

  ELFIO::note_section_accessor note_writer( m_elfio, note_sec );
  note_writer.add_note( type, "XRT", dec.c_str(), dec.size() );
}

std::vector<char>
elf_writer::
finalize()
{
  std::cout << "UID:" << m_uid.calculate() << "\n";
  add_note(NT_XRT_UID, ".note.xrt.UID", m_uid.calculate());
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

void
elf_writer::
add_text_data_section(std::vector<writer>& mwriter, std::vector<symbol>& syms)
{
  for(auto buffer : mwriter)
  {
    if(buffer.get_data().size())
    {
      m_uid.update(buffer.get_data());
      elf_section sec_data;
      sec_data.set_name(buffer.get_name());
      sec_data.set_type(ELFIO::SHT_PROGBITS);
      if (buffer.get_type() == code_section::text)
        sec_data.set_flags(ELFIO::SHF_ALLOC | ELFIO::SHF_EXECINSTR);
      else
        sec_data.set_flags(ELFIO::SHF_ALLOC | ELFIO::SHF_WRITE);
      sec_data.set_align(align);
      sec_data.set_buffer(buffer.get_data());
      sec_data.set_link("");

      elf_segment seg_data;
      seg_data.set_type(ELFIO::PT_LOAD);
      if (buffer.get_type() == code_section::text)
        seg_data.set_flags(ELFIO::PF_X | ELFIO::PF_R);
      else
        seg_data.set_flags(ELFIO::PF_W | ELFIO::PF_R);
      seg_data.set_vaddr(0x0);
      seg_data.set_paddr(0x0);
      seg_data.set_link(buffer.get_name());
      seg_data.set_align(text_align);

      add_section(sec_data);
      add_segment(seg_data);
      if (buffer.hassymbols())
      {
        auto lsyms = buffer.get_symbols();
        syms.insert(syms.end(), lsyms.begin(), lsyms.end());
      }
    }
  }
}

std::vector<char>
elf_writer::
process(std::vector<writer> mwriter)
{
  // add sections
  std::vector<symbol> syms;
  add_text_data_section(mwriter, syms);
  if (syms.size())
  {
    ELFIO::string_section_accessor str = add_dynstr_section();
    add_dynsym_section(&str, syms);
    add_reldyn_section(syms);
    add_dynamic_section_segment();
  }
  return finalize();
}

}

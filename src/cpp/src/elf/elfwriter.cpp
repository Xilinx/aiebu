// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "elfwriter.h"

namespace aiebu {

section*
elf_writer::
add_section(elf_section data)
{
  // add section
  section* sec = m_elfio.sections.add(data.get_name());
  sec->set_type(data.get_type());
  sec->set_flags(data.get_flags());
  sec->set_addr_align(data.get_align());
  std::vector<uint8_t> buf = data.get_buffer();
  
  if(buf.size())
    sec->set_data((char*)buf.data(), buf.size());
  //sec->set_info( data.get_info() );
  if (!data.get_link().empty())
  {
    section* lsec = m_elfio.sections[data.get_link()];
    sec->set_link(lsec->get_index());
  }
  return sec;
}

segment*
elf_writer::
add_segment(elf_segment data)
{
  // add segment
  segment* seg = m_elfio.segments.add();
  seg->set_type(data.get_type());
  seg->set_virtual_address(data.get_vaddr());
  seg->set_physical_address(data.get_paddr());
  seg->set_flags(data.get_flags());
  seg->set_align(data.get_align());
  if (!data.get_link().empty())
  {
    section* sec = m_elfio.sections[data.get_link()];
    seg->add_section_index(sec->get_index(),
                           sec->get_addr_align());
  }
  return seg;
}

string_section_accessor
elf_writer::
add_dynstr_section()
{
  // add .dynstr section
  section* dstr_sec = m_elfio.sections.add( ".dynstr" );
  dstr_sec->set_type( SHT_STRTAB );
  string_section_accessor stra( dstr_sec );
  return stra;
}

void
elf_writer::
add_dynsym_section(string_section_accessor* stra, std::shared_ptr<writer> mwriter)
{
  // add .dynsym section
  section* dsym_sec = m_elfio.sections.add(".dynsym");
  dsym_sec->set_type( SHT_DYNSYM );
  dsym_sec->set_flags(SHF_ALLOC);
  dsym_sec->set_addr_align( 0x8 );
  dsym_sec->set_entry_size( m_elfio.get_default_entry_size(SHT_DYNSYM));
  section* dstr_sec = m_elfio.sections[".dynstr"];
  dsym_sec->set_link( dstr_sec->get_index() );
  dsym_sec->set_info( 1 );


  // Create symbol table writer
  symbol_section_accessor syma( m_elfio, dsym_sec );
  auto &symbs = mwriter->get_symbols();
  for (auto & sym : symbs) {
    section* text_sec = m_elfio.sections[get_textname(sym.get_colnum(), sym.get_pagenum())];
    section* data_sec = m_elfio.sections[get_dataname(sym.get_colnum(), sym.get_pagenum())];
    sym.set_index(syma.add_symbol(*stra, sym.get_name().c_str(), 0, 0, STB_GLOBAL, STT_OBJECT, 0,
                                  sym.get_section_index() == 0 ? text_sec->get_index(): data_sec->get_index()));
  }

}

void
elf_writer::
add_reldyn_section(std::shared_ptr<writer> mwriter)
{
  // Create relocation table section
  section* rel_sec = m_elfio.sections.add( ".rel.dyn" );
  rel_sec->set_type( SHT_RELA );
  rel_sec->set_flags(SHF_ALLOC);
  //section* data_sec = m_elfio.sections[".data"];
  //rel_sec->set_info( data_sec->get_index());
  rel_sec->set_addr_align(phdr_align);
  rel_sec->set_entry_size(m_elfio.get_default_entry_size(SHT_RELA));
  section* dsym_sec = m_elfio.sections[".dynsym"];
  rel_sec->set_link( dsym_sec->get_index() );

  // Create relocation table writer
  relocation_section_accessor rela( m_elfio, rel_sec );
  auto &symbs = mwriter->get_symbols();
  for (auto & sym : symbs) {
      rela.add_entry(sym.get_pos(), sym.get_index(), (unsigned char)R_X86_64_32,
                     (Elf_Sxword)sym.get_schema());
  }
}

void
elf_writer::
add_dynamic_section_segment()
{
  // add dynamic section
  elf_section sec_data;
  sec_data.set_name(".dynamic");
  sec_data.set_type(SHT_DYNAMIC);
  sec_data.set_flags(SHF_ALLOC);
  sec_data.set_align(phdr_align);
  //sec_data.set_buffer();
  sec_data.set_link(".dynstr");

  section *dyn_sec = add_section(sec_data);
  dyn_sec->set_entry_size(m_elfio.get_default_entry_size(SHT_DYNAMIC));
  dyn_sec->set_info( 0 );

  dynamic_section_accessor dyn(m_elfio, dyn_sec);
  section* rel_sec = m_elfio.sections[".rel.dyn"];
  dyn.add_entry(DT_RELA, rel_sec->get_index());
  dyn.add_entry(DT_RELASZ, rel_sec->get_size());


  elf_segment seg_data;
  seg_data.set_type(PT_DYNAMIC);
  seg_data.set_flags(PF_W | PF_R);
  seg_data.set_vaddr(0x0);
  seg_data.set_paddr(0x0);
  seg_data.set_link(".dynamic");
  seg_data.set_align(phdr_align);
  add_segment(seg_data);
}

std::vector<char>
elf_writer::
finalize()
{
  std::stringstream stream;
  stream << std::noskipws;
  //m_elfio.save( "hello_32" );
  m_elfio.save( stream );
  std::vector<char> v; 
  std::copy(std::istream_iterator<char>(stream), 
            std::istream_iterator<char>( ),
            std::back_inserter(v));
  return std::move(v);
}

void
elf_writer::
add_text_data_section(std::shared_ptr<writer> mwriter)
{
  // add test and data section
  auto &buffermap = mwriter->get_buffermap();
  for(auto entry : buffermap)
  {
    uint32_t pagenum = 0;
    uint32_t col = entry.first;
    for(auto buffer : entry.second)
    {
      std::vector<uint8_t> &textbuffer = buffer.first;
      std::vector<uint8_t> &databuffer = buffer.second;
      if(textbuffer.size())
      {
        std::string textname = get_textname(entry.first, pagenum);
        elf_section sec_data;
        sec_data.set_name(textname);
        sec_data.set_type(SHT_PROGBITS);
        sec_data.set_flags(SHF_ALLOC | SHF_EXECINSTR);
        sec_data.set_align(text_align);
        sec_data.set_buffer(textbuffer);
        sec_data.set_link("");

        elf_segment seg_data;
        seg_data.set_type(PT_LOAD);
        seg_data.set_flags(PF_X | PF_R);
        seg_data.set_vaddr(0x0);
        seg_data.set_paddr(0x0);
        seg_data.set_link(textname);
        seg_data.set_align(text_align);

        add_section(sec_data);
        add_segment(seg_data);
      }

      if(databuffer.size())
      {
        std::string dataname = get_dataname(entry.first, pagenum);
        elf_section sec_data;
        sec_data.set_name(dataname);
        sec_data.set_type(SHT_PROGBITS);
        sec_data.set_flags(SHF_ALLOC | SHF_WRITE);
        sec_data.set_align(data_align);
        sec_data.set_buffer(databuffer);
        sec_data.set_link("");

        elf_segment seg_data;
        seg_data.set_type(PT_LOAD);
        seg_data.set_flags(PF_W | PF_R);
        seg_data.set_vaddr(0x0);
        seg_data.set_paddr(0x0);
        seg_data.set_link(dataname);
        seg_data.set_align(data_align);

        add_section(sec_data);
        add_segment(seg_data);
      }
      ++pagenum;
    }
  }
}

std::vector<char>
elf_writer::
process(std::shared_ptr<writer> mwriter)
{
  // add sections
  add_text_data_section(mwriter);
  if (mwriter->hassymbols())
  {
    string_section_accessor str = add_dynstr_section();
    add_dynsym_section(&str, mwriter);
    add_reldyn_section(mwriter);
    add_dynamic_section_segment();
  }
  return std::move(finalize());
}

}

// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_BASICPASS_H_
#define AIEBU_BASICPASS_H_

#include <string>
#include <list>
#include <elfio/elfio.hpp>
#include <elfio/elfio_section.hpp>

#include "aiebu/aiebu_assembler.h"

namespace aiebu {

inline bool is_ctrldata(const std::string& name)
{
  return !name.compare(".ctrldata");
}

inline bool is_pm_ctrlpkt(const std::string& name)
{
  return !name.substr(0,8).compare(".ctrlpkt");
}

enum basic_node_state {
  original,
  dropped,
  fresh
};

template <typename aie2p_type> struct basic_node {
  const aie2p_type *m_op;
  size_t m_size;
  basic_node_state m_state;
  basic_node(const aie2p_type *op, size_t size,
             basic_node_state state = original)
      : m_op(op), m_size(size), m_state(state) {}
  ~basic_node() {
    if (m_state == fresh)
      delete m_op;
  }
};


template <typename aie2p_type> class aie2p_basicpass {
protected:
  std::list<basic_node<aie2p_type>> &m_nodes;
  const aiebu::aiebu_assembler::buffer_type m_buffer_type;

public:
  aie2p_basicpass(std::list<basic_node<aie2p_type>> &nodes, aiebu::aiebu_assembler::buffer_type buffer_type)
    : m_nodes(nodes), m_buffer_type(buffer_type) {}

  virtual ~aie2p_basicpass() = default;
  // Delete copy and move constructors and assignment operators
  aie2p_basicpass(const aie2p_basicpass&) = delete;               // Copy constructor
  aie2p_basicpass& operator=(const aie2p_basicpass&) = delete;    // Copy assignment operator
  aie2p_basicpass(aie2p_basicpass&&) = delete;                    // Move constructor
  aie2p_basicpass& operator=(aie2p_basicpass&&) = delete;         // Move assignment operator

  virtual void serialize() const {};
  virtual void transform() = 0;
};

class passmanager {
private:
  ELFIO::elfio *m_elf;

private:
  void run_transforms(ELFIO::section *psec);

public:
  explicit passmanager(ELFIO::elfio *elf) : m_elf(elf) {}
  void run_transforms() {
    ELFIO::Elf_Half sec_num = m_elf->sections.size();
    for ( int i = 0; i < sec_num; ++i ) {
      ELFIO::section *psec = m_elf->sections[i];
      if (psec->get_type() != ELFIO::SHT_PROGBITS)
        continue;
      if (is_pm_ctrlpkt(psec->get_name()) || is_ctrldata(psec->get_name()))
        continue;
      run_transforms(psec);
    }
  }

};

}


#endif

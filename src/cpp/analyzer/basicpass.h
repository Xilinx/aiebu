// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_BASICPASS_H_
#define AIEBU_BASICPASS_H_

#include <string>
#include <list>
#include <elfio/elfio.hpp>
#include <elfio/elfio_section.hpp>

namespace aiebu {


enum class basic_node_state {
  original, // original ctrlcode node from input ELF is in pristine condition
  dropped, // original node from input ELF has been removed from the ctrlcode list
  fresh // a new node has been created and added to the ctrlcode list
};

template <typename aie2p_type> struct basic_node {
  const aie2p_type *m_op;
  size_t m_size;
  basic_node_state m_state;

  basic_node(const aie2p_type *op, size_t size,
             basic_node_state state = basic_node_state::original)
      : m_op(op), m_size(size), m_state(state) {}

  ~basic_node() {
    if (m_state == basic_node_state::fresh)
      delete m_op;
  }
};


template <typename aie2p_type> class aie2p_basicpass {
public:
  aie2p_basicpass() = default;
  virtual ~aie2p_basicpass() = default;
  // Delete copy and move constructors and assignment operators
  aie2p_basicpass(const aie2p_basicpass&) = delete;               // Copy constructor
  aie2p_basicpass& operator=(const aie2p_basicpass&) = delete;    // Copy assignment operator
  aie2p_basicpass(aie2p_basicpass&&) = delete;                    // Move constructor
  aie2p_basicpass& operator=(aie2p_basicpass&&) = delete;         // Move assignment operator

  virtual void serialize() const {};
  virtual void transform() = 0;
};


}


#endif

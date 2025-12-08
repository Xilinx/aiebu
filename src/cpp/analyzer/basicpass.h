// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_BASICPASS_H_
#define AIEBU_BASICPASS_H_

#include <cstdint>
#include <elfio/elfio.hpp>
#include <elfio/elfio_section.hpp>

namespace aiebu {

enum class pass_kind {
  print,
  drop_preempt,
  add_loadpdi
};


enum class basic_node_state {
  original, // original ctrlcode node from input ELF which is in the
            // pristine condition
  dropped,  // original ctrlcode node from input ELF but has been
            // removed from the ctrlcode list
  added,    // a new node which has been created and added to the ctrlcode
            // list. basic_node should perform life cycle management of this node
  zombie    // an empty node
};

template <typename aie2p_type>
struct basic_node {
  // m_op may point to an existing node in the current ELF when
  // basic_node does not own the memory. The client would set
  // m_state to "original" as a hint to basic_node.
  // If the client explicitly allocates a aie2p_type node on the
  // heap then they would set m_state to "added" which is used as
  // a hint for basic_node to own the memory of m_op.
  const aie2p_type *m_op;
  size_t m_size;
  size_t m_original_offset;
  size_t m_transformed_offset = 0;
  basic_node_state m_state;

  basic_node(const aie2p_type *op, size_t size, size_t offset,
             basic_node_state state = basic_node_state::original)
    : m_op(op), m_size(size), m_original_offset(offset), m_state(state) {}

  basic_node(basic_node &&other) noexcept
      : m_op(other.m_op), m_size(other.m_size),
        m_original_offset(other.m_original_offset),
        m_transformed_offset(other.m_transformed_offset), m_state(other.m_state) {
    other.m_op = nullptr;
    other.m_size = 0;
    other.m_original_offset = 0;
    other.m_transformed_offset = 0;
    other.m_state = basic_node_state::zombie;
  }

  basic_node(const basic_node &other) = delete;
  basic_node &operator=(basic_node const&other) = delete;
  basic_node &operator=(basic_node &&other) = delete;

  ~basic_node() {
    if (m_state == basic_node_state::added) {
      std::free((void*)m_op); // NOLINT
      m_op = nullptr;
    }
  }
};


template <typename aie2p_type>
class aie2p_basicpass {
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

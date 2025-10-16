// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_AIE2P_PASSES_H_
#define AIEBU_AIE2P_PASSES_H_

#include "basicpass.h"
#include "xaiengine.h"

namespace aiebu {

template <typename opnode> opnode *allocXaie() {
  // Need to use C classic malloc/free since the XAie op nodes do not have
  // C++ object model. This prevents usage of delete operator in the free path
  // as we have to first cast the basic_node::m_op of type XAie_OpHdr to the
  // right XAie op node before calling delete which will make basic_node
  // destructor code ugly.
  auto result = static_cast<opnode *>(std::malloc(sizeof(opnode))); // NOLINT
  std::memset(result, 0, sizeof(opnode));
  return result;
}

class XAie_OpHdr_null : public aie2p_basicpass<XAie_OpHdr> {
  std::list<basic_node<XAie_OpHdr>> &m_nodes;
public:
  explicit XAie_OpHdr_null(std::list<basic_node<XAie_OpHdr>> &nodes)
    : m_nodes(nodes)  {}

  void transform() override;
};


class XAie_OpHdr_print : public aie2p_basicpass<XAie_OpHdr> {
private:
  std::ostream &m_stream;
  std::list<basic_node<XAie_OpHdr>> &m_nodes;

public:
  XAie_OpHdr_print(std::list<basic_node<XAie_OpHdr>> &nodes,  std::ostream &stream)
    : m_stream(stream), m_nodes(nodes) {}
  void transform() override;
};

class XAie_OpHdr_drop_preempt : public aie2p_basicpass<XAie_OpHdr> {
private:
  std::list<basic_node<XAie_OpHdr>> &m_nodes;

public:
  explicit XAie_OpHdr_drop_preempt(std::list<basic_node<XAie_OpHdr>> &nodes)
    : m_nodes(nodes)  {}

  void transform() override;
};

class XAie_OpHdr_add_loadpdi : public aie2p_basicpass<XAie_OpHdr> {
private:
  std::list<basic_node<XAie_OpHdr>> &m_nodes;
  uint16_t m_pdiid;
  uint64_t m_pdisize;

public:
  explicit XAie_OpHdr_add_loadpdi(std::list<basic_node<XAie_OpHdr>> &nodes, uint16_t pdiid, uint64_t pdisize)
    : m_nodes(nodes), m_pdiid(pdiid), m_pdisize(pdisize) {}

  void transform() override;
};

}

#endif

// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef __AIEBU_TRANSACTION_HPP__
#define __AIEBU_TRANSACTION_HPP__

#include <string>
#include <unordered_map>
#include <cinttypes>
#include <memory>

// Original source code came from
// https://gitenterprise.xilinx.com/tsiddaga/dynamic_op_dispatch/blob/main/include/transaction.hpp

class transaction {
  struct transaction_impl;

public:
  struct arg_map {
    // map of arg_idx to new_arg_idx, offset
    std::unordered_map<uint32_t, std::pair<uint32_t, size_t>> amap;
  };

  transaction(const char *txn, unsigned size);
  [[nodiscard]] std::string get_txn_summary() const;
  [[nodiscard]] std::string get_all_ops() const;

//  void update_txns(struct arg_map &amap);

private:
  std::shared_ptr<transaction_impl> impl;
};


#endif

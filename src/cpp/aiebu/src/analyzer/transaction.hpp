// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef __AIEBU_TRANSACTION_HPP__
#define __AIEBU_TRANSACTION_HPP__

#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <cinttypes>

// https://gitenterprise.xilinx.com/tsiddaga/dynamic_op_dispatch/blob/main/include/transaction.hpp
// Subroutines to read the transaction binary

class transaction {

public:
  struct arg_map {
    // map of arg_idx to new_arg_idx, offset
    std::unordered_map<uint32_t, std::pair<uint32_t, size_t>> amap;
  };

  transaction(const char *txn, unsigned size);
  std::string get_txn_summary();
  std::string get_all_ops();
  void update_txns(struct arg_map &amap);

private:
  std::vector<uint8_t> txn_;
  uint32_t txn_size_;
  uint32_t txn_num_ops_;
  std::stringstream ss_ops_;

  void txn_pass_through(uint8_t *ptr);
  void txn_pass_through(uint8_t **ptr);
  std::string get_txn_summary(uint8_t *txn_ptr);
  void stringify_txn_ops();
  void stringify_w32(uint8_t **ptr);
  void stringify_bw32(uint8_t **ptr);
  void stringify_mw32(uint8_t **ptr);
  void stringify_mp32(uint8_t **ptr);
  void stringify_tct(uint8_t **ptr);
  void stringify_patchop(uint8_t **ptr);
  void stringify_rdreg(uint8_t **ptr);
  void stringify_rectimer(uint8_t **ptr);
  void stringify_merge_sync(uint8_t **ptr);
  void stringify_txn_bin();

  uint32_t num_w_ops = 0;
  uint32_t num_bw_ops = 0;
  uint32_t num_mw_ops = 0;
  uint32_t num_mp_ops = 0;
  uint32_t num_tct_ops = 0;
  uint32_t num_patch_ops = 0;
  uint32_t num_read_ops = 0;
  uint32_t num_readtimer_ops = 0;
  uint32_t num_merge_sync_ops = 0;
};


#endif /* __TRANSACTION_HPP__ */

// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.
#ifndef AIEBU_REPORTER_H_
#define AIEBU_REPORTER_H_

#include "aiebu/aiebu_assembler.h"

#include <elfio/elfio_dump.hpp>

namespace aiebu {

  class reporter {
  private:
      ELFIO::elfio my_elf_reader;
      const aiebu::aiebu_assembler::buffer_type m_buffer_type;
      const std::vector<char> m_buffer;


  protected:
      inline bool is_ctrldata(const std::string& name) const
      {
        return !name.compare(".ctrldata");
      }

      inline bool is_pm_ctrlpkt(const std::string& name) const
      {
        return !name.substr(0,8).compare(".ctrlpkt");
      }
  public:
      reporter(aiebu::aiebu_assembler::buffer_type type, const std::vector<char>& buffer);
      void elf_summary(std::ostream &stream) const;
      void ctrlcode_summary(std::ostream &stream) const;
      void disassemble(const std::filesystem::path &root, bool all = false) const;
      void disassemble(std::ostream &stream, bool all = false) const;
      void ctrlcode_blob_summary(std::ostream &stream) const;
      void disassemble_blob(std::ostream &stream) const;

  };
}

#endif
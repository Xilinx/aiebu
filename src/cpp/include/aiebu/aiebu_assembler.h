// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.
#ifndef AIEBU_ASSEMBLER_H_
#define AIEBU_ASSEMBLER_H_
#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace aiebu {

/*!
 * @struct instinfo
 *
 * @brief
 * instinfo represents the arginfo (a table of xrt_idx and its
 * bd_offset) of a instance. It also has the instance name.
 */
 struct instinfo {
  /*!
   * @struct arginfo
   *
   * @brief
   * arginfo represents a pair of xrt_idx and its BD offset in a
   * control code.
   * On AIE2PS and AIE4 platform where we use ASM control code,
   * APPLY_OFFSET_57 opcode has the xrt_idx and BD offset that the
   * address of xrt_idx (at runtime) needs to be patched. This
   * struct represent one patch pair (xrt_idx and its BD offset)
   */
  struct arginfo {
    uint32_t xrt_idx;
    uint64_t bd_offset;
  };
  std::string inst_name;
  std::vector<arginfo> inst_arginfo;
};

class file_artifact_impl;

/*
 * The file_artifact class provides an interface for managing
 * virtual files (in-memory buffers).
 * It uses PIMPL (file_artifact_impl) to encapsulate internal data and logic
 */
class file_artifact
{
  public:
    file_artifact();
    ~file_artifact();
    /*
     * Add a virtual file (in-memory buffer) into the artifact by reference.
     *
     * Note: it involvs copy of name and the buffer. But the caller still
     * owns the name and buffer
     * @param name   name of the buffer or virtual file.
     * @param buffer contents stored as a vector of chars.
     */
    void add_vfile(const std::string& name, const std::vector<char>& buffer);
    /*
     * Add a virtual file (in-memory buffer) into the artifact by rvalue.
     *
     * Note: there is no extra copy of the buffer. But the ownership
     * of the buffer is transferred.
     *
     * @param name   name of the buffer or virtual file.
     * @param buffer contents stored as a vector of chars.
     */
    void add_vfile(std::string& name, std::vector<char>&& buffer);

    /*
     * Retrieve the contents of a virual file (in-memory buffer) from the artifacts
     *
     * @param name   name of the in-mem buffer/virtual file.
     * @return buffer contents in a vector of chars.
     */
    const std::vector<char>& get(const std::string& name) const;
    /*
     * Retrieve the contents of a virual file (in-memory buffer) from the artifact
     * or file from the disk
     *
     * @param name   name of the in-mem buffer/virtual file or physical file.
     * @param paths  paths to search if the file is in disk
     * @return buffer contents in a vector of chars.
     */
    std::vector<char> get(const std::string& name,
                          const std::vector<std::string>& paths) const;
  private:
    std::unique_ptr<file_artifact_impl> pimpl;
};

// Assembler Class

class aiebu_assembler
{
  std::vector<char> elf_data;

  public:
    enum class buffer_type {
      blob_instr_dpu,
      blob_instr_prepost,
      blob_instr_transaction,
      blob_control_packet,
      asm_aie2ps,
      asm_aie2,
      asm_aie4,
      aie2_config,
      aie2ps_config,
      aie4_config,
      elf_aie2,
      elf_aie2ps,
      pdi_aie2,
      pdi_aie2ps,
      blob_control_packet_aie2,
      elf_aie2_config,
      elf_aie2ps_config,
      elf_aie4,
      elf_aie4_config,
      unspecified,
    };

  private:
    buffer_type m_type;
    buffer_type m_output_type;
    class argtbl_impl;  // Forward declaration
    file_artifact artifacts;
  public:
    /*
     * Constructor takes buffer type , 2 buffer and a vector of symbols with
     * external_buffer_id json as argument.
     * its throws aiebu::error object.
     * User may pass any combination like
     * 1. type as blob_instr_transaction, buffer1 as instruction buffer
     *    and buffer2 as control_packet and pm_ctrlpkt as map of <pm_ctrlpkt_ID, pm_ctrlpkt_buf>
     *    : in this case it will package buffers in text section, data section and
     *    ctrlpkt_pm_N section of elf respectively.
     * 2. type as blob_instr_transaction, buffer1 as instruction buffer
     *    and buffer2 as empty and and pm_ctrlpkt as map of <pm_ctrlpkt_ID, pm_ctrlpkt_buf>
     *    : in this case it will package buffer in text section and ctrlpkt_pm_N section of elf respectively.
     * 3. type as asm_aie2ps/asm_aie4, buffer1 as asm buffer and buffer2
     *    as empty: in this case it will assemble the asm code and package in elf.
     *    This api can do fileops for include asm/ctrlpkt.
     *
     * @type           buffer type
     * @instr_buf      first buffer
     * @constrol_buf   second buffer
     * @patch_json     external_buffer_id json
     * @libs           libs to include in elf
     * @libpaths       paths to search for libs, paths to search for included asm, ctrlpkt.
                       only paths provided in this, are used for searching.
     * @ctrlpkt        map of pm id and pm control packet buffer
     */
     aiebu_assembler(buffer_type type,
               const std::vector<char>& buffer1,
               const std::vector<char>& buffer2,
               const std::vector<char>& patch_json,
               const std::vector<std::string>& libs = {},
               const std::vector<std::string>& libpaths = {},
               const std::map<uint32_t, std::vector<char> >& pm_ctrlpkt = {});

    /*
     * Constructor takes buffer type, buffer,
     * and a vector of symbols with their patching information as argument.
     * This api can do fileops for include asm/ctrlpkt.
     * its throws aiebu::error object.
     *
     * @type           buffer type
     * @instr_buf      first buffer
     * @libs           libs to include in elf
     * @libpaths       paths to search for libs, paths to search for included asm, ctrlpkt.
                       only paths provided in this, are used for searching.
     * @patch_json     external_buffer_id json
     */
    aiebu_assembler(buffer_type type,
              const std::vector<char>& buffer,
              const std::vector<std::string>& libs = {},
              const std::vector<std::string>& libpaths = {},
              const std::vector<char>& patch_json = {});

    /*
     * In memory api for full elfs.
     * Construct aiebu_assembler from config json buffer and in memory buffers
     *
     * @type:               ELF buffer type (aie2_config, aie2ps_config, aie4_config)
     * @config_json_buffer: Config json content
     * @artifact:           file_artifact object contains the mapping between
     *                      virtual file (in-memory buffer) name and its binary
     * @flags:              for passing configuration flags to the assembler
     *                      ex: disabledump (disable debug dump),
     *                          fulldump (enable debug dump),
     *                          opt_level_1 (for optimization)
     */
    aiebu_assembler(buffer_type type,
                    const std::vector<char>& config_json_buffer,
                    const file_artifact& artifact,
                    const std::vector<std::string>& flags);

    /*
     * This function return vector with elf content.
     *
     * Inside elf for IPU, instr_buf will be placed in .text section and control_buf will
     * be placed in .data section. There are other dynamic sections in the elf
     * containing the relocatable information. With this elf, at runtime, XRT
     * will patch the symbols (value or address based on the schema) into their
     * instruction buffer and control buffer before sending the buffer to device.
     *
     * return: vector of char with elf content
     */
    std::vector<char>
    get_elf() const;

    void
    get_report(std::ostream &stream) const;

    void
    disassemble(const std::filesystem::path &root) const;

    /*
     * Construct aiebu_assembler from ELF file
     *
     * @elf_fnm:     ELF Full path to ELF file
     */
    explicit aiebu_assembler(const std::string& elf_fnm);

    /*
     * Construct aiebu_assembler from ELF buffer
     *
     * @buffers:     ELF buffers
     */
    explicit aiebu_assembler(const std::vector<char>& buffer);

    /*!
     * @class argtbl
     *
     * @brief
     * aiebu_assembler::argtbls represents a vector of instance infor.
     * Inside each element, there is a table of xrt_idx and BD offset for
     * that instance.
     *
     * The class is constructed from an aiebu_assembler object based on the
     * control code that indicate which xrt_idx should patched into which BD
     * for each instance within the given kernel name.
     *
     * This object can be used to dump the table and modify the xrt_idx and its
     * BD offset in any entry in any instance in a given kernel. And then flush it
     * back to aiebu_assembler so that the xrt_id and its BD offset can be updated
     * in the control code and .dynamic sections of ELF.
     *
     * The kernel name can be updated at the same time by calling set_name() API.
     * So that when this object is flushed back, the kernel name can be updated in
     * ELF as well.
     *
     * When using this class to do xrt argument transform
     *      1. Only host patching is supported.
     *      2. Only support AIE2PS and AIE4
     */
     class argtbl
     {
       private:
         std::shared_ptr<argtbl_impl> handle;
       public:
         explicit argtbl(std::shared_ptr<argtbl_impl> in_impl);

         /*
          * Get the reference of vector of instance info. Inside
          * each instance, there is table of xrt argument (xrt_idx)
          * and BD offset of that instance. Caller can modify entries
          * in the table in place. Then the whole argtbl object can be flushed
          * back to aiebu_assembler to update the ELF to do the
          * xrt_idx transform.
          */
         std::vector<instinfo>& get();

         /*
          * Update the kernel name in the instance info object
          * @param name: the new kernel name (e.g., "NewKernel")
          *
          * The kernel name in ELF won't be updated until the whole
          * object is flushed back.
          */
         void set_name(const std::string& name);

         /*
          * Get the handle of the argtbl_impl object
          */
         const std::shared_ptr<argtbl_impl>&
         get_handle() const
         {
           return handle;
         }
     };

     /*
      * Get an argtbl object from aiebu_assembler for a given
      * kernel name. In this function aiebu_assember will scan
      * the ELF and construct a vector of instance info (instinfo).
      * Inside each instinfo, we have a table (vector)
      * of xrt_idx and BD offset of that kernel:inst's control code.
      *
      * The object can be used to dump the reference of the instance
      * info and modify the table in place inside each instance.
      *
      * NOTE: applicable for only full elf's
      */
     argtbl get_argtbl(const std::string& kernel_name);

     /*
      * Flush the argtbl object to aiebu_assembler. In this function,
      * aiebu will take the argtbl object and update control code and
      * patching metadata based on the xrt_idx and BD offset in the
      * arg table for each instance.
      *
      * Also, if the kernel name associated with the argtbl object changes,
      * the kernel name in the ELF will be updated accordingly.
      *
      * NOTE: applicable for only full elf's
      */
      void flush_argtbl(const argtbl& arg_table);
};

} //namespace aiebu

#endif // AIEBU_ASSEMBLER_H_

// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.
#ifndef AIEBU_ASSEMBLER_H_
#define AIEBU_ASSEMBLER_H_
#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace ELFIO {
class elfio;
}

namespace aiebu {

/*!
 * @enum aie_context_status
 *
 * @brief
 * State of the AIE context at the time the coredump was captured.
 * Stored as a uint32_t in the NT_AIE_DUMP_HDR note wire format.
 *
 */
enum class aie_context_status : uint32_t {
  idle     = 0,  ///< Context is idle — no work queued or running
  ready    = 1,  ///< Context is ready to run but not yet scheduled
  running  = 2,  ///< Context is actively executing on the hardware
  timeout  = 3,  ///< Context timed out waiting for hardware completion
  error    = 4,  ///< Context encountered any other error 
};

/*!
 * @struct aie_coredump_meta
 *
 * @brief
 * Optional metadata attached to an AIE coredump ELF.
 */
struct aie_coredump_meta {
  uint64_t           timestamp_ns;    ///< Capture timestamp in nanoseconds
  std::string        driver_version;  ///< Driver version string
  std::string        fw_version;      ///< Firmware version string
  std::string        device_info;     ///< Device identification string
  aie_context_status context_status;  ///< Context state at time of dump
  std::string        uuid;            ///< UUID uniquely identifying the AIE ELF loaded on target
};

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
    [[nodiscard]]
    const std::vector<char>& get(const std::string& name) const;
    /*
     * Retrieve the contents of a virual file (in-memory buffer) from the artifact
     * or file from the disk
     *
     * @param name   name of the in-mem buffer/virtual file or physical file.
     * @param paths  paths to search if the file is in disk
     * @return buffer contents in a vector of chars.
     */
    [[nodiscard]]
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
      asm_aie4a,
      asm_aie4z,
      aie2_config,
      aie2ps_config,
      aie4_config,
      aie4a_config,
      aie4z_config,
      elf_aie2,
      elf_aie2ps,
      pdi_aie2,
      pdi_aie2ps,
      blob_control_packet_aie2,
      elf_aie2_config,
      elf_aie2ps_config,
      elf_aie4,
      elf_aie4_config,
      elf_aie4a,
      elf_aie4a_config,
      elf_aie4z,
      elf_aie4z_config,
      unspecified,
      blob_aie2ps,    // Raw binary file for aie2ps architecture
      blob_aie4,      // Raw binary file for aie4 architecture
      blob_aie4a,     // Raw binary file for aie4a architecture
      blob_aie4z,     // Raw binary file for aie4z architecture
      coredump_aie2p,
      coredump_aie2ps,
      coredump_aie4,
      coredump_aie4a,
      coredump_aie4z,
  };

  private:
    buffer_type m_type;
    buffer_type m_output_type;
    class argtbl_impl;  // Forward declaration
    class op_tbl_impl;  // Forward declaration
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
     * @type:               ELF buffer type (aie2_config, aie2ps_config, aie4_config,
     *                      aie4a_config, aie4z_config)
     * @config_json_buffer: Config json content
     * @artifact:           file_artifact object contains the mapping between
     *                      virtual file (in-memory buffer) name and its binary
     * @flags:              Configuration flags for the assembler. Recognized values:
     *                        disabledump           — disable debug dump sections
     *                        fulldump              — enable full debug dump
     *                        opt_level_1           — enable optimization level 1
     *                        compress             — compress .ctrltext* / .ctrldata*
     *                                               sections using zstd at default
     *                                               level 3 (SHF_COMPRESSED)
     *                        compress=zstd         — same as "compress"
     *                        compress=zstd:<level> — zstd at explicit level; <level>
     *                                               is an integer in
     *                                               [ZSTD_minCLevel(), ZSTD_maxCLevel()]
     *                                               (typically -131072..22); higher =
     *                                               better ratio, slower; e.g.
     *                                               "compress=zstd:1"  fastest
     *                                               "compress=zstd:19" best ratio
     *                        compress=none         — no compression (default)
     *                      Multiple flags may be combined, e.g. {"fulldump","compress=zstd:5"}.
     *                      Only one compress flag is allowed; duplicates throw
     *                      std::invalid_argument. Unknown values also throw.
     */
    aiebu_assembler(buffer_type type,
                    const std::vector<char>& config_json_buffer,
                    const file_artifact& artifact,
                    const std::vector<std::string>& flags);

    /// Tag type: pass aiebu_assembler::no_meta to select the no-metadata
    /// coredump constructor.
    struct no_meta_t { explicit no_meta_t() = default; };
    static constexpr no_meta_t no_meta{};

    /*
     * Construct an AIE coredump ELF from a raw dump blob, without metadata.
     * Only coredump_aie2p, coredump_aie2ps, coredump_aie4, coredump_aie4a,
     * and coredump_aie4z buffer types are accepted; all others throw
     * aiebu::error with error_code::invalid_buffer_type.
     *
     * @type   One of the coredump_* buffer_type values.
     * @blob   Raw AIE memory dump bytes.
     * @       Pass aiebu_assembler::no_meta as the third argument.
     */
    aiebu_assembler(buffer_type type,
                    const std::vector<char>& blob,
                    no_meta_t);

    /*
     * Construct an AIE coredump ELF from a raw dump blob, with metadata.
     * Only coredump_aie2p, coredump_aie2ps, coredump_aie4, coredump_aie4a,
     * and coredump_aie4z buffer types are accepted; all others throw
     * aiebu::error with error_code::invalid_buffer_type.
     *
     * @type   One of the coredump_* buffer_type values.
     * @blob   Raw AIE memory dump bytes.
     * @meta   Metadata to embed (timestamp, versions, uuid, context status).
     */
    aiebu_assembler(buffer_type type,
                    const std::vector<char>& blob,
                    const aie_coredump_meta& meta);

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
    [[nodiscard]]
    std::vector<char>
    get_elf() const;

    /*
     * Return true if the ELF contains any SHF_COMPRESSED sections.
     *
     * Implemented as a direct raw scan of ELF section headers — no ELFIO, no
     * heap allocation.  XRT callers should use this to avoid the full ELFIO
     * parse cost of decompress_elf() for ELFs that are not compressed:
     *
     *   if (aiebu::aiebu_assembler::is_elf_compressed(elf))
     *       elf = aiebu::aiebu_assembler::decompress_elf(std::move(elf));
     *
     * @elf_bytes: ELF bytes to inspect.
     * @return:    true if at least one section has SHF_COMPRESSED set.
     */
    static bool
    is_elf_compressed(const std::vector<char>& elf_bytes);

    /*
     * Decompress a compressed AIE ELF in-place and return the decompressed bytes.
     *
     * Callers such as XRT invoke this before patching to ensure the ELF sections
     * are in their original uncompressed form.  The algorithm is auto-detected
     * per section from ch_type in the standard ELF Elf_Chdr header — callers do
     * not need to know which algorithm was used during assembly.
     *
     * Pass-by-value is intentional: callers should std::move their buffer in to
     * avoid an extra copy of a potentially large ELF on the critical path.
     * If the ELF contains no SHF_COMPRESSED sections, the buffer is returned
     * unchanged via move — zero allocation, zero copy.
     *
     * @elf_bytes: ELF bytes, possibly containing SHF_COMPRESSED sections.
     * @return:    Decompressed ELF bytes.  Throws std::runtime_error on corrupt
     *             data or unsupported compression type.
     *
     * Example (XRT call site):
     *   auto patching_buf = aiebu::aiebu_assembler::decompress_elf(std::move(elf));
     */
    static std::vector<char>
    decompress_elf(std::vector<char> elf_bytes);

    /*
     * Parse the NT_AIE_DUMP_HDR note from a coredump ELF and return its
     * metadata.  Returns nullopt if the ELF is not ET_CORE or contains
     * no AMDAIE_CORE note.
     */
    std::optional<aie_coredump_meta>
    get_coredump_meta() const;

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

    /*
     * Construct aiebu_assembler from an ELFIO in-memory ELF object.
     *
     * @param elf  Non-null pointer to ELFIO::elfio; nullptr throws aiebu::error.
     */
    explicit aiebu_assembler(ELFIO::elfio* elf);

    /*
     * Construct aiebu_assembler from an const ELFIO in-memory ELF object.
     *
     * @param elf  Non-null pointer to ELFIO::elfio; nullptr throws aiebu::error.
     */
    explicit aiebu_assembler(const ELFIO::elfio* elf);

    /*!
     * @struct op_loc
     *
     * @brief
     * op_loc groups all occurrences of a queried opcode found across the
     * .dump section of one kernel instance.
     *   - inst_name  — instance name (empty for standalone target ELFs)
     *   - line_info  — opcode occurrences grouped by column number
     */
    struct op_loc {
      /*!
       * @struct lineinfo
       *
       * @brief
       * lineinfo groups all opcode occurrences that share the same column.
       *
       * @col      AIE column number ("column" field in the .dump JSON entry)
       * @entries  One entry per opcode occurrence on this column:
       *             first  — linenumber (source line in the .asm file)
       *             second — filename   (source .asm file path)
       */
      struct lineinfo {
        uint32_t col;
        std::vector<std::pair<uint32_t, std::string>> entries;  // linenumber, filename
      };
      std::string inst_name;   // instance name
      std::vector<lineinfo> line_info;  // one entry per col
    };

    /*!
     * @class op_tbl
     *
     * @brief
     * op_tbl is a read-only container returned by get_op_locations().
     * It holds one op_loc per instance that contains at least one
     * occurrence of the queried opcode.
     */
    class op_tbl
    {
      private:
        std::shared_ptr<op_tbl_impl> handle;
      public:
        explicit op_tbl(std::shared_ptr<op_tbl_impl> in_impl);

        /*!
         * Return a const reference to the vector of op_loc entries, one per
         * instance, in section-traversal order.
         */
        [[nodiscard]]
        std::vector<op_loc> get_line_info() const;
    };

    /*!
     * @brief
     * Scan the .dump section JSON of every instance of the given kernel and
     * return an op_tbl containing one op_loc per instance that has at least
     * one occurrence of the specified opcode.
     *
     * The .dump section holds the debug JSON written by the encoder at
     * assembly time.  It is present by default and suppressed only when the
     * ELF is assembled with the "disabledump" flag.
     *
     * Each op_loc carries:
     *   inst_name  — instance name
     *   line_info  — occurrences grouped by column; each lineinfo holds:
     *     col     — AIE column number
     *     entries — one {linenumber, filename} pair per opcode occurrence
     *
     * Applicable to full config ELFs (aie2ps_config / aie4_config).
     *
     * @param opcode       Raw ISA opcode value (e.g. OPCODE_SAVE_TIMESTAMPS = 0x1c)
     * @param kernel_name  Kernel to scan (e.g. "DPU")
     * @return             op_tbl in instance-traversal order; empty if no
     *                     .dump section exists or no matching opcodes are found
     * @throws aiebu::error  if kernel_name is not found or opcode is unknown
     */
    [[nodiscard]]
    op_tbl
    get_op_locations(uint8_t opcode, const std::string& kernel_name) const;

    /*!
     * @brief
     * Overload for xclbin + elf ELFs.
     *
     * Scans the single .dump section and returns an op_tbl containing one
     * op_loc for all occurrences of the specified opcode.
     *
     * @param opcode  Raw ISA opcode value (e.g. OPCODE_SAVE_TIMESTAMPS = 0x1c)
     * @return        op_tbl — empty if no .dump section exists or the ELF was
     *                assembled with the "disabledump" flag
     * @throws aiebu::error  if opcode is unknown
     */
    [[nodiscard]]
    op_tbl
    get_op_locations(uint8_t opcode) const;

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
         [[nodiscard]]
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

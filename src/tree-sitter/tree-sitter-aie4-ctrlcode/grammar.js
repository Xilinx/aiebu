/**
 * @file AMD aie4 / aie2ps ctrlcode parser for tree-sitter
 * Grammar derived from test/aie4-ctrlcode/*.asm, test/aie2ps-ctrlcode/*.asm,
 * and src/tree-sitter/tree-sitter-aie2p/grammar.js
 * @license MIT
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

module.exports = grammar({
  name: 'aie4_ctrlcode',

  extras: $ => [/[\s\t\n]+/],

  rules: {
    source_file: $ => repeat(
      choice(
        $._statement,
        $._directive,
        $.label_definition,
        $._comment
      )
    ),

    _directive: $ => choice(
      $.partition_directive,
      $.attach_to_group_directive,
      $.eop_directive,
      $.include_directive,
      $.endl_directive,
      $.align_directive,
      $.long_directive,
      $.section_directive,
      $.target_directive,
      $.aie_row_topology_directive
    ),

    section_directive: $ => seq(
      '.section',
      $.section_name
    ),

    section_name: $ => /\.?[a-zA-Z_][a-zA-Z0-9_]*/,

    partition_directive: $ => seq(
      '.partition',
      $.partition_spec
    ),

    partition_spec: $ => token(seq(/\d+/, 'column')),

    attach_to_group_directive: $ => seq(
      '.attach_to_group',
      $.decint
    ),

    eop_directive: $ => '.eop',

    include_directive: $ => seq(
      '.include',
      $.include_path
    ),

    include_path: $ => /[\.\/_\-a-zA-Z0-9]+\.asm/,

    endl_directive: $ => seq(
      '.endl',
      $.identifier
    ),

    align_directive: $ => seq(
      token(/\.align\s+/i),
      $.decint
    ),

    long_directive: $ => seq(
      token(/\.long\s+/i),
      $.hexint
    ),

    target_directive: $ => seq(
      '.target',
      $.identifier
    ),

    aie_row_topology_directive: $ => seq(
      '.aie_row_topology',
      token(/\d+(?:-\d+)*/)
    ),

    _statement: $ => choice(
      $.start_job_statement,
      $.end_job_statement,
      $.start_cond_job_preempt_statement,
      $.start_job_deferred_statement,
      $.launch_job_statement,
      $.uc_dma_write_des_sync_statement,
      $.mask_write_32_statement,
      $.mask_poll_32_statement,
      $.uc_dma_mask_poll_ext_statement,
      $.apply_offset_statement,
      $.apply_offset_sram_statement,
      $.uc_dma_bd_statement,
      $.load_pdi_statement,
      $.load_cores_statement,
      $.load_cores_cp_statement,
      $.write_32_statement,
      $.write_32_d_statement,
      $.read_32_statement,
      $.read_32_d_statement,
      $.wait_tcts_statement,
      $.local_barrier_statement,
      $.remote_barrier_statement,
      $.uc_dma_write_des_statement,
      $.wait_uc_dma_statement,
      $.save_timestamps_statement,
      $.save_register_statement,
      $.preempt_statement,
      $.annotation_metadata,
      $.nop_statement,
      $.eof_marker
    ),

    start_job_statement: $ => seq(
      token(choice('START_JOB', 'start_job')),
      $.decint
    ),

    start_cond_job_preempt_statement: $ => seq(
      'START_COND_JOB_PREEMPT',
      $.decint
    ),

    start_job_deferred_statement: $ => seq(
      'START_JOB_DEFERRED',
      $.decint
    ),

    launch_job_statement: $ => seq(
      'LAUNCH_JOB',
      $.decint
    ),

    end_job_statement: $ => token(choice('END_JOB', 'end_job')),

    uc_dma_write_des_sync_statement: $ => seq(
      token(choice('UC_DMA_WRITE_DES_SYNC', 'uC_DMA_WRITE_DES_SYNC')),
      $.address
    ),

    mask_write_32_statement: $ => seq(
      'MASK_WRITE_32',
      $.hexint,
      ',',
      $.hexint,
      ',',
      $.hexint
    ),

    mask_poll_32_statement: $ => seq(
      'MASK_POLL_32',
      $.hexint,
      ',',
      $.hexint,
      ',',
      $.hexint
    ),

    uc_dma_mask_poll_ext_statement: $ => seq(
      token(choice('UC_DMA_MASK_POLL_EXT', 'uc_dma_mask_poll_ext')),
      $.hexint,
      ',',
      $.hexint,
      ',',
      $.hexint,
      ',',
      $.hexint
    ),

    apply_offset_statement: $ => seq(
      token(seq('APPLY_OFFSET_', /\d+/)),
      $.address,
      ',',
      $.decint,
      ',',
      $.decint
    ),

    apply_offset_sram_statement: $ => seq(
      'APPLY_OFFSET_SRAM',
      $.address,
      ',',
      $.decint,
      ',',
      $.hexint
    ),

    uc_dma_bd_statement: $ => seq(
      'UC_DMA_BD',
      choice($.decint, $.hexint),
      ',',
      $.hexint,
      ',',
      $.address,
      ',',
      choice($.decint, $.hexint),
      ',',
      $.decint,
      ',',
      $.decint
    ),

    load_pdi_statement: $ => seq(
      token(choice('LOAD_PDI', 'load_pdi')),
      choice($.decint, $.hexint),
      ',',
      $.address
    ),

    load_cores_statement: $ => seq(
      'LOAD_CORES',
      $.decint,
      ',',
      $.address
    ),

    load_cores_cp_statement: $ => seq(
      'LOAD_CORES_CP',
      choice($.decint, $.hexint)
    ),

    write_32_statement: $ => seq(
      token(choice('WRITE_32', 'write_32')),
      $.hexint,
      ',',
      $.hexint
    ),

    write_32_d_statement: $ => seq(
      token(choice('WRITE_32_D', 'write_32_d')),
      $.decint,
      ',',
      $.hexint,
      ',',
      $.decint
    ),

    read_32_statement: $ => seq(
      token(choice('READ_32', 'read_32')),
      $.register_ref,
      ',',
      $.hexint
    ),

    read_32_d_statement: $ => seq(
      token(choice('READ_32_D', 'read_32_d')),
      $.register_ref,
      ',',
      $.register_ref
    ),

    wait_tcts_statement: $ => seq(
      token(choice('WAIT_TCTS', 'wait_tcts')),
      $.tile_spec,
      ',',
      $.channel_spec,
      ',',
      $.decint
    ),

    tile_spec: $ => token(seq('TILE_', /\d+/, '_', /\d+/)),

    channel_spec: $ => token(choice(
      seq('SHIM_MM2S_', /\d+/),
      seq('SHIM_S2MM_', /\d+/),
      seq('MEM_S2MM_', /\d+/),
      seq('MEM_MM2S_', /\d+/),
      seq('TILE_S2MM_', /\d+/)
    )),

    local_barrier_statement: $ => seq(
      token(choice('LOCAL_BARRIER', 'local_barrier')),
      $.local_barrier_reg,
      ',',
      $.decint
    ),

    local_barrier_reg: $ => token(seq(/\$lb/, /\d+/)),

    remote_barrier_statement: $ => seq(
      'REMOTE_BARRIER',
      $.remote_barrier_reg,
      ',',
      $.hexint
    ),

    remote_barrier_reg: $ => token(seq(/\$rb/, /\d+/)),

    uc_dma_write_des_statement: $ => seq(
      token(choice('uC_DMA_WRITE_DES', 'uc_dma_write_des')),
      $.register_ref,
      ',',
      $.address
    ),

    wait_uc_dma_statement: $ => seq(
      token(choice('WAIT_uC_DMA', 'wait_uc_dma')),
      $.register_ref
    ),

    register_ref: $ => token(seq(/\$r/, /\d+/)),

    save_timestamps_statement: $ => seq(
      'SAVE_TIMESTAMPS',
      $.decint
    ),

    save_register_statement: $ => seq(
      'SAVE_REGISTER',
      $.hexint,
      ',',
      $.decint
    ),

    preempt_statement: $ => seq(
      token(choice('PREEMPT', 'preempt')),
      choice($.decint, $.hexint),
      ',',
      $.address,
      ',',
      $.address,
      optional(seq(',', $.address))
    ),

    nop_statement: $ => token(choice('NOP', 'nop')),

    eof_marker: $ => token(choice('EOF', 'eof')),

    annotation_metadata: $ => choice(
      seq(
        choice('id', 'name'),
        ':',
        choice($.decint, $.hexint, $.identifier)
      ),
      prec.right(seq(
        'description',
        ':',
        repeat1($.identifier)
      ))
    ),

    label_definition: $ => seq(
      $.identifier,
      ':'
    ),

    address: $ => seq(
      '@',
      $.identifier
    ),

    identifier: $ => /[A-Za-z_][A-Za-z0-9_]*/,

    hexint: $ => /(0x)[0-9A-Fa-f][0-9A-Fa-f_]*/,

    decint: $ => /\d+/,

    _comment: $ => /(;).*/
  }
});

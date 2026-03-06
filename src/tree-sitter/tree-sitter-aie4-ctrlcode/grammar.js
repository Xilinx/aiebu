/**
 * @file AMD aie4 ctrlcode parser for tree-sitter
 * Grammar derived from test/aie4-ctrlcode/*.asm and
 * src/tree-sitter/tree-sitter-aie2p/grammar.js
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
      $.long_directive
    ),

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

    include_path: $ => /[A-Za-z0-9_.]+\.asm/,

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

    _statement: $ => choice(
      $.start_job_statement,
      $.end_job_statement,
      $.uc_dma_write_des_sync_statement,
      $.mask_write_32_statement,
      $.mask_poll_32_statement,
      $.apply_offset_statement,
      $.uc_dma_bd_statement,
      $.load_pdi_statement,
      $.write_32_statement,
      $.wait_tcts_statement,
      $.local_barrier_statement,
      $.uc_dma_write_des_statement,
      $.wait_uc_dma_statement,
      $.save_timestamps_statement,
      $.preempt_statement,
      $.eof_marker
    ),

    start_job_statement: $ => seq(
      'START_JOB',
      $.decint
    ),

    end_job_statement: $ => 'END_JOB',

    uc_dma_write_des_sync_statement: $ => seq(
      'UC_DMA_WRITE_DES_SYNC',
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

    apply_offset_statement: $ => seq(
      token(seq('APPLY_OFFSET_', /\d+/)),
      $.address,
      ',',
      $.decint,
      ',',
      $.decint
    ),

    uc_dma_bd_statement: $ => seq(
      'UC_DMA_BD',
      $.decint,
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
      'LOAD_PDI',
      $.decint,
      ',',
      $.address
    ),

    write_32_statement: $ => seq(
      'WRITE_32',
      $.hexint,
      ',',
      $.hexint
    ),

    wait_tcts_statement: $ => seq(
      'WAIT_TCTS',
      $.tile_spec,
      ',',
      $.channel_spec,
      ',',
      $.decint
    ),

    tile_spec: $ => token(seq('TILE_', /\d+/, '_', /\d+/)),

    channel_spec: $ => token(choice(
      seq('SHIM_MM2S_', /\d+/),
      seq('MEM_S2MM_', /\d+/),
      seq('TILE_S2MM_', /\d+/)
    )),

    local_barrier_statement: $ => seq(
      'LOCAL_BARRIER',
      $.local_barrier_reg,
      ',',
      $.decint
    ),

    local_barrier_reg: $ => token(seq(/\$lb/, /\d+/)),

    uc_dma_write_des_statement: $ => seq(
      'uC_DMA_WRITE_DES',
      $.register_ref,
      ',',
      $.address
    ),

    wait_uc_dma_statement: $ => seq(
      'WAIT_uC_DMA',
      $.register_ref
    ),

    register_ref: $ => token(seq(/\$r/, /\d+/)),

    save_timestamps_statement: $ => seq(
      'SAVE_TIMESTAMPS',
      $.decint
    ),

    preempt_statement: $ => seq(
      'PREEMPT',
      $.hexint,
      ',',
      $.address,
      ',',
      $.address
    ),

    eof_marker: $ => 'EOF',

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

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
      $.section_directive
    ),

    section_directive: $ => seq(
      '.section',
      $.section_name
    ),

    section_name: $ => token(seq(/\./, /[a-zA-Z_][a-zA-Z0-9_]*/)),

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
      $.nop_statement,
      $.eof_marker
    ),

    start_job_statement: $ => seq(
      token(choice('START_JOB', 'start_job')),
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
      'LOAD_PDI',
      $.decint,
      ',',
      $.address
    ),

    write_32_statement: $ => seq(
      token(choice('WRITE_32', 'write_32')),
      $.hexint,
      ',',
      $.hexint
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

    preempt_statement: $ => seq(
      'PREEMPT',
      $.hexint,
      ',',
      $.address,
      ',',
      $.address
    ),

    nop_statement: $ => token(choice('NOP', 'nop')),

    eof_marker: $ => token(choice('EOF', 'eof')),

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

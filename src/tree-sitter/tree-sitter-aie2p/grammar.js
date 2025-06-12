/**
 * @file AMD aie2p ctrlcode parser for tree-sitter
 * @author XRT Team <runtimeca39d@amd.com>
 * @license MIT
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

module.exports = grammar({
  name: 'aie2p-ctrlcode',

  rules: {
      source_file: $ => repeat(
	  choice(
	      $._statement,
	      $._directive,
	      $._comment
	  )
      ),

      _directive : $ => choice(
	  $.attach_to_group
	  // TODO: other kinds of directives
      ),

      _statement: $ => choice(
	  $.xaie_io_write_statement,
	  $.xaie_io_blockwrite_statement,
	  $.xaie_io_custom_op_ddr_patch,
	  $.xaie_io_maskpoll,
	  $.xaie_io_load_pm_start,
	  $.xaie_io_custom_op_record_timer,
	  $.xaie_io_preempt,
	  $.xaie_io_custom_op_tct,
	  $.xaie_io_custom_op_merge_sync
	  // TODO: other kinds of statements
      ),

      xaie_io_write_statement: $ => seq(
	  'XAIE_IO_WRITE',
	  $.address,
	  ',',
	  $.hexint
      ),

      xaie_io_blockwrite_statement: $ => seq(
	  'XAIE_IO_BLOCKWRITE',
	  $.address,
	  ',',
	  '[',
	  $.decint,
	  ']',
	  repeat($.xaie_io_blockwrite_substatement)
      ),

      xaie_io_custom_op_ddr_patch: $ => seq(
	  'XAIE_IO_CUSTOM_OP_DDR_PATCH',
	  $.address,
	  ',',
	  $.indexid,
	  ',',
	  $.hexaddend
      ),

      xaie_io_blockwrite_substatement: $ => seq(
	  'XAIE_IO_BLOCKWRITE.',
	  $.decint,
	  $.hexint
      ),

      xaie_io_maskpoll: $ => seq(
	  'XAIE_IO_MASKPOLL',
          $.address,
	  ',',
	  $.hexint,
	  '()',
	  '==',
	  $.hexint
      ),

      xaie_io_load_pm_start: $ => seq(
	  'XAIE_IO_LOAD_PM_START',
	  $.hexint,
	  ',',
	  $.indexid
      ),

      xaie_io_custom_op_record_timer: $ => seq(
	  'XAIE_IO_CUSTOM_OP_RECORD_TIMER',
	  $.indexid
      ),

      xaie_io_preempt: $ => seq(
	  'XAIE_IO_PREEMPT',
	  $.preemptlevel
      ),

      xaie_io_custom_op_tct: $ => seq(
	  'XAIE_IO_CUSTOM_OP_TCT',
	  $.row,
	  $.decaddend,
	  ',',
	  $.column,
	  $.decaddend,
	  ',',
	  $.dmadir,
	  ',',
	  $.decint
      ),

      xaie_io_custom_op_merge_sync: $ => seq(
	  'XAIE_IO_CUSTOM_OP_MERGE_SYNC',
	  $.column,
	  '==',
	  $.decint
      ),

      attach_to_group: $ => seq(
	  '.attach_to_group',
	  $.decint
      ),

      address: $ => seq(
	  '@',
	  $.hexint
      ),

      indexid: $ => seq(
	  '#',
	  $.decint
      ),

      hexaddend: $ => seq(
	  '+',
	  $.hexint
      ),

      decaddend: $ => seq(
	  '+',
	  $.decint
      ),

      preemptlevel: $ => token(seq(
	  '#',
	  choice(
	      'NOOP',
	      'MEM_TILE',
	      'AIE_TILE',
	      'AIE_REGISTERS',
	      'INVALID'
	  )
      )),

      dmadir: $ => token(seq(
	  '#',
	  choice(
	      'DMA_MM2S',
	      'DMA_S2MM'
	  )
      )),

      column: $ => seq(
	  'C[',
	  $.decint,
	  ']'
      ),

      row: $ => seq(
	  'R[',
	  $.decint,
	  ']'
      ),

      hexint: $ => /(0x)[0-9A-Fa-f][0-9A-Fa-f_]*/,

      decint: $ => /\d+/,

      _comment: $ =>  /(;).*/
  }
});

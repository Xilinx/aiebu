subgraph_1:
.include op_Quant_float32xuint16_layer_id_0/pm_reload/Work_AIE4/layer_0_pm_reload_control.asm
.eop
.include fused_hw_package_subgraph_1/lcp_stream_layer_0/Work_AIE4/layer_0_lcp_control.asm
.eop
.include op_Quant_float32xuint16_layer_id_0/Work_AIE4/layer_0_control.asm
.eop
.include op_Reshape_qdq_uint16xuint16_layer_id_1/Work_AIE4/layer_1_control.asm
.eop
.include op_Transpose_qdq_uint16xuint16_layer_id_2/Work_AIE4/layer_2_control.asm
.eop
.include op_MatMul_qdq_bias_uint16xint8xuint16_layer_id_4/Work_AIE4/layer_4_control.asm
.eop
.endl subgraph_1

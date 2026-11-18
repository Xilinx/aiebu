subgraph_1:
.include op_MatMul_qdq_bias_uint16xuint16xuint16_layer_id_30/pm_reload/Work_AIE4/layer_30_pm_reload_control.asm
.eop
.include fused_hw_package_subgraph_1/lcp_stream_layer_30/Work_AIE4/layer_30_lcp_control.asm
.eop
.include op_MatMul_qdq_bias_uint16xuint16xuint16_layer_id_30/Work_AIE4/layer_30_control.asm
.eop
.include op_Softmax_qdq_uint16xuint16_layer_id_31/Work_AIE4/layer_31_control.asm
.eop
.include op_Dequant_uint16xfloat32_layer_id_32/Work_AIE4/layer_32_control.asm
.eop
.endl subgraph_1

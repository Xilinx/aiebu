subgraph_0:
.include op_Quant_float32xuint16_layer_id_0/pm_reload/Work_AIE4/layer_0_pm_reload_control.asm
.eop
.include fused_hw_package_subgraph_0/lcp_stream_layer_0/Work_AIE4/layer_0_lcp_control.asm
.eop
.include op_Quant_float32xuint16_layer_id_0/Work_AIE4/layer_0_control.asm
.eop
.include op_LpNormalization_qdq_uint16xuint16_layer_id_66/Work_AIE4/layer_66_control.asm
.eop
.include op_MatMul_qdq_uint16xint4xuint16_layer_id_131/Work_AIE4/layer_131_control.asm
.eop
.include op_MatMul_qdq_actxact_uint16xint8xuint16_layer_id_132/Work_AIE4/layer_132_control.asm
.eop
.include op_MatMul_qdq_actxact_uint16xint8xuint16_layer_id_165/Work_AIE4/layer_165_control.asm
.eop
.include op_Add_qdq_EleWise_uint16xuint16xuint16_layer_id_198/Work_AIE4/layer_198_control.asm
.eop
.endl subgraph_0

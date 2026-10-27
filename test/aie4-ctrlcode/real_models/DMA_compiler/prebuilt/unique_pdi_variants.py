"""
Auto-generated unique PDI variants (for operators supported on TOT).
Generated on: 2026-07-24 14:37:29

Supported models at generation time (when this file was generated):
  - CUA_context
  - CUA_iterator
  - DeepSeek-R1-Distill-Qwen-1_5B_qdq_context
  - DeepSeek-R1-Distill-Qwen-1_5B_qdq_iterator
  - GT
  - GTC
  - HT
  - HTC
  - Intel_bert
  - Llama-3.2-1B-Instruct_qdq_context
  - Llama-3.2-1B-Instruct_qdq_iterator
  - MB_A16W8_combo_ATTN_FC_SMAX_6stacked_2048x4096
  - MB_A16W8_combo_CONV_LRL_6stacked_224x224
  - MB_A16W8_combo_CONV_RL_6stacked_224x224
  - MB_A16W8_combo_FC_LN_6stacked_2048x4096
  - MB_A16W8_combo_FC_SMAX_6stacked_2048x4096
  - MB_A16W8_combo_LN_FC_GL_6stacked_2048x4096
  - MB_A16W8_combo_LN_FC_SL_6stacked_2048x4096
  - MB_A16W8_combo_TR_FC_1stacked_2048x4096
  - MB_A16W8_single_BW_FC_12stacked_weights4096x4096
  - MB_A16W8_single_BW_FC_24stacked_weights4096x4096
  - MB_A16W8_single_BW_FC_6stacked_weights4096x4096
  - MB_A16W8_single_BatchNorm_6stacked_ch512
  - MB_A16W8_single_FC_6stacked_weights4096x4096
  - MB_A16W8_single_Gelu_12stacked_2048x4096
  - MB_A16W8_single_Gelu_24stacked_2048x4096
  - MB_A16W8_single_Gelu_6stacked_2048x4096
  - MB_A16W8_single_LayerNorm17_6stacked_2048x4096
  - MB_A16W8_single_LayerNorm_6stacked_2048x4096
  - MB_A16W8_single_LeakyRelu_12stacked_2048x4096
  - MB_A16W8_single_LeakyRelu_24stacked_2048x4096
  - MB_A16W8_single_LeakyRelu_6stacked_2048x4096
  - MB_A16W8_single_Relu_12stacked_2048x4096
  - MB_A16W8_single_Relu_24stacked_2048x4096
  - MB_A16W8_single_Relu_6stacked_2048x4096
  - MB_A16W8_single_Silu_12stacked_2048x4096
  - MB_A16W8_single_Silu_24stacked_2048x4096
  - MB_A16W8_single_Silu_6stacked_2048x4096
  - MB_A16W8_single_Softmax_6stacked_2048x4096
  - MB_A16W8_single_conv_6stacked_ch512_3x3
  - MB_A8W8_combo_ATTN_FC_SMAX_6stacked_2048x4096
  - MB_A8W8_combo_CONV_LRL_6stacked_224x224
  - MB_A8W8_combo_CONV_RL_6stacked_224x224
  - MB_A8W8_combo_FC_LN_6stacked_2048x4096
  - MB_A8W8_combo_FC_SMAX_6stacked_2048x4096
  - MB_A8W8_combo_LN_FC_GL_6stacked_2048x4096
  - MB_A8W8_combo_LN_FC_SL_6stacked_2048x4096
  - MB_A8W8_combo_TR_FC_1stacked_2048x4096
  - MB_A8W8_single_BW_FC_12stacked_weights4096x4096
  - MB_A8W8_single_BW_FC_24stacked_weights4096x4096
  - MB_A8W8_single_BW_FC_6stacked_weights4096x4096
  - MB_A8W8_single_BatchNorm_6stacked_ch512
  - MB_A8W8_single_FC_6stacked_weights4096x4096
  - MB_A8W8_single_Gelu_12stacked_2048x4096
  - MB_A8W8_single_Gelu_24stacked_2048x4096
  - MB_A8W8_single_Gelu_6stacked_2048x4096
  - MB_A8W8_single_LayerNorm17_6stacked_2048x4096
  - MB_A8W8_single_LayerNorm_6stacked_2048x4096
  - MB_A8W8_single_LeakyRelu_12stacked_2048x4096
  - MB_A8W8_single_LeakyRelu_24stacked_2048x4096
  - MB_A8W8_single_LeakyRelu_6stacked_2048x4096
  - MB_A8W8_single_Relu_12stacked_2048x4096
  - MB_A8W8_single_Relu_24stacked_2048x4096
  - MB_A8W8_single_Relu_6stacked_2048x4096
  - MB_A8W8_single_Silu_12stacked_2048x4096
  - MB_A8W8_single_Silu_24stacked_2048x4096
  - MB_A8W8_single_Silu_6stacked_2048x4096
  - MB_A8W8_single_Softmax_6stacked_2048x4096
  - MB_A8W8_single_conv_6stacked_ch512_3x3
  - P1_FacebookAI_xlm-roberta-large
  - P1_Intel--bert-base-uncased-mrpc
  - P1_NeuML_pubmedbert_base_embeddings
  - P1_WinKawaks-vit-tiny-patch16-224
  - P1_autoguon-chronos-decoder
  - P1_autoguon-chronos-encoder
  - P1_cross_encoder_ms_marco_MiniLM_L_6_v2
  - P1_cross_encoder_ms_marco_MiniLM_L_6_v2_p2
  - P1_distilbert_distilbert_base_uncased_finetuned_sst_2_english
  - P1_emilyalsentzer_Bio_ClinicalBERT
  - P1_google--vit-base-patch16-224
  - P1_google-bert--bert-base-multilingual-cased
  - P1_google_bert_bert_base_cased
  - P1_google_bert_bert_base_chinese
  - P1_google_bert_bert_base_multilingual_uncased
  - P1_google_bert_bert_large_uncased
  - P1_google_bert_uncased_L_2_H_128_A_2
  - P1_google_t5_t5_small_decoder_model
  - P1_google_t5_t5_small_encoder_model
  - P1_laion--CLIP-ViT-B-32-laion2B-s34B-b79K
  - P1_laion--CLIP-ViT-B-32-laion2B-s34B-b79K_p10
  - P1_microsoft-beit-base-patch16-224-pt22k-ft22k
  - P1_microsoft_infoxlm-large
  - P1_microsoft_table_transformer_detection
  - P1_nateraw-vit-age-classifier
  - P1_neuralmind_bert_base_portuguese_cased
  - P1_openai--clip-vit-base-patch16
  - P1_openai--clip-vit-base-patch16_p10
  - P1_openai--clip-vit-base-patch32
  - P1_openai--clip-vit-base-patch32_p10
  - P1_openai--clip-vit-large-patch14
  - P1_openai--clip-vit-large-patch14_p10
  - P1_openai--whisper-base-en-Decoder
  - P1_openai--whisper-base-en-Decoder_Static_180_1500
  - P1_openai--whisper-base-en-Encoder
  - P1_openai--whisper-large-v3-Decoder
  - P1_openai--whisper-large-v3-Encoder
  - P1_openai--whisper-large-v3-turbo-Decoder
  - P1_openai--whisper-large-v3-turbo-Encoder
  - P1_openai--whisper-small-Decoder_Static
  - P1_openai--whisper-small-Decoder_Static_180_1500
  - P1_openai--whisper-small-Encoder
  - P1_patrickjohncyh-fashion-clip_Static
  - P1_patrickjohncyh-fashion-clip_Static_p10
  - P1_pyannote-speaker-diarization-3.1_segmentation-3.0
  - P1_pyannote-wespeaker-voxceleb-resnet34-LM-similar
  - P1_rizvandwiki-gender-classification
  - P1_sd1_5_text_encoder
  - P1_sd1_5_vae_decoder
  - P1_sd1_5_vae_encoder
  - P1_sentence_transformers_paraphrase_MiniLM_L6_v2
  - P1_sentence_transformers_paraphrase_multilingual_MiniLM_L12_v2
  - P1_text_encoder_ByteDance_AnimateDiff-Lightning
  - P1_vae_decoder_ByteDance_AnimateDiff-Lightning
  - P1_vae_encoder_ByteDance_AnimateDiff-Lightning
  - PSF_v1.1.0
  - PSMU_ST0_v3.8.0
  - PSMU_ST1_v3.8.0
  - PSU0_2.5
  - PSU1_2.5
  - PSU2_2.5
  - PSU_DT0_2.5
  - PSU_DT1_2.5
  - PSU_DT2_2.5
  - PSU_MF-model-1.0.1_decoder
  - PSU_MF-model-1.0.1_encoder
  - Phi-3.5-mini-instruct_qdq_context
  - Phi-3.5-mini-instruct_qdq_iterator
  - Phi-v-next-7B-4bit_psu0
  - Phi-v-next-7B-4bit_psu1
  - PhiVNext_7B_2bit_context
  - PhiVNext_7B_2bit_iterator
  - Phi_2bit
  - Qwen2.5-1.5B-Instruct_qdq_context
  - Qwen2.5-1.5B-Instruct_qdq_iterator
  - blip_image_encoder_au16w8_v0.2.2
  - blip_text_decoder_au16w8_v0.2.1
  - clip_16
  - clip_32
  - clip_laion
  - google_bert
  - llama2_7b_chat_hf_context
  - llama2_7b_chat_hf_iterator
  - llama3.1_8b_instr_context
  - llama3.1_8b_instr_iterator
  - psd1
  - psd2
  - psd3
  - psd3_new
  - psd4
  - psd5
  - psd5_all
  - psd6
  - psd7
  - psd8
  - psf_refresh_2.0.45
  - psh
  - psh_3.0
  - psh_refresh_2.0.9
  - psi_3.0
  - psi_refresh_3.0.0
  - psj
  - psj_refresh_3.0.0
  - psj_v3.0
  - psl
  - pso11
  - pso2_08_160_v2.3.1
  - pso2_08_320_v2.3.1
  - pso2_08_640_v2.3.1
  - pso2_08_80_v2.3.1
  - pso3
  - psu0
  - psu1
  - psw
  - psw_refresh_1.1.2
  - psy0
  - psy1
  - qwen3_0_6B_context
  - qwen3_0_6B_iterator
  - qwen_7b_vlm
  - sam2_image_decoder_au8w8_v0.2.2
  - sam2_image_encoder_au8w8_v0.2.1
  - unet
  - vit-base

Unsupported or Skipped models at generation time (when this file was generated):
  - P1_BAAI_bge_small_en_v1.5
  - P1_MahmoudAshraf-mms-300m-1130-forced-aligner
  - P1_Sentence_transformers_all_MiniLM_L12_v2
  - P1_bge-m3
  - P1_facebook-segmenet-anything-model-vision-encoder
  - P1_fasttext_direct_bagging_true_categorical_vocabulary_sizes
  - P1_fasttext_direct_bagging_true_categorical_vocabulary_sizes_None
  - P1_jonatasgrosman-wav2vec2-large-xlsr-53-chinese-zh-cn
  - P1_pyannote-speaker-diarization_part_1
  - P1_pyannote-speaker-diarization_part_3
  - P1_sentence_transformers_all_MiniLM_L6_v2
  - P1_sentence_transformers_multi_qa_MiniLM_L6_cos_v1
  - P1_theainerd-Wav2Vec2-large-xlsr-hindi
  - test_model

Supported operators at generation time (derived from OperatorsRegistry):
  - Add_qdq_BroadCast_int16xint16xint16
  - Add_qdq_BroadCast_int16xint16xint8
  - Add_qdq_BroadCast_int16xint16xuint16
  - Add_qdq_BroadCast_int16xint16xuint8
  - Add_qdq_BroadCast_int16xint8xint16
  - Add_qdq_BroadCast_int16xint8xint8
  - Add_qdq_BroadCast_int16xint8xuint16
  - Add_qdq_BroadCast_int16xint8xuint8
  - Add_qdq_BroadCast_int16xuint16xint16
  - Add_qdq_BroadCast_int16xuint16xint8
  - Add_qdq_BroadCast_int16xuint16xuint16
  - Add_qdq_BroadCast_int16xuint16xuint8
  - Add_qdq_BroadCast_int16xuint8xint16
  - Add_qdq_BroadCast_int16xuint8xint8
  - Add_qdq_BroadCast_int16xuint8xuint16
  - Add_qdq_BroadCast_int16xuint8xuint8
  - Add_qdq_BroadCast_int8xint16xint16
  - Add_qdq_BroadCast_int8xint16xint8
  - Add_qdq_BroadCast_int8xint16xuint16
  - Add_qdq_BroadCast_int8xint16xuint8
  - Add_qdq_BroadCast_int8xint8xint16
  - Add_qdq_BroadCast_int8xint8xint8
  - Add_qdq_BroadCast_int8xint8xuint16
  - Add_qdq_BroadCast_int8xint8xuint8
  - Add_qdq_BroadCast_int8xuint16xint16
  - Add_qdq_BroadCast_int8xuint16xint8
  - Add_qdq_BroadCast_int8xuint16xuint16
  - Add_qdq_BroadCast_int8xuint16xuint8
  - Add_qdq_BroadCast_int8xuint8xint16
  - Add_qdq_BroadCast_int8xuint8xint8
  - Add_qdq_BroadCast_int8xuint8xuint16
  - Add_qdq_BroadCast_int8xuint8xuint8
  - Add_qdq_BroadCast_uint16xint16xint16
  - Add_qdq_BroadCast_uint16xint16xint8
  - Add_qdq_BroadCast_uint16xint16xuint16
  - Add_qdq_BroadCast_uint16xint16xuint8
  - Add_qdq_BroadCast_uint16xint8xint16
  - Add_qdq_BroadCast_uint16xint8xint8
  - Add_qdq_BroadCast_uint16xint8xuint16
  - Add_qdq_BroadCast_uint16xint8xuint8
  - Add_qdq_BroadCast_uint16xuint16xint16
  - Add_qdq_BroadCast_uint16xuint16xint8
  - Add_qdq_BroadCast_uint16xuint16xuint16
  - Add_qdq_BroadCast_uint16xuint16xuint8
  - Add_qdq_BroadCast_uint16xuint8xint16
  - Add_qdq_BroadCast_uint16xuint8xint8
  - Add_qdq_BroadCast_uint16xuint8xuint16
  - Add_qdq_BroadCast_uint16xuint8xuint8
  - Add_qdq_BroadCast_uint8xint16xint16
  - Add_qdq_BroadCast_uint8xint16xint8
  - Add_qdq_BroadCast_uint8xint16xuint16
  - Add_qdq_BroadCast_uint8xint16xuint8
  - Add_qdq_BroadCast_uint8xint8xint16
  - Add_qdq_BroadCast_uint8xint8xint8
  - Add_qdq_BroadCast_uint8xint8xuint16
  - Add_qdq_BroadCast_uint8xint8xuint8
  - Add_qdq_BroadCast_uint8xuint16xint16
  - Add_qdq_BroadCast_uint8xuint16xint8
  - Add_qdq_BroadCast_uint8xuint16xuint16
  - Add_qdq_BroadCast_uint8xuint16xuint8
  - Add_qdq_BroadCast_uint8xuint8xint16
  - Add_qdq_BroadCast_uint8xuint8xint8
  - Add_qdq_BroadCast_uint8xuint8xuint16
  - Add_qdq_BroadCast_uint8xuint8xuint8
  - Add_qdq_EleWise_int16xint16xint16
  - Add_qdq_EleWise_int16xint16xint8
  - Add_qdq_EleWise_int16xint16xuint16
  - Add_qdq_EleWise_int16xint16xuint8
  - Add_qdq_EleWise_int16xint8xint16
  - Add_qdq_EleWise_int16xint8xint8
  - Add_qdq_EleWise_int16xint8xuint16
  - Add_qdq_EleWise_int16xint8xuint8
  - Add_qdq_EleWise_int16xuint16xint16
  - Add_qdq_EleWise_int16xuint16xint8
  - Add_qdq_EleWise_int16xuint16xuint16
  - Add_qdq_EleWise_int16xuint16xuint8
  - Add_qdq_EleWise_int16xuint8xint16
  - Add_qdq_EleWise_int16xuint8xint8
  - Add_qdq_EleWise_int16xuint8xuint16
  - Add_qdq_EleWise_int16xuint8xuint8
  - Add_qdq_EleWise_int8xint16xint16
  - Add_qdq_EleWise_int8xint16xint8
  - Add_qdq_EleWise_int8xint16xuint16
  - Add_qdq_EleWise_int8xint16xuint8
  - Add_qdq_EleWise_int8xint8xint16
  - Add_qdq_EleWise_int8xint8xint8
  - Add_qdq_EleWise_int8xint8xuint16
  - Add_qdq_EleWise_int8xint8xuint8
  - Add_qdq_EleWise_int8xuint16xint16
  - Add_qdq_EleWise_int8xuint16xint8
  - Add_qdq_EleWise_int8xuint16xuint16
  - Add_qdq_EleWise_int8xuint16xuint8
  - Add_qdq_EleWise_int8xuint8xint16
  - Add_qdq_EleWise_int8xuint8xint8
  - Add_qdq_EleWise_int8xuint8xuint16
  - Add_qdq_EleWise_int8xuint8xuint8
  - Add_qdq_EleWise_uint16xint16xint16
  - Add_qdq_EleWise_uint16xint16xint8
  - Add_qdq_EleWise_uint16xint16xuint16
  - Add_qdq_EleWise_uint16xint16xuint8
  - Add_qdq_EleWise_uint16xint8xint16
  - Add_qdq_EleWise_uint16xint8xint8
  - Add_qdq_EleWise_uint16xint8xuint16
  - Add_qdq_EleWise_uint16xint8xuint8
  - Add_qdq_EleWise_uint16xuint16xint16
  - Add_qdq_EleWise_uint16xuint16xint8
  - Add_qdq_EleWise_uint16xuint16xuint16
  - Add_qdq_EleWise_uint16xuint16xuint8
  - Add_qdq_EleWise_uint16xuint8xint16
  - Add_qdq_EleWise_uint16xuint8xint8
  - Add_qdq_EleWise_uint16xuint8xuint16
  - Add_qdq_EleWise_uint16xuint8xuint8
  - Add_qdq_EleWise_uint8xint16xint16
  - Add_qdq_EleWise_uint8xint16xint8
  - Add_qdq_EleWise_uint8xint16xuint16
  - Add_qdq_EleWise_uint8xint16xuint8
  - Add_qdq_EleWise_uint8xint8xint16
  - Add_qdq_EleWise_uint8xint8xint8
  - Add_qdq_EleWise_uint8xint8xuint16
  - Add_qdq_EleWise_uint8xint8xuint8
  - Add_qdq_EleWise_uint8xuint16xint16
  - Add_qdq_EleWise_uint8xuint16xint8
  - Add_qdq_EleWise_uint8xuint16xuint16
  - Add_qdq_EleWise_uint8xuint16xuint8
  - Add_qdq_EleWise_uint8xuint8xint16
  - Add_qdq_EleWise_uint8xuint8xint8
  - Add_qdq_EleWise_uint8xuint8xuint16
  - Add_qdq_EleWise_uint8xuint8xuint8
  - Concat_qdq_int16
  - Concat_qdq_int8
  - Concat_qdq_uint16
  - Concat_qdq_uint8
  - Conv
  - Conv_DWC
  - Conv_qdq_bias_int16xint16xint16
  - Conv_qdq_bias_int16xint8xint16
  - Conv_qdq_bias_int8xint8xint8
  - Conv_qdq_bias_uint16xint16xuint16
  - Conv_qdq_bias_uint16xint8xuint16
  - Conv_qdq_bias_uint16xuint16xuint16
  - Conv_qdq_bias_uint16xuint8xuint16
  - Conv_qdq_bias_uint8xint8xuint8
  - Conv_qdq_bias_uint8xuint8xuint8
  - Conv_qdq_biasrelu_uint16xint16xuint16
  - Conv_qdq_biasrelu_uint16xint8xuint16
  - Conv_qdq_biasrelu_uint16xuint16xuint16
  - Conv_qdq_biasrelu_uint16xuint8xuint16
  - Conv_qdq_int16xint16xint16
  - Conv_qdq_int16xint8xint16
  - Conv_qdq_int8xint8xint8
  - Conv_qdq_uint16xint16xuint16
  - Conv_qdq_uint16xint8xuint16
  - Conv_qdq_uint16xuint16xuint16
  - Conv_qdq_uint16xuint8xuint16
  - Conv_qdq_uint8xint8xuint8
  - Conv_qdq_uint8xuint8xuint8
  - Conversion_uint16xuint8
  - Conversion_uint8xuint16
  - Copy_uint16xuint16
  - Dequant_int16xbfloat16
  - Dequant_int16xfloat32
  - Dequant_int8xfloat32
  - Dequant_uint16xbfloat16
  - Dequant_uint16xfloat32
  - Dequant_uint8xfloat32
  - Div_qdq_BroadCast_int16xint16xint16
  - Div_qdq_BroadCast_int16xint16xint8
  - Div_qdq_BroadCast_int16xint16xuint16
  - Div_qdq_BroadCast_int16xint16xuint8
  - Div_qdq_BroadCast_int16xint8xint16
  - Div_qdq_BroadCast_int16xint8xint8
  - Div_qdq_BroadCast_int16xint8xuint16
  - Div_qdq_BroadCast_int16xint8xuint8
  - Div_qdq_BroadCast_int16xuint16xint16
  - Div_qdq_BroadCast_int16xuint16xint8
  - Div_qdq_BroadCast_int16xuint16xuint16
  - Div_qdq_BroadCast_int16xuint16xuint8
  - Div_qdq_BroadCast_int16xuint8xint16
  - Div_qdq_BroadCast_int16xuint8xint8
  - Div_qdq_BroadCast_int16xuint8xuint16
  - Div_qdq_BroadCast_int16xuint8xuint8
  - Div_qdq_BroadCast_int8xint16xint16
  - Div_qdq_BroadCast_int8xint16xint8
  - Div_qdq_BroadCast_int8xint16xuint16
  - Div_qdq_BroadCast_int8xint16xuint8
  - Div_qdq_BroadCast_int8xint8xint16
  - Div_qdq_BroadCast_int8xint8xint8
  - Div_qdq_BroadCast_int8xint8xuint16
  - Div_qdq_BroadCast_int8xint8xuint8
  - Div_qdq_BroadCast_int8xuint16xint16
  - Div_qdq_BroadCast_int8xuint16xint8
  - Div_qdq_BroadCast_int8xuint16xuint16
  - Div_qdq_BroadCast_int8xuint16xuint8
  - Div_qdq_BroadCast_int8xuint8xint16
  - Div_qdq_BroadCast_int8xuint8xint8
  - Div_qdq_BroadCast_int8xuint8xuint16
  - Div_qdq_BroadCast_int8xuint8xuint8
  - Div_qdq_BroadCast_uint16xint16xint16
  - Div_qdq_BroadCast_uint16xint16xint8
  - Div_qdq_BroadCast_uint16xint16xuint16
  - Div_qdq_BroadCast_uint16xint16xuint8
  - Div_qdq_BroadCast_uint16xint8xint16
  - Div_qdq_BroadCast_uint16xint8xint8
  - Div_qdq_BroadCast_uint16xint8xuint16
  - Div_qdq_BroadCast_uint16xint8xuint8
  - Div_qdq_BroadCast_uint16xuint16xint16
  - Div_qdq_BroadCast_uint16xuint16xint8
  - Div_qdq_BroadCast_uint16xuint16xuint16
  - Div_qdq_BroadCast_uint16xuint16xuint8
  - Div_qdq_BroadCast_uint16xuint8xint16
  - Div_qdq_BroadCast_uint16xuint8xint8
  - Div_qdq_BroadCast_uint16xuint8xuint16
  - Div_qdq_BroadCast_uint16xuint8xuint8
  - Div_qdq_BroadCast_uint8xint16xint16
  - Div_qdq_BroadCast_uint8xint16xint8
  - Div_qdq_BroadCast_uint8xint16xuint16
  - Div_qdq_BroadCast_uint8xint16xuint8
  - Div_qdq_BroadCast_uint8xint8xint16
  - Div_qdq_BroadCast_uint8xint8xint8
  - Div_qdq_BroadCast_uint8xint8xuint16
  - Div_qdq_BroadCast_uint8xint8xuint8
  - Div_qdq_BroadCast_uint8xuint16xint16
  - Div_qdq_BroadCast_uint8xuint16xint8
  - Div_qdq_BroadCast_uint8xuint16xuint16
  - Div_qdq_BroadCast_uint8xuint16xuint8
  - Div_qdq_BroadCast_uint8xuint8xint16
  - Div_qdq_BroadCast_uint8xuint8xint8
  - Div_qdq_BroadCast_uint8xuint8xuint16
  - Div_qdq_BroadCast_uint8xuint8xuint8
  - Div_qdq_EleWise_int16xint16xint16
  - Div_qdq_EleWise_int16xint16xint8
  - Div_qdq_EleWise_int16xint16xuint16
  - Div_qdq_EleWise_int16xint16xuint8
  - Div_qdq_EleWise_int16xint8xint16
  - Div_qdq_EleWise_int16xint8xint8
  - Div_qdq_EleWise_int16xint8xuint16
  - Div_qdq_EleWise_int16xint8xuint8
  - Div_qdq_EleWise_int16xuint16xint16
  - Div_qdq_EleWise_int16xuint16xint8
  - Div_qdq_EleWise_int16xuint16xuint16
  - Div_qdq_EleWise_int16xuint16xuint8
  - Div_qdq_EleWise_int16xuint8xint16
  - Div_qdq_EleWise_int16xuint8xint8
  - Div_qdq_EleWise_int16xuint8xuint16
  - Div_qdq_EleWise_int16xuint8xuint8
  - Div_qdq_EleWise_int8xint16xint16
  - Div_qdq_EleWise_int8xint16xint8
  - Div_qdq_EleWise_int8xint16xuint16
  - Div_qdq_EleWise_int8xint16xuint8
  - Div_qdq_EleWise_int8xint8xint16
  - Div_qdq_EleWise_int8xint8xint8
  - Div_qdq_EleWise_int8xint8xuint16
  - Div_qdq_EleWise_int8xint8xuint8
  - Div_qdq_EleWise_int8xuint16xint16
  - Div_qdq_EleWise_int8xuint16xint8
  - Div_qdq_EleWise_int8xuint16xuint16
  - Div_qdq_EleWise_int8xuint16xuint8
  - Div_qdq_EleWise_int8xuint8xint16
  - Div_qdq_EleWise_int8xuint8xint8
  - Div_qdq_EleWise_int8xuint8xuint16
  - Div_qdq_EleWise_int8xuint8xuint8
  - Div_qdq_EleWise_uint16xint16xint16
  - Div_qdq_EleWise_uint16xint16xint8
  - Div_qdq_EleWise_uint16xint16xuint16
  - Div_qdq_EleWise_uint16xint16xuint8
  - Div_qdq_EleWise_uint16xint8xint16
  - Div_qdq_EleWise_uint16xint8xint8
  - Div_qdq_EleWise_uint16xint8xuint16
  - Div_qdq_EleWise_uint16xint8xuint8
  - Div_qdq_EleWise_uint16xuint16xint16
  - Div_qdq_EleWise_uint16xuint16xint8
  - Div_qdq_EleWise_uint16xuint16xuint16
  - Div_qdq_EleWise_uint16xuint16xuint8
  - Div_qdq_EleWise_uint16xuint8xint16
  - Div_qdq_EleWise_uint16xuint8xint8
  - Div_qdq_EleWise_uint16xuint8xuint16
  - Div_qdq_EleWise_uint16xuint8xuint8
  - Div_qdq_EleWise_uint8xint16xint16
  - Div_qdq_EleWise_uint8xint16xint8
  - Div_qdq_EleWise_uint8xint16xuint16
  - Div_qdq_EleWise_uint8xint16xuint8
  - Div_qdq_EleWise_uint8xint8xint16
  - Div_qdq_EleWise_uint8xint8xint8
  - Div_qdq_EleWise_uint8xint8xuint16
  - Div_qdq_EleWise_uint8xint8xuint8
  - Div_qdq_EleWise_uint8xuint16xint16
  - Div_qdq_EleWise_uint8xuint16xint8
  - Div_qdq_EleWise_uint8xuint16xuint16
  - Div_qdq_EleWise_uint8xuint16xuint8
  - Div_qdq_EleWise_uint8xuint8xint16
  - Div_qdq_EleWise_uint8xuint8xint8
  - Div_qdq_EleWise_uint8xuint8xuint16
  - Div_qdq_EleWise_uint8xuint8xuint8
  - Elu_qdq_uint16xuint16
  - Expand_qdq_int16xint16
  - Expand_qdq_int8xint8
  - Expand_qdq_uint16xuint16
  - Expand_qdq_uint8xuint8
  - Fill_qdq_uint16xuint16
  - GatherElement_qdq_int16xint16
  - GatherElement_qdq_int8xint8
  - GatherElement_qdq_uint16xuint16
  - GatherElement_qdq_uint8xuint8
  - GatherElements_qdq_uint16xint64xuint16
  - Gather_element_qdq_int16xint16
  - Gather_element_qdq_int8xint8
  - Gather_element_qdq_uint16xuint16
  - Gather_element_qdq_uint8xuint8
  - Gather_qdq_int16xint64xint16
  - Gather_qdq_int8xint32xint8
  - Gather_qdq_int8xint64xint8
  - Gather_qdq_uint16xint64xuint16
  - Gather_qdq_uint8xint32xuint8
  - Gather_qdq_uint8xint64xuint8
  - Gelu_qdq_uint16xuint16
  - GlobalAveragePool_qdq_int16xint16
  - GlobalAveragePool_qdq_int8xint8
  - GlobalAveragePool_qdq_uint16xuint16
  - GlobalAveragePool_qdq_uint8xuint8
  - GroupNorm_qdq_uint16xint16xuint16
  - GroupNorm_qdq_uint16xuint16
  - GroupNorm_qdq_uint16xuint16xuint16
  - GroupNormalization_qdq_uint16xint16xuint16
  - GroupNormalization_qdq_uint16xuint16xuint16
  - GroupNormalization_qdq_uint16xuint8xuint16
  - LayerNorm_qdq_uint16xuint16
  - LayerNormalization_qdq_int16xint16xint16
  - LayerNormalization_qdq_int16xint8xint16
  - LayerNormalization_qdq_int16xuint16xint16
  - LayerNormalization_qdq_int16xuint8xint16
  - LayerNormalization_qdq_int8xint16xint8
  - LayerNormalization_qdq_int8xint8xint8
  - LayerNormalization_qdq_int8xuint16xint8
  - LayerNormalization_qdq_int8xuint8xint8
  - LayerNormalization_qdq_uint16xint16xuint16
  - LayerNormalization_qdq_uint16xint8xuint16
  - LayerNormalization_qdq_uint16xuint16xuint16
  - LayerNormalization_qdq_uint16xuint8xuint16
  - LayerNormalization_qdq_uint8xint16xuint8
  - LayerNormalization_qdq_uint8xint8xuint8
  - LayerNormalization_qdq_uint8xuint16xuint8
  - LayerNormalization_qdq_uint8xuint8xuint8
  - LpNormalization_qdq_int8xint8
  - LpNormalization_qdq_uint16xuint16
  - LpNormalization_qdq_uint8xuint8
  - MatMul_qdq_Transpose_int16xint8xint16
  - MatMul_qdq_Transpose_uint16xint8xuint16
  - MatMul_qdq_Transpose_uint16xuint8xuint16
  - MatMul_qdq_actxact_Transpose_int16xint16xint16
  - MatMul_qdq_actxact_Transpose_uint16xuint16xuint16
  - MatMul_qdq_actxact_int16xint16xint16
  - MatMul_qdq_actxact_int16xint8xint16
  - MatMul_qdq_actxact_int8xint8xint8
  - MatMul_qdq_actxact_uint16xint8xuint16
  - MatMul_qdq_actxact_uint16xuint16xuint16
  - MatMul_qdq_actxact_uint16xuint8xuint16
  - MatMul_qdq_actxact_uint8xuint8xuint8
  - MatMul_qdq_bias_int16xint2xint16
  - MatMul_qdq_bias_int16xint4xint16
  - MatMul_qdq_bias_int16xint8xint16
  - MatMul_qdq_bias_int16xuint2xint16
  - MatMul_qdq_bias_int8xint8xint8
  - MatMul_qdq_bias_int8xint8xuint8
  - MatMul_qdq_bias_int8xuint8xint8
  - MatMul_qdq_bias_int8xuint8xuint8
  - MatMul_qdq_bias_uint16xint2xuint16
  - MatMul_qdq_bias_uint16xint4xuint16
  - MatMul_qdq_bias_uint16xint8xuint16
  - MatMul_qdq_bias_uint16xuint16xuint16
  - MatMul_qdq_bias_uint16xuint2xuint16
  - MatMul_qdq_bias_uint16xuint4xuint16
  - MatMul_qdq_bias_uint16xuint8xuint16
  - MatMul_qdq_bias_uint8xint8xint8
  - MatMul_qdq_bias_uint8xint8xuint8
  - MatMul_qdq_bias_uint8xuint8xint8
  - MatMul_qdq_bias_uint8xuint8xuint8
  - MatMul_qdq_biasrelu_uint16xint4xuint16
  - MatMul_qdq_biasrelu_uint16xint8xuint16
  - MatMul_qdq_biasrelu_uint16xuint16xuint16
  - MatMul_qdq_biasrelu_uint16xuint4xuint16
  - MatMul_qdq_biasrelu_uint16xuint8xuint16
  - MatMul_qdq_int16xint2xint16
  - MatMul_qdq_int16xint4xint16
  - MatMul_qdq_int16xint8xint16
  - MatMul_qdq_int16xuint2xint16
  - MatMul_qdq_int8xint8xint8
  - MatMul_qdq_int8xint8xuint8
  - MatMul_qdq_int8xuint8xint8
  - MatMul_qdq_int8xuint8xuint8
  - MatMul_qdq_uint16xint2xuint16
  - MatMul_qdq_uint16xint4xuint16
  - MatMul_qdq_uint16xint8xuint16
  - MatMul_qdq_uint16xuint16xuint16
  - MatMul_qdq_uint16xuint2xuint16
  - MatMul_qdq_uint16xuint4xuint16
  - MatMul_qdq_uint16xuint8xuint16
  - MatMul_qdq_uint8xint8xint8
  - MatMul_qdq_uint8xint8xuint8
  - MatMul_qdq_uint8xuint8xint8
  - MatMul_qdq_uint8xuint8xuint8
  - MaxPool_qdq_int16xint16
  - MaxPool_qdq_int8xint8
  - MaxPool_qdq_uint16xuint16
  - MaxPool_qdq_uint8xuint8
  - Mul_qdq_BroadCast_int16xint16xint16
  - Mul_qdq_BroadCast_int16xint16xint8
  - Mul_qdq_BroadCast_int16xint16xuint16
  - Mul_qdq_BroadCast_int16xint16xuint8
  - Mul_qdq_BroadCast_int16xint8xint16
  - Mul_qdq_BroadCast_int16xint8xint8
  - Mul_qdq_BroadCast_int16xint8xuint16
  - Mul_qdq_BroadCast_int16xint8xuint8
  - Mul_qdq_BroadCast_int16xuint16xint16
  - Mul_qdq_BroadCast_int16xuint16xint8
  - Mul_qdq_BroadCast_int16xuint16xuint16
  - Mul_qdq_BroadCast_int16xuint16xuint8
  - Mul_qdq_BroadCast_int16xuint8xint16
  - Mul_qdq_BroadCast_int16xuint8xint8
  - Mul_qdq_BroadCast_int16xuint8xuint16
  - Mul_qdq_BroadCast_int16xuint8xuint8
  - Mul_qdq_BroadCast_int8xint16xint16
  - Mul_qdq_BroadCast_int8xint16xint8
  - Mul_qdq_BroadCast_int8xint16xuint16
  - Mul_qdq_BroadCast_int8xint16xuint8
  - Mul_qdq_BroadCast_int8xint8xint16
  - Mul_qdq_BroadCast_int8xint8xint8
  - Mul_qdq_BroadCast_int8xint8xuint16
  - Mul_qdq_BroadCast_int8xint8xuint8
  - Mul_qdq_BroadCast_int8xuint16xint16
  - Mul_qdq_BroadCast_int8xuint16xint8
  - Mul_qdq_BroadCast_int8xuint16xuint16
  - Mul_qdq_BroadCast_int8xuint16xuint8
  - Mul_qdq_BroadCast_int8xuint8xint16
  - Mul_qdq_BroadCast_int8xuint8xint8
  - Mul_qdq_BroadCast_int8xuint8xuint16
  - Mul_qdq_BroadCast_int8xuint8xuint8
  - Mul_qdq_BroadCast_uint16xint16xint16
  - Mul_qdq_BroadCast_uint16xint16xint8
  - Mul_qdq_BroadCast_uint16xint16xuint16
  - Mul_qdq_BroadCast_uint16xint16xuint8
  - Mul_qdq_BroadCast_uint16xint8xint16
  - Mul_qdq_BroadCast_uint16xint8xint8
  - Mul_qdq_BroadCast_uint16xint8xuint16
  - Mul_qdq_BroadCast_uint16xint8xuint8
  - Mul_qdq_BroadCast_uint16xuint16xint16
  - Mul_qdq_BroadCast_uint16xuint16xint8
  - Mul_qdq_BroadCast_uint16xuint16xuint16
  - Mul_qdq_BroadCast_uint16xuint16xuint8
  - Mul_qdq_BroadCast_uint16xuint8xint16
  - Mul_qdq_BroadCast_uint16xuint8xint8
  - Mul_qdq_BroadCast_uint16xuint8xuint16
  - Mul_qdq_BroadCast_uint16xuint8xuint8
  - Mul_qdq_BroadCast_uint8xint16xint16
  - Mul_qdq_BroadCast_uint8xint16xint8
  - Mul_qdq_BroadCast_uint8xint16xuint16
  - Mul_qdq_BroadCast_uint8xint16xuint8
  - Mul_qdq_BroadCast_uint8xint8xint16
  - Mul_qdq_BroadCast_uint8xint8xint8
  - Mul_qdq_BroadCast_uint8xint8xuint16
  - Mul_qdq_BroadCast_uint8xint8xuint8
  - Mul_qdq_BroadCast_uint8xuint16xint16
  - Mul_qdq_BroadCast_uint8xuint16xint8
  - Mul_qdq_BroadCast_uint8xuint16xuint16
  - Mul_qdq_BroadCast_uint8xuint16xuint8
  - Mul_qdq_BroadCast_uint8xuint8xint16
  - Mul_qdq_BroadCast_uint8xuint8xint8
  - Mul_qdq_BroadCast_uint8xuint8xuint16
  - Mul_qdq_BroadCast_uint8xuint8xuint8
  - Mul_qdq_EleWise_int16xint16xint16
  - Mul_qdq_EleWise_int16xint16xint8
  - Mul_qdq_EleWise_int16xint16xuint16
  - Mul_qdq_EleWise_int16xint16xuint8
  - Mul_qdq_EleWise_int16xint8xint16
  - Mul_qdq_EleWise_int16xint8xint8
  - Mul_qdq_EleWise_int16xint8xuint16
  - Mul_qdq_EleWise_int16xint8xuint8
  - Mul_qdq_EleWise_int16xuint16xint16
  - Mul_qdq_EleWise_int16xuint16xint8
  - Mul_qdq_EleWise_int16xuint16xuint16
  - Mul_qdq_EleWise_int16xuint16xuint8
  - Mul_qdq_EleWise_int16xuint8xint16
  - Mul_qdq_EleWise_int16xuint8xint8
  - Mul_qdq_EleWise_int16xuint8xuint16
  - Mul_qdq_EleWise_int16xuint8xuint8
  - Mul_qdq_EleWise_int8xint16xint16
  - Mul_qdq_EleWise_int8xint16xint8
  - Mul_qdq_EleWise_int8xint16xuint16
  - Mul_qdq_EleWise_int8xint16xuint8
  - Mul_qdq_EleWise_int8xint8xint16
  - Mul_qdq_EleWise_int8xint8xint8
  - Mul_qdq_EleWise_int8xint8xuint16
  - Mul_qdq_EleWise_int8xint8xuint8
  - Mul_qdq_EleWise_int8xuint16xint16
  - Mul_qdq_EleWise_int8xuint16xint8
  - Mul_qdq_EleWise_int8xuint16xuint16
  - Mul_qdq_EleWise_int8xuint16xuint8
  - Mul_qdq_EleWise_int8xuint8xint16
  - Mul_qdq_EleWise_int8xuint8xint8
  - Mul_qdq_EleWise_int8xuint8xuint16
  - Mul_qdq_EleWise_int8xuint8xuint8
  - Mul_qdq_EleWise_uint16xint16xint16
  - Mul_qdq_EleWise_uint16xint16xint8
  - Mul_qdq_EleWise_uint16xint16xuint16
  - Mul_qdq_EleWise_uint16xint16xuint8
  - Mul_qdq_EleWise_uint16xint8xint16
  - Mul_qdq_EleWise_uint16xint8xint8
  - Mul_qdq_EleWise_uint16xint8xuint16
  - Mul_qdq_EleWise_uint16xint8xuint8
  - Mul_qdq_EleWise_uint16xuint16xint16
  - Mul_qdq_EleWise_uint16xuint16xint8
  - Mul_qdq_EleWise_uint16xuint16xuint16
  - Mul_qdq_EleWise_uint16xuint16xuint8
  - Mul_qdq_EleWise_uint16xuint8xint16
  - Mul_qdq_EleWise_uint16xuint8xint8
  - Mul_qdq_EleWise_uint16xuint8xuint16
  - Mul_qdq_EleWise_uint16xuint8xuint8
  - Mul_qdq_EleWise_uint8xint16xint16
  - Mul_qdq_EleWise_uint8xint16xint8
  - Mul_qdq_EleWise_uint8xint16xuint16
  - Mul_qdq_EleWise_uint8xint16xuint8
  - Mul_qdq_EleWise_uint8xint8xint16
  - Mul_qdq_EleWise_uint8xint8xint8
  - Mul_qdq_EleWise_uint8xint8xuint16
  - Mul_qdq_EleWise_uint8xint8xuint8
  - Mul_qdq_EleWise_uint8xuint16xint16
  - Mul_qdq_EleWise_uint8xuint16xint8
  - Mul_qdq_EleWise_uint8xuint16xuint16
  - Mul_qdq_EleWise_uint8xuint16xuint8
  - Mul_qdq_EleWise_uint8xuint8xint16
  - Mul_qdq_EleWise_uint8xuint8xint8
  - Mul_qdq_EleWise_uint8xuint8xuint16
  - Mul_qdq_EleWise_uint8xuint8xuint8
  - PWLA_qdq_int8xint8
  - PWLA_qdq_uint16xuint16
  - PWLA_qdq_uint8xuint8
  - Pad_qdq_int16xint16
  - Pad_qdq_int8xint8
  - Pad_qdq_uint16xuint16
  - Pad_qdq_uint8xuint8
  - Quant_bfloat16xint16
  - Quant_bfloat16xuint16
  - Quant_float32xint16
  - Quant_float32xint8
  - Quant_float32xuint16
  - Quant_float32xuint8
  - Reduce_qdq_int16xint16
  - Reduce_qdq_int8xint8
  - Reduce_qdq_uint16xuint16
  - Reduce_qdq_uint8xuint8
  - Relu_qdq_uint16xuint16
  - Reshape_qdq_int16xint16
  - Reshape_qdq_int8xint8
  - Reshape_qdq_uint16xuint16
  - Reshape_qdq_uint8xuint8
  - Resize_qdq_uint16xuint16
  - RoPE_actxact_qdq_int16xint16
  - RoPE_actxact_qdq_int16xint16xint16
  - RoPE_actxact_qdq_int16xint16xuint16
  - RoPE_actxact_qdq_int16xuint16
  - RoPE_actxact_qdq_int16xuint16xint16
  - RoPE_actxact_qdq_int16xuint16xuint16
  - RoPE_actxact_qdq_uint16xint16
  - RoPE_actxact_qdq_uint16xint16xint16
  - RoPE_actxact_qdq_uint16xint16xuint16
  - RoPE_actxact_qdq_uint16xuint16
  - RoPE_actxact_qdq_uint16xuint16xint16
  - RoPE_actxact_qdq_uint16xuint16xuint16
  - RoPE_qdq_int16xint16
  - RoPE_qdq_int16xint16xint16
  - RoPE_qdq_int16xint16xuint16
  - RoPE_qdq_int16xuint16
  - RoPE_qdq_int16xuint16xint16
  - RoPE_qdq_int16xuint16xuint16
  - RoPE_qdq_uint16xint16
  - RoPE_qdq_uint16xint16xint16
  - RoPE_qdq_uint16xint16xuint16
  - RoPE_qdq_uint16xuint16
  - RoPE_qdq_uint16xuint16xint16
  - RoPE_qdq_uint16xuint16xuint16
  - Scatter_element_qdq_bfloat16
  - Scatter_element_qdq_int16
  - Scatter_element_qdq_int16xint16
  - Scatter_element_qdq_int8
  - Scatter_element_qdq_int8xint8
  - Scatter_element_qdq_uint16
  - Scatter_element_qdq_uint16xuint16
  - Scatter_element_qdq_uint8
  - Scatter_element_qdq_uint8xuint8
  - Sigmoid_qdq_uint16xuint16
  - Silu_qdq_uint16xuint16
  - Slice_qdq_int16xint16
  - Slice_qdq_int8xint8
  - Slice_qdq_uint16xuint16
  - Slice_qdq_uint8xuint8
  - Softmax_qdq_int8xuint8
  - Softmax_qdq_uint16xuint16
  - Softmax_qdq_uint8xuint8
  - Sub_qdq_BroadCast_int16xint16xint16
  - Sub_qdq_BroadCast_int16xint16xint8
  - Sub_qdq_BroadCast_int16xint16xuint16
  - Sub_qdq_BroadCast_int16xint16xuint8
  - Sub_qdq_BroadCast_int16xint8xint16
  - Sub_qdq_BroadCast_int16xint8xint8
  - Sub_qdq_BroadCast_int16xint8xuint16
  - Sub_qdq_BroadCast_int16xint8xuint8
  - Sub_qdq_BroadCast_int16xuint16xint16
  - Sub_qdq_BroadCast_int16xuint16xint8
  - Sub_qdq_BroadCast_int16xuint16xuint16
  - Sub_qdq_BroadCast_int16xuint16xuint8
  - Sub_qdq_BroadCast_int16xuint8xint16
  - Sub_qdq_BroadCast_int16xuint8xint8
  - Sub_qdq_BroadCast_int16xuint8xuint16
  - Sub_qdq_BroadCast_int16xuint8xuint8
  - Sub_qdq_BroadCast_int8xint16xint16
  - Sub_qdq_BroadCast_int8xint16xint8
  - Sub_qdq_BroadCast_int8xint16xuint16
  - Sub_qdq_BroadCast_int8xint16xuint8
  - Sub_qdq_BroadCast_int8xint8xint16
  - Sub_qdq_BroadCast_int8xint8xint8
  - Sub_qdq_BroadCast_int8xint8xuint16
  - Sub_qdq_BroadCast_int8xint8xuint8
  - Sub_qdq_BroadCast_int8xuint16xint16
  - Sub_qdq_BroadCast_int8xuint16xint8
  - Sub_qdq_BroadCast_int8xuint16xuint16
  - Sub_qdq_BroadCast_int8xuint16xuint8
  - Sub_qdq_BroadCast_int8xuint8xint16
  - Sub_qdq_BroadCast_int8xuint8xint8
  - Sub_qdq_BroadCast_int8xuint8xuint16
  - Sub_qdq_BroadCast_int8xuint8xuint8
  - Sub_qdq_BroadCast_uint16xint16xint16
  - Sub_qdq_BroadCast_uint16xint16xint8
  - Sub_qdq_BroadCast_uint16xint16xuint16
  - Sub_qdq_BroadCast_uint16xint16xuint8
  - Sub_qdq_BroadCast_uint16xint8xint16
  - Sub_qdq_BroadCast_uint16xint8xint8
  - Sub_qdq_BroadCast_uint16xint8xuint16
  - Sub_qdq_BroadCast_uint16xint8xuint8
  - Sub_qdq_BroadCast_uint16xuint16xint16
  - Sub_qdq_BroadCast_uint16xuint16xint8
  - Sub_qdq_BroadCast_uint16xuint16xuint16
  - Sub_qdq_BroadCast_uint16xuint16xuint8
  - Sub_qdq_BroadCast_uint16xuint8xint16
  - Sub_qdq_BroadCast_uint16xuint8xint8
  - Sub_qdq_BroadCast_uint16xuint8xuint16
  - Sub_qdq_BroadCast_uint16xuint8xuint8
  - Sub_qdq_BroadCast_uint8xint16xint16
  - Sub_qdq_BroadCast_uint8xint16xint8
  - Sub_qdq_BroadCast_uint8xint16xuint16
  - Sub_qdq_BroadCast_uint8xint16xuint8
  - Sub_qdq_BroadCast_uint8xint8xint16
  - Sub_qdq_BroadCast_uint8xint8xint8
  - Sub_qdq_BroadCast_uint8xint8xuint16
  - Sub_qdq_BroadCast_uint8xint8xuint8
  - Sub_qdq_BroadCast_uint8xuint16xint16
  - Sub_qdq_BroadCast_uint8xuint16xint8
  - Sub_qdq_BroadCast_uint8xuint16xuint16
  - Sub_qdq_BroadCast_uint8xuint16xuint8
  - Sub_qdq_BroadCast_uint8xuint8xint16
  - Sub_qdq_BroadCast_uint8xuint8xint8
  - Sub_qdq_BroadCast_uint8xuint8xuint16
  - Sub_qdq_BroadCast_uint8xuint8xuint8
  - Sub_qdq_EleWise_int16xint16xint16
  - Sub_qdq_EleWise_int16xint16xint8
  - Sub_qdq_EleWise_int16xint16xuint16
  - Sub_qdq_EleWise_int16xint16xuint8
  - Sub_qdq_EleWise_int16xint8xint16
  - Sub_qdq_EleWise_int16xint8xint8
  - Sub_qdq_EleWise_int16xint8xuint16
  - Sub_qdq_EleWise_int16xint8xuint8
  - Sub_qdq_EleWise_int16xuint16xint16
  - Sub_qdq_EleWise_int16xuint16xint8
  - Sub_qdq_EleWise_int16xuint16xuint16
  - Sub_qdq_EleWise_int16xuint16xuint8
  - Sub_qdq_EleWise_int16xuint8xint16
  - Sub_qdq_EleWise_int16xuint8xint8
  - Sub_qdq_EleWise_int16xuint8xuint16
  - Sub_qdq_EleWise_int16xuint8xuint8
  - Sub_qdq_EleWise_int8xint16xint16
  - Sub_qdq_EleWise_int8xint16xint8
  - Sub_qdq_EleWise_int8xint16xuint16
  - Sub_qdq_EleWise_int8xint16xuint8
  - Sub_qdq_EleWise_int8xint8xint16
  - Sub_qdq_EleWise_int8xint8xint8
  - Sub_qdq_EleWise_int8xint8xuint16
  - Sub_qdq_EleWise_int8xint8xuint8
  - Sub_qdq_EleWise_int8xuint16xint16
  - Sub_qdq_EleWise_int8xuint16xint8
  - Sub_qdq_EleWise_int8xuint16xuint16
  - Sub_qdq_EleWise_int8xuint16xuint8
  - Sub_qdq_EleWise_int8xuint8xint16
  - Sub_qdq_EleWise_int8xuint8xint8
  - Sub_qdq_EleWise_int8xuint8xuint16
  - Sub_qdq_EleWise_int8xuint8xuint8
  - Sub_qdq_EleWise_uint16xint16xint16
  - Sub_qdq_EleWise_uint16xint16xint8
  - Sub_qdq_EleWise_uint16xint16xuint16
  - Sub_qdq_EleWise_uint16xint16xuint8
  - Sub_qdq_EleWise_uint16xint8xint16
  - Sub_qdq_EleWise_uint16xint8xint8
  - Sub_qdq_EleWise_uint16xint8xuint16
  - Sub_qdq_EleWise_uint16xint8xuint8
  - Sub_qdq_EleWise_uint16xuint16xint16
  - Sub_qdq_EleWise_uint16xuint16xint8
  - Sub_qdq_EleWise_uint16xuint16xuint16
  - Sub_qdq_EleWise_uint16xuint16xuint8
  - Sub_qdq_EleWise_uint16xuint8xint16
  - Sub_qdq_EleWise_uint16xuint8xint8
  - Sub_qdq_EleWise_uint16xuint8xuint16
  - Sub_qdq_EleWise_uint16xuint8xuint8
  - Sub_qdq_EleWise_uint8xint16xint16
  - Sub_qdq_EleWise_uint8xint16xint8
  - Sub_qdq_EleWise_uint8xint16xuint16
  - Sub_qdq_EleWise_uint8xint16xuint8
  - Sub_qdq_EleWise_uint8xint8xint16
  - Sub_qdq_EleWise_uint8xint8xint8
  - Sub_qdq_EleWise_uint8xint8xuint16
  - Sub_qdq_EleWise_uint8xint8xuint8
  - Sub_qdq_EleWise_uint8xuint16xint16
  - Sub_qdq_EleWise_uint8xuint16xint8
  - Sub_qdq_EleWise_uint8xuint16xuint16
  - Sub_qdq_EleWise_uint8xuint16xuint8
  - Sub_qdq_EleWise_uint8xuint8xint16
  - Sub_qdq_EleWise_uint8xuint8xint8
  - Sub_qdq_EleWise_uint8xuint8xuint16
  - Sub_qdq_EleWise_uint8xuint8xuint8
  - Swish_qdq_uint16xuint16
  - Tanh_qdq_uint16xuint16
  - TopK_qdq_int16xint16
  - TopK_qdq_int8xint8
  - TopK_qdq_uint16xuint16
  - TopK_qdq_uint8xuint8
  - Transpose_qdq_int16xint16
  - Transpose_qdq_int8xint8
  - Transpose_qdq_uint16xuint16
  - Transpose_qdq_uint8xuint8
  - conv_noqdq_a8w8
  - gap
  - gap_a16
  - maxpool_noqdq_a16
  - maxpool_noqdq_a8
  - pdi
  - reduce
  - reduce_a16

To retrieve the latest operator list, call:
  OperatorsRegistry.get_operators()
"""

pdi_variants = {
    "pdi_combination_0": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_conversion_fp16x16": 36,
            "run_dequant_int8": 34,
            "run_gemm_a8a8_transpose": 40,
            "run_gemm_int8x8": 5,
            "run_generic_reduce_int8x8": 45,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_quant_int8": 35,
            "run_reduce_all_int8x8": 53,
            "run_reshape_a8": 52,
            "run_slice_a8": 28,
            "run_softmax_fp16x16": 6,
            "run_transpose_a8": 26,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "conversion/conversion_fp16x16_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int8x8/gemm_int8x8_wrapper.cc",
            "gemm_qdq_int8x8_transpose/gemm_a8a8_transpose_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "reshape/wrapper_reshape.cc",
            "slice/wrapper_slice.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_1": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_expand": 57,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_reduce_all_int16x16": 54,
            "run_reshape": 51,
            "run_slice": 27,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "expand/wrapper_expand.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "reshape/wrapper_reshape.cc",
            "slice/wrapper_slice.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_2": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_concat": 23,
            "run_dequant": 9,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_l2norm_fp16x16": 7,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_rope_16": 33,
            "run_slice": 27,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastdiv_wrapper.cc",
            "broadcast/run_rope_wrapper.cc",
            "concat/wrapper_concat.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "l2norm_fp16x16/l2norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "slice/wrapper_slice.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_3": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_concat": 23,
            "run_dequant": 9,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_l2norm_fp16x16": 7,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_reshape": 51,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "concat/wrapper_concat.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "l2norm_fp16x16/l2norm_fp16x16_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "reshape/wrapper_reshape.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_4": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_concat": 23,
            "run_dequant": 9,
            "run_expand": 57,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_quant": 10,
            "run_reduce_all_int16x16": 54,
            "run_slice": 27,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "concat/wrapper_concat.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "expand/wrapper_expand.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "slice/wrapper_slice.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_5": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_concat": 23,
            "run_dequant": 9,
            "run_expand": 57,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_reduce_all_int16x16": 54,
            "run_slice": 27,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "concat/wrapper_concat.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "expand/wrapper_expand.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "slice/wrapper_slice.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_6": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_concat": 23,
            "run_dequant": 9,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_layernorm_fp16x16": 13,
            "run_quant": 10,
            "run_reduce_all_int16x16": 54,
            "run_slice": 27,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "concat/wrapper_concat.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "slice/wrapper_slice.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_7": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_dequant": 9,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_reduce_all_int16x16": 54,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_8": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_dequant": 9,
            "run_gather_64x16": 42,
            "run_gather_64x8": 43,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_reshape": 51,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gather/wrapper_gather.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "reshape/wrapper_reshape.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_9": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_concat": 23,
            "run_dequant": 9,
            "run_expand": 57,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_layernorm_fp16x16": 13,
            "run_reduce_all_int16x16": 54,
            "run_slice": 27,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "concat/wrapper_concat.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "expand/wrapper_expand.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "q/q_impl.hpp",
            "slice/wrapper_slice.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_10": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_concat_a8": 24,
            "run_dequant_int8": 34,
            "run_gemm_a8a8_transpose": 40,
            "run_gemm_int8x8": 5,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_quant_int8": 35,
            "run_reshape_a8": 52,
            "run_softmax_fp16x16": 6,
            "run_transpose_a8": 26,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "concat/wrapper_concat.cc",
            "dq/dq.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int8x8/gemm_int8x8_wrapper.cc",
            "gemm_qdq_int8x8_transpose/gemm_a8a8_transpose_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_wrapper.cc",
            "reshape/wrapper_reshape.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_11": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_dequant": 9,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_l2norm_fp16x16": 7,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_rope_16": 33,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastdiv_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "broadcast/run_rope_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "l2norm_fp16x16/l2norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_12": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_concat": 23,
            "run_conv_qdq_a16w8": 14,
            "run_dequant": 9,
            "run_gemm_int16x8": 38,
            "run_layernorm_fp16x16": 13,
            "run_quant": 10,
            "run_reshape": 51,
            "run_slice": 27,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "concat/wrapper_concat.cc",
            "conv_qdq_int16x8/conv_qdq_a16w8_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "reshape/wrapper_reshape.cc",
            "slice/wrapper_slice.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_13": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_concat": 23,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x2to4_block": 56,
            "run_l2norm_fp16x16": 7,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_slice": 27,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "concat/wrapper_concat.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "gemm_int16x2to4_block/gemm_int16x2to4_block_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "l2norm_fp16x16/l2norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "slice/wrapper_slice.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_14": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_dequant": 9,
            "run_gather_element": 47,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gather_element/wrapper_gather_element.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_15": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_concat": 23,
            "run_dequant": 9,
            "run_gather_64x16": 42,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_slice": 27,
            "run_softmax_fp16x16": 6,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "concat/wrapper_concat.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gather/wrapper_gather.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "slice/wrapper_slice.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
        ],
    },
    "pdi_combination_16": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_concat": 23,
            "run_conv_qdq_a16w8": 14,
            "run_dequant": 9,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_reduce_all_int16x16": 54,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "concat/wrapper_concat.cc",
            "conv_qdq_int16x8/conv_qdq_a16w8_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_17": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_dequant": 9,
            "run_expand": 57,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_layernorm_fp16x16": 13,
            "run_reduce_all_int16x16": 54,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "expand/wrapper_expand.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_18": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_reduce_all_int16x16": 54,
            "run_reshape": 51,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "reshape/wrapper_reshape.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_19": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_reduce_all_int16x16": 54,
            "run_slice": 27,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "slice/wrapper_slice.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_20": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_lut_fp16x16": 19,
            "run_reduce_all_int16x16": 54,
            "run_reshape": 51,
            "run_slice": 27,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "reshape/wrapper_reshape.cc",
            "slice/wrapper_slice.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_21": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_reduce_all_int16x16": 54,
            "run_reshape": 51,
            "run_slice": 27,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "reshape/wrapper_reshape.cc",
            "slice/wrapper_slice.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_22": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_dequant_int8": 34,
            "run_gemm_a8a8_transpose": 40,
            "run_gemm_int8x8": 5,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_maxpool_int8x8": 1,
            "run_pad_a8": 61,
            "run_slice_a8": 28,
            "run_softmax_fp16x16": 6,
            "run_transpose_a8": 26,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int8x8/gemm_int8x8_wrapper.cc",
            "gemm_qdq_int8x8_transpose/gemm_a8a8_transpose_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "maxpool/maxpool_wrapper.cc",
            "pad/wrapper_pad.cc",
            "q/q.hpp",
            "slice/wrapper_slice.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_23": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_concat": 23,
            "run_dequant": 9,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_layernorm_fp16x16": 13,
            "run_slice": 27,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "concat/wrapper_concat.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "slice/wrapper_slice.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_24": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_dequant": 9,
            "run_gather_64x16": 42,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gather/wrapper_gather.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_25": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_dequant": 9,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_group_norm_qdq": 21,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "groupnorm/norm.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_26": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_dequant": 9,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_l2norm_fp16x16": 7,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "l2norm_fp16x16/l2norm_fp16x16_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_27": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_dequant": 9,
            "run_gemm_int16x4": 4,
            "run_gemm_int16x8": 38,
            "run_gemm_int16x8_transpose": 39,
            "run_l2norm_fp16x16": 7,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_slice": 27,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x4/gemm_int16x4_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "gemm_qdq_int16x8_transpose/gemm_int16x8_transpose_wrapper.cc",
            "l2norm_fp16x16/l2norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "slice/wrapper_slice.cc",
        ],
    },
    "pdi_combination_28": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_reshape": 51,
            "run_slice": 27,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "reshape/wrapper_reshape.cc",
            "slice/wrapper_slice.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_29": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastdiv_16": 22,
            "run_bdcastmul_16": 17,
            "run_dequant": 9,
            "run_gather_64x16": 42,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_reduce_all_int16x16": 54,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastdiv_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gather/wrapper_gather.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
        ],
    },
    "pdi_combination_30": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastdiv_16": 22,
            "run_dequant": 9,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_quant": 10,
            "run_reduce_all_int16x16": 54,
            "run_reshape": 51,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastdiv_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "reshape/wrapper_reshape.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_31": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_dequant": 9,
            "run_expand": 57,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_quant": 10,
            "run_reduce_all_int16x16": 54,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "expand/wrapper_expand.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_32": {
        "combined_kernel_names": {
            "run_bdcastmul_16": 17,
            "run_conv_qdq_a16w8": 14,
            "run_dequant": 9,
            "run_gemm_int16x8": 38,
            "run_generic_reduce_int16x16": 46,
            "run_maxpool_int16x16": 31,
            "run_quant": 10,
            "run_reduce_all_int16x16": 54,
            "run_reshape": 51,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastmul_wrapper.cc",
            "conv_qdq_int16x8/conv_qdq_a16w8_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "maxpool/maxpool_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "reshape/wrapper_reshape.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_33": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastdiv_16": 22,
            "run_bdcastmul_16": 17,
            "run_dequant": 9,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_quant": 10,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastdiv_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_34": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_concat": 23,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "concat/wrapper_concat.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_35": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_conv_qdq_a16w8": 14,
            "run_dequant": 9,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_group_norm_qdq": 21,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_softmax_fp16x16": 6,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "conv_qdq_int16x8/conv_qdq_a16w8_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "groupnorm/norm.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
        ],
    },
    "pdi_combination_36": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_conv_qdq_a16w8": 14,
            "run_dequant": 9,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "conv_qdq_int16x8/conv_qdq_a16w8_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_37": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_conv_qdq_a16w8": 14,
            "run_dequant": 9,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_layernorm_fp16x16": 13,
            "run_quant": 10,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "conv_qdq_int16x8/conv_qdq_a16w8_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_38": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_conv_qdq_a16w8": 14,
            "run_dequant": 9,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "conv_qdq_int16x8/conv_qdq_a16w8_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_39": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_conv_qdq_a16w8": 14,
            "run_dequant": 9,
            "run_gemm_int16x8": 38,
            "run_group_norm_qdq": 21,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "conv_qdq_int16x8/conv_qdq_a16w8_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "groupnorm/norm.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_40": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_conv_qdq_a16w8": 14,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "conv_qdq_int16x8/conv_qdq_a16w8_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_41": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_conv_qdq_a8w8": 32,
            "run_gemm_a8a8_transpose": 40,
            "run_gemm_int8x8": 5,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_quant_int8": 35,
            "run_softmax_fp16x16": 6,
            "run_transpose_a8": 26,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "conv_qdq_int8x8/conv_qdq_a8w8_wrapper.cc",
            "dq/dq.hpp",
            "gemm_qdq_int8x8/gemm_int8x8_wrapper.cc",
            "gemm_qdq_int8x8_transpose/gemm_a8a8_transpose_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_wrapper.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_42": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastdiv_16": 22,
            "run_bdcastmul_16": 17,
            "run_dequant_int8": 34,
            "run_gemm_int8x8": 5,
            "run_generic_reduce_int8x8": 45,
            "run_lut_fp16x16": 19,
            "run_quant_int8": 35,
            "run_reduce_all_int8x8": 53,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastdiv_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int8x8/gemm_int8x8_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_wrapper.cc",
        ],
    },
    "pdi_combination_43": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastdiv_16": 22,
            "run_dequant": 9,
            "run_gemm_int16x8": 38,
            "run_l2norm_fp16x16": 7,
            "run_lut_fp16x16": 19,
            "run_slice": 27,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastdiv_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "l2norm_fp16x16/l2norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "slice/wrapper_slice.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_44": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_concat": 23,
            "run_dequant": 9,
            "run_gemm_int16x4": 4,
            "run_l2norm_fp16x16": 7,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "concat/wrapper_concat.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x4/gemm_int16x4_wrapper.cc",
            "l2norm_fp16x16/l2norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
        ],
    },
    "pdi_combination_45": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_concat": 23,
            "run_dequant": 9,
            "run_gemm_int16x8": 38,
            "run_l2norm_fp16x16": 7,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "concat/wrapper_concat.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "l2norm_fp16x16/l2norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
        ],
    },
    "pdi_combination_46": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastmul_16": 17,
            "run_dequant": 9,
            "run_gemm_int16x2to4_block": 56,
            "run_l2norm_fp16x16": 7,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_slice": 27,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_int16x2to4_block/gemm_int16x2to4_block_wrapper.cc",
            "l2norm_fp16x16/l2norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "slice/wrapper_slice.cc",
        ],
    },
    "pdi_combination_47": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_concat": 23,
            "run_conv_qdq_a16w8": 14,
            "run_gemm_int16x8": 38,
            "run_group_norm_qdq": 21,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "concat/wrapper_concat.cc",
            "conv_qdq_int16x8/conv_qdq_a16w8_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "groupnorm/norm.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_48": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_conv_qdq_a16w8": 14,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_group_norm_qdq": 21,
            "run_layernorm_fp16x16": 13,
            "run_lut_fp16x16": 19,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "conv_qdq_int16x8/conv_qdq_a16w8_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "groupnorm/norm.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_49": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_conv_qdq_a16w8": 14,
            "run_gemm_int16x16_transpose": 15,
            "run_gemm_int16x8": 38,
            "run_group_norm_qdq": 21,
            "run_lut_fp16x16": 19,
            "run_quant": 10,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "conv_qdq_int16x8/conv_qdq_a16w8_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "groupnorm/norm.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_50": {
        "combined_kernel_names": {
            "run_conv_qdq_a16w16": 59,
            "run_dequant": 9,
            "run_gemm_a16w16_transpose": 64,
            "run_maxpool_int16x16": 31,
            "run_quant": 10,
            "run_reshape": 51,
            "run_softmax_fp16x16": 6,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "conv_qdq_int16x16/conv_qdq_a16w16_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_a16w16_transpose_wrapper.cc",
            "maxpool/maxpool_wrapper.cc",
            "q/q.hpp",
            "q/q_wrapper.cc",
            "reshape/wrapper_reshape.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_51": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_bdcastdiv_16": 22,
            "run_conv_qdq_a16w8": 14,
            "run_lut_fp16x16": 19,
            "run_maxpool_int16x16": 31,
            "run_quant": 10,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastdiv_wrapper.cc",
            "conv_qdq_int16x8/conv_qdq_a16w8_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "maxpool/maxpool_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_52": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_concat": 23,
            "run_conv_qdq_a16w8": 14,
            "run_dequant": 9,
            "run_gemm_int16x8": 38,
            "run_group_norm_qdq": 21,
            "run_lut_fp16x16": 19,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "concat/wrapper_concat.cc",
            "conv_qdq_int16x8/conv_qdq_a16w8_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "groupnorm/norm.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
        ],
    },
    "pdi_combination_53": {
        "combined_kernel_names": {
            "run_conversion_fp16x16": 36,
            "run_dequant": 9,
            "run_gemm_int16x8": 38,
            "run_globalavgpool_int16x16": 37,
            "run_l2norm_fp16x16": 7,
            "run_layernorm_fp16x16": 13,
            "run_transpose": 25,
        },
        "combined_kernel_includes": [
            "super.hh",
            "conversion/conversion_fp16x16_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_wrapper.cc",
            "gap/globalavgpool_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "l2norm_fp16x16/l2norm_fp16x16_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "q/q.hpp",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_54": {
        "combined_kernel_names": {
            "run_bdcastadd_16": 16,
            "run_dequant": 9,
            "run_gather_64x8": 43,
            "run_generic_reduce_int16x16": 46,
            "run_reduce_all_int16x16": 54,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "gather/wrapper_gather.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "q/q_impl.hpp",
        ],
    },
    "pdi_combination_55": {
        "combined_kernel_names": {
            "run_conv_qdq_a8w8": 32,
            "run_dequant_int8": 34,
            "run_quant_int8": 35,
            "run_transpose_a8": 26,
        },
        "combined_kernel_includes": [
            "super.hh",
            "conv_qdq_int8x8/conv_qdq_a8w8_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_wrapper.cc",
            "q/q.hpp",
            "q/q_wrapper.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_56": {
        "combined_kernel_names": {
            "run_dequant_int8": 34,
            "run_gather_64x8": 43,
        },
        "combined_kernel_includes": [
            "super.hh",
            "dq/dq.hpp",
            "dq/dq_wrapper.cc",
            "gather/wrapper_gather.cc",
        ],
    },
    "pdi_combination_57": {
        "combined_kernel_names": {
            "run_conv_noqdq_a8w8": 0,
            "run_copy_fp16x16": 8,
            "run_dwc_qdq_a16w8": 20,
            "run_expand_a8": 58,
            "run_fill_int16x16": 55,
            "run_gather_32x8": 44,
            "run_gather_element_a8": 50,
            "run_globalavgpool_int8x8": 3,
            "run_pad": 60,
            "run_scatter_element": 48,
            "run_scatter_element_a8": 49,
            "run_topk": 62,
            "run_topk_a8": 63,
        },
        "combined_kernel_includes": [
            "super.hh",
            "broadcast/run_bdcastadd_wrapper.cc",
            "broadcast/run_bdcastdiv_wrapper.cc",
            "broadcast/run_bdcastmul_wrapper.cc",
            "broadcast/run_rope_wrapper.cc",
            "concat/wrapper_concat.cc",
            "conv/conv_noqdq_a8w8_wrapper.cc",
            "conv_qdq_int16x16/conv_qdq_a16w16_wrapper.cc",
            "conv_qdq_int16x8/conv_qdq_a16w8_wrapper.cc",
            "conv_qdq_int8x8/conv_qdq_a8w8_wrapper.cc",
            "conversion/conversion_fp16x16_wrapper.cc",
            "dq/dq.hpp",
            "dq/dq_impl.hpp",
            "dq/dq_wrapper.cc",
            "dwc_int16x8/dwc_qdq_a16w8_wrapper.cc",
            "expand/wrapper_expand.cc",
            "fill/fill_wrapper.cc",
            "gap/globalavgpool_wrapper.cc",
            "gather/wrapper_gather.cc",
            "gather_element/wrapper_gather_element.cc",
            "gemm_int16x2to4_block/gemm_int16x2to4_block_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_a16w16_transpose_wrapper.cc",
            "gemm_qdq_int16x16_transpose/gemm_int16x16_transpose_wrapper.cc",
            "gemm_qdq_int16x4/gemm_int16x4_wrapper.cc",
            "gemm_qdq_int16x8/gemm_int16x8_wrapper.cc",
            "gemm_qdq_int16x8_transpose/gemm_int16x8_transpose_wrapper.cc",
            "gemm_qdq_int8x8/gemm_int8x8_wrapper.cc",
            "gemm_qdq_int8x8_transpose/gemm_a8a8_transpose_wrapper.cc",
            "generic_reduce_bf16/reduce_wrapper.cc",
            "groupnorm/norm.cc",
            "l2norm_fp16x16/l2norm_fp16x16_wrapper.cc",
            "layer_norm_fp16x16/layer_norm_fp16x16_wrapper.cc",
            "linear_approx_bf16/linear_approx_bf16_wrapper.cc",
            "maxpool/maxpool_wrapper.cc",
            "pad/wrapper_pad.cc",
            "q/q.hpp",
            "q/q_impl.hpp",
            "q/q_wrapper.cc",
            "reshape/wrapper_reshape.cc",
            "scatter_element/wrapper_scatter_element.cc",
            "slice/wrapper_slice.cc",
            "softmax_fp16x16/copy_fp16x16_wrapper.cc",
            "softmax_fp16x16/softmax_fp16x16_wrapper.cc",
            "topk/wrapper_topk.cc",
            "transpose/wrapper_transpose.cc",
        ],
    },
    "pdi_combination_86": {
        "combined_kernel_names": {
            "run_flash_mha_uint16": 65,
        },
        "combined_kernel_includes": [
            "super.hh",
            "flash_mha_qdq_uint16/flash_mha_wrapper.cc",
        ],
    },
    "pdi_combination_87": {
        "combined_kernel_names": {
            "run_flash_mha_uint8": 66,
        },
        "combined_kernel_includes": [
            "super.hh",
            "flash_mha_qdq_uint8/flash_mha_wrapper.cc",
        ],
    },
}

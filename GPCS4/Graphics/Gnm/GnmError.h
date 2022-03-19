/* SIE CONFIDENTIAL
PlayStation(R)4 Programmer Tool Runtime Library Release 08.008.011
* Copyright (C) 2016 Sony Interactive Entertainment Inc.
*/

#ifndef _SCE_GNM_ERROR_GEN_H
#define _SCE_GNM_ERROR_GEN_H

/*
 * Error number definition
 */

/**
 * @j 不正な引数がsubmit関数に渡されました。 @ej
 * @e Invalid arguments passed to the submit function. @ee
 */
#define SCE_GNM_ERROR_SUBMISSION_FAILED_INVALID_ARGUMENT -2133782528	/* 0x80D11000 */

/**
 * @j submitするコマンドバッファのスペースがありません。DCB/CCBの数を分割して少なくしてからsubmitを行ってください。 @ej
 * @e Not enough resources to submit the command buffers; try to split reduced the number of DCB/CCB to submit at once. @ee
 */
#define SCE_GNM_ERROR_SUBMISSION_NOT_ENOUGH_RESOURCES -2133782527	/* 0x80D11001 */

/**
 * @j Prepare flipが呼ばれていません。 @ej
 * @e prepareFlip() hasn't been called. @ee
 */
#define SCE_GNM_ERROR_SUBMISSION_AND_FLIP_FAILED_INVALID_COMMAND_BUFFER -2133782400	/* 0x80D11080 */

/**
 * @j Flip Queueがいっぱいです。 @ej
 * @e Flip queue full. @ee
 */
#define SCE_GNM_ERROR_SUBMISSION_AND_FLIP_FAILED_INVALID_QUEUE_FULL -2133782399	/* 0x80D11081 */

/**
 * @j Flipリクエストに失敗しました。 @ej
 * @e Flip request failed. @ee
 */
#define SCE_GNM_ERROR_SUBMISSION_AND_FLIP_FAILED_REQUEST_FAILED -2133782398	/* 0x80D11082 */

/**
 * @j 内部エラー @ej
 * @e Internal Error. @ee
 */
#define SCE_GNM_ERROR_SUBMISSION_FAILED_INTERNAL_ERROR -2133782273	/* 0x80D110FF */

/**
 * @j Gnm Validateのエラーを検出した。 @ej
 * @e Gnm Validate error detected. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_ERROR			-2133774336	/* 0x80D13000 */

/**
 * @j V#のアドレスが不正です。 @ej
 * @e The address of a V# is invalid. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_VSHARP			-2133774335	/* 0x80D13001 */

/**
 * @j T#のアドレスが不正です。 @ej
 * @e The address of a T# is invalid. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_TSHARP			-2133774334	/* 0x80D13002 */

/**
 * @j T#またはV#のアドレスのいずれかが不正です。 @ej
 * @e The address of either a T# or a V# is invalid. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_RESOURCE		-2133774333	/* 0x80D13003 */

/**
 * @j Sampler, Constant Buffer, GlobalTableが不正です。 @ej
 * @e The address of a resource, sampler, constant buffer, or global table is invalid. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_TABLE_MEMORY	-2133774332	/* 0x80D13004 */

/**
 * @j Gnm::writeAtEndOfPipe()関数が不正なEndOfPipeEventTypeで呼ばれました。 @ej
 * @e A Gnm::writeAtEndOfPipe() function was called with an invalid EndOfPipeEventType. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_WRITE_EVENT_OP	-2133774331	/* 0x80D13005 */

/**
 * @j Index bufferのアドレスが不正です。 @ej
 * @e The address of the index buffer is invalid. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_INDEX_BUFFER	-2133774330	/* 0x80D13006 */

/**
 * @j Tesselation Factor bufferが不正です。 @ej
 * @e The tessellation factor buffer is invalid. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_TESS_FACTOR_BUFFER -2133774329	/* 0x80D13007 */

/**
 * @j Scratch ringが不正です。 @ej
 * @e The scratch ring is invalid. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_SCRATCH_RING	-2133774328	/* 0x80D13008 */

/**
 * @j Draw Callの前にPrimitive Typeが指定されていません。 @ej
 * @e The primitive type is not set before a draw call. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_PRIMITIVE_TYPE	-2133774327	/* 0x80D13009 */

/**
 * @j Index SizeがIndex Draw Callの前に指定されていません。 @ej
 * @e The index size is not set before an indexed draw call. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_INDEX_SIZE		-2133774326	/* 0x80D1300A */

/**
 * @j DrawIndexInlineで利用するInlined bufferのサイズが不正です。 @ej
 * @e The size of the inlined buffer of a drawIndexInline() call is incorrect. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_INLINE_DRAW_SIZE -2133774325	/* 0x80D1300B */

/**
 * @j Patchを使う場合、patch数をは、Gnm::TessellationRegistersで設定する必要があります。 @ej
 * @e In order to use patches, the number of patches must be set via the Gnm::TessellationRegisters. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_NUM_INPUT_PATCHES -2133774324	/* 0x80D1300C */

/**
 * @j Gnm::DrawCommandBuffer::setActiveShaderStages()でGSステージが無効の場合は、GsGnm::DrawCommandBuffer::setGsMode()でkGsModeDisbleにする必要があります。 @ej
 * @e Gnm::DrawCommandBuffer::setGsMode() is called with GsMode kGsModeDisable when Gnm::DrawCommandBuffer::setActiveShaderStages() has the GS stage disabled. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_GS_MODE		-2133774323	/* 0x80D1300D */

/**
 * @j 有効化されたシェーダーは不正なアドレスにあります。 @ej
 * @e An enabled shader has an invalid address. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_SHADER_ADDRESS	-2133774322	/* 0x80D1300E */

/**
 * @j ボーダーカラーテーブルのアドレスが無効な値になっています。 @ej
 * @e The address of border color table is invalid. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_BORDER_COLOR_TABLE -2133774321	/* 0x80D1300F */

/**
 * @j S#に無効なデータが設定されています。 @ej
 * @e S# has invalid data. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_SSHARP			-2133774320	/* 0x80D13010 */

/**
 * @j グラフィックスパイプラインの設定で誤ったactive shader stageがセットされました。 @ej
 * @e Incorrect active shader stage has been set for draw type. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_ACTIVE_SHADER_STAGE -2133774318	/* 0x80D13012 */

/**
 * @j コマンドバッファの検査でDCBエラーが見つかりました。 @ej
 * @e DCB error detected while validating command buffer. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_DCB			-2133774317	/* 0x80D13013 */

/**
 * @j シェーダバイナリが間違ったシェーダステージにバインドされています。 @ej
 * @e Shader binary is bound to an incorrect shader stage. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_MISMATCH_SHADER_STAGE -2133774316	/* 0x80D13014 */

/**
 * @j レンダーターゲットのバインドが描画コールの現在のレンダーステート設定と矛盾している。 @ej
 * @e Render target binding is inconsistent with current render state settings for draw call. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_MRT_SETUP		-2133774315	/* 0x80D13015 */

/**
 * @j コマンドバッファ内で不正なオペコードが見つかった。 @ej
 * @e Invalid opcode found in command buffer. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_BAD_OP_CODE	-2133774314	/* 0x80D13016 */

/**
 * @j デプスレンダーターゲットのバインドが描画コールの現在のレンダーステート設定と矛盾している。 @ej
 * @e Depth render target binding is inconsistent with current render state settings for draw call. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_DEPTH_RT_SETUP	-2133774313	/* 0x80D13017 */

/**
 * @j 間接描画コールの後、標準描画コールの前にsetNumInstances()の呼び出しが必要である。 @ej
 * @e setNumInstances() call is required prior to draw call, after indirect-draw call has been called. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_NUM_INSTANCES	-2133774312	/* 0x80D13018 */

/**
 * @j SRTバインディングが無効である。 @ej
 * @e SRT Binding is invalid. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_SRT			-2133774311	/* 0x80D13019 */

/**
 * @j Gnm検証モジュールに無効な引数が渡された。 @ej
 * @e An invalid argument was passed to Gnm validation module. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_INVALID_ARGUMENT -2133770243	/* 0x80D13FFD */

/**
 * @j gnmコマンド検査中に内部エラーが発生したため、以降の検査をスキップします。 @ej
 * @e An internal error occurred in the Validation module and all subsequent validation passes will be skipped. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_FAILED_INTERNAL_ERROR -2133770242	/* 0x80D13FFE */

/**
 * @j Validation機能が無効です。Target Mangerの"PA Debug" 設定で有効にしてください。 @ej
 * @e Validation is not enabled. Please enable "PA Debug" in the target manager. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_NOT_ENABLED	-2133770241	/* 0x80D13FFF */

/**
 * @j Gnm Validateのワーニングが検出された。 @ej
 * @e Gnm Validate warning detected. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_WARNING		-2133778432	/* 0x80D12000 */

/**
 * @j setResourceMemoryType()でリードオンリー属性が設定されたメモリ領域を読み書き可能なバッファ（もしくはテクスチャ）として使用しようとしています。V#またはT#でROビットが立っているにも関わらず、シェーダにRWResourceの値が指定されていないかチェックしてください。 @ej
 * @e A RW buffer or texture was used when the memory type was set to read-only via setResourceMemoryType(). This is determined by checking RWResource inputs to shaders against the RO bit set in the V# or T#. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_WARNING_RWBUFFER_ROTYPE -2133778431	/* 0x80D12001 */

/**
 * @j ブレンディングパラメータは現在のレンダーターゲットのバインドに対して無効である。 @ej
 * @e Blending parameters are invalid for current render target binding. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_WARNING_BLENDING -2133778430	/* 0x80D12002 */

/**
 * @j CUマスク設定のためにGPUがハングアップする可能性がある。 @ej
 * @e CU mask configuration could lead to GPU hangs. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_WARNING_DRAW_CU_MASK -2133778429	/* 0x80D12003 */

/**
 * @j レンダーターゲットのバインドが描画コールの現在のレンダーステート設定と矛盾しているため、未定義の結果が得られる可能性がある。 @ej
 * @e Render target binding is inconsistent with current render state settings for draw call. This may lead to undefined results. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_WARNING_MRT_SETUP -2133778428	/* 0x80D12004 */

/**
 * @j デプスレンダーターゲットのバインドが描画コールの現在のレンダーステート設定と矛盾しているため、未定義の結果が得られる可能性がある。 @ej
 * @e Depth render target binding is inconsistent with current render state settings for draw call. This may lead to undefined results. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_WARNING_DEPTH_RT_SETUP -2133778427	/* 0x80D12005 */

/**
 * @j ピクセルシェーダ出力と境界レンダーターゲットのフォーマットが矛盾しているため、未定義の結果が得られる可能性がある。 @ej
 * @e Pixel Shader output and bound render target formats are incompatible. This may lead to undefined results. @ee
 */
#define SCE_GNM_ERROR_VALIDATION_WARNING_PS_AND_MRT_FORMAT -2133778417	/* 0x80D1200F */

/**
 * @j 指定されたファイルを開くことができませんでした。 @ej
 * @e Cannot open the specified file for writing. @ee
 */
#define SCE_GNM_ERROR_CAPTURE_FILE_IO			-2133766144	/* 0x80D15000 */

/**
 * @j Razorがロードされていないためキャプチャできませんでした。 @ej
 * @e Cannot capture if Razor for PlayStation®4 (Razor) is not enabled. @ee
 */
#define SCE_GNM_ERROR_CAPTURE_RAZOR_NOT_LOADED	-2133766143	/* 0x80D15001 */

/**
 * @j CaptureしようとしたコマンドバッファがEmptyです。 @ej
 * @e The command buffer being captured is empty. @ee
 */
#define SCE_GNM_ERROR_CAPTURE_NOTHING_TO_CAPTURE -2133766142	/* 0x80D15002 */

/**
 * @j 内部エラーです。 @ej
 * @e Internal error. @ee
 */
#define SCE_GNM_ERROR_CAPTURE_FAILED_INTERNAL	-2133766129	/* 0x80D1500F */

/**
 * @j The pipe ID must between 0 or 3 (inclusive). @ej
 * @e The pipe ID must between 0 or 3 (inclusive). @ee
 */
#define SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_PIPE_ID -2133757952	/* 0x80D17000 */

/**
 * @j The queue ID must between 0 or 7 (inclusive). @ej
 * @e The queue ID must between 0 or 7 (inclusive). @ee
 */
#define SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_QUEUE_ID -2133757951	/* 0x80D17001 */

/**
 * @j Invalid Ring Size (must be multiple of DW and a power of 2). @ej
 * @e Invalid Ring Size (must be multiple of DW and a power of 2). @ee
 */
#define SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_RING_SIZE -2133757950	/* 0x80D17002 */

/**
 * @j The Ring Base Address must be 256 bytes aligned. @ej
 * @e The Ring Base Address must be 256 bytes aligned. @ee
 */
#define SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_RING_BASE_ADDR -2133757949	/* 0x80D17003 */

/**
 * @j The Read Ptr Address must be 4 bytes aligned. @ej
 * @e The Read Ptr Address must be 4 bytes aligned. @ee
 */
#define SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_READ_PTR_ADDR -2133757948	/* 0x80D17004 */

/**
 * @j Invalid pipe priority. @ej
 * @e Invalid pipe priority. @ee
 */
#define SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_PIPE_PRIORITY -2133757947	/* 0x80D17005 */

/**
 * @j 内部エラーです。 @ej
 * @e Internal error. @ee
 */
#define SCE_GNM_ERROR_COMPUTEQUEUE_INTERNAL		-2133757697	/* 0x80D170FF */

/**
 * @j Invalid argument. @ej
 * @e Invalid argument. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_INVALID_ARGUMENT -2133749760	/* 0x80D19000 */

/**
 * @j The shader is invalid. @ej
 * @e The shader is invalid. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_INVALID_SHADER -2133749759	/* 0x80D19001 */

/**
 * @j The handle has been used after freeing. @ej
 * @e The handle has been used after freeing. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_STALE_HANDLE -2133749758	/* 0x80D19002 */

/**
 * @j No space available. @ej
 * @e No space available. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_FULL -2133749757	/* 0x80D19003 */

/**
 * @j The operation is not permitted. @ej
 * @e The operation is not permitted. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_NOT_PERMITTED -2133749756	/* 0x80D19004 */

/**
 * @j The output argument is <c>NULL</c>. @ej
 * @e The output argument is <c>NULL</c>. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_OUTPUT_ARGUMENT_IS_NULL -2133749755	/* 0x80D19005 */

/**
 * @j The owner handle is invalid. @ej
 * @e The owner handle is invalid. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_OWNER_HANDLE_INVALID -2133749754	/* 0x80D19006 */

/**
 * @j The resource handle is invalid. @ej
 * @e The resource handle is invalid. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_RESOURCE_HANDLE_INVALID -2133749753	/* 0x80D19007 */

/**
 * @j The resource type is invalid. @ej
 * @e The resource type is invalid. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_RESOURCE_TYPE_INVALID -2133749752	/* 0x80D19008 */

/**
 * @j The GDS resource type is invalid. @ej
 * @e The GDS resource type is invalid. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_GDS_RESOURCE_TYPE_INVALID -2133749751	/* 0x80D19009 */

/**
 * @j The resource size is invalid. @ej
 * @e The resource size is invalid. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_RESOURCE_SIZE_INVALID -2133749750	/* 0x80D1900A */

/**
 * @j The resource address is <c>NULL</c>. @ej
 * @e The resource address is <c>NULL</c>. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_RESOURCE_ADDRESS_IS_NULL -2133749749	/* 0x80D1900B */

/**
 * @j The GDS offset is invalid. @ej
 * @e The GDS offset is invalid. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_GDS_OFFSET_INVALID -2133749748	/* 0x80D1900C */

/**
 * @j The resource name is <c>NULL</c>. @ej
 * @e The resource name is <c>NULL</c>. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_RESOURCE_NAME_IS_NULL -2133749747	/* 0x80D1900D */

/**
 * @j The owner name is <c>NULL</c>. @ej
 * @e The owner name is <c>NULL</c>. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_OWNER_NAME_IS_NULL -2133749746	/* 0x80D1900E */

/**
 * @j The find callback is <c>NULL</c>. @ej
 * @e The find callback is <c>NULL</c>. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_FIND_CALLBACK_IS_NULL -2133749745	/* 0x80D1900F */

/**
 * @j The resource is not a shader. @ej
 * @e The resource is not a shader. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_RESOURCE_IS_NOT_SHADER -2133749744	/* 0x80D19010 */

/**
 * @j The user memory parameter is <c>NULL</c>. @ej
 * @e The user memory parameter is <c>NULL</c>. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_USER_MEMORY_PARAM_IS_NULL -2133749743	/* 0x80D19011 */

/**
 * @j The user memory parameter is not aligned. @ej
 * @e The user memory parameter is not aligned. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_USER_MEMORY_PARAM_NOT_ALIGNED -2133749742	/* 0x80D19012 */

/**
 * @j The name length parameter is invalid. @ej
 * @e The name length parameter is invalid. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_INVALID_NAME_LENGTH_PARAM -2133749741	/* 0x80D19013 */

/**
 * @j The size parameter is invalid. @ej
 * @e The size parameter is invalid. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_INVALID_SIZE_PARAM -2133749740	/* 0x80D19014 */

/**
 * @j The number of resources param is invalid. @ej
 * @e The number of resources param is invalid. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_INVALID_NUM_RESOURCES_PARAM -2133749739	/* 0x80D19015 */

/**
 * @j 内部エラーです。 @ej
 * @e Internal error. @ee
 */
#define SCE_GNM_ERROR_RESOURCE_REGISTRATION_INTERNAL -2133745665	/* 0x80D19FFF */

/**
 * @j いずれかの引数がNULLです。 @ej
 * @e A parameter is null. @ee
 */
#define SCE_GNM_ERROR_GET_GPU_INFO_PARAMETER_NULL -2133741568	/* 0x80D1B000 */

/**
 * @j 情報の取得に失敗しました。 @ej
 * @e Failed to get the information. @ee
 */
#define SCE_GNM_ERROR_GET_GPU_INFO_FAILED		-2133741567	/* 0x80D1B001 */

/**
 * @j いずれかの引数が不正です。 @ej
 * @e A parameter is invalid. @ee
 */
#define SCE_GNM_ERROR_GET_GPU_INFO_PARAMETER_INVALID -2133741566	/* 0x80D1B002 */


#endif //_SCE_GNM_ERROR_GEN_H

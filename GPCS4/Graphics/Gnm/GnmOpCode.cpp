#include "GnmOpCode.h"

namespace sce::Gnm
{

const char* opcodePrivName(IT_OpCodePriv priv)
{
	const char* name = nullptr;
	switch (priv)
	{
	case OP_PRIV_INITIALIZE_DEFAULT_HARDWARE_STATE:
		name = "OP_PRIV_INITIALIZE_DEFAULT_HARDWARE_STATE";
		break;
	case OP_PRIV_INITIALIZE_TO_DEFAULT_CONTEXT_STATE:
		name = "OP_PRIV_INITIALIZE_TO_DEFAULT_CONTEXT_STATE";
		break;
	case OP_PRIV_SET_EMBEDDED_VS_SHADER:
		name = "OP_PRIV_SET_EMBEDDED_VS_SHADER";
		break;
	case OP_PRIV_SET_EMBEDDED_PS_SHADER:
		name = "OP_PRIV_SET_EMBEDDED_PS_SHADER";
		break;
	case OP_PRIV_SET_VS_SHADER:
		name = "OP_PRIV_SET_VS_SHADER";
		break;
	case OP_PRIV_SET_PS_SHADER:
		name = "OP_PRIV_SET_PS_SHADER";
		break;
	case OP_PRIV_SET_ES_SHADER:
		name = "OP_PRIV_SET_ES_SHADER";
		break;
	case OP_PRIV_SET_GS_SHADER:
		name = "OP_PRIV_SET_GS_SHADER";
		break;
	case OP_PRIV_SET_HS_SHADER:
		name = "OP_PRIV_SET_HS_SHADER";
		break;
	case OP_PRIV_SET_LS_SHADER:
		name = "OP_PRIV_SET_LS_SHADER";
		break;
	case OP_PRIV_UPDATE_GS_SHADER:
		name = "OP_PRIV_UPDATE_GS_SHADER";
		break;
	case OP_PRIV_UPDATE_HS_SHADER:
		name = "OP_PRIV_UPDATE_HS_SHADER";
		break;
	case OP_PRIV_UPDATE_PS_SHADER:
		name = "OP_PRIV_UPDATE_PS_SHADER";
		break;
	case OP_PRIV_UPDATE_VS_SHADER:
		name = "OP_PRIV_UPDATE_VS_SHADER";
		break;
	case OP_PRIV_SET_VGT_CONTROL:
		name = "OP_PRIV_SET_VGT_CONTROL";
		break;
	case OP_PRIV_RESET_VGT_CONTROL:
		name = "OP_PRIV_RESET_VGT_CONTROL";
		break;
	case OP_PRIV_DRAW_INDEX:
		name = "OP_PRIV_DRAW_INDEX";
		break;
	case OP_PRIV_DRAW_INDEX_AUTO:
		name = "OP_PRIV_DRAW_INDEX_AUTO";
		break;
	case OP_PRIV_DRAW_INDEX_INDIRECT:
		name = "OP_PRIV_DRAW_INDEX_INDIRECT";
		break;
	case OP_PRIV_DRAW_INDEX_INDIRECT_COUNT_MULTI:
		name = "OP_PRIV_DRAW_INDEX_INDIRECT_COUNT_MULTI";
		break;
	case OP_PRIV_DRAW_INDEX_MULTI_INSTANCED:
		name = "OP_PRIV_DRAW_INDEX_MULTI_INSTANCED";
		break;
	case OP_PRIV_DRAW_INDEX_OFFSET:
		name = "OP_PRIV_DRAW_INDEX_OFFSET";
		break;
	case OP_PRIV_DRAW_INDIRECT:
		name = "OP_PRIV_DRAW_INDIRECT";
		break;
	case OP_PRIV_DRAW_INDIRECT_COUNT_MULTI:
		name = "OP_PRIV_DRAW_INDIRECT_COUNT_MULTI";
		break;
	case OP_PRIV_DRAW_OPAQUE_AUTO:
		name = "OP_PRIV_DRAW_OPAQUE_AUTO";
		break;
	case OP_PRIV_WAIT_UNTIL_SAFE_FOR_RENDERING:
		name = "OP_PRIV_WAIT_UNTIL_SAFE_FOR_RENDERING";
		break;
	case OP_PRIV_PUSH_MARKER:
		name = "OP_PRIV_PUSH_MARKER";
		break;
	case OP_PRIV_PUSH_COLOR_MARKER:
		name = "OP_PRIV_PUSH_COLOR_MARKER";
		break;
	case OP_PRIV_POP_MARKER:
		name = "OP_PRIV_POP_MARKER";
		break;
	case OP_PRIV_SET_MARKER:
		name = "OP_PRIV_SET_MARKER";
		break;
	case OP_PRIV_SET_CS_SHADER:
		name = "OP_PRIV_SET_CS_SHADER";
		break;
	case OP_PRIV_DISPATCH_DIRECT:
		name = "OP_PRIV_DISPATCH_DIRECT";
		break;
	case OP_PRIV_DISPATCH_INDIRECT:
		name = "OP_PRIV_DISPATCH_INDIRECT";
		break;
	default:
		name = "Error private opcode";
		break;
	}
	return name;
}

const char* opcodeName(uint32_t header)
{
	const char*        name = nullptr;
	PPM4_TYPE_3_HEADER hdr  = (PPM4_TYPE_3_HEADER)&header;
	IT_OpCodeType      op   = (IT_OpCodeType)hdr->opcode;
	switch (op)
	{
	case IT_NOP:
		name = "IT_NOP";
		break;
	case IT_SET_BASE:
		name = "IT_SET_BASE";
		break;
	case IT_INDEX_BUFFER_SIZE:
		name = "IT_INDEX_BUFFER_SIZE";
		break;
	case IT_SET_PREDICATION:
		name = "IT_SET_PREDICATION";
		break;
	case IT_COND_EXEC:
		name = "IT_COND_EXEC";
		break;
	case IT_INDEX_BASE:
		name = "IT_INDEX_BASE";
		break;
	case IT_INDEX_TYPE:
		name = "IT_INDEX_TYPE";
		break;
	case IT_NUM_INSTANCES:
		name = "IT_NUM_INSTANCES";
		break;
	case IT_STRMOUT_BUFFER_UPDATE:
		name = "IT_STRMOUT_BUFFER_UPDATE";
		break;
	case IT_WRITE_DATA:
		name = "IT_WRITE_DATA";
		break;
	case IT_MEM_SEMAPHORE:
		name = "IT_MEM_SEMAPHORE";
		break;
	case IT_WAIT_REG_MEM:
		name = "IT_WAIT_REG_MEM";
		break;
	case IT_INDIRECT_BUFFER:
		name = "IT_INDIRECT_BUFFER";
		break;
	case IT_PFP_SYNC_ME:
		name = "IT_PFP_SYNC_ME";
		break;
	case IT_EVENT_WRITE:
		name = "IT_EVENT_WRITE";
		break;
	case IT_EVENT_WRITE_EOP:
		name = "IT_EVENT_WRITE_EOP";
		break;
	case IT_EVENT_WRITE_EOS:
		name = "IT_EVENT_WRITE_EOS";
		break;
	case IT_DMA_DATA:
		name = "IT_DMA_DATA";
		break;
	case IT_ACQUIRE_MEM:
		name = "IT_ACQUIRE_MEM";
		break;
	case IT_REWIND:
		name = "IT_REWIND";
		break;
	case IT_SET_CONFIG_REG:
		name = "IT_SET_CONFIG_REG";
		break;
	case IT_SET_CONTEXT_REG:
		name = "IT_SET_CONTEXT_REG";
		break;
	case IT_SET_SH_REG:
		name = "IT_SET_SH_REG";
		break;
	case IT_SET_UCONFIG_REG:
		name = "IT_SET_UCONFIG_REG";
		break;
	case IT_INCREMENT_DE_COUNTER:
		name = "IT_INCREMENT_DE_COUNTER";
		break;
	case IT_WAIT_ON_CE_COUNTER:
		name = "IT_WAIT_ON_CE_COUNTER";
		break;
	case IT_DISPATCH_DRAW_PREAMBLE__GFX09:
		name = "IT_DISPATCH_DRAW_PREAMBLE__GFX09";
		break;
	case IT_DISPATCH_DRAW__GFX09:
		name = "IT_DISPATCH_DRAW__GFX09";
		break;
	case IT_GET_LOD_STATS__GFX09:
		name = "IT_GET_LOD_STATS__GFX09";
		break;
	case IT_GNM_PRIVATE:
	{
		IT_OpCodePriv priv = PM4_PRIV(header);
		name               = opcodePrivName(priv);
	}
	break;
	///
	case IT_CLEAR_STATE:
		name = "IT_CLEAR_STATE";
		break;
	case IT_DISPATCH_DIRECT:
		name = "IT_DISPATCH_DIRECT";
		break;
	case IT_DISPATCH_INDIRECT:
		name = "IT_DISPATCH_INDIRECT";
		break;
	case IT_INDIRECT_BUFFER_END:
		name = "IT_INDIRECT_BUFFER_END";
		break;
	case IT_INDIRECT_BUFFER_CNST_END:
		name = "IT_INDIRECT_BUFFER_CNST_END";
		break;
	case IT_ATOMIC_GDS:
		name = "IT_ATOMIC_GDS";
		break;
	case IT_ATOMIC_MEM:
		name = "IT_ATOMIC_MEM";
		break;
	case IT_OCCLUSION_QUERY:
		name = "IT_OCCLUSION_QUERY";
		break;
	case IT_REG_RMW:
		name = "IT_REG_RMW";
		break;
	case IT_PRED_EXEC:
		name = "IT_PRED_EXEC";
		break;
	case IT_DRAW_INDIRECT:
		name = "IT_DRAW_INDIRECT";
		break;
	case IT_DRAW_INDEX_INDIRECT:
		name = "IT_DRAW_INDEX_INDIRECT";
		break;
	case IT_DRAW_INDEX_2:
		name = "IT_DRAW_INDEX_2";
		break;
	case IT_CONTEXT_CONTROL:
		name = "IT_CONTEXT_CONTROL";
		break;
	case IT_DRAW_INDIRECT_MULTI:
		name = "IT_DRAW_INDIRECT_MULTI";
		break;
	case IT_DRAW_INDEX_AUTO:
		name = "IT_DRAW_INDEX_AUTO";
		break;
	case IT_DRAW_INDEX_MULTI_AUTO:
		name = "IT_DRAW_INDEX_MULTI_AUTO";
		break;
	case IT_INDIRECT_BUFFER_PRIV:
		name = "IT_INDIRECT_BUFFER_PRIV";
		break;
	case IT_INDIRECT_BUFFER_CNST:
		name = "IT_INDIRECT_BUFFER_CNST";
		break;
	case IT_DRAW_INDEX_OFFSET_2:
		name = "IT_DRAW_INDEX_OFFSET_2";
		break;
	case IT_DRAW_PREAMBLE:
		name = "IT_DRAW_PREAMBLE";
		break;
	case IT_DRAW_INDEX_INDIRECT_MULTI:
		name = "IT_DRAW_INDEX_INDIRECT_MULTI";
		break;
	case IT_DRAW_INDEX_MULTI_INST:
		name = "IT_DRAW_INDEX_MULTI_INST";
		break;
	case IT_COPY_DW:
		name = "IT_COPY_DW";
		break;
	case IT_COPY_DATA:
		name = "IT_COPY_DATA";
		break;
	case IT_CP_DMA:
		name = "IT_CP_DMA";
		break;
	case IT_SURFACE_SYNC:
		name = "IT_SURFACE_SYNC";
		break;
	case IT_ME_INITIALIZE:
		name = "IT_ME_INITIALIZE";
		break;
	case IT_COND_WRITE:
		name = "IT_COND_WRITE";
		break;
	case IT_RELEASE_MEM:
		name = "IT_RELEASE_MEM";
		break;
	case IT_PREAMBLE_CNTL:
		name = "IT_PREAMBLE_CNTL";
		break;
	case IT_DRAW_RESERVED0:
		name = "IT_DRAW_RESERVED0";
		break;
	case IT_DRAW_RESERVED1:
		name = "IT_DRAW_RESERVED1";
		break;
	case IT_DRAW_RESERVED2:
		name = "IT_DRAW_RESERVED2";
		break;
	case IT_DRAW_RESERVED3:
		name = "IT_DRAW_RESERVED3";
		break;
	case IT_CONTEXT_REG_RMW:
		name = "IT_CONTEXT_REG_RMW";
		break;
	case IT_GFX_CNTX_UPDATE:
		name = "IT_GFX_CNTX_UPDATE";
		break;
	case IT_BLK_CNTX_UPDATE:
		name = "IT_BLK_CNTX_UPDATE";
		break;
	case IT_INCR_UPDT_STATE:
		name = "IT_INCR_UPDT_STATE";
		break;
	case IT_INTERRUPT:
		name = "IT_INTERRUPT";
		break;
	case IT_GEN_PDEPTE:
		name = "IT_GEN_PDEPTE";
		break;
	case IT_INDIRECT_BUFFER_PASID:
		name = "IT_INDIRECT_BUFFER_PASID";
		break;
	case IT_PRIME_UTCL2:
		name = "IT_PRIME_UTCL2";
		break;
	case IT_LOAD_UCONFIG_REG:
		name = "IT_LOAD_UCONFIG_REG";
		break;
	case IT_LOAD_SH_REG:
		name = "IT_LOAD_SH_REG";
		break;
	case IT_LOAD_CONFIG_REG:
		name = "IT_LOAD_CONFIG_REG";
		break;
	case IT_LOAD_CONTEXT_REG:
		name = "IT_LOAD_CONTEXT_REG";
		break;
	case IT_LOAD_COMPUTE_STATE:
		name = "IT_LOAD_COMPUTE_STATE";
		break;
	case IT_LOAD_SH_REG_INDEX:
		name = "IT_LOAD_SH_REG_INDEX";
		break;
	case IT_SET_CONTEXT_REG_INDEX:
		name = "IT_SET_CONTEXT_REG_INDEX";
		break;
	case IT_SET_VGPR_REG_DI_MULTI:
		name = "IT_SET_VGPR_REG_DI_MULTI";
		break;
	case IT_SET_SH_REG_DI:
		name = "IT_SET_SH_REG_DI";
		break;
	case IT_SET_CONTEXT_REG_INDIRECT:
		name = "IT_SET_CONTEXT_REG_INDIRECT";
		break;
	case IT_SET_SH_REG_DI_MULTI:
		name = "IT_SET_SH_REG_DI_MULTI";
		break;
	case IT_GFX_PIPE_LOCK:
		name = "IT_GFX_PIPE_LOCK";
		break;
	case IT_SET_SH_REG_OFFSET:
		name = "IT_SET_SH_REG_OFFSET";
		break;
	case IT_SET_QUEUE_REG:
		name = "IT_SET_QUEUE_REG";
		break;
	case IT_SET_UCONFIG_REG_INDEX:
		name = "IT_SET_UCONFIG_REG_INDEX";
		break;
	case IT_FORWARD_HEADER:
		name = "IT_FORWARD_HEADER";
		break;
	case IT_SCRATCH_RAM_WRITE:
		name = "IT_SCRATCH_RAM_WRITE";
		break;
	case IT_SCRATCH_RAM_READ:
		name = "IT_SCRATCH_RAM_READ";
		break;
	case IT_LOAD_CONST_RAM:
		name = "IT_LOAD_CONST_RAM";
		break;
	case IT_WRITE_CONST_RAM:
		name = "IT_WRITE_CONST_RAM";
		break;
	case IT_DUMP_CONST_RAM:
		name = "IT_DUMP_CONST_RAM";
		break;
	case IT_INCREMENT_CE_COUNTER:
		name = "IT_INCREMENT_CE_COUNTER";
		break;
	case IT_WAIT_ON_DE_COUNTER_DIFF:
		name = "IT_WAIT_ON_DE_COUNTER_DIFF";
		break;
	case IT_SWITCH_BUFFER:
		name = "IT_SWITCH_BUFFER";
		break;
	case IT_FRAME_CONTROL:
		name = "IT_FRAME_CONTROL";
		break;
	case IT_INDEX_ATTRIBUTES_INDIRECT:
		name = "IT_INDEX_ATTRIBUTES_INDIRECT";
		break;
	case IT_WAIT_REG_MEM64:
		name = "IT_WAIT_REG_MEM64";
		break;
	case IT_COND_PREEMPT:
		name = "IT_COND_PREEMPT";
		break;
	case IT_HDP_FLUSH:
		name = "IT_HDP_FLUSH";
		break;
	case IT_INVALIDATE_TLBS:
		name = "IT_INVALIDATE_TLBS";
		break;
	case IT_DMA_DATA_FILL_MULTI:
		name = "IT_DMA_DATA_FILL_MULTI";
		break;
	case IT_SET_SH_REG_INDEX:
		name = "IT_SET_SH_REG_INDEX";
		break;
	case IT_DRAW_INDIRECT_COUNT_MULTI:
		name = "IT_DRAW_INDIRECT_COUNT_MULTI";
		break;
	case IT_DRAW_INDEX_INDIRECT_COUNT_MULTI:
		name = "IT_DRAW_INDEX_INDIRECT_COUNT_MULTI";
		break;
	case IT_DUMP_CONST_RAM_OFFSET:
		name = "IT_DUMP_CONST_RAM_OFFSET";
		break;
	case IT_LOAD_CONTEXT_REG_INDEX:
		name = "IT_LOAD_CONTEXT_REG_INDEX";
		break;
	case IT_SET_RESOURCES:
		name = "IT_SET_RESOURCES";
		break;
	case IT_MAP_PROCESS:
		name = "IT_MAP_PROCESS";
		break;
	case IT_MAP_QUEUES:
		name = "IT_MAP_QUEUES";
		break;
	case IT_UNMAP_QUEUES:
		name = "IT_UNMAP_QUEUES";
		break;
	case IT_QUERY_STATUS:
		name = "IT_QUERY_STATUS";
		break;
	case IT_RUN_LIST:
		name = "IT_RUN_LIST";
		break;
	case IT_MAP_PROCESS_VM:
		name = "IT_MAP_PROCESS_VM";
		break;
	case IT_DRAW_MULTI_PREAMBLE__GFX09:
		name = "IT_DRAW_MULTI_PREAMBLE__GFX09";
		break;
	case IT_AQL_PACKET__GFX09:
		name = "IT_AQL_PACKET__GFX09";
		break;
	default:
		name = "Error OpCode";
		break;
	}
	return name;
}


}  // namespace sce::Gnm

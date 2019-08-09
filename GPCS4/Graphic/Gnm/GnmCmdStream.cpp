#include "GnmCmdStream.h"


GnmCmdStream::GnmCmdStream(std::shared_ptr<GnmCommandBuffer>& cb):
	m_cb(cb)
{

}

GnmCmdStream::~GnmCmdStream()
{

}

bool GnmCmdStream::processCommandBuffer(uint32_t* commandBuffer, uint32_t commandSize)
{
	bool bRet = false;
	do
	{
		uint32_t processedCmdSize = 0;
		uint32_t* command = commandBuffer;
		while (processedCmdSize < commandSize)
		{
			uint32_t pm4Type = PM4_TYPE(*command);
			uint32_t pm4LengthDw = 0;

			switch (pm4Type)
			{
			case PM4_TYPE_0:
				pm4LengthDw = processPM4Type0((PPM4_TYPE_0_HEADER)command, command + 1);
				break;
			case PM4_TYPE_2:
				// opcode should be 0x80000000, this is an 1 dword NOP
				pm4LengthDw = 1;
				break;
			case PM4_TYPE_3:
				pm4LengthDw = processPM4Type3((PPM4_TYPE_3_HEADER)command, command + 1);
				break;
			default:
				LOG_ERR("Invalid pm4 type %d", pm4Type);
				break;
			}

			command += pm4LengthDw;
			processedCmdSize += (pm4LengthDw * sizeof(uint32_t));
		}

		bRet  = true;
	}while(false);
	return bRet;
}

uint32_t GnmCmdStream::processPM4Type0(PPM4_TYPE_0_HEADER pm4Hdr, uint32_t* regDataX)
{
	LOG_FIXME("Type 0 PM4 packet is not supported.");
	// Type 0 PM4 packet is fixed sized of 2 dwords.
	return 2;
}

uint32_t GnmCmdStream::processPM4Type3(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{
	uint32_t pm4LengthDw = pm4Hdr->count + 2;

	IT_OpCodeType opcode = (IT_OpCodeType)pm4Hdr->opcode;

	switch (opcode)
	{
	case IT_NOP:
		onNop(pm4Hdr, itBody);
		break;
	case IT_SET_BASE:
		onSetBase(pm4Hdr, itBody);
		break;
	case IT_INDEX_BUFFER_SIZE:
		onIndexBufferSize(pm4Hdr, itBody);
		break;
	case IT_SET_PREDICATION:
		onSetPredication(pm4Hdr, itBody);
		break;
	case IT_COND_EXEC:
		onCondExec(pm4Hdr, itBody);
		break;
	case IT_INDEX_BASE:
		onIndexBase(pm4Hdr, itBody);
		break;
	case IT_INDEX_TYPE:
		onIndexType(pm4Hdr, itBody);
		break;
	case IT_NUM_INSTANCES:
		onNumInstances(pm4Hdr, itBody);
		break;
	case IT_STRMOUT_BUFFER_UPDATE:
		onStrmoutBufferUpdate(pm4Hdr, itBody);
		break;
	case IT_WRITE_DATA:
		onWriteData(pm4Hdr, itBody);
		break;
	case IT_MEM_SEMAPHORE:
		onMemSemaphore(pm4Hdr, itBody);
		break;
	case IT_WAIT_REG_MEM:
		onWaitRegMem(pm4Hdr, itBody);
		break;
	case IT_INDIRECT_BUFFER:
		onIndirectBuffer(pm4Hdr, itBody);
		break;
	case IT_PFP_SYNC_ME:
		onPfpSyncMe(pm4Hdr, itBody);
		break;
	case IT_EVENT_WRITE:
		onEventWrite(pm4Hdr, itBody);
		break;
	case IT_EVENT_WRITE_EOP:
		onEventWriteEop(pm4Hdr, itBody);
		break;
	case IT_EVENT_WRITE_EOS:
		onEventWriteEos(pm4Hdr, itBody);
		break;
	case IT_DMA_DATA:
		onDmaData(pm4Hdr, itBody);
		break;
	case IT_ACQUIRE_MEM:
		onAcquireMem(pm4Hdr, itBody);
		break;
	case IT_REWIND:
		onRewind(pm4Hdr, itBody);
		break;
	case IT_SET_CONFIG_REG:
		onSetConfigReg(pm4Hdr, itBody);
		break;
	case IT_SET_CONTEXT_REG:
		onSetContextReg(pm4Hdr, itBody);
		break;
	case IT_SET_SH_REG:
		onSetShReg(pm4Hdr, itBody);
		break;
	case IT_SET_UCONFIG_REG:
		onSetUconfigReg(pm4Hdr, itBody);
		break;
	case IT_INCREMENT_DE_COUNTER:
		onIncrementDeCounter(pm4Hdr, itBody);
		break;
	case IT_WAIT_ON_CE_COUNTER:
		onWaitOnCeCounter(pm4Hdr, itBody);
		break;
	case IT_DISPATCH_DRAW_PREAMBLE__GFX09:
		onDispatchDrawPreambleGfx09(pm4Hdr, itBody);
		break;
	case IT_DISPATCH_DRAW__GFX09:
		onDispatchDrawGfx09(pm4Hdr, itBody);
		break;
	case IT_GET_LOD_STATS__GFX09:
		onGetLodStatsGfx09(pm4Hdr, itBody);
		break;

	// Private handler
	case IT_GNM_PRIVATE:
		onGnmPrivate(pm4Hdr, itBody);
		break;
	// The following opcode types are not used by Gnm
	// TODO:
	// There maybe still some opcodes belongs to Gnm that is not found.
	// We should find all and place them above.
	case IT_CLEAR_STATE:
	case IT_DISPATCH_DIRECT:
	case IT_DISPATCH_INDIRECT:
	case IT_INDIRECT_BUFFER_END:
	case IT_INDIRECT_BUFFER_CNST_END:
	case IT_ATOMIC_GDS:
	case IT_ATOMIC_MEM:
	case IT_OCCLUSION_QUERY:
	case IT_REG_RMW:
	case IT_PRED_EXEC:
	case IT_DRAW_INDIRECT:
	case IT_DRAW_INDEX_INDIRECT:
	case IT_DRAW_INDEX_2:
	case IT_CONTEXT_CONTROL:
	case IT_DRAW_INDIRECT_MULTI:
	case IT_DRAW_INDEX_AUTO:
	case IT_DRAW_INDEX_MULTI_AUTO:
	case IT_INDIRECT_BUFFER_PRIV:
	case IT_INDIRECT_BUFFER_CNST:
	case IT_DRAW_INDEX_OFFSET_2:
	case IT_DRAW_PREAMBLE:
	case IT_DRAW_INDEX_INDIRECT_MULTI:
	case IT_DRAW_INDEX_MULTI_INST:
	case IT_COPY_DW:
	case IT_COPY_DATA:
	case IT_CP_DMA:
	case IT_SURFACE_SYNC:
	case IT_ME_INITIALIZE:
	case IT_COND_WRITE:
	case IT_RELEASE_MEM:
	case IT_PREAMBLE_CNTL:
	case IT_DRAW_RESERVED0:
	case IT_DRAW_RESERVED1:
	case IT_DRAW_RESERVED2:
	case IT_DRAW_RESERVED3:
	case IT_CONTEXT_REG_RMW:
	case IT_GFX_CNTX_UPDATE:
	case IT_BLK_CNTX_UPDATE:
	case IT_INCR_UPDT_STATE:
	case IT_INTERRUPT:
	case IT_GEN_PDEPTE:
	case IT_INDIRECT_BUFFER_PASID:
	case IT_PRIME_UTCL2:
	case IT_LOAD_UCONFIG_REG:
	case IT_LOAD_SH_REG:
	case IT_LOAD_CONFIG_REG:
	case IT_LOAD_CONTEXT_REG:
	case IT_LOAD_COMPUTE_STATE:
	case IT_LOAD_SH_REG_INDEX:
	case IT_SET_CONTEXT_REG_INDEX:
	case IT_SET_VGPR_REG_DI_MULTI:
	case IT_SET_SH_REG_DI:
	case IT_SET_CONTEXT_REG_INDIRECT:
	case IT_SET_SH_REG_DI_MULTI:
	case IT_GFX_PIPE_LOCK:
	case IT_SET_SH_REG_OFFSET:
	case IT_SET_QUEUE_REG:
	case IT_SET_UCONFIG_REG_INDEX:
	case IT_FORWARD_HEADER:
	case IT_SCRATCH_RAM_WRITE:
	case IT_SCRATCH_RAM_READ:
	case IT_LOAD_CONST_RAM:
	case IT_WRITE_CONST_RAM:
	case IT_DUMP_CONST_RAM:
	case IT_INCREMENT_CE_COUNTER:
	case IT_WAIT_ON_DE_COUNTER_DIFF:
	case IT_SWITCH_BUFFER:
	case IT_FRAME_CONTROL:
	case IT_INDEX_ATTRIBUTES_INDIRECT:
	case IT_WAIT_REG_MEM64:
	case IT_COND_PREEMPT:
	case IT_HDP_FLUSH:
	case IT_INVALIDATE_TLBS:
	case IT_DMA_DATA_FILL_MULTI:
	case IT_SET_SH_REG_INDEX:
	case IT_DRAW_INDIRECT_COUNT_MULTI:
	case IT_DRAW_INDEX_INDIRECT_COUNT_MULTI:
	case IT_DUMP_CONST_RAM_OFFSET:
	case IT_LOAD_CONTEXT_REG_INDEX:
	case IT_SET_RESOURCES:
	case IT_MAP_PROCESS:
	case IT_MAP_QUEUES:
	case IT_UNMAP_QUEUES:
	case IT_QUERY_STATUS:
	case IT_RUN_LIST:
	case IT_MAP_PROCESS_VM:
	case IT_DRAW_MULTI_PREAMBLE__GFX09:
	case IT_AQL_PACKET__GFX09:
		LOG_ERR("Opcode not supported %X", opcode);
		break;
	default:
		LOG_ERR("Invalid opcode %X", opcode);
		break;
	}

	return pm4LengthDw;
}

void GnmCmdStream::onNop(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onSetBase(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onIndexBufferSize(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onSetPredication(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onCondExec(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onIndexBase(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onIndexType(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onNumInstances(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onStrmoutBufferUpdate(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onWriteData(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onMemSemaphore(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onWaitRegMem(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onIndirectBuffer(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onPfpSyncMe(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onEventWrite(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onEventWriteEop(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onEventWriteEos(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onDmaData(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onAcquireMem(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onRewind(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onSetConfigReg(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onSetContextReg(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onSetShReg(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onSetUconfigReg(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onIncrementDeCounter(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onWaitOnCeCounter(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onDispatchDrawPreambleGfx09(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onDispatchDrawGfx09(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onGetLodStatsGfx09(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{

}

void GnmCmdStream::onGnmPrivate(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody)
{
	IT_OpCodePriv priv = PM4_PRIV(*(uint32_t*)pm4Hdr);
	switch (priv)
	{
	case OP_PRIV_INITIALIZE_DEFAULT_HARDWARE_STATE:
		break;
	case OP_PRIV_INITIALIZE_TO_DEFAULT_CONTEXT_STATE:
		break;
	case OP_PRIV_SET_EMBEDDED_VS_SHADER:
		break;
	case OP_PRIV_SET_EMBEDDED_PS_SHADER:
		break;
	case OP_PRIV_SET_VS_SHADER:
		break;
	case OP_PRIV_SET_PS_SHADER:
		break;
	case OP_PRIV_SET_ES_SHADER:
		break;
	case OP_PRIV_SET_GS_SHADER:
		break;
	case OP_PRIV_SET_HS_SHADER:
		break;
	case OP_PRIV_SET_LS_SHADER:
		break;
	case OP_PRIV_UPDATE_GS_SHADER:
		break;
	case OP_PRIV_UPDATE_HS_SHADER:
		break;
	case OP_PRIV_UPDATE_PS_SHADER:
		break;
	case OP_PRIV_UPDATE_VS_SHADER:
		break;
	case OP_PRIV_SET_VGT_CONTROL:
		break;
	case OP_PRIV_RESET_VGT_CONTROL:
		break;
	case OP_PRIV_DRAW_INDEX:
		break;
	case OP_PRIV_DRAW_INDEX_AUTO:
		break;
	case OP_PRIV_DRAW_INDEX_INDIRECT:
		break;
	case OP_PRIV_DRAW_INDEX_INDIRECT_COUNT_MULTI:
		break;
	case OP_PRIV_DRAW_INDEX_MULTI_INSTANCED:
		break;
	case OP_PRIV_DRAW_INDEX_OFFSET:
		break;
	case OP_PRIV_DRAW_INDIRECT:
		break;
	case OP_PRIV_DRAW_INDIRECT_COUNT_MULTI:
		break;
	case OP_PRIV_DRAW_OPAQUE_AUTO:
		break;
	case OP_PRIV_WAIT_UNTIL_SAFE_FOR_RENDERING:
		break;
	case OP_PRIV_PUSH_MARKER:
		break;
	case OP_PRIV_PUSH_COLOR_MARKER:
		break;
	case OP_PRIV_POP_MARKER:
		break;
	case OP_PRIV_SET_MARKER:
		break;
	case OP_PRIV_SET_CS_SHADER:
		break;
	case OP_PRIV_DISPATCH_DIRECT:
		break;
	case OP_PRIV_DISPATCH_INDIRECT:
		break;
	default:
		break;
	}
}


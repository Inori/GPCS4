#include "GnmCmdStreamDraw.h"
#include "GnmOpCode.h"




const uint32_t GnmCmdStreamDraw::s_stageBases[kShaderStageCount] = { 0x2E40, 0x2C0C, 0x2C4C, 0x2C8C, 0x2CCC, 0x2D0C, 0x2D4C };


GnmCmdStreamDraw::GnmCmdStreamDraw(std::shared_ptr<GnmCommandBufferDraw> dcb):
	m_dcb(dcb)
{

}

GnmCmdStreamDraw::~GnmCmdStreamDraw()
{
}

bool GnmCmdStreamDraw::processCommandBuffer(uint32_t count, void* dcbGpuAddrs[], uint32_t* dcbSizesInBytes)
{
	bool ret = false;
	do 
	{
		LOG_ASSERT((count == 1), "currently only support 1 cmdbuff.");
		
		uint32_t* cmdBuff = (uint32_t*)dcbGpuAddrs[0];
		uint32_t cmdSize = dcbSizesInBytes[0];

		if (!processCommandBuffer(cmdBuff, cmdSize))
		{
			break;
		}

		ret = true;
	} while (false);
	return ret;
}

bool GnmCmdStreamDraw::processCommandBuffer(uint32_t* commandBuffer, uint32_t commandSize)
{
	bool ret = false;
	do 
	{
		uint32_t* command = commandBuffer;
		uint32_t processedCmdSize = 0;

		uint32_t* packetBuffer = NULL;
		uint32_t packetSizeDwords = 0;
		// the returned packet size.
		// for most packets, the packet size is fixed,
		// so the passed in 'packetSizeDwords' is equal to 'realPacketSizeDwords'
		// but there are some variable-length packets, such as NOP, PAUSE
		// which realPacketSizeDwords == packetSizeDwords is not always true.
		uint32_t realPacketSizeDwords = 0;

		uint32_t opcode = 0;
		uint8_t cmdType;

		// reset marker
		m_lastPacketDone = false;

		while (processedCmdSize < commandSize)
		{
			opcode = *command;
			cmdType = OPCODE_TYPE(opcode);
			packetSizeDwords = OPCODE_LENGTH(opcode);
			packetBuffer = command;

			LOG_DEBUG("opcode 0x%08X", opcode);

			switch (cmdType)
			{
			case OP_TYPE_BASE:
				realPacketSizeDwords = onPacketBase(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_BASE_INDIRECT_ARGS:
				realPacketSizeDwords = onPacketBaseIndirectArgs(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_INDEX_COUNT:
				realPacketSizeDwords = onPacketIndexCount(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_Z_PASS_PREDICATION:
				realPacketSizeDwords = onPacketZPassPredication(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_PREDICATION:
				realPacketSizeDwords = onPacketPredication(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_INDEX_BUFFER:
				realPacketSizeDwords = onPacketIndexBuffer(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_INDEX_SIZE:
				realPacketSizeDwords = onPacketIndexSize(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_NUM_INSTANCES:
				realPacketSizeDwords = onPacketNumInstances(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_WRITE_STREAMOUT_BUFFER:
				realPacketSizeDwords = onPacketWriteStreamoutBuffer(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_WRITE_DATA_INLINE:
				realPacketSizeDwords = onPacketWriteDataInline(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_SEMAPHORE:
				realPacketSizeDwords = onPacketSemaphore(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_WAIT_ON:
				realPacketSizeDwords = onPacketWaitOn(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_CHAIN_COMMAND_BUFFER:
				realPacketSizeDwords = onPacketChainCommandBuffer(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_STALL_COMMAND_BUFFER_PARSER:
				realPacketSizeDwords = onPacketStallCommandBufferParser(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_EVENT:
				realPacketSizeDwords = onPacketEvent(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_END_OF_PIPE:
				realPacketSizeDwords = onPacketEndOfPipe(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_END_OF_SHADER:
				realPacketSizeDwords = onPacketEndOfShader(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_GPU_FACILITY:
				realPacketSizeDwords = onPacketGpuFacility(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_WAIT_FOR_GRAPHICS_WRITES:
				realPacketSizeDwords = onPacketWaitForGraphicsWrites(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_PAUSE:
				realPacketSizeDwords = onPacketPause(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_CONFIG_REGISTER:
				realPacketSizeDwords = onPacketConfigRegister(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_SET_CONTEXT:
				realPacketSizeDwords = onPacketSetContext(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_SET_RESOURCE:
				realPacketSizeDwords = onPacketSetResource(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_SET_3:
				realPacketSizeDwords = onPacketSet3(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_INCREMENT_DE_COUNTER:
				realPacketSizeDwords = onPacketIncrementDeCounter(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_WAIT_ON_CE:
				realPacketSizeDwords = onPacketWaitOnCe(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_WAIT_FOR_SETUP_DISPATCH_DRAW_KICK_RING_BUFFER:
				realPacketSizeDwords = onPacketWaitForSetupDispatchDrawKickRingBuffer(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_DISPATCH_DRAW:
				realPacketSizeDwords = onPacketDispatchDraw(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_REQUEST_MIP_STATS_REPORT_AND_RESET:
				realPacketSizeDwords = onPacketRequestMipStatsReportAndReset(opcode, packetBuffer, packetSizeDwords);
				break;

			// private types

			case OP_TYPE_PRIV_SHARED:
				realPacketSizeDwords = onPacketPrivateShared(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_PRIV_DRAW:
				realPacketSizeDwords = onPacketPrivateDraw(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_PRIV_DISPATCH:
				realPacketSizeDwords = onPacketPrivateDispatch(opcode, packetBuffer, packetSizeDwords);
				break;
			default:
				LOG_WARN("unknown opcode 0x%08X", opcode);
				break;
			}

			if (m_lastPacketDone)
			{
				break;
			}

			command = command + realPacketSizeDwords;
			processedCmdSize += realPacketSizeDwords;
		}
		ret = true;
	} while (false);
	return ret;
}

//////////////////////////////////////////////////////////////////////////
uint32_t GnmCmdStreamDraw::onPacketBase(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	uint32_t opSub = OPCODE_SUB(packetBuffer);
	uint32_t realPktSizeDwords = 0;
	do 
	{
		switch (opSub)
		{
		case OP_SUB_SET_USER_DATA_REGION:
			realPktSizeDwords = onSetUserDataRegion(opcode, packetBuffer, packetSizeInDwords);
			break;
		case OP_SUB_PREPARE_FLIP_VOID:
		case OP_SUB_PREPARE_FLIP_LABEL:
		case OP_SUB_PREPARE_FLIP_WITH_EOP_INTERRUPT_VOID:
		case OP_SUB_PREPARE_FLIP_WITH_EOP_INTERRUPT_LABEL:
			realPktSizeDwords = onPrepareFlipOrEopInterrupt(opcode, packetBuffer, packetSizeInDwords);
			break;

		// setPsShaderUsage with 0 numItems
		case OP_SUB_SET_PS_SHADER_USAGE:
			realPktSizeDwords = packetSizeInDwords;
			break;

		default:
			realPktSizeDwords = packetSizeInDwords;
			break;
		}
	} while (false);
	return realPktSizeDwords;
}

uint32_t GnmCmdStreamDraw::onPacketBaseIndirectArgs(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketIndexCount(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketZPassPredication(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketPredication(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketIndexBuffer(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketIndexSize(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketNumInstances(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketWriteStreamoutBuffer(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketWriteDataInline(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketSemaphore(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketWaitOn(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketChainCommandBuffer(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketStallCommandBufferParser(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketEvent(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketEndOfPipe(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	uint32_t value1 = packetBuffer[1];
	uint32_t loDword = packetBuffer[2];
	uint32_t hiDword = packetBuffer[3] & 0xFFFF;
	uint32_t value3 = packetBuffer[3];
	
	value1 = value1 - 0x500;

	// TODO:
	// this is a GPU relative address lacking of the highest byte (masked by 0xFFFFFFFFF8 or 0xFFFFFFFFFC)
	// I'm not sure this relative to what, maybe to the command buffer.
	uint64_t relaGpuAddr = (((uint64_t)hiDword << 32) | loDword);
	void* gpuAddr = (void*)(((uint64_t)packetBuffer & 0x0000FF0000000000) | relaGpuAddr);
	uint64_t immValue = *((uint64_t*)packetBuffer + 2);

	uint8_t eventType = value1 & 0x3F;
	uint8_t cachePolicy = (value1 >> 25) & 0x03;
	uint8_t cacheAction = (value1 >> 12) & 0x3F;

	uint8_t dstSel = ((value3 >> 16) & 1) | ((value1 >> 23) & 0b10);
	uint8_t srcSel = (value3 >> 29) & 0b111;

	if (value3 & 0x02000000)
	{
		m_dcb->writeAtEndOfPipeWithInterrupt((EndOfPipeEventType)eventType, 
			(EventWriteDest)dstSel, gpuAddr, 
			(EventWriteSource)srcSel, immValue,
			(CacheAction)cacheAction, (CachePolicy)cachePolicy);
	}
	else
	{
		m_dcb->writeAtEndOfPipe((EndOfPipeEventType)eventType,
			(EventWriteDest)dstSel, gpuAddr,
			(EventWriteSource)srcSel, immValue,
			(CacheAction)cacheAction, (CachePolicy)cachePolicy);
	}
	
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketEndOfShader(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketGpuFacility(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketWaitForGraphicsWrites(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	//m_dcb.waitForGraphicsWrites();
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketPause(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketConfigRegister(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketSetContext(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	uint8_t opInfo = OPCODE_INFO(opcode);
	uint32_t opLen = OPCODE_LENGTH(opcode);
	uint32_t opSub = OPCODE_SUB(packetBuffer);
	do 
	{
		switch (opSub)
		{
		case OP_SUB_SET_PS_SHADER_USAGE:
		{
			const uint32_t* inputTable = &packetBuffer[2];
			uint32_t numItems = opLen - 2;
			m_dcb->setPsShaderUsage(inputTable, numItems);
		}
			break;
		default:
			break;
		}
	} while (false);
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketSetResource(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	uint8_t opInfo = OPCODE_INFO(opcode);
	uint32_t opLen = OPCODE_LENGTH(opcode);
	uint32_t opSub = OPCODE_SUB(packetBuffer);
	
	do 
	{
		if (opLen == 3)
		{
			if (opSub == OP_SUB_SET_COMPUTE_SHADER_CONTROL)
			{
				//m_dcb.setComputeShaderControl();
			}
			else if (opSub == OP_SUB_SET_COMPUTE_SCRATCH_SIZE)
			{
				//m_dcb.setComputeScratchSize();
			}
			else if (opInfo == 0 && opSub >= 0x0C && opSub <= 0x14C)
			{
				// non cs
				//m_dcb.setUserData()
			}
			else if (opInfo == 2 && opSub == 0x240)
			{
				//cs
				//m_dcb.setUserData();
			}
			else if (opSub >= 0x216 && opSub <= 0x21A)
			{
				//m_dcb.setComputeResourceManagement()
			}
		}
		else if (opLen == 4)
		{
			if (opSub >= 0x0C && opSub <= 0x240)
			{
				ShaderStage stage; 
				uint32_t slot = 0;

				uint32_t value = packetBuffer[1];
				void* gpuAddr = (void*)*((uint64_t*)packetBuffer + 1);
				if (opInfo == 2)
				{
					stage = kShaderStageCs;
				}
				else
				{
					stage = (ShaderStage)(((value & 0xFFFFFFE0) >> 4) / 4 + 1);
				}
				uint32_t stageBase = s_stageBases[stage];
				slot = value + 0x2C00 - stageBase;
				m_dcb->setPointerInUserData(stage, slot, gpuAddr);
			}
		}
		else
		{
			LOG_WARN("op not processed, op %08X opLen %d", opcode, opLen);
		}

	} while (false);
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketSet3(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketIncrementDeCounter(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketWaitOnCe(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketWaitForSetupDispatchDrawKickRingBuffer(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketDispatchDraw(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketRequestMipStatsReportAndReset(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

//

uint32_t GnmCmdStreamDraw::onPacketPrivateDraw(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	OpSubPrivateDraw opInfo = (OpSubPrivateDraw)OPCODE_INFO(opcode);
	switch (opInfo)
	{
	case OP_INFO_DRAW_INITIALIZE_DEFAULT_HARDWARE_STATE:
		break;
	case OP_INFO_DRAW_INITIALIZE_TO_DEFAULT_CONTEXT_STATE:
		break;
	case OP_INFO_SET_EMBEDDED_VS_SHADER:
		break;
	case OP_INFO_SET_EMBEDDED_PS_SHADER:
		break;
	case OP_INFO_SET_VS_SHADER:
	{
		GnmCmdVSShader* param = (GnmCmdVSShader*)packetBuffer;
		m_dcb->setVsShader(&param->vsRegs, param->modifier);
	}
		break;
	case OP_INFO_SET_PS_SHADER:
	{
		GnmCmdPSShader* param = (GnmCmdPSShader*)packetBuffer;
		m_dcb->setPsShader(&param->psRegs);
	}
		break;
	case OP_INFO_SET_ES_SHADER:
		break;
	case OP_INFO_SET_GS_SHADER:
		break;
	case OP_INFO_SET_HS_SHADER:
		break;
	case OP_INFO_SET_LS_SHADER:
		break;
	case OP_INFO_UPDATE_GS_SHADER:
		break;
	case OP_INFO_UPDATE_HS_SHADER:
		break;
	case OP_INFO_UPDATE_PS_SHADER:
		break;
	case OP_INFO_UPDATE_VS_SHADER:
		break;
	case OP_INFO_SET_VGT_CONTROL:
		break;
	case OP_INFO_RESET_VGT_CONTROL:
		break;
	case OP_INFO_DRAW_INDEX:
	{
		GnmCmdDrawIndex* param = (GnmCmdDrawIndex*)packetBuffer;
		DrawModifier modifier = { 0 };
		modifier.renderTargetSliceOffset = (param->predAndMod >> 29) & 0b111;
		if (!modifier.renderTargetSliceOffset)
		{
			m_dcb->drawIndex(param->indexCount, (const void*)param->indexAddr);
		}
		else
		{
			m_dcb->drawIndex(param->indexCount, (const void*)param->indexAddr, modifier);
		}
	}
		break;
	case OP_INFO_DRAW_INDEX_AUTO:
		break;
	case OP_INFO_DRAW_INDEX_INDIRECT:
		break;
	case OP_INFO_DRAW_INDEX_INDIRECT_COUNT_MULTI:
		break;
	case OP_INFO_DRAW_INDEX_MULTI_INSTANCED:
		break;
	case OP_INFO_DRAW_INDEX_OFFSET:
		break;
	case OP_INFO_DRAW_INDIRECT:
		break;
	case OP_INFO_DRAW_INDIRECT_COUNT_MULTI:
		break;
	case OP_INFO_DRAW_OPAQUE_AUTO:
		break;
	case OP_INFO_WAIT_UNTIL_SAFE_FOR_RENDERING:
		break;
	default:
		break;
	}
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketPrivateDispatch(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}


//////////////////////////////////////////////////////////////////////////
uint32_t GnmCmdStreamDraw::onSetUserDataRegion(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	uint32_t realPktSizeDwords = 0;
	uint32_t* packet = packetBuffer + 2;
	do 
	{
		uint32_t numDwords = 0;
		uint8_t opType = OPCODE_TYPE(*packet);
		if (opType == OP_TYPE_SET_RESOURCE)
		{
			numDwords = OPCODE_LENGTH(*packet);
		}
		else if (opType == OP_TYPE_BASE)
		{
			numDwords = 0;
		}
		else
		{
			LOG_ERR("error packet.");
		}

		//m_dcb.setUserDataRegion()
		realPktSizeDwords = 2 + numDwords;

	} while (false);
	return realPktSizeDwords;
}

uint32_t GnmCmdStreamDraw::onPrepareFlipOrEopInterrupt(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	uint32_t opSub = OPCODE_SUB(packetBuffer);

	void* labelAddr = (void*)*((uint64_t*)packetBuffer + 1);
	uint32_t value = packetBuffer[4];

	switch (opSub)
	{
	case OP_SUB_PREPARE_FLIP_VOID:
		break;
	case OP_SUB_PREPARE_FLIP_LABEL:
	{
		m_dcb->prepareFlip(labelAddr, value);
	}
		break;
	case OP_SUB_PREPARE_FLIP_WITH_EOP_INTERRUPT_VOID:
		break;
	case OP_SUB_PREPARE_FLIP_WITH_EOP_INTERRUPT_LABEL:
		break;
	default:
		break;
	}

	// mark this is the last packet in cmd buff.
	m_lastPacketDone = true;

	return packetSizeInDwords;
}

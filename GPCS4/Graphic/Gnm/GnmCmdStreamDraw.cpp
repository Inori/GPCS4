#include "GnmCmdStreamDraw.h"
#include "GnmOpCode.h"

GnmCmdStreamDraw::GnmCmdStreamDraw()
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
		if (count != 1)
		{
			LOG_ASSERT("currently only support 1 cmdbuff.");
		}

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

		while (processedCmdSize < commandSize)
		{
			opcode = *command;
			cmdType = OPCODE_TYPE(opcode);
			packetSizeDwords = OPCODE_LENGTH(opcode);
			packetBuffer = command;

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
			case OP_TYPE_SET_1:
				realPacketSizeDwords = onPacketSet1(opcode, packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_SET_2:
				realPacketSizeDwords = onPacketSet2(opcode, packetBuffer, packetSizeDwords);
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
				LOG_WARN("unknows opcode %X", opcode);
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

uint32_t GnmCmdStreamDraw::onPacketSet1(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketSet2(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
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
				//m_dcb.setPointerInUserData();
			}
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
		break;
	case OP_INFO_SET_PS_SHADER:
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
		if (opType == OP_TYPE_SET_2)
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

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
				realPacketSizeDwords = onPacketBase(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_BASE_INDIRECT_ARGS:
				realPacketSizeDwords = onPacketBaseIndirectArgs(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_INDEX_COUNT:
				realPacketSizeDwords = onPacketIndexCount(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_Z_PASS_PREDICATION:
				realPacketSizeDwords = onPacketZPassPredication(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_PREDICATION:
				realPacketSizeDwords = onPacketPredication(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_INDEX_BUFFER:
				realPacketSizeDwords = onPacketIndexBuffer(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_INDEX_SIZE:
				realPacketSizeDwords = onPacketIndexSize(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_NUM_INSTANCES:
				realPacketSizeDwords = onPacketNumInstances(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_WRITE_STREAMOUT_BUFFER:
				realPacketSizeDwords = onPacketWriteStreamoutBuffer(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_WRITE_DATA_INLINE:
				realPacketSizeDwords = onPacketWriteDataInline(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_SEMAPHORE:
				realPacketSizeDwords = onPacketSemaphore(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_WAIT_ON:
				realPacketSizeDwords = onPacketWaitOn(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_CHAIN_COMMAND_BUFFER:
				realPacketSizeDwords = onPacketChainCommandBuffer(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_STALL_COMMAND_BUFFER_PARSER:
				realPacketSizeDwords = onPacketStallCommandBufferParser(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_EVENT:
				realPacketSizeDwords = onPacketEvent(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_END_OF_PIPE:
				realPacketSizeDwords = onPacketEndOfPipe(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_END_OF_SHADER:
				realPacketSizeDwords = onPacketEndOfShader(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_GPU_FACILITY:
				realPacketSizeDwords = onPacketGpuFacility(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_WAIT_FOR_GRAPHICS_WRITES:
				realPacketSizeDwords = onPacketWaitForGraphicsWrites(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_PAUSE:
				realPacketSizeDwords = onPacketPause(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_CONFIG_REGISTER:
				realPacketSizeDwords = onPacketConfigRegister(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_SET_1:
				realPacketSizeDwords = onPacketSet1(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_SET_2:
				realPacketSizeDwords = onPacketSet2(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_SET_3:
				realPacketSizeDwords = onPacketSet3(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_INCREMENT_DE_COUNTER:
				realPacketSizeDwords = onPacketIncrementDeCounter(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_WAIT_ON_CE:
				realPacketSizeDwords = onPacketWaitOnCe(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_WAIT_FOR_SETUP_DISPATCH_DRAW_KICK_RING_BUFFER:
				realPacketSizeDwords = onPacketWaitForSetupDispatchDrawKickRingBuffer(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_DISPATCH_DRAW:
				realPacketSizeDwords = onPacketDispatchDraw(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_REQUEST_MIP_STATS_REPORT_AND_RESET:
				realPacketSizeDwords = onPacketRequestMipStatsReportAndReset(packetBuffer, packetSizeDwords);
				break;

			// private types

			case OP_TYPE_PRIV_SHARED:
				realPacketSizeDwords = onPacketPrivateShared(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_PRIV_DRAW:
				realPacketSizeDwords = onPacketPrivateDraw(packetBuffer, packetSizeDwords);
				break;
			case OP_TYPE_PRIV_DISPATCH:
				realPacketSizeDwords = onPacketPrivateDispatch(packetBuffer, packetSizeDwords);
				break;
			default:
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

uint32_t GnmCmdStreamDraw::onPacketBase(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketBaseIndirectArgs(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketIndexCount(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketZPassPredication(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketPredication(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketIndexBuffer(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketIndexSize(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketNumInstances(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketWriteStreamoutBuffer(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketWriteDataInline(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketSemaphore(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketWaitOn(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketChainCommandBuffer(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketStallCommandBufferParser(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketEvent(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketEndOfPipe(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketEndOfShader(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketGpuFacility(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketWaitForGraphicsWrites(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketPause(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketConfigRegister(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketSet1(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketSet2(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketSet3(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketIncrementDeCounter(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketWaitOnCe(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketWaitForSetupDispatchDrawKickRingBuffer(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketDispatchDraw(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketRequestMipStatsReportAndReset(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

//////////////////////////////////////////////////////////////////////////


uint32_t GnmCmdStreamDraw::onPacketPrivateDraw(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

uint32_t GnmCmdStreamDraw::onPacketPrivateDispatch(uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

#pragma once

#include "GnmCommon.h"
#include "GnmOpCode.h"
#include "GnmCommandBuffer.h"

#include <memory>

// This class takes all the reverse engining work, parsing PM4 packets (aka command buffer),
// restore the original high level Gnm API calls, and the forward to CnmCommandBufferXXX class,
// we handle graphic staffs there.
// This way we gain the game developers' original idea without
// interacting with low level command buffers.
// 
// You should open IDA Pro on libSceGnm.a when read/write code here.
//
// The parsing process takes references from:
// AMD manual: Radeon Southern Islands Acceleration
// PAL: https://github.com/GPUOpen-Drivers/pal

class GnmCmdStream
{
public:
	GnmCmdStream(std::shared_ptr<GnmCommandBuffer>& cb);
	virtual ~GnmCmdStream();

	bool processCommandBuffer(uint32_t* commandBuffer, uint32_t commandSize);

	std::shared_ptr<GnmCommandBuffer> getCommandBuffer();

private:
	
	void processPM4Type0(PPM4_TYPE_0_HEADER pm4Hdr, uint32_t* regDataX);
	void processPM4Type3(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);

	// Type 3 pm4 packet handlers
	void onNop(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onSetBase(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onIndexBufferSize(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onSetPredication(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onCondExec(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onIndexBase(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onIndexType(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onNumInstances(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onStrmoutBufferUpdate(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onWriteData(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onMemSemaphore(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onWaitRegMem(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onIndirectBuffer(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onPfpSyncMe(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onEventWrite(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onEventWriteEop(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onEventWriteEos(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onDmaData(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onAcquireMem(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onRewind(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onSetConfigReg(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onSetContextReg(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onSetShReg(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onSetUconfigReg(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onIncrementDeCounter(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onWaitOnCeCounter(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onDispatchDrawPreambleGfx09(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onDispatchDrawGfx09(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onGetLodStatsGfx09(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);

	// Private
	void onGnmPrivate(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	// Legacy packets used in old SDKs.
	void onGnmLegacy(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);

	// Parsing methods
	void onPrepareFlipOrEopInterrupt(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onDrawIndex(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onDrawIndexAuto(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onSetViewport(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onSetRenderTarget(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);
	void onSetDepthRenderTarget(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);

	template <typename HdrType>
	HdrType getNextNPm4(HdrType thisPm4, uint32_t n)
	{
		HdrType curPm4 = thisPm4;
		while (n)
		{
			curPm4 = reinterpret_cast<HdrType>((uint32_t*)curPm4 + PM4_LENGTH_DW(curPm4->u32All));
			--n;
		}
		return curPm4;
	}

	template <typename HdrType>
	HdrType getNextPm4(HdrType thisPm4)
	{
		return getNextNPm4<HdrType>(thisPm4, 1);
	}


private:
	std::shared_ptr<GnmCommandBuffer> m_cb;

	// Flip packet is the last pm4 packet of a command buffer,
	// when flip packet had been processed, we end processing command buffer.
	bool m_flipPacketDone = false;

	// Used for recording hint, usually provided by IT_NOP
	// Note: This MUST clear to 0 every time after we read it.
	uint32_t m_lastHint = 0;

	// Some Gnm calls formed with several pm4 packets
	// so after recover that call, we need to skip N packets.
	// NOte:
	// This should be the the real pm4 packet count which forms a gnm call minus one.
	// e.g. 2 packets makes gnm call, m_skipPm4Count = 1
	uint32_t m_skipPm4Count = 0;

};




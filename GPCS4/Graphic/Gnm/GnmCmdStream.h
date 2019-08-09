#pragma once

#include "GPCS4Common.h"
#include "GnmOpCode.h"
#include "GnmCommandBuffer.h"

#include <memory>

class GnmCmdStream
{
public:
	GnmCmdStream(std::shared_ptr<GnmCommandBuffer>& cb);
	virtual ~GnmCmdStream();

	bool processCommandBuffer(uint32_t* commandBuffer, uint32_t commandSize);

private:
	
	uint32_t processPM4Type0(PPM4_TYPE_0_HEADER pm4Hdr, uint32_t* regDataX);
	uint32_t processPM4Type3(PPM4_TYPE_3_HEADER pm4Hdr, uint32_t* itBody);

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

	// Private Handlers


private:
	std::shared_ptr<GnmCommandBuffer> m_cb;
};




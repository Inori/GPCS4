#pragma once

#include "GPCS4Common.h"
#include "GnmCmdStream.h"

class GnmCmdStreamDraw : public GnmCmdStream
{
public:
	GnmCmdStreamDraw();
	virtual ~GnmCmdStreamDraw();

	bool processCommandBuffer(uint32_t count, void* dcbGpuAddrs[], uint32_t* dcbSizesInBytes);
	
private:

	bool processCommandBuffer(uint32_t* commandBuffer, uint32_t commandSize);

	uint32_t onPacketBase(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketBaseIndirectArgs(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketIndexCount(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketZPassPredication(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketPredication(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketIndexBuffer(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketIndexSize(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketNumInstances(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketWriteStreamoutBuffer(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketWriteDataInline(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketSemaphore(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketWaitOn(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketChainCommandBuffer(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketStallCommandBufferParser(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketEvent(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketEndOfPipe(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketEndOfShader(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketGpuFacility(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketWaitForGraphicsWrites(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketPause(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketConfigRegister(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketSet1(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketSet2(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketSet3(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketIncrementDeCounter(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketWaitOnCe(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketWaitForSetupDispatchDrawKickRingBuffer(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketDispatchDraw(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketRequestMipStatsReportAndReset(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	//
	uint32_t onPacketPrivateShared(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketPrivateDraw(uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketPrivateDispatch(uint32_t* packetBuffer, uint32_t packetSizeInDwords);

};




#pragma once

#include "GPCS4Common.h"
#include "GnmCmdStream.h"
#include "GnmCommandBufferDraw.h"
#include <memory>

// The command buffer itself is a PM4Packet queue. 
// Since PS4 use a unified memory architecture,
// there no device abstraction in Gnm like Vulkan or DirectX does.
// The high level Gnm API calls are pretty thin functions, just record the parameters passed in,
// building one or more PM4 packets, then insert it in the queue (command buffer),
// no kernel or hardware interaction occurs except submit calls.
// And as for submit calls, they are thin functions too in UserMode,
// they just pass the command buffer to the kernel using ioctl.
// All the actual works is done in kernel after submit calls  (but the submit calls themselves are async calls).
// 
// Our goal is to recover the high level Gnm API calls using the PM4 packets in command buffers.
//
// Each PM4 packet has a 32 bits opcode header, some have a sub opcode in the next dword.
// Most packets have a fixed length, but some special packets have a variable length, such as NOP, PAUSE
//
// The format of opcode is as follow:
// flag  | length-2  |   type   |   info
// 31 30 | 29 ... 16 | 15 ... 8 | 7 ... 0
//
// If flag == 0b11, it's a normal packet, if opcode == 0x80000000, it's a nop packet.
//
//
// I don't know why Sony's engineers designed such a complex command buffer,
// we can not dispatch the packet in a straight forward way like a single switch statement.
// I'm pretty sure current implement is far from Sony does.


class GnmCmdStreamDraw : public GnmCmdStream
{
public:
	GnmCmdStreamDraw(std::shared_ptr<GnmCommandBufferDraw> dcb);
	virtual ~GnmCmdStreamDraw();

	bool processCommandBuffer(uint32_t count, void* dcbGpuAddrs[], uint32_t* dcbSizesInBytes);
	
private:

	bool processCommandBuffer(uint32_t* commandBuffer, uint32_t commandSize);

	//
	uint32_t onPacketBase(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketBaseIndirectArgs(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketIndexCount(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketZPassPredication(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketPredication(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketIndexBuffer(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketIndexSize(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketNumInstances(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketWriteStreamoutBuffer(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketWriteDataInline(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketSemaphore(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketWaitOn(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketChainCommandBuffer(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketStallCommandBufferParser(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketEvent(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketEndOfPipe(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketEndOfShader(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketGpuFacility(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketWaitForGraphicsWrites(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketPause(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketConfigRegister(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketSet1(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketSet2(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketSet3(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketIncrementDeCounter(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketWaitOnCe(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketWaitForSetupDispatchDrawKickRingBuffer(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketDispatchDraw(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketRequestMipStatsReportAndReset(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);

	//
	uint32_t onPacketPrivateDraw(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	uint32_t onPacketPrivateDispatch(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);

	//
	uint32_t onSetUserDataRegion(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);
	// deal with all prepareFlip and prepareFlipWithEopInterrupt
	uint32_t onPrepareFlipOrEopInterrupt(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);

private:
	static const uint32_t s_stageBases[kShaderStageCount];

private:
	bool m_lastPacketDone = false;  // last packet marker, usually prepareFlip packet.
	std::shared_ptr<GnmCommandBufferDraw> m_dcb;
};




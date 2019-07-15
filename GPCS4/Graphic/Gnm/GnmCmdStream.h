#pragma once

#include "GPCS4Common.h"


class GnmCmdStream
{
public:
	GnmCmdStream();
	virtual ~GnmCmdStream();

protected:
	uint32_t onPacketPrivateShared(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords);

};




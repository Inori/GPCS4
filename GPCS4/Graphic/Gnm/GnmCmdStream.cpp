#include "GnmCmdStream.h"

GnmCmdStream::GnmCmdStream()
{

}

GnmCmdStream::~GnmCmdStream()
{

}

uint32_t GnmCmdStream::onPacketPrivateShared(uint32_t opcode, uint32_t* packetBuffer, uint32_t packetSizeInDwords)
{
	return packetSizeInDwords;
}

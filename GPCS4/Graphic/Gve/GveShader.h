#pragma once

#include "GveCommon.h"
#include "../Pssl/PsslKey.h"
#include "../SpirV/SpirvCodeBuffer.h"
#include "../SpirV/SpirvCompression.h"

namespace gve
{;

class GveShader : public RcObject
{
	using PsslKey = pssl::PsslKey;
	using SpirvCodeBuffer = pssl::SpirvCodeBuffer;
	using SpirvCompressedBuffer = pssl::SpirvCompressedBuffer;

public:
	GveShader(VkShaderStageFlagBits stage,
			  SpirvCodeBuffer code,
			  const PsslKey& key);
	virtual ~GveShader();

private:
	void dumpShader();
	
private:
	VkShaderStageFlagBits m_stage;
	SpirvCompressedBuffer m_code;
	PsslKey m_key;
};



}  // namespace gve

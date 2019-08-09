#pragma once

#include "GPCS4Common.h"
#include "GnmCommandBuffer.h"

#include "../Gve/GveShader.h"
#include <vector>

// This class is designed for graphic development,
// no reverse engining knowledge should be required.

class GnmCommandBufferDraw : public GnmCommandBuffer
{
private:
	typedef std::vector<std::pair<uint32_t, void*>> UDSTVector;

public:
	GnmCommandBufferDraw();
	virtual ~GnmCommandBufferDraw();


	virtual void drawIndex(uint32_t indexCount, const void *indexAddr, DrawModifier modifier);
	virtual void drawIndex(uint32_t indexCount, const void *indexAddr);

	virtual void prepareFlip(void *labelAddr, uint32_t value);

	
	virtual void setPsShaderUsage(const uint32_t *inputTable, uint32_t numItems);
	virtual void setPsShader(const pssl::PsStageRegisters *psRegs);
	virtual void setVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier);

	virtual void setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const Buffer *buffer);
	virtual void setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const Texture *tex);
	virtual void setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const Sampler *sampler);
	virtual void setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t *userData, uint32_t numDwords);
	virtual void setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void *gpuAddr);
	virtual void writeAtEndOfPipe(EndOfPipeEventType eventType,
		EventWriteDest dstSelector, void *dstGpuAddr,
		EventWriteSource srcSelector, uint64_t immValue,
		CacheAction cacheAction, CachePolicy cachePolicy);

	virtual void writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType,
		EventWriteDest dstSelector, void *dstGpuAddr,
		EventWriteSource srcSelector, uint64_t immValue,
		CacheAction cacheAction, CachePolicy cachePolicy);

private:
	uint32_t* getFetchShaderCode(void* vsCode);
	void clearRenderState();

private:

	void* m_vsCode;
	void* m_psCode;
	UDSTVector m_vsUserDataSlotTable;
	UDSTVector m_psUserDataSlotTable;

	RcPtr<gve::GveShader> m_vsShader;
	RcPtr<gve::GveShader> m_psShader;
};



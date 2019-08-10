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

	virtual void initializeDefaultHardwareState() override;

	virtual void setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmBuffer *buffer) override;
	virtual void setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmTexture *tex) override;
	virtual void setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmSampler *sampler) override;
	virtual void setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t *userData, uint32_t numDwords) override;
	virtual void setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void *gpuAddr) override;

	virtual void setPsShaderUsage(const uint32_t *inputTable, uint32_t numItems) override;

	virtual void setVgtControl(uint8_t primGroupSizeMinusOne) override;
	virtual void setVgtControl(uint8_t primGroupSizeMinusOne, VgtPartialVsWaveMode partialVsWaveMode) override;

	virtual void setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier) override;

	virtual void setPsShader(const pssl::PsStageRegisters *psRegs);
	virtual void setVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier) override;
	virtual void updatePsShader(const pssl::PsStageRegisters *psRegs) override;
	virtual void updateVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier) override;

	virtual void drawIndex(uint32_t indexCount, const void *indexAddr, DrawModifier modifier) override;
	virtual void drawIndex(uint32_t indexCount, const void *indexAddr) override;
	virtual void drawIndexAuto(uint32_t indexCount, DrawModifier modifier) override;
	virtual void drawIndexAuto(uint32_t indexCount) override;

	virtual void prepareFlip(void *labelAddr, uint32_t value) override;

	virtual void writeAtEndOfPipe(EndOfPipeEventType eventType,
		EventWriteDest dstSelector, void *dstGpuAddr,
		EventWriteSource srcSelector, uint64_t immValue,
		CacheAction cacheAction, CachePolicy cachePolicy) override;

	virtual void writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType,
		EventWriteDest dstSelector, void *dstGpuAddr,
		EventWriteSource srcSelector, uint64_t immValue,
		CacheAction cacheAction, CachePolicy cachePolicy) override;

	virtual void waitUntilSafeForRendering(uint32_t videoOutHandle, uint32_t displayBufferIndex) override;

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



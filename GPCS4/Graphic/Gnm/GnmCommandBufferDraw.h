#pragma once

#include "GnmCommon.h"
#include "GnmCommandBuffer.h"
#include "GnmConstant.h"
#include "GnmContextState.h"

#include <vector>

//
class GnmBuffer;


// This class is designed for graphics development,
// no reverse engining knowledge should be required.
// It's responsible for mapping Gnm input/structures to Gve input/structures,
// and convert Gnm calls into Gve calls.

class GnmCommandBufferDraw : public GnmCommandBuffer
{
	using PsslShaderResource = pssl::PsslShaderResource;
	using VertexInputSemantic = pssl::VertexInputSemantic;
public:
	GnmCommandBufferDraw(
		const sce::SceGpuQueueDevice& device,
		const RcPtr<gve::GveContext>& context
	);

	virtual ~GnmCommandBufferDraw();

	virtual void initializeDefaultHardwareState() override;

	virtual void setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const VSharpBuffer *buffer) override;
	virtual void setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const TSharpBuffer *tex) override;
	virtual void setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const SSharpBuffer *sampler) override;
	virtual void setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t *userData, uint32_t numDwords) override;
	virtual void setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void *gpuAddr) override;

	virtual void setPsShaderUsage(const uint32_t *inputTable, uint32_t numItems) override;
	virtual void setViewport(uint32_t viewportId, float dmin, float dmax, const float scale[3], const float offset[3]) override;

	virtual void setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier) override;
	virtual void setVgtControl(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode,
		VgtPartialVsWaveMode partialVsWaveMode) override;

	virtual void setPsShader(const pssl::PsStageRegisters *psRegs) override;
	virtual void setVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier) override;
	virtual void updatePsShader(const pssl::PsStageRegisters *psRegs) override;
	virtual void updateVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier) override;

	virtual void setPrimitiveType(PrimitiveType primType) override;

	virtual void drawIndex(uint32_t indexCount, const void *indexAddr, DrawModifier modifier) override;
	virtual void drawIndex(uint32_t indexCount, const void *indexAddr) override;
	virtual void drawIndexAuto(uint32_t indexCount, DrawModifier modifier) override;
	virtual void drawIndexAuto(uint32_t indexCount) override;

	virtual void prepareFlip() override;
	virtual void prepareFlip(void *labelAddr, uint32_t value) override;
	virtual void prepareFlipWithEopInterrupt(EndOfPipeEventType eventType,
		void *labelAddr, uint32_t value,
		CacheAction cacheAction) override;

	virtual void writeAtEndOfPipe(EndOfPipeEventType eventType,
		EventWriteDest dstSelector, void *dstGpuAddr,
		EventWriteSource srcSelector, uint64_t immValue,
		CacheAction cacheAction, CachePolicy cachePolicy) override;

	virtual void writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType,
		EventWriteDest dstSelector, void *dstGpuAddr,
		EventWriteSource srcSelector, uint64_t immValue,
		CacheAction cacheAction, CachePolicy cachePolicy) override;

	virtual void waitUntilSafeForRendering(uint32_t videoOutHandle, uint32_t displayBufferIndex) override;

	virtual void setViewportTransformControl(ViewportTransformControl vportControl) override;

	virtual void setScreenScissor(int32_t left, int32_t top, int32_t right, int32_t bottom) override;

	virtual void setGuardBands(float horzClip, float vertClip, float horzDiscard, float vertDiscard) override;

	virtual void setHardwareScreenOffset(uint32_t offsetX, uint32_t offsetY) override;

	virtual void setRenderTarget(uint32_t rtSlot, GnmRenderTarget const *target) override;

	virtual void setDepthRenderTarget(DepthRenderTarget const *depthTarget) override;

	virtual void setRenderTargetMask(uint32_t mask) override;

	virtual void setDepthStencilControl(DepthStencilControl depthStencilControl) override;

	virtual void setBlendControl(uint32_t rtSlot, BlendControl blendControl) override;

	virtual void setPrimitiveSetup(PrimitiveSetup primSetup) override;

	virtual void setActiveShaderStages(ActiveShaderStages activeStages) override;

	virtual void setIndexSize(IndexSize indexSize, CachePolicy cachePolicy) override;

	virtual void dispatch(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ) override;

	virtual void waitOnAddress(void* gpuAddr, uint32_t mask, WaitCompareFunc compareFunc, uint32_t refValue) override;

	virtual void waitOnAddressAndStallCommandBufferParser(void* gpuAddr, uint32_t mask, uint32_t refValue) override;

	virtual void dispatchWithOrderedAppend(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ, DispatchOrderedAppendMode orderedAppendMode) override;

	virtual void setCsShader(const pssl::CsStageRegisters* csRegs, uint32_t shaderModifier) override;

	virtual void flushShaderCachesAndWait(CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode) override;

	virtual void setDbRenderControl(DbRenderControl reg) override;

	virtual void setDepthClearValue(float clearValue) override;

	virtual void setStencilClearValue(uint8_t clearValue) override;

	virtual void waitForGraphicsWrites(uint32_t baseAddr256, uint32_t sizeIn256ByteBlocks, uint32_t targetMask, CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode) override;

	virtual void writeAtEndOfShader(EndOfShaderEventType eventType, void* dstGpuAddr, uint32_t immValue) override;

	virtual void writeDataInline(void* dstGpuAddr, const void* data, uint32_t sizeInDwords, WriteDataConfirmMode writeConfirm) override;

	virtual void writeDataInlineThroughL2(void* dstGpuAddr, const void* data, uint32_t sizeInDwords, CachePolicy cachePolicy, WriteDataConfirmMode writeConfirm) override;

	virtual void writeReleaseMemEventWithInterrupt(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy) override;

	virtual void writeReleaseMemEvent(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy) override;

private:
	
	uint32_t* getFetchShaderCode(const GnmShaderContext& vsCtx);

	void setUserDataSlots(ShaderStage stage, uint32_t startSlot, 
		const uint32_t* data, uint32_t numDwords);

	void clearUserDataSlots();

	void insertUniqueUserDataSlot(
		std::vector<pssl::PsslShaderResource>& container,
		uint32_t                               startSlot,
		pssl::PsslShaderResource&              shaderRes);

	RcPtr<gve::GveImageView> getDepthTarget(const DepthRenderTarget* depthTarget);

	// Stages setup
	void commitVsStage();
	void commitPsStage();
	void commitCsStage();

	void bindFrameBuffer();
	void bindIndexBuffer(const void* indexAddr, uint32_t indexCount);
	void bindImmConstBuffer(const PsslShaderResource& res, pssl::PsslProgramType prgType);
	bool bindVertexBuffer(uint32_t bindingId, const GnmBuffer& vsharp);
	void setVertexInputLayout(
		const PsslShaderResource& res,
		const std::vector<VertexInputSemantic>& inputSemantics);
	void bindVertexBuffers(
		const PsslShaderResource& res, 
		const std::vector<VertexInputSemantic>& inputSemantics);
	void bindImmResource(const PsslShaderResource& res);
	void bindSampler(const PsslShaderResource& res);

private:

	gve::GveSharpResourceManager& m_sharpRes;

	GnmContextState  m_state;
	GnmShaderContext m_vsContext;
	GnmShaderContext m_psContext;
	GnmShaderContext m_csContext;

	VkIndexType m_indexType = VK_INDEX_TYPE_UINT16;

};



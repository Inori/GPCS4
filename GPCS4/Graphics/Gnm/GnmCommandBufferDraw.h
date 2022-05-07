#pragma once

#include "GnmCommandBuffer.h"
#include "GnmCommon.h"
#include "GnmRenderState.h"

#include "Gcn/GcnShaderBinary.h"

namespace sce::gcn
{
	class GcnModule;
}  // namespace sce::gcn

namespace sce::Gnm
{


	// This class is designed for graphics development,
	// no reverse engineering knowledge should be required.
	// It's responsible for mapping Gnm input/structures to Violet input/structures,
	// and convert Gnm calls into Violet calls.

	class GnmCommandBufferDraw : public GnmCommandBuffer
	{
	public:
		GnmCommandBufferDraw(vlt::VltDevice* device);

		virtual ~GnmCommandBufferDraw();

		virtual void initializeDefaultHardwareState() override;

		virtual void setViewportTransformControl(ViewportTransformControl vportControl) override;

		virtual void setPrimitiveSetup(PrimitiveSetup reg) override;

		virtual void setScreenScissor(int32_t left, int32_t top, int32_t right, int32_t bottom) override;

		virtual void setViewport(uint32_t viewportId, float dmin, float dmax, const float scale[3], const float offset[3]) override;

		virtual void setHardwareScreenOffset(uint32_t offsetX, uint32_t offsetY) override;

		virtual void setGuardBands(float horzClip, float vertClip, float horzDiscard, float vertDiscard) override;

		virtual void setPsShaderUsage(const uint32_t* inputTable, uint32_t numItems) override;

		virtual void setActiveShaderStages(ActiveShaderStages activeStages) override;

		virtual void setPsShader(const gcn::PsStageRegisters* psRegs) override;

		virtual void updatePsShader(const gcn::PsStageRegisters* psRegs) override;

		virtual void setVsShader(const gcn::VsStageRegisters* vsRegs, uint32_t shaderModifier) override;

		virtual void setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier) override;

		virtual void updateVsShader(const gcn::VsStageRegisters* vsRegs, uint32_t shaderModifier) override;

		virtual void setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const Buffer* buffer) override;

		virtual void setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const Texture* tex) override;

		virtual void setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const Sampler* sampler) override;

		virtual void setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void* gpuAddr) override;

		virtual void setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t* userData, uint32_t numDwords) override;

		virtual void setRenderTarget(uint32_t rtSlot, RenderTarget const* target) override;

		virtual void setDepthRenderTarget(DepthRenderTarget const* depthTarget) override;

		virtual void setDepthClearValue(float clearValue) override;

		virtual void setStencilClearValue(uint8_t clearValue) override;

		virtual void setRenderTargetMask(uint32_t mask) override;

		virtual void setBlendControl(uint32_t rtSlot, BlendControl blendControl) override;

		virtual void setDepthStencilControl(DepthStencilControl depthControl) override;

		virtual void setDbRenderControl(DbRenderControl reg) override;

		virtual void setVgtControl(uint8_t primGroupSizeMinusOne) override;

		virtual void setPrimitiveType(PrimitiveType primType) override;

		virtual void setIndexSize(IndexSize indexSize, CachePolicy cachePolicy) override;

		virtual void drawIndexAuto(uint32_t indexCount, DrawModifier modifier) override;

		virtual void drawIndexAuto(uint32_t indexCount) override;

		virtual void drawIndex(uint32_t indexCount, const void* indexAddr, DrawModifier modifier) override;

		virtual void drawIndex(uint32_t indexCount, const void* indexAddr) override;

		virtual void dispatch(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ) override;

		virtual void dispatchWithOrderedAppend(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ, DispatchOrderedAppendMode orderedAppendMode) override;

		virtual void writeDataInline(void* dstGpuAddr, const void* data, uint32_t sizeInDwords, WriteDataConfirmMode writeConfirm) override;

		virtual void writeDataInlineThroughL2(void* dstGpuAddr, const void* data, uint32_t sizeInDwords, CachePolicy cachePolicy, WriteDataConfirmMode writeConfirm) override;

		virtual void writeAtEndOfPipe(EndOfPipeEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy) override;

		virtual void writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy) override;

		virtual void writeAtEndOfShader(EndOfShaderEventType eventType, void* dstGpuAddr, uint32_t immValue) override;

		virtual void waitOnAddress(void* gpuAddr, uint32_t mask, WaitCompareFunc compareFunc, uint32_t refValue) override;

		virtual void waitOnAddressAndStallCommandBufferParser(void* gpuAddr, uint32_t mask, uint32_t refValue) override;

		virtual void flushShaderCachesAndWait(CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode) override;

		virtual void waitUntilSafeForRendering(uint32_t videoOutHandle, uint32_t displayBufferIndex) override;

		virtual void prepareFlip() override;

		virtual void prepareFlip(void* labelAddr, uint32_t value) override;

		virtual void prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, CacheAction cacheAction) override;

		virtual void prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, void* labelAddr, uint32_t value, CacheAction cacheAction) override;

		virtual void setCsShader(const gcn::CsStageRegisters* computeData, uint32_t shaderModifier) override;

		virtual void writeReleaseMemEventWithInterrupt(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy) override;

		virtual void writeReleaseMemEvent(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy) override;

		virtual void setVgtControlForNeo(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode) override;

		virtual void waitForGraphicsWrites(uint32_t baseAddr256, uint32_t sizeIn256ByteBlocks, uint32_t targetMask, CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode) override;

		virtual void setDepthStencilDisable() override;



	private:

		int32_t findUsageRegister(
			const gcn::GcnShaderResourceTable& table,
			uint32_t                           usage);

		const void* findFetchShader(
			const gcn::GcnShaderResourceTable& table,
			const UserDataArray&               userData);

		const uint32_t* findUserData(
			const gcn::GcnShaderResource& res,
			uint32_t                      eudIndex,
			const UserDataArray&          userData);

		bool isSingleVertexBinding(
			const uint32_t*                      vtxTable,
			const gcn::VertexInputSemanticTable& semanticTable);

		vlt::Rc<vlt::VltBuffer> generateIndexBuffer(
			const void* data,
			uint32_t    size);

		vlt::Rc<vlt::VltBuffer> generateIndexBufferAuto(
			uint32_t indexCount);

		inline void bindVertexBuffer(
			const Buffer* vsharp, uint32_t binding);

		void bindResourceBuffer(
			const Buffer*         vsharp,
			uint32_t              startRegister,
			VkBufferUsageFlags    usage,
			VkPipelineStageFlags2 stage,
			VkAccessFlagBits2     access);

		void bindResourceImage(
			const Texture*        tsharp,
			uint32_t              startRegister,
			VkImageUsageFlags     usage,
			VkPipelineStageFlags2 stage,
			VkAccessFlagBits2     access,
			VkImageTiling         tiling,
			VkImageLayout         layout);

		void bindResourceSampler(
			const Sampler*        ssharp,
			uint32_t              startRegister,
			VkPipelineStageFlags2 stage);

		void bindResource(
			VkPipelineStageFlags               stage,
			const gcn::GcnShaderResourceTable& table,
			const UserDataArray&               userData);

		void updateVertexBinding(gcn::GcnModule& vsModule);

		void updateVertexShaderStage();
		void updatePixelShaderStage();

		void commitGraphicsState();
		void commitComputeState();

		void onPrepareFlip();

		ShaderStage getShaderStage(
			VkPipelineStageFlags pipeStage);

		void updateMetaBufferInfo(
			VkPipelineStageFlags stage,
			uint32_t             startRegister,
			const Buffer*        vsharp);

		void updateMetaTextureInfo(
			VkPipelineStageFlags stage,
			uint32_t             startRegister,
			bool                 isDepth,
			const Texture*       tsharp);

		SceBuffer getResourceBuffer(
			const GnmBufferCreateInfo& info);

	private:
		GnmGraphicsState m_state;
		GnmContextFlags  m_flags; 
	};

}  // namespace sce::Gnm

#include "GnmCommandBufferDraw.h"

#include "GnmSharpBuffer.h"
#include "GnmBuffer.h"
#include "GnmTexture.h"
#include "GnmSampler.h"
#include "GnmConvertor.h"

#include "GpuAddress/GnmGpuAddress.h"

#include "../Gve/GveCmdList.h"
#include "../Gve/GveShader.h"
#include "../Gve/GveBuffer.h"
#include "../Gve/GveImage.h"
#include "../Gve/GveSampler.h"
#include "../Gve/GveContext.h"
#include "../Gve/GvePresenter.h"
#include "../Gve/GveSharpResourceManager.h"
#include "../Pssl/PsslShaderModule.h"

#include <algorithm>

LOG_CHANNEL(Graphic.Gnm.GnmCommandBufferDraw);

using namespace gve;
using namespace pssl;

GnmCommandBufferDraw::GnmCommandBufferDraw(
	const sce::SceGpuQueueDevice& device,
	const RcPtr<gve::GveContext>& context) :
	GnmCommandBuffer(device, context),
	m_sharpRes(m_device->getSharpResManager())
{
	
}

GnmCommandBufferDraw::~GnmCommandBufferDraw()
{
}

void GnmCommandBufferDraw::initializeDefaultHardwareState()
{
	
}

void GnmCommandBufferDraw::setViewportTransformControl(ViewportTransformControl vportControl)
{
	
}

void GnmCommandBufferDraw::setPrimitiveSetup(PrimitiveSetup reg)
{
	
}

void GnmCommandBufferDraw::setScreenScissor(int32_t left, int32_t top, int32_t right, int32_t bottom)
{
	
}

void GnmCommandBufferDraw::setViewport(uint32_t viewportId, float dmin, float dmax, const float scale[3], const float offset[3])
{
	
}

void GnmCommandBufferDraw::setHardwareScreenOffset(uint32_t offsetX, uint32_t offsetY)
{
	
}

void GnmCommandBufferDraw::setGuardBands(float horzClip, float vertClip, float horzDiscard, float vertDiscard)
{
	
}

void GnmCommandBufferDraw::setPsShaderUsage(const uint32_t* inputTable, uint32_t numItems)
{
	
}

void GnmCommandBufferDraw::setActiveShaderStages(ActiveShaderStages activeStages)
{
	
}

void GnmCommandBufferDraw::setPsShader(const pssl::PsStageRegisters* psRegs)
{
	
}

void GnmCommandBufferDraw::updatePsShader(const pssl::PsStageRegisters* psRegs)
{
	
}

void GnmCommandBufferDraw::setVsShader(const pssl::VsStageRegisters* vsRegs, uint32_t shaderModifier)
{
	
}

void GnmCommandBufferDraw::setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier)
{
	
}

void GnmCommandBufferDraw::updateVsShader(const pssl::VsStageRegisters* vsRegs, uint32_t shaderModifier)
{
	
}

void GnmCommandBufferDraw::setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmBuffer* buffer)
{
	
}

void GnmCommandBufferDraw::setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmTexture* tex)
{
	
}

void GnmCommandBufferDraw::setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmSampler* sampler)
{
	
}

void GnmCommandBufferDraw::setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void* gpuAddr)
{
	
}

void GnmCommandBufferDraw::setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t* userData, uint32_t numDwords)
{
	
}

void GnmCommandBufferDraw::setRenderTarget(uint32_t rtSlot, GnmRenderTarget const* target)
{
	
}

void GnmCommandBufferDraw::setDepthRenderTarget(DepthRenderTarget const* depthTarget)
{
	
}

void GnmCommandBufferDraw::setDepthClearValue(float clearValue)
{
	
}

void GnmCommandBufferDraw::setStencilClearValue(uint8_t clearValue)
{
	
}

void GnmCommandBufferDraw::setRenderTargetMask(uint32_t mask)
{
	
}

void GnmCommandBufferDraw::setBlendControl(uint32_t rtSlot, BlendControl blendControl)
{
	
}

void GnmCommandBufferDraw::setDepthStencilControl(DepthStencilControl depthControl)
{
	
}

void GnmCommandBufferDraw::setDbRenderControl(DbRenderControl reg)
{
	
}

void GnmCommandBufferDraw::setVgtControl(uint8_t primGroupSizeMinusOne)
{
	
}

void GnmCommandBufferDraw::setPrimitiveType(PrimitiveType primType)
{
	
}

void GnmCommandBufferDraw::setIndexSize(IndexSize indexSize, CachePolicy cachePolicy)
{
	
}

void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount, DrawModifier modifier)
{
	
}

void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount)
{
	
}

void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void* indexAddr, DrawModifier modifier)
{
	
}

void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void* indexAddr)
{
	
}

void GnmCommandBufferDraw::dispatch(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ)
{
	
}

void GnmCommandBufferDraw::dispatchWithOrderedAppend(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ, DispatchOrderedAppendMode orderedAppendMode)
{
	
}

void GnmCommandBufferDraw::writeDataInline(void* dstGpuAddr, const void* data, uint32_t sizeInDwords, WriteDataConfirmMode writeConfirm)
{
	
}

void GnmCommandBufferDraw::writeDataInlineThroughL2(void* dstGpuAddr, const void* data, uint32_t sizeInDwords, CachePolicy cachePolicy, WriteDataConfirmMode writeConfirm)
{
	
}

void GnmCommandBufferDraw::writeAtEndOfPipe(EndOfPipeEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy)
{
	emuWriteGpuLabel(srcSelector, dstGpuAddr, immValue);
}

void GnmCommandBufferDraw::writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy)
{
	emuWriteGpuLabel(srcSelector, dstGpuAddr, immValue);
}

void GnmCommandBufferDraw::writeAtEndOfShader(EndOfShaderEventType eventType, void* dstGpuAddr, uint32_t immValue)
{
	
}

void GnmCommandBufferDraw::waitOnAddress(void* gpuAddr, uint32_t mask, WaitCompareFunc compareFunc, uint32_t refValue)
{
	
}

void GnmCommandBufferDraw::waitOnAddressAndStallCommandBufferParser(void* gpuAddr, uint32_t mask, uint32_t refValue)
{
	
}

void GnmCommandBufferDraw::waitForGraphicsWrites(uint32_t baseAddr256, uint32_t sizeIn256ByteBlocks, uint32_t targetMask, CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode)
{
	
}

void GnmCommandBufferDraw::flushShaderCachesAndWait(CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode)
{
	
}

void GnmCommandBufferDraw::waitUntilSafeForRendering(uint32_t videoOutHandle, uint32_t displayBufferIndex)
{
	
}

void GnmCommandBufferDraw::prepareFlip()
{
	
}

void GnmCommandBufferDraw::prepareFlip(void* labelAddr, uint32_t value)
{
	*(uint32_t*)labelAddr = value;
}

void GnmCommandBufferDraw::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, CacheAction cacheAction)
{
	
}

void GnmCommandBufferDraw::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, void* labelAddr, uint32_t value, CacheAction cacheAction)
{
	*(uint32_t*)labelAddr = value;
}

void GnmCommandBufferDraw::setCsShader(const pssl::CsStageRegisters* computeData, uint32_t shaderModifier)
{
	
}

void GnmCommandBufferDraw::writeReleaseMemEventWithInterrupt(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy)
{
	
}

void GnmCommandBufferDraw::writeReleaseMemEvent(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy)
{
	
}

void GnmCommandBufferDraw::setVgtControlForNeo(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode)
{
	
}


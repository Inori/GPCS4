#include "GnmCommandBufferDraw.h"
#include "GnmSharpBuffer.h"

#include "Platform/PlatformUtils.h"

#include <algorithm>

using namespace gve;
using namespace pssl;

GnmCommandBufferDraw::GnmCommandBufferDraw(const RcPtr<GveDevice>& device,
	const RcPtr<GveContex>& context,
	GveResourceManager* resMgr,
	const GveRenderTarget& renderTarget) :
	GnmCommandBuffer(device, context, resMgr),
	m_renderTarget(renderTarget),
	m_vsCode(nullptr),
	m_psCode(nullptr)
{

}


GnmCommandBufferDraw::~GnmCommandBufferDraw()
{
}


void GnmCommandBufferDraw::prepareFlip(void *labelAddr, uint32_t value)
{
	*(uint32_t*)labelAddr = value;
	m_context->endRecording();
}

void GnmCommandBufferDraw::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, void *labelAddr, uint32_t value, CacheAction cacheAction)
{
	*(uint32_t*)labelAddr = value;
	m_context->endRecording();
}

void GnmCommandBufferDraw::setPsShaderUsage(const uint32_t *inputTable, uint32_t numItems)
{

}

void GnmCommandBufferDraw::setViewport(uint32_t viewportId, float dmin, float dmax, const float scale[3], const float offset[3])
{
	float width = scale[0] / 0.5f;
	float height = -scale[1] / 0.5f;
	float left = offset[0] - scale[0];
	float top = offset[1] + scale[1];

	VkViewport viewport;
	viewport.x = left;
	viewport.y = top;
	viewport.width = width;
	viewport.height = height;
	viewport.minDepth = dmin;
	viewport.maxDepth = dmax;

	VkRect2D scissor;
	scissor.offset.x = viewport.x;
	scissor.offset.y = viewport.y;
	scissor.extent.width = viewport.width;
	scissor.extent.height = viewport.height;

	m_context->setViewport(viewport, scissor);
}

void GnmCommandBufferDraw::setPsShader(const pssl::PsStageRegisters *psRegs)
{
	m_psCode = psRegs->getCodeAddress();
}

void GnmCommandBufferDraw::setVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier)
{
	m_vsCode = vsRegs->getCodeAddress();
}

void GnmCommandBufferDraw::setVgtControl(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode)
{
	
}

void GnmCommandBufferDraw::setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const VSharpBuffer *buffer)
{
	onSetUserDataRegister(stage, startUserDataSlot, (uint32_t*)buffer, sizeof(VSharpBuffer)/sizeof(uint32_t));
}

void GnmCommandBufferDraw::setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const TSharpBuffer *tex)
{
	onSetUserDataRegister(stage, startUserDataSlot, (uint32_t*)tex, sizeof(TSharpBuffer) / sizeof(uint32_t));
}

void GnmCommandBufferDraw::setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const SSharpBuffer *sampler)
{
	onSetUserDataRegister(stage, startUserDataSlot, (uint32_t*)sampler, sizeof(SSharpBuffer) / sizeof(uint32_t));
}

void GnmCommandBufferDraw::setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void *gpuAddr)
{
	onSetUserDataRegister(stage, startUserDataSlot, (uint32_t*)gpuAddr, sizeof(void*) / sizeof(uint32_t));
}

void GnmCommandBufferDraw::setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t *userData, uint32_t numDwords)
{
	onSetUserDataRegister(stage, startUserDataSlot, userData, numDwords);
}


void GnmCommandBufferDraw::writeAtEndOfPipe(EndOfPipeEventType eventType, 
	EventWriteDest dstSelector, void *dstGpuAddr, 
	EventWriteSource srcSelector, uint64_t immValue, 
	CacheAction cacheAction, CachePolicy cachePolicy)
{
	if (srcSelector == kEventWriteSource32BitsImmediate)
	{
		*(uint32_t*)dstGpuAddr = immValue;
	}
	else if (srcSelector == kEventWriteSource64BitsImmediate)
	{
		*(uint64_t*)dstGpuAddr = immValue;
	}
	else
	{
		*(uint64_t*)dstGpuAddr = UtilProcess::GetProcessTimeCounter();
	}
}

void GnmCommandBufferDraw::writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType, 
	EventWriteDest dstSelector, void *dstGpuAddr, 
	EventWriteSource srcSelector, uint64_t immValue, 
	CacheAction cacheAction, CachePolicy cachePolicy)
{
	if (srcSelector == kEventWriteSource32BitsImmediate)
	{
		*(uint32_t*)dstGpuAddr = immValue;
	}
	else if (srcSelector == kEventWriteSource64BitsImmediate)
	{
		*(uint64_t*)dstGpuAddr = immValue;
	}
	else
	{
		*(uint64_t*)dstGpuAddr = UtilProcess::GetProcessTimeCounter();
	}
}


void GnmCommandBufferDraw::waitUntilSafeForRendering(uint32_t videoOutHandle, uint32_t displayBufferIndex)
{
	
}


void GnmCommandBufferDraw::setViewportTransformControl(ViewportTransformControl vportControl)
{

}

void GnmCommandBufferDraw::setScreenScissor(int32_t left, int32_t top, int32_t right, int32_t bottom)
{

}

void GnmCommandBufferDraw::setGuardBands(float horzClip, float vertClip, float horzDiscard, float vertDiscard)
{

}

void GnmCommandBufferDraw::setHardwareScreenOffset(uint32_t offsetX, uint32_t offsetY)
{

}

void GnmCommandBufferDraw::setRenderTarget(uint32_t rtSlot, RenderTarget const *target)
{
	m_context->bindRenderTargets(m_renderTarget);
}

void GnmCommandBufferDraw::setDepthRenderTarget(DepthRenderTarget const *depthTarget)
{

}

void GnmCommandBufferDraw::setRenderTargetMask(uint32_t mask)
{

}

void GnmCommandBufferDraw::setDepthStencilControl(DepthStencilControl depthStencilControl)
{
	
}

void GnmCommandBufferDraw::setBlendControl(uint32_t rtSlot, BlendControl blendControl)
{

}

void GnmCommandBufferDraw::setPrimitiveSetup(PrimitiveSetup reg)
{

}

void GnmCommandBufferDraw::setActiveShaderStages(ActiveShaderStages activeStages)
{

}

void GnmCommandBufferDraw::setIndexSize(IndexSize indexSize, CachePolicy cachePolicy)
{

}

// Will be called on every frame start.
void GnmCommandBufferDraw::initializeDefaultHardwareState()
{
	m_context->beginRecording(m_cmd);
}

void GnmCommandBufferDraw::setPrimitiveType(PrimitiveType primType)
{
	VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	switch (primType)
	{
	case kPrimitiveTypeTriList:
		topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		break;
	default:
		break;
	}
	
	m_context->setPrimitiveType(topology);
}

void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void *indexAddr, DrawModifier modifier)
{
	do
	{
		uint32_t* fsCode = getFetchShaderCode(m_vsCode);
		LOG_ASSERT(fsCode != nullptr, "can not find fetch shader code.");

		pssl::PsslShaderModule vsModule((const uint32_t*)m_vsCode, fsCode, m_vsUserDataSlotTable);
		RcPtr<GveShader> vsShader = vsModule.compile();

		auto inputUsageSlots = vsModule.inputUsageSlots();
		for (const auto& inputSlot : inputUsageSlots)
		{
			auto pred = [&inputSlot](const auto& item)
			{
				return inputSlot.startRegister == item.startSlot;
			};
			auto iter = std::find_if(m_vsUserDataSlotTable.begin(), m_vsUserDataSlotTable.end(), pred);
			switch (inputSlot.usageType)
			{
			case pssl::kShaderInputUsageImmConstBuffer:
			{
				const GnmBuffer* buffer = reinterpret_cast<const GnmBuffer*>(iter->resource);
				GveBufferCreateInfoGnm info;
				info.buffer = *buffer;
				info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
				auto uniformBuffer = m_resourceManager->getBuffer(info, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
				void* data = uniformBuffer->mapPtr(0);
				void* addr = buffer->getBaseAddress();
				uint32_t size = buffer->getSize();
				memcpy(data, addr, size);
				uint32_t regSlot = computeResBinding(VertexShader, inputSlot.startRegister);
				m_context->bindResourceBuffer(regSlot, uniformBuffer);
			}
				break;
			case kShaderInputUsagePtrVertexBufferTable:
			{
				const GnmBuffer* vertexTable = reinterpret_cast<const GnmBuffer*>(iter->resource);
				GveBufferCreateInfoVk stagingInfo;
				stagingInfo.size = vertexTable->getSize();
				stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;



			}
				break;
			default:
				break;
			}
		}
		

		pssl::PsslShaderModule psModule((const uint32_t*)m_psCode, m_psUserDataSlotTable);
		RcPtr<GveShader> psShader = psModule.compile();

		m_context->bindShader(VK_SHADER_STAGE_VERTEX_BIT, vsShader);
		m_context->bindShader(VK_SHADER_STAGE_FRAGMENT_BIT, psShader);
	} while (false);
}

void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void *indexAddr)
{
	DrawModifier mod = { 0 };
	drawIndex(indexCount, indexAddr, mod);
}

void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount, DrawModifier modifier)
{
}

void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount)
{
}

void GnmCommandBufferDraw::setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier)
{
	
}

void GnmCommandBufferDraw::updatePsShader(const pssl::PsStageRegisters *psRegs)
{
	
}

void GnmCommandBufferDraw::updateVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier)
{
	
}


uint32_t* GnmCommandBufferDraw::getFetchShaderCode(void* vsCode)
{
	uint32_t* fsCode = nullptr;
	do 
	{
		uint32_t fsStartReg = pssl::getFetchShaderStartRegister((const uint8_t*)vsCode);
		if (fsStartReg == UINT_MAX)
		{
			break;
		}

		for (const auto& res : m_vsUserDataSlotTable)
		{
			if (res.startSlot != fsStartReg)
			{
				continue;
			}

			fsCode = (uint32_t*)res.resource;
			break;
		}

	} while (false);
	return fsCode;
}

void GnmCommandBufferDraw::onSetUserDataRegister(ShaderStage stage, uint32_t startSlot, const uint32_t* data, uint32_t numDwords)
{
	do 
	{
		if (!data || !numDwords)
		{
			break;
		}

		pssl::PsslShaderResource shaderRes = { startSlot, data, numDwords };

		switch (stage)
		{
		case kShaderStageVs:
			insertUniqueShaderResource(m_vsUserDataSlotTable, startSlot, shaderRes);
			break;
		case kShaderStagePs:
			insertUniqueShaderResource(m_psUserDataSlotTable, startSlot, shaderRes);
			break;
		default:
			break;
		}

	} while (false);
}

void GnmCommandBufferDraw::insertUniqueShaderResource(UDSTVector& container, uint32_t startSlot, pssl::PsslShaderResource& shaderRes)
{
	auto pred = [startSlot](const pssl::PsslShaderResource& item)
	{
		return item.startSlot == startSlot;
	};

	auto iter = std::find_if(container.begin(), container.end(), pred);
	if (iter == container.end())
	{
		container.push_back(shaderRes);
	}
	else
	{
		*iter = shaderRes;
	}
}

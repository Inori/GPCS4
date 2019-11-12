#include "GnmCommandBufferDraw.h"
#include "GnmSharpBuffer.h"
#include "GnmBuffer.h"
#include "GnmTexture.h"
#include "GnmSampler.h"
#include "../Gve/GveBuffer.h"
#include "../Gve/GveImage.h"
#include "../Gve/GveSampler.h"
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
	GveBlendControl bc;
	bc.enableBlending = blendControl.blendEnable;
	bc.colorSrcFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	bc.colorBlendOp = VK_BLEND_OP_ADD;
	bc.colorDstFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	bc.alphaSrcFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	bc.alphaBlendOp = VK_BLEND_OP_ADD;
	bc.alphaDstFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	bc.writeMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	m_context->setBlendControl(bc);
}

void GnmCommandBufferDraw::setPrimitiveSetup(PrimitiveSetup reg)
{
	GveRasterizerState rs;
	rs.polygonMode = VK_POLYGON_MODE_FILL;
	//rs.cullMode = reg.cullMode == kPrimitiveSetupCullFaceBack ? VK_CULL_MODE_BACK_BIT : VK_CULL_MODE_FRONT_BIT;
	rs.cullMode = VK_CULL_MODE_NONE;
	rs.frontFace = reg.frontFace == kPrimitiveSetupFrontFaceCcw ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
	rs.depthBiasEnable = VK_FALSE;
	rs.depthClampEnable = VK_FALSE;
	m_context->setRasterizerState(rs);

	GveMultisampleState ms;
	ms.sampleCount = VK_SAMPLE_COUNT_1_BIT;
	ms.sampleShadingEnable = VK_FALSE;
	m_context->setMultiSampleState(ms);
}

void GnmCommandBufferDraw::setActiveShaderStages(ActiveShaderStages activeStages)
{

}

void GnmCommandBufferDraw::setIndexSize(IndexSize indexSize, CachePolicy cachePolicy)
{
	m_indexSize = indexSize;
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
	typedef struct Vertex
	{
		float x, y, z;	// Position
		float r, g, b;	// Color
		float u, v;		// UVs
	} Vertex;

	m_context->beginRecording(m_cmd);

	do
	{
		uint32_t* fsCode = getFetchShaderCode(m_vsCode);
		LOG_ASSERT(fsCode != nullptr, "can not find fetch shader code.");

		pssl::PsslShaderModule vsModule((const uint32_t*)m_vsCode, fsCode, m_vsUserDataSlotTable);
		RcPtr<GveShader> vsShader = vsModule.compile();

		auto vsInputUsageSlots = vsModule.inputUsageSlots();
		for (const auto& inputSlot : vsInputUsageSlots)
		{
			auto pred = [&inputSlot](const auto& item)
			{
				return inputSlot.startRegister == item.startSlot;
			};
			auto iter = std::find_if(m_vsUserDataSlotTable.begin(), m_vsUserDataSlotTable.end(), pred);
			switch (inputSlot.usageType)
			{
			case kShaderInputUsageImmConstBuffer:
			{
				const GnmBuffer* buffer = reinterpret_cast<const GnmBuffer*>(iter->resource);
				GveBufferCreateInfo info;
				info.size = buffer->getSize();
				info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
				uint64_t key = reinterpret_cast<uint64_t>(buffer->getBaseAddress());
				auto uniformBuffer = m_resourceManager->createBufferVsharp(info, key, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
				void* data = uniformBuffer->mapPtr(0);
				void* addr = buffer->getBaseAddress();
				uint32_t size = buffer->getSize();
				memcpy(data, addr, size);
				uint32_t regSlot = computeConstantBufferBinding(VertexShader, inputSlot.startRegister);
				m_context->bindResourceBuffer(regSlot, uniformBuffer);
			}
				break;
			case kShaderInputUsagePtrVertexBufferTable:
			{
				const GnmBuffer* vertexTable = reinterpret_cast<const GnmBuffer*>(iter->resource);
				VkDeviceSize bufferSize = vertexTable->getSize();
				GveBufferCreateInfo stagingInfo;
				stagingInfo.size = bufferSize;
				stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				auto stagingBuffer = m_resourceManager->createBuffer(stagingInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

				void* data = vertexTable->getBaseAddress();
				void* addr = stagingBuffer->mapPtr(0);
				memcpy(addr, data, bufferSize);

				GveBufferCreateInfo vtxInfo;
				vtxInfo.size = bufferSize;
				vtxInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
				uint64_t key = reinterpret_cast<uint64_t>(vertexTable->getBaseAddress());
				auto vertexBuffer = m_resourceManager->createBufferVsharp(vtxInfo, key, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
				m_context->copyBuffer(vertexBuffer->handle(), stagingBuffer->handle(), bufferSize);
				m_context->bindVertexBuffer(0, vertexBuffer, vertexTable->getStride());

				VkVertexInputBindingDescription bindingDescription = {};
				bindingDescription.binding = 0;
				bindingDescription.stride = vertexTable->getStride();
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

				attributeDescriptions[0].binding = 0;
				attributeDescriptions[0].location = 0;
				attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[0].offset = offsetof(Vertex, x);

				attributeDescriptions[1].binding = 0;
				attributeDescriptions[1].location = 1;
				attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[1].offset = offsetof(Vertex, r);;

				attributeDescriptions[2].binding = 0;
				attributeDescriptions[2].location = 2;
				attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
				attributeDescriptions[2].offset = offsetof(Vertex, u);;

				m_context->setInputLayout(attributeDescriptions.size(), attributeDescriptions.data(),
					1, &bindingDescription);
			}
				break;
			default:
				break;
			}
		}
		

		pssl::PsslShaderModule psModule((const uint32_t*)m_psCode, m_psUserDataSlotTable);
		RcPtr<GveShader> psShader = psModule.compile();

		auto psInputUsageSlots = psModule.inputUsageSlots();
		for (const auto& inputSlot : psInputUsageSlots)
		{
			auto pred = [&inputSlot](const auto& item)
			{
				return inputSlot.startRegister == item.startSlot;
			};
			auto iter = std::find_if(m_psUserDataSlotTable.begin(), m_psUserDataSlotTable.end(), pred);
			switch (inputSlot.usageType)
			{
			case kShaderInputUsageImmResource:
			{
				const GnmTexture* tsharp = reinterpret_cast<const GnmTexture*>(iter->resource);
				GveImageCreateInfo info;
				info.extent.width = tsharp->getWidth();
				info.extent.height = tsharp->getHeight();
				info.extent.depth = tsharp->getDepth();
				info.mipLevels = 1;
				info.format = VK_FORMAT_R8G8B8A8_UNORM;
				info.tiling = VK_IMAGE_TILING_OPTIMAL;
				info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
				uint64_t key = reinterpret_cast<uint64_t>(tsharp->getBaseAddress());
				auto texture = m_resourceManager->createImageTsharp(info, key, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

				VkDeviceSize bufferSize = tsharp->getSizeAlign().m_size;
				GveBufferCreateInfo stagingInfo;
				stagingInfo.size = bufferSize;
				stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				auto stagingBuffer = m_resourceManager->createBuffer(stagingInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

				void* data = GNM_GPU_ABS_ADDR(indexAddr, tsharp->getBaseAddress());
				void* addr = stagingBuffer->mapPtr(0);
				LOG_DEBUG("addr %p data %p", addr, data);
				memcpy(addr, data, bufferSize);

				m_context->transitionImageLayout(texture->handle(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
				m_context->copyBufferToImage(stagingBuffer->handle(), texture->handle(), tsharp->getWidth(), tsharp->getHeight());
				m_context->transitionImageLayout(texture->handle(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

				GveImageViewCreateInfo viewInfo;
				auto texView = m_resourceManager->createImageViewTsharp(texture, key, viewInfo);
				uint32_t regSlot = computeResBinding(PixelShader, inputSlot.startRegister);
				m_context->bindResourceView(regSlot, texView, nullptr);
			}
				break;
			case kShaderInputUsageImmSampler:
			{
				const GnmSampler* ssharp = reinterpret_cast<const GnmSampler*>(iter->resource);
				
				uint64_t key = ssharp->m_regs[0] | ssharp->m_regs[1] << 32;
				GveSamplerCreateInfo info;
				auto sampler = m_resourceManager->createSamplerSsharp(info, key);
				
				uint32_t regSlot = computeSamplerBinding(PixelShader, inputSlot.startRegister);
				m_context->bindSampler(regSlot, sampler);
			}
				break;
			default:
				break;
			}
		}

		m_context->bindShader(VK_SHADER_STAGE_VERTEX_BIT, vsShader);
		m_context->bindShader(VK_SHADER_STAGE_FRAGMENT_BIT, psShader);


		VkDeviceSize indexSize = indexCount * sizeof(uint16_t);
		GveBufferCreateInfo stagingInfo;
		stagingInfo.size = indexSize;
		stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		auto stagingBuffer = m_resourceManager->createBuffer(stagingInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (indexAddr)
		{
			const void* data = indexAddr;
			void* addr = stagingBuffer->mapPtr(0);
			memcpy(addr, data, indexSize);

			GveBufferCreateInfo indexInfo;
			indexInfo.size = indexSize;
			indexInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			uint64_t key = reinterpret_cast<uint64_t>(indexAddr);
			auto indexBuffer = m_resourceManager->createBufferVsharp(indexInfo, key, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			m_context->copyBuffer(indexBuffer->handle(), stagingBuffer->handle(), indexSize);
			m_context->bindIndexBuffer(indexBuffer, VK_INDEX_TYPE_UINT16);
		}


		m_context->drawIndex(indexCount, 0);

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
	DrawModifier mod = { 0 };
	drawIndex(indexCount, nullptr, mod);
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

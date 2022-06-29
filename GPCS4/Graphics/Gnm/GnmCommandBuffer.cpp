#include "GnmCommandBuffer.h"

#include "Emulator.h"
#include "GnmRenderState.h"
#include "GnmGpuLabel.h"
#include "VirtualGPU.h"

#include "Gcn/GcnShaderRegField.h"
#include "Gcn/GcnUtil.h"
#include "Sce/SceGpuQueue.h"
#include "Sce/SceResource.h"
#include "Sce/SceResourceTracker.h"
#include "Sce/SceLabelManager.h"
#include "Violet/VltCmdList.h"
#include "Violet/VltDevice.h"

#include <fstream>

using namespace sce::vlt;
using namespace sce::gcn;

LOG_CHANNEL(Graphic.Gnm.GnmCommandBuffer);

namespace sce::Gnm
{

	GnmCommandBuffer::GnmCommandBuffer(vlt::VltDevice* device) :
		m_device(device),
		m_factory(device)
	{
		initGcnModuleInfo();
	}

	GnmCommandBuffer::~GnmCommandBuffer()
	{
	}

	Rc<VltCommandList> GnmCommandBuffer::finalize()
	{
		return m_context->endRecording();
	}

	void GnmCommandBuffer::initializeDefaultHardwareState()
	{
		m_tracker      = &(GPU().resourceTracker());
		m_labelManager = &(GPU().labelManager());
	}

	void GnmCommandBuffer::writeDataInline(void* dstGpuAddr, const void* data, uint32_t sizeInDwords, WriteDataConfirmMode writeConfirm)
	{
		auto label = m_labelManager->getLabel(dstGpuAddr);
		if (sizeInDwords == 1)
		{
			label->set(*reinterpret_cast<const uint32_t*>(data));
		}
		else if (sizeInDwords == 2)
		{
			label->set(*reinterpret_cast<const uint64_t*>(data));
		}

		std::memcpy(dstGpuAddr, data, sizeInDwords * sizeof(uint32_t));
	}

	const uint32_t* GnmCommandBuffer::findUserData(
		const gcn::GcnShaderResource& res,
		uint32_t                      eudIndex,
		const UserDataArray&          userData)
	{
		const uint32_t* registerData = nullptr;
		if (!res.inEud)
		{
			registerData = &userData[res.startRegister];
		}
		else
		{
			const uintptr_t eudPtr   = *reinterpret_cast<const uintptr_t*>(&userData[eudIndex]);
			const uint32_t* eudTable = reinterpret_cast<const uint32_t*>(eudPtr);
			registerData             = &eudTable[res.eudOffsetInDwords];
		}
		return registerData;
	}

	int32_t GnmCommandBuffer::findUsageRegister(
		const GcnShaderResourceTable& table,
		uint32_t                      usage)
	{
		int32_t regIndex = -1;
		auto    iter     = std::find_if(table.begin(), table.end(),
										[usage](const GcnShaderResource& res)
										{
                                     return res.usage == usage;
										});
		if (iter != table.end())
		{
			regIndex = static_cast<uint32_t>(iter->startRegister);
		}

		return regIndex;
	}

	void GnmCommandBuffer::setCsShader(
		GnmShaderContext&            ctx,
		const gcn::CsStageRegisters* computeData,
		uint32_t                     shaderModifier)
	{
		ctx.code = computeData->getCodeAddress();

		ctx.meta.cs.computeNumThreadX = computeData->computeNumThreadX;
		ctx.meta.cs.computeNumThreadY = computeData->computeNumThreadY;
		ctx.meta.cs.computeNumThreadZ = computeData->computeNumThreadZ;

		const COMPUTE_PGM_RSRC2* rsrc2   = reinterpret_cast<const COMPUTE_PGM_RSRC2*>(&computeData->computePgmRsrc2);
		ctx.meta.cs.userSgprCount        = rsrc2->user_sgpr;
		ctx.meta.cs.enableTgidX          = rsrc2->tgid_x_en;
		ctx.meta.cs.enableTgidY          = rsrc2->tgid_y_en;
		ctx.meta.cs.enableTgidZ          = rsrc2->tgid_z_en;
		ctx.meta.cs.enableTgSize         = rsrc2->tg_size_en;
		ctx.meta.cs.threadIdInGroupCount = rsrc2->tidig_comp_cnt;
		// LDS is allocated in 128 dword blocks on PS4
		constexpr uint32_t LDSAlignShift = 9;
		ctx.meta.cs.ldsSize              = rsrc2->lds_size << LDSAlignShift;
	}

	void GnmCommandBuffer::bindResourceBuffer(
		const Buffer*         vsharp,
		uint32_t              startRegister,
		VkBufferUsageFlags    usage,
		VkPipelineStageFlags2 stage,
		VkAccessFlagBits2     access)
	{

		GnmBufferCreateInfo info;
		info.vsharp = vsharp;
		info.usage  = usage;
		info.stage  = stage;
		info.access = access;

		if (usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
		{
			info.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
							  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}
		else
		{
			info.stage |= VK_PIPELINE_STAGE_TRANSFER_BIT;
			info.memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		}

		SceBuffer buffer   = getResourceBuffer(info);
		auto      progType = gcnProgramTypeFromVkStage(stage);
		uint32_t  slot     = usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT ? computeConstantBufferBinding(
																			  progType, startRegister)
																		: computeResourceBinding(
																			  progType, startRegister);
		m_context->bindResourceBuffer(slot, VltBufferSlice(buffer.buffer));
	}

	void GnmCommandBuffer::bindResourceImage(
		const Texture*        tsharp,
		uint32_t              startRegister,
		VkImageUsageFlags     usage,
		VkPipelineStageFlags2 stage,
		VkAccessFlagBits2     access,
		VkImageTiling         tiling,
		VkImageLayout         layout)
	{
		SceTexture texture;

		GnmImageCreateInfo info;
		info.tsharp     = tsharp;
		info.usage      = usage;
		info.stage      = stage | VK_PIPELINE_STAGE_TRANSFER_BIT;
		info.access     = access;
		info.tiling     = tiling;
		info.layout     = layout;
		info.memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		m_factory.createImage(info, texture);
		m_tracker->track(texture);

		m_initializer->initTexture(texture.image, tsharp);

		uint32_t slot = computeResourceBinding(
			gcnProgramTypeFromVkStage(stage), startRegister);

		m_context->bindResourceView(slot, texture.imageView, nullptr);
	}

	void GnmCommandBuffer::bindResourceSampler(
		const Sampler*        ssharp,
		uint32_t              startRegister,
		VkPipelineStageFlags2 stage)
	{
		SceSampler sampler;
		m_factory.createSampler(ssharp, sampler);

		uint32_t slot = computeSamplerBinding(
			gcnProgramTypeFromVkStage(stage), startRegister);

		m_context->bindResourceSampler(slot, sampler.sampler);
	}

	void GnmCommandBuffer::bindResource(
		VkPipelineStageFlags          stage,
		const GcnShaderResourceTable& table,
		const UserDataArray&          userData)
	{
		// Find EUD
		uint32_t eudIndex = findUsageRegister(table, kShaderInputUsagePtrExtendedUserData);
		for (const auto& res : table)
		{
			switch (res.type)
			{
				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				{
					const Buffer* vsharp = reinterpret_cast<const Buffer*>(findUserData(res, eudIndex, userData));

					bindResourceBuffer(
						vsharp,
						res.startRegister,
						VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
						stage,
						VK_ACCESS_UNIFORM_READ_BIT);

					updateMetaBufferInfo(stage, res.startRegister, vsharp);
				}
					break;
				case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
				{
					const Buffer* vsharp = reinterpret_cast<const Buffer*>(findUserData(res, eudIndex, userData));

					bindResourceBuffer(
						vsharp,
						res.startRegister,
						VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
						stage,
						VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);

					updateMetaBufferInfo(stage, res.startRegister, vsharp);
				}
					break;
				case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
				{
					const Texture* tsharp = reinterpret_cast<const Texture*>(findUserData(res, eudIndex, userData));

					bindResourceImage(
						tsharp,
						res.startRegister,
						VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
						stage,
						VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT,
						VK_IMAGE_TILING_OPTIMAL,
						VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

					updateMetaTextureInfo(stage, res.startRegister, false, tsharp);
				}
					break;
				case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
				{
					const Texture* tsharp = reinterpret_cast<const Texture*>(findUserData(res, eudIndex, userData));

					bindResourceImage(
						tsharp,
						res.startRegister,
						VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
						stage,
						VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT,
						VK_IMAGE_TILING_OPTIMAL,
						VK_IMAGE_LAYOUT_GENERAL);

					updateMetaTextureInfo(stage, res.startRegister, false, tsharp);
				}
					break;
				case VK_DESCRIPTOR_TYPE_SAMPLER:
				{
					const Sampler* ssharp = reinterpret_cast<const Sampler*>(findUserData(res, eudIndex, userData));

					bindResourceSampler(
						ssharp,
						res.startRegister,
						stage);
				}
					break;
				case VK_DESCRIPTOR_TYPE_MAX_ENUM:
					break;
				default:
					LOG_ASSERT(false, "resource type not supported.");
					break;
			}
		}
	}

	void GnmCommandBuffer::commitComputeState(GnmShaderContext& ctx)
	{
		GcnModule csModule(
			GcnProgramType::ComputeShader,
			reinterpret_cast<const uint8_t*>(ctx.code));

		auto& resTable = csModule.getResourceTable();

		// create and bind shader resources
		bindResource(VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, resTable, ctx.userData);

		// bind the shader
		m_context->bindShader(
			VK_SHADER_STAGE_COMPUTE_BIT,
			csModule.compile(ctx.meta, m_moduleInfo));
	}

	ShaderStage GnmCommandBuffer::getShaderStage(
		VkPipelineStageFlags pipeStage)
	{
		ShaderStage shaderStage = kShaderStageCount;
		// clang-format off
		switch (pipeStage)
		{
		case VK_PIPELINE_STAGE_VERTEX_SHADER_BIT: shaderStage = kShaderStageVs; break;
		case VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT: shaderStage = kShaderStagePs; break;
		case VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT: shaderStage = kShaderStageCs; break;
		case VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT: shaderStage = kShaderStageGs; break;
		case VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT: shaderStage = kShaderStageHs; break;
		// Don't know which stage equals domain, and what the fuck is ES LS?
		//case VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT: shaderStage = kShaderStageEs; break;
		default:
			LOG_ASSERT(false, "pipeline stage can not be converted.");
			break;
		}
		// clang-format on
		return shaderStage;
	}

	SceBuffer GnmCommandBuffer::getResourceBuffer(const GnmBufferCreateInfo& info)
	{
		// Lookup or create buffer

		SceBuffer result = {};

		auto  vsharp        = info.vsharp;
		void* bufferAddress = vsharp->getBaseAddress();
		auto  resource      = m_tracker->find(bufferAddress);
		if (resource != nullptr)
		{
			auto type = resource->type();
			if (type.test(SceResourceType::Texture) ||
				type.test(SceResourceType::RenderTarget) ||
				type.test(SceResourceType::DepthRenderTarget))
			{
				// An image backend buffer,
				// we create and fill the buffer,
				// then pending a transform to do be done.

				// We going to copy this buffer to it's original image,
				// so update usage flag.
				auto createInfo = info;
				createInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				m_factory.createBuffer(createInfo, result);

				// upload content
				m_initializer->initBuffer(result.buffer, vsharp);

				resource->setBuffer(result);
				// Pending upload
				resource->setTransform(SceTransformFlag::GpuUpload);
			}
			else
			{
				// An already exist buffer,
				// we don't need to create a new one,
				// just return it.
				result = resource->buffer();
			}
		}
		else
		{
			// A fresh new buffer,
			// we create and fill it's content
			m_factory.createBuffer(info, result);
			// upload content
			m_initializer->initBuffer(result.buffer, vsharp);
			// track the new buffer
			m_tracker->track(result);
		}

		return result;
	}

	GcnBufferMeta GnmCommandBuffer::populateBufferMeta(
		const Buffer* vsharp)
	{
		static const uint32_t indexStrideTable[] = { 8, 16, 32, 64 };
		static const uint32_t elementSizeTable[] = { 2, 4, 8, 16 };

		GcnBufferMeta meta;
		meta.stride      = vsharp->getStride();
		meta.numRecords  = vsharp->getNumElements();
		meta.dfmt        = (Gnm::BufferFormat)vsharp->m_vsharp.dfmt;
		meta.nfmt        = (Gnm::BufferChannelType)vsharp->m_vsharp.nfmt;
		meta.isSwizzle   = vsharp->isSwizzled();
		meta.indexStride = indexStrideTable[vsharp->m_vsharp.index_stride];
		meta.elementSize = elementSizeTable[vsharp->m_vsharp.element_size];
		return meta;
	}

	GcnTextureMeta GnmCommandBuffer::populateTextureMeta(
		const Texture* tsharp,
		bool           isDepth)
	{
		GcnTextureMeta meta;
		meta.textureType = tsharp->getTextureType();
		meta.channelType = tsharp->getTextureChannelType();
		meta.isDepth     = isDepth;
		return meta;
	}

	void GnmCommandBuffer::initGcnModuleInfo()
	{
		const auto& devInfo = m_device->properties();

		const uint32_t amdWavefrontSize       = 64;
		m_moduleInfo.options.separateSubgroup = devInfo.coreSubgroup.subgroupSize < amdWavefrontSize;

		m_moduleInfo.maxComputeSubgroupCount =
			devInfo.core.properties.limits.maxComputeWorkGroupInvocations / devInfo.coreSubgroup.subgroupSize;
	}

	void GnmCommandBuffer::writeReleaseMemEventWithInterrupt(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy)
	{
	}

	void GnmCommandBuffer::writeReleaseMemEvent(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy)
	{
	}

}  // namespace sce::Gnm
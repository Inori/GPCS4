#include "GveRenderPass.h"
#include "GveDevice.h"

LOG_CHANNEL(Graphic.Gve.GveRenderPass);

namespace gve
{;

bool GveRenderPassFormat::operator==(const GveRenderPassFormat& fmt) const
{
	bool equal = sampleCount == fmt.sampleCount;

	for (uint32_t i = 0; i < MaxNumRenderTargets && equal; ++i)
	{
		equal &= (color[i].format == fmt.color[i].format) && (color[i].layout == fmt.color[i].layout);
	}

	equal &= (depth.format == fmt.depth.format) && (depth.layout == fmt.depth.layout);

	return equal;
}

size_t GveRenderPassFormat::hash() const
{
	GveHashState state;
	state.add(uint32_t(sampleCount));

	for (uint32_t i = 0; i < MaxNumRenderTargets; ++i)
	{
		state.add(uint32_t(color[i].format));
		state.add(uint32_t(color[i].layout));
	}

	state.add(uint32_t(depth.format));
	state.add(uint32_t(depth.layout));
	return state;
}

///

GveRenderPass::GveRenderPass(const RcPtr<GveDevice>& device, const GveRenderPassFormat& fmt):
	m_device(device),
	m_format(fmt),
	m_default(createRenderPass(GveRenderPassOps()))
{
}

GveRenderPass::~GveRenderPass()
{
	vkDestroyRenderPass(*m_device, m_default, nullptr);

	for (const auto& i : m_instances) 
	{
		vkDestroyRenderPass(*m_device, i.handle, nullptr);
	}
}
 

bool GveRenderPass::hasCompatibleFormat(const GveRenderPassFormat& fmt) const
{
	return m_format == fmt;
}

VkRenderPass GveRenderPass::getHandle(const GveRenderPassOps& ops)
{
	VkRenderPass rp = VK_NULL_HANDLE;
	do 
	{
		std::lock_guard<Spinlock> lock(m_mutex);

		for (const auto& i : m_instances)
		{
			if (!compareOps(i.ops, ops))
			{
				continue;
			}
			rp = i.handle;
			break;
		}

		if (rp != VK_NULL_HANDLE)
		{
			break;
		}

		rp = createRenderPass(ops);
		m_instances.push_back({ ops, rp });

	} while (false);
	return rp;
}

VkRenderPass GveRenderPass::createRenderPass(const GveRenderPassOps& ops)
{
	VkRenderPass renderPass = VK_NULL_HANDLE;
	do
	{
		std::vector<VkAttachmentDescription> attachments;

		VkAttachmentReference                                  depthRef;
		std::array<VkAttachmentReference, MaxNumRenderTargets> colorRef;
		uint32_t colorRefCount = 0;

		// Render passes may not require the previous
		// contents of the attachments to be preserved.
		for (uint32_t i = 0; i < MaxNumRenderTargets; i++) 
		{
			if (m_format.color[i].format != VK_FORMAT_UNDEFINED) 
			{
				VkAttachmentDescription desc;
				desc.flags = 0;
				desc.format = m_format.color[i].format;
				desc.samples = m_format.sampleCount;
				desc.loadOp = ops.colorOps[i].loadOp;
				desc.storeOp = ops.colorOps[i].storeOp;
				desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				desc.initialLayout = ops.colorOps[i].loadLayout;
				desc.finalLayout = ops.colorOps[i].storeLayout;

				colorRef[i].attachment = attachments.size();
				colorRef[i].layout = m_format.color[i].layout;
				++colorRefCount;

				attachments.push_back(desc);
			}
			else 
			{
				colorRef[i].attachment = VK_ATTACHMENT_UNUSED;
				colorRef[i].layout = VK_IMAGE_LAYOUT_UNDEFINED;
			}
		}

		if (m_format.depth.format != VK_FORMAT_UNDEFINED) 
		{
			VkAttachmentDescription desc;
			desc.flags = 0;
			desc.format = m_format.depth.format;
			desc.samples = m_format.sampleCount;
			desc.loadOp = ops.depthOps.loadOpD;
			desc.storeOp = ops.depthOps.storeOpD;
			desc.stencilLoadOp = ops.depthOps.loadOpS;
			desc.stencilStoreOp = ops.depthOps.storeOpS;
			desc.initialLayout = ops.depthOps.loadLayout;
			desc.finalLayout = ops.depthOps.storeLayout;

			depthRef.attachment = attachments.size();
			depthRef.layout = m_format.depth.layout;

			attachments.push_back(desc);
		}
		else 
		{
			depthRef.attachment = VK_ATTACHMENT_UNUSED;
			depthRef.layout = VK_IMAGE_LAYOUT_UNDEFINED;
		}

		VkSubpassDescription subpass;
		subpass.flags = 0;
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.inputAttachmentCount = 0;
		subpass.pInputAttachments = nullptr;
		subpass.colorAttachmentCount = colorRefCount;
		subpass.pColorAttachments = colorRef.data();
		subpass.pResolveAttachments = nullptr;
		subpass.pDepthStencilAttachment = &depthRef;
		subpass.preserveAttachmentCount = 0;
		subpass.pPreserveAttachments = nullptr;

		if (m_format.depth.format == VK_FORMAT_UNDEFINED)
		{
			subpass.pDepthStencilAttachment = nullptr;
		}

		std::array<VkSubpassDependency, 3> subpassDeps;
		uint32_t                           subpassDepCount = 0;

		if (ops.barrier.srcStages & (
			VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT |
			VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT |
			VK_PIPELINE_STAGE_ALL_COMMANDS_BIT)) 
		{
			subpassDeps[subpassDepCount++] = 
			{ 0, 0,
			  VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT,
			  VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT, /* XXX */
			  VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT,
			  VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT, 0 
			};
		}

		if (ops.barrier.srcStages & (
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT |
			VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT |
			VK_PIPELINE_STAGE_ALL_COMMANDS_BIT)) 
		{
			subpassDeps[subpassDepCount++] = 
			{ 0, 0,
			  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			  VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			  VK_ACCESS_SHADER_READ_BIT,
			  VK_DEPENDENCY_BY_REGION_BIT 
			};
		}

		if (ops.barrier.srcStages && ops.barrier.dstStages) 
		{
			subpassDeps[subpassDepCount++] = 
			{
			  VK_SUBPASS_EXTERNAL, 0,
			  ops.barrier.srcStages,
			  ops.barrier.dstStages,
			  ops.barrier.srcAccess,
			  ops.barrier.dstAccess, 0 
			};
		}

		VkRenderPassCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = 0;
		info.attachmentCount = attachments.size();
		info.pAttachments = attachments.data();
		info.subpassCount = 1;
		info.pSubpasses = &subpass;
		info.dependencyCount = subpassDepCount;
		info.pDependencies = subpassDepCount ? subpassDeps.data() : nullptr;

		if (vkCreateRenderPass(*m_device, &info, nullptr, &renderPass) != VK_SUCCESS) 
		{
			LOG_ERR("Failed to create render pass object");
			break;
		}

	} while (false);
	return renderPass;
}

bool GveRenderPass::compareOps(const GveRenderPassOps& a, const GveRenderPassOps& b)
{
	bool equal = a.barrier.srcStages == b.barrier.srcStages
		&& a.barrier.srcAccess == b.barrier.srcAccess
		&& a.barrier.dstStages == b.barrier.dstStages
		&& a.barrier.dstAccess == b.barrier.dstAccess;

	if (equal) 
	{
		equal &= a.depthOps.loadOpD == b.depthOps.loadOpD
			&& a.depthOps.loadOpS == b.depthOps.loadOpS
			&& a.depthOps.loadLayout == b.depthOps.loadLayout
			&& a.depthOps.storeOpD == b.depthOps.storeOpD
			&& a.depthOps.storeOpS == b.depthOps.storeOpS
			&& a.depthOps.storeLayout == b.depthOps.storeLayout;
	}

	for (uint32_t i = 0; i < MaxNumRenderTargets && equal; i++) 
	{
		equal &= a.colorOps[i].loadOp == b.colorOps[i].loadOp
			&& a.colorOps[i].loadLayout == b.colorOps[i].loadLayout
			&& a.colorOps[i].storeOp == b.colorOps[i].storeOp
			&& a.colorOps[i].storeLayout == b.colorOps[i].storeLayout;
	}

	return equal;
}

GveRenderPassPool::GveRenderPassPool(GveDevice* device):
	m_device(device)
{

}

GveRenderPassPool::~GveRenderPassPool()
{

}

GveRenderPass* GveRenderPassPool::getRenderPass(const GveRenderPassFormat& fmt)
{
	GveRenderPass* renderPass = nullptr;
	do 
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		auto entry = m_renderPasses.find(fmt);
		if (entry != m_renderPasses.end())
		{
			renderPass = &entry->second;
			break;
		}

		auto result = m_renderPasses.emplace(std::piecewise_construct,
			std::tuple(fmt),
			std::tuple(m_device, fmt));
		renderPass = &result.first->second;
	} while (false);
	
	return renderPass;
}

} // namespace gve
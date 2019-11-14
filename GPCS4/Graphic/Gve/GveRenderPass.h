#pragma once

#include "GveCommon.h"
#include "SyncHelper.h"

#include <mutex>

namespace gve
{;

class GveDevice;

/**
 * \brief Format and layout for a render target
 *
 * Stores the image format of the attachment and
 * the image layout that is used while rendering.
 */
struct GveAttachmentFormat 
{
	VkFormat      format = VK_FORMAT_UNDEFINED;
	VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
};


/**
 * \brief Render pass format
 *
 * Stores the attachment formats for all depth and
 * color attachments, as well as the sample count.
 */
struct GveRenderPassFormat
{
	VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;
	GveAttachmentFormat   depth;
	GveAttachmentFormat   color[MaxNumRenderTargets];

	bool operator == (const GveRenderPassFormat& fmt) const;

	size_t hash() const;
};


/**
 * \brief Color attachment transitions
 *
 * Stores the load/store ops and the initial
 * and final layout of a single attachment.
 */
struct GveColorAttachmentOps 
{
	VkAttachmentLoadOp  loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	VkImageLayout       loadLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	VkImageLayout       storeLayout = VK_IMAGE_LAYOUT_GENERAL;
};


/**
 * \brief Depth attachment transitions
 *
 * Stores the load/store ops and the initial and
 * final layout of the depth-stencil attachment.
 */
struct GveDepthAttachmentOps 
{
	VkAttachmentLoadOp  loadOpD = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	VkAttachmentLoadOp  loadOpS = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	VkImageLayout       loadLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	VkAttachmentStoreOp storeOpD = VK_ATTACHMENT_STORE_OP_STORE;
	VkAttachmentStoreOp storeOpS = VK_ATTACHMENT_STORE_OP_STORE;
	VkImageLayout       storeLayout = VK_IMAGE_LAYOUT_GENERAL;
};


/**
 * \brief Render pass barrier
 *
 * External subpass dependency that is to be
 * executed after a render pass has completed.
 */
struct GveRenderPassBarrier 
{
	VkPipelineStageFlags  srcStages = 0;
	VkAccessFlags         srcAccess = 0;
	VkPipelineStageFlags  dstStages = 0;
	VkAccessFlags         dstAccess = 0;
};


/**
 * \brief Render pass transitions
 *
 * Stores transitions for all depth and color attachments.
 * This is used to select a specific render pass object
 * from a group of render passes with the same format.
 */
struct GveRenderPassOps 
{
	GveRenderPassBarrier  barrier;
	GveDepthAttachmentOps depthOps;
	GveColorAttachmentOps colorOps[MaxNumRenderTargets];
};



class GveRenderPass : public RcObject
{
public:
	GveRenderPass(
		const RcPtr<GveDevice>& device,
		GveRenderPassFormat& fmt);

	~GveRenderPass();

	/**
	 * \brief Retrieves render pass format
	 * \returns The render pass format
	 */
	GveRenderPassFormat format() const 
	{
		return m_format;
	}

	/**
	 * \brief Checks whether a format is compatible
	 *
	 * Two render pass formats are considered compatible
	 * if all the relevant attachment formats match.
	 * \param [in] fmt The render pass format to check
	 * \returns \c true if this render pass is compatible.
	 */
	bool hasCompatibleFormat(const GveRenderPassFormat&  fmt) const;

	/**
	 * \brief Retrieves sample count
	 *
	 * If no sample count has been explicitly specitied,
	 * this will return \c VK_SAMPLE_COUNT_1_BIT.
	 * \returns Sample count
	 */
	VkSampleCountFlagBits getSampleCount() const 
	{
		return m_format.sampleCount;
	}

	/**
	 * \brief Returns handle of default render pass
	 *
	 * The default render pass handle should be used to
	 * create pipelines and framebuffer objects. It can
	 * \e not be used for \c vkCmdBeginRenderPass calls.
	 * \returns The default render pass handle
	 */
	VkRenderPass getDefaultHandle() const 
	{
		return m_default;
	}

	/**
	 * \brief Returns handle to a specialized render pass
	 *
	 * Returns a handle to a render pass with the given
	 * set of parameters. This should be used for calls
	 * to \c vkCmdBeginRenderPass.
	 * \param [in] ops Attachment ops
	 * \returns Render pass handle
	 */
	VkRenderPass getHandle(const GveRenderPassOps& ops);

private:
	VkRenderPass createRenderPass(GveRenderPassFormat& fmt);

private:

	struct Instance 
	{
		GveRenderPassOps ops;
		VkRenderPass handle;
	};

	GveRenderPassFormat		m_format;
	VkRenderPass            m_default;

	Spinlock				m_mutex;
	std::vector<Instance>   m_instances;

	VkRenderPass createRenderPass(
		const GveRenderPassOps& ops);

	static bool compareOps(
		const GveRenderPassOps& a,
		const GveRenderPassOps& b);

	RcPtr<GveDevice> m_device;
	VkRenderPass m_renderPass;
};


///


  /**
   * \brief Render pass pool
   *
   * Manages render pass objects. For each render
   * pass format, a new render pass object will
   * be created, but no two render pass objects
   * will have the same format.
   */
class GveRenderPassPool 
{

public:

	GveRenderPassPool(GveDevice* device);
	~GveRenderPassPool();

	/**
	 * \brief Retrieves a render pass object
	 *
	 * \param [in] fmt The render pass format
	 * \returns Matching render pass object
	 */
	GveRenderPass* getRenderPass(const GveRenderPassFormat&  fmt);

private:

	GveDevice* m_device;

	std::mutex m_mutex;
	std::unordered_map<GveRenderPassFormat, GveRenderPass,
		GveHash, GveEqual> m_renderPasses;

};


} // namespace gve

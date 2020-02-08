#pragma once

#include "GveCommon.h"
#include "GveDeviceInfo.h"
#include "GveResourceObjects.h"
#include "GveRecycler.h"

namespace sce
{
	class SceVideoOut;
}

namespace gve
{;

struct GveRenderTargets;
struct GveImageCreateInfo;
struct GveImageViewCreateInfo;
struct GveSamplerCreateInfo;

class GveCmdList;
class GveDescriptorPool;
class GveBuffer;
class GveImage;
class GveImageView;
class GveSampler;
class GveSwapChain;
class GveContext;
class GveFrameBuffer;
class GvePhysicalDevice;
class GveSharpResourceManager;

struct GveDeviceQueue 
{
	VkQueue   queueHandle = VK_NULL_HANDLE;
	uint32_t  queueFamily = 0;
	uint32_t  queueIndex = 0;
};

struct GveDeviceQueueSet 
{
	GveDeviceQueue graphics;
};


class GveDevice : public RcObject
{
	friend class GveContext;
	friend class GveDescriptorPoolTracker;

public:
	GveDevice(VkDevice device, const RcPtr<GvePhysicalDevice>& phyDevice);
	~GveDevice();

	operator VkDevice() const;

	RcPtr<GvePhysicalDevice> physicalDevice() const;

	GveDeviceQueueSet queues() const;

	RcPtr<GveSwapChain> createSwapchain(std::shared_ptr<sce::SceVideoOut>& videoOut, uint32_t displayBufferCount);

	RcPtr<GveFrameBuffer> createFrameBuffer(const GveRenderTargets& renderTargets);

	RcPtr<GveCmdList> createCmdList();

	RcPtr<GveContext> createContext();

	RcPtr<GveDescriptorPool> createDescriptorPool();

	GveSharpResourceManager& getSharpResManager();

	/// Buffer
	RcPtr<GveBuffer> createBuffer(const GveBufferCreateInfo& info, VkMemoryPropertyFlags memoryType);

	/// Image
	RcPtr<GveImage> createImage(const GveImageCreateInfo& info, VkMemoryPropertyFlags memoryType);

	RcPtr<GveImageView> createImageView(const RcPtr<GveImage>& image, const GveImageViewCreateInfo& createInfo);

	/// Sampler
	RcPtr<GveSampler> createSampler(const GveSamplerCreateInfo& info);

private:
	void recycleDescriptorPool(const RcPtr<GveDescriptorPool>& pool);

private:
	void initQueues();

private:
	VkDevice m_device;
	RcPtr<GvePhysicalDevice> m_phyDevice;

	GveDeviceQueueSet m_queues;
	GveDeviceInfo  m_properties;

	GveMemoryAllocator m_memAllocator;
	GveResourceObjects m_resObjects;

	GveRecycler<GveDescriptorPool, 16> m_recycledDescriptorPools;
	
};

} // namespace gve

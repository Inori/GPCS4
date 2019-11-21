#pragma once

#include "GveCommon.h"
#include "GvePhysicalDevice.h"
#include "GveFrameBuffer.h"
#include "GveContext.h"
#include "GveMemory.h"
#include "GveResourceObjects.h"

namespace gve
{;

class GveCmdList;
class GveDescriptorPool;
class GveBuffer;
class GveImage;

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
public:
	GveDevice(VkDevice device, const RcPtr<GvePhysicalDevice>& phyDevice);
	~GveDevice();

	operator VkDevice() const;

	RcPtr<GvePhysicalDevice> physicalDevice() const;

	GveDeviceQueueSet queues() const;

	RcPtr<GveFrameBuffer> createFrameBuffer(const GveRenderTargets& renderTargets);

	RcPtr<GveCmdList> createCmdList();

	RcPtr<GveContex> createContext();

	RcPtr<GveDescriptorPool> createDescriptorPool();

	/// Buffer
	
	RcPtr<GveBuffer> createBuffer(
		const GveBufferCreateInfo&	info, 
		VkMemoryPropertyFlags		memoryType);

	RcPtr<GveBuffer> createBufferVsharp(
		const GveBufferCreateInfo&	info,
		VkMemoryPropertyFlags		memoryType,
		uint64_t					key);

	void freeBufferVsharp(uint64_t key);

	/// Image

	RcPtr<GveImage> createImage(
		const GveImageCreateInfo&	info, 
		VkMemoryPropertyFlags		memoryType);

	RcPtr<GveImage> createImageTsharp(
		const GveImageCreateInfo&	info,
		VkMemoryPropertyFlags		memoryType,
		uint64_t					key);

	void freeImageTsharp(uint64_t key);
	
	RcPtr<GveImageView> createImageView(
		const RcPtr<GveImage>&            image,
		const GveImageViewCreateInfo&     createInfo);

	RcPtr<GveImageView> createImageViewTsharp(
		const RcPtr<GveImage>&            image,
		const GveImageViewCreateInfo&     createInfo,
		uint64_t						  key);

	void freeImageViewTsharp(uint64_t key);

	/// Sampler

	RcPtr<GveSampler> createSampler(const GveSamplerCreateInfo& info);

	RcPtr<GveSampler> createSamplerSsharp(
		const GveSamplerCreateInfo&		info, 
		uint64_t						key);

	void freeSamplerSsharp(uint64_t key);

	void GCSharpResource();

private:
	void initQueues();

private:
	VkDevice m_device;
	RcPtr<GvePhysicalDevice> m_phyDevice;

	GveDeviceQueueSet m_queues;
	GveDeviceInfo  m_properties;

	GveResourceObjects m_resObjects;
	
};

} // namespace gve

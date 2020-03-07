#include "GnmResourceFactory.h"

#include "GnmBuffer.h"
#include "GnmDepthRenderTarget.h"
#include "GnmRenderTarget.h"
#include "GnmSampler.h"
#include "GnmTexture.h"
#include "UtilBit.h"

#include "../Gve/GveBuffer.h"
#include "../Gve/GveImage.h"
#include "../Gve/GvePresenter.h"
#include "../Gve/GveSampler.h"
#include "../Sce/SceGpuQueue.h"
#include "../Sce/SceVideoOut.h"
#include "Algorithm/MurmurHash2.h"

using namespace gve;
using namespace sce;

GnmResourceFactory::GnmResourceFactory(sce::SceGpuQueueDevice* device) :
	m_device(device)
{
	collectRenderTargets();
}

GnmResourceFactory::~GnmResourceFactory()
{
}

RcPtr<GveBuffer> GnmResourceFactory::grabIndex(const GnmIndexBuffer& desc)
{
	GnmResourceEntry entry = {};
	entry.memory           = desc.buffer;
	entry.size             = desc.size;
	auto create            = [this, &desc]() { return createIndex(desc); };
	return grabResource(entry, m_bufferMap, create);
}

RcPtr<GveBuffer> GnmResourceFactory::grabBuffer(const GnmBuffer& desc)
{
	GnmResourceEntry entry = {};
	entry.memory           = desc.getBaseAddress();
	entry.size             = desc.getSize();
	auto create            = [this, &desc]() { return createBuffer(desc); };
	return grabResource(entry, m_bufferMap, create);
}

GnmCombinedImageView GnmResourceFactory::grabImage(const GnmTexture& desc)
{
	GnmResourceEntry entry = {};
	entry.memory           = desc.getBaseAddress();
	entry.size             = desc.getSizeAlign().m_size;
	auto create            = [this, &desc]() { return createImage(desc); };
	return grabResource(entry, m_imageMap, create);
}

GnmCombinedImageView GnmResourceFactory::grabRenderTarget(const GnmRenderTarget& desc)
{
	GnmResourceEntry entry = {};
	entry.memory           = desc.getBaseAddress();
	entry.size             = desc.getColorSizeAlign().m_size;
	auto create            = [this, &desc]() { return createRenderTarget(desc); };
	return grabResource(entry, m_imageMap, create);
}

GnmCombinedImageView GnmResourceFactory::grabDepthRenderTarget(const GnmDepthRenderTarget& desc)
{
	GnmResourceEntry entry = {};
	entry.memory           = desc.getZReadAddress();
	entry.size             = desc.getZSizeAlign().m_size;
	auto create            = [this, &desc]() { return createDepthRenderTarget(desc); };
	return grabResource(entry, m_imageMap, create);
}

RcPtr<GveSampler> GnmResourceFactory::grabSampler(const GnmSampler& desc)
{
	uint64_t         hash  = algo::MurmurHash(desc.m_regs, sizeof(desc.m_regs));
	GnmResourceEntry entry = {};
	entry.memory           = reinterpret_cast<const void*>(bit::extract(hash, 32, 63));
	entry.size             = static_cast<uint32_t>(bit::extract(hash, 0, 31));
	auto create            = [this, &desc]() { return createSampler(desc); };
	return grabResource(entry, m_samplerMap, create);
}

template <typename MapType>
typename MapType::mapped_type GnmResourceFactory::grabResource(
	const GnmResourceEntry&                        entry,
	MapType&                                       map,
	std::function<typename MapType::mapped_type()> create)
{
	typename MapType::mapped_type resource = {};
	auto                          iter     = map.find(entry);
	if (iter != map.end())
	{
		resource = iter->second;
	}
	else
	{
		resource = create();
		map.insert(std::make_pair(entry, resource));
	}
	return resource;
}

void GnmResourceFactory::collectRenderTargets()
{
	// Insert
	// display buffer - swapchain image 
	// pair to the image map.
	uint32_t numDisplayBuffer = m_device->videoOut->numDisplayBuffer();
	for (uint32_t i = 0; i != numDisplayBuffer; ++i)
	{
		SceDisplayBuffer bufferInfo = m_device->videoOut->getDisplayBuffer(i);
		GnmResourceEntry entry      = {};
		entry.memory                = bufferInfo.address;
		entry.size                  = bufferInfo.size;

		PresenterImage       swapImage    = m_device->presenter->getImage(i);
		GnmCombinedImageView renderTarget = {};
		renderTarget.image                = swapImage.image;
		renderTarget.view                 = swapImage.view;
		m_imageMap.insert(std::make_pair(entry, renderTarget));
	}
}

RcPtr<gve::GveBuffer> GnmResourceFactory::createIndex(const GnmIndexBuffer& desc)
{
}

RcPtr<gve::GveBuffer> GnmResourceFactory::createBuffer(const GnmBuffer& desc)
{
}

GnmCombinedImageView GnmResourceFactory::createImage(const GnmTexture& desc)
{
}

GnmCombinedImageView GnmResourceFactory::createRenderTarget(const GnmRenderTarget& desc)
{
}

GnmCombinedImageView GnmResourceFactory::createDepthRenderTarget(const GnmDepthRenderTarget& desc)
{
}

RcPtr<gve::GveSampler> GnmResourceFactory::createSampler(const GnmSampler& desc)
{
}

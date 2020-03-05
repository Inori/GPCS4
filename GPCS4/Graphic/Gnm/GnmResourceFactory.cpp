#include "GnmResourceFactory.h"

#include "../Gve/GveBuffer.h"
#include "../Gve/GveImage.h"
#include "../Gve/GveSampler.h"
#include "../Sce/SceGpuQueue.h"

using namespace gve;
using namespace sce;

GnmResourceFactory::GnmResourceFactory(sce::SceGpuQueueDevice* device):
	m_device(device)
{
}

GnmResourceFactory::~GnmResourceFactory()
{
}

RcPtr<GveBuffer> GnmResourceFactory::grabIndex(const GnmIndexBuffer& desc)
{
}

RcPtr<GveBuffer> GnmResourceFactory::grabBuffer(const GnmBuffer& desc)
{
}

GnmCombinedImageView GnmResourceFactory::grabImage(const GnmTexture& desc)
{
}

GnmCombinedImageView GnmResourceFactory::grabRenderTarget(const GnmRenderTarget& desc)
{
}

GnmCombinedImageView GnmResourceFactory::grabDepthRenderTarget(const GnmDepthRenderTarget& desc)
{
}

RcPtr<GveSampler> GnmResourceFactory::grabSampler(const GnmSampler& desc)
{
}

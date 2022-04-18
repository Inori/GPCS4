#include "VltRenderTarget.h"
#include "VltImage.h"

namespace sce::vlt
{
	
	bool VltAttachmentFormat::eq(const VltAttachmentFormat& other) const
	{
		bool eq = (this->depth == other.depth);

		for (uint32_t i = 0; i < MaxNumRenderTargets && eq; i++)
		{
			eq &= (this->color[i] == other.color[i]);
		}

		return eq;
	}

	uint32_t VltAttachmentFormat::colorCount() const
	{
		uint32_t count = 0;
		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			if (this->color[i] != VK_FORMAT_UNDEFINED)
			{
				++count;
			}
		}
		return count;
	}

	VltAttachmentFormat VltRenderTargets::generateAttachmentFormat() const
	{
		VltAttachmentFormat format;
		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			if (this->color[i].view != nullptr)
			{
				format.color[i] = this->color[i].view->info().format;
			}
		}

		if (this->depth.view != nullptr)
		{
			format.depth = this->depth.view->imageInfo().format;
		}
		
		return format;
	}

}  // namespace sce::vlt
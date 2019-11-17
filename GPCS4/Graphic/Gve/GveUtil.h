#pragma once

#include "GveCommon.h"

#include <algorithm>

namespace gve::util
{;
	
/**
 * \brief Computes mip level extent
 *
 * \param [in] size Base mip level extent
 * \param [in] level mip level to compute
 * \returns Extent of the given mip level
 */
inline VkExtent3D computeMipLevelExtent(VkExtent3D size, uint32_t level) 
{
	size.width = std::max(1u, size.width >> level);
	size.height = std::max(1u, size.height >> level);
	size.depth = std::max(1u, size.depth >> level);
	return size;
}

}  // namespace gve
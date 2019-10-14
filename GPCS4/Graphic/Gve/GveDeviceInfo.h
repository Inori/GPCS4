#pragma once

#include "GveCommon.h"

namespace gve
{;

/**
 * \brief Device info
 *
 * Stores core properties and a bunch of extension-specific
 * properties, if the respective extensions are available.
 * Structures for unsupported extensions will be undefined,
 * so before using them, check whether they are supported.
 */
struct GveDeviceInfo 
{
	VkPhysicalDeviceProperties2KHR                      core;
	VkPhysicalDeviceIDProperties                        coreDeviceId;
	VkPhysicalDeviceSubgroupProperties                  coreSubgroup;
	VkPhysicalDeviceTransformFeedbackPropertiesEXT      extTransformFeedback;
	VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT extVertexAttributeDivisor;
	VkPhysicalDeviceDepthStencilResolvePropertiesKHR    khrDepthStencilResolve;
	VkPhysicalDeviceDriverPropertiesKHR                 khrDeviceDriverProperties;
};


/**
 * \brief Device features
 *
 * Stores core features and extension-specific features.
 * If the respective extensions are not available, the
 * extended features will be marked as unsupported.
 */
struct GveDeviceFeatures 
{
	VkPhysicalDeviceFeatures2KHR                              core;
	VkPhysicalDeviceConditionalRenderingFeaturesEXT           extConditionalRendering;
	VkPhysicalDeviceDepthClipEnableFeaturesEXT                extDepthClipEnable;
	VkPhysicalDeviceHostQueryResetFeaturesEXT                 extHostQueryReset;
	VkPhysicalDeviceMemoryPriorityFeaturesEXT                 extMemoryPriority;
	VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT extShaderDemoteToHelperInvocation;
	VkPhysicalDeviceTransformFeedbackFeaturesEXT              extTransformFeedback;
	VkPhysicalDeviceVertexAttributeDivisorFeaturesEXT         extVertexAttributeDivisor;
};

}

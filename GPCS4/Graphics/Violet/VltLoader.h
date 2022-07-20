#pragma once

#include "VltCommon.h"

#define VULKAN_FN(name) \
	::PFN_ ## name name = reinterpret_cast<::PFN_ ## name>(sym(#name))

namespace sce::vlt::vk
{
	/**
	 * \brief Vulkan instance loader
	 *
	 * Loads Vulkan functions that can be
	 * called for a specific instance.
	 */
	struct InstanceLoader : public RcObject
	{
		InstanceLoader(bool owned, VkInstance instance);
		PFN_vkVoidFunction sym(const char* name) const;
		VkInstance         instance() const
		{
			return m_instance;
		}

	protected:
		const VkInstance m_instance;
		const bool       m_owned;
	};

	/**
	 * \brief Vulkan device loader
	 *
	 * Loads Vulkan functions for a
	 * specific device.
	 */
	struct DeviceLoader : public RcObject
	{
		DeviceLoader(bool owned, VkInstance instance, VkDevice device);
		PFN_vkVoidFunction sym(const char* name) const;
		VkDevice           device() const
		{
			return m_device;
		}

	protected:
		const PFN_vkGetDeviceProcAddr m_getDeviceProcAddr;
		const VkDevice                m_device;
		const bool                    m_owned;
	};


	/**
	 * \brief Vulkan instance functions
	 *
	 * Vulkan functions for a given instance that
	 * are independent of any Vulkan devices.
	 */
	struct InstanceFn : InstanceLoader
	{
		InstanceFn(bool owned, VkInstance instance);
		~InstanceFn();

#ifdef VK_KHR_get_surface_capabilities2
		VULKAN_FN(vkGetPhysicalDeviceSurfaceCapabilities2KHR);
		VULKAN_FN(vkGetPhysicalDeviceSurfaceFormats2KHR);
#endif

#ifdef VK_KHR_surface
		VULKAN_FN(vkDestroySurfaceKHR);

		VULKAN_FN(vkGetPhysicalDeviceSurfaceSupportKHR);
		VULKAN_FN(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
		VULKAN_FN(vkGetPhysicalDeviceSurfaceFormatsKHR);
		VULKAN_FN(vkGetPhysicalDeviceSurfacePresentModesKHR);
#endif

#ifdef VK_EXT_debug_report
		VULKAN_FN(vkCreateDebugReportCallbackEXT);
		VULKAN_FN(vkDestroyDebugReportCallbackEXT);
		VULKAN_FN(vkDebugReportMessageEXT);
#endif

#ifdef VK_EXT_full_screen_exclusive
		VULKAN_FN(vkGetPhysicalDeviceSurfacePresentModes2EXT);
#endif
	};

	/**
	 * \brief Vulkan device functions
	 *
	 * Vulkan functions for a specific Vulkan device.
	 * This ensures that no slow dispatch code is executed.
	 */
	struct DeviceFn : DeviceLoader
	{
		DeviceFn(bool owned, VkInstance instance, VkDevice device);
		~DeviceFn();

#ifdef VK_KHR_create_renderpass2
		VULKAN_FN(vkCreateRenderPass2KHR);
		VULKAN_FN(vkCmdBeginRenderPass2KHR);
		VULKAN_FN(vkCmdNextSubpass2KHR);
		VULKAN_FN(vkCmdEndRenderPass2KHR);
#endif

#ifdef VK_KHR_draw_indirect_count
		VULKAN_FN(vkCmdDrawIndirectCountKHR);
		VULKAN_FN(vkCmdDrawIndexedIndirectCountKHR);
#endif

#ifdef VK_KHR_swapchain
		VULKAN_FN(vkCreateSwapchainKHR);
		VULKAN_FN(vkDestroySwapchainKHR);
		VULKAN_FN(vkGetSwapchainImagesKHR);
		VULKAN_FN(vkAcquireNextImageKHR);
		VULKAN_FN(vkQueuePresentKHR);
#endif

#ifdef VK_EXT_conditional_rendering
		VULKAN_FN(vkCmdBeginConditionalRenderingEXT);
		VULKAN_FN(vkCmdEndConditionalRenderingEXT);
#endif

#ifdef VK_EXT_extended_dynamic_state
		VULKAN_FN(vkCmdBindVertexBuffers2EXT);
		VULKAN_FN(vkCmdSetCullModeEXT);
		VULKAN_FN(vkCmdSetDepthBoundsTestEnableEXT);
		VULKAN_FN(vkCmdSetDepthCompareOpEXT);
		VULKAN_FN(vkCmdSetDepthTestEnableEXT);
		VULKAN_FN(vkCmdSetDepthWriteEnableEXT);
		VULKAN_FN(vkCmdSetFrontFaceEXT);
		VULKAN_FN(vkCmdSetPrimitiveTopologyEXT);
		VULKAN_FN(vkCmdSetScissorWithCountEXT);
		VULKAN_FN(vkCmdSetStencilOpEXT);
		VULKAN_FN(vkCmdSetStencilTestEnableEXT);
		VULKAN_FN(vkCmdSetViewportWithCountEXT);
#endif

#ifdef VK_EXT_full_screen_exclusive
		VULKAN_FN(vkAcquireFullScreenExclusiveModeEXT);
		VULKAN_FN(vkReleaseFullScreenExclusiveModeEXT);
		VULKAN_FN(vkGetDeviceGroupSurfacePresentModes2EXT);
#endif

#ifdef VK_EXT_host_query_reset
		VULKAN_FN(vkResetQueryPoolEXT);
#endif

#ifdef VK_EXT_shader_module_identifier
		VULKAN_FN(vkGetShaderModuleCreateInfoIdentifierEXT);
		VULKAN_FN(vkGetShaderModuleIdentifierEXT);
#endif

#ifdef VK_EXT_transform_feedback
		VULKAN_FN(vkCmdBindTransformFeedbackBuffersEXT);
		VULKAN_FN(vkCmdBeginTransformFeedbackEXT);
		VULKAN_FN(vkCmdEndTransformFeedbackEXT);
		VULKAN_FN(vkCmdDrawIndirectByteCountEXT);
		VULKAN_FN(vkCmdBeginQueryIndexedEXT);
		VULKAN_FN(vkCmdEndQueryIndexedEXT);
#endif

#ifdef VK_NVX_image_view_handle
		VULKAN_FN(vkGetImageViewHandleNVX);
		VULKAN_FN(vkGetImageViewAddressNVX);
#endif

#ifdef VK_NVX_binary_import
		VULKAN_FN(vkCreateCuModuleNVX);
		VULKAN_FN(vkCreateCuFunctionNVX);
		VULKAN_FN(vkDestroyCuModuleNVX);
		VULKAN_FN(vkDestroyCuFunctionNVX);
		VULKAN_FN(vkCmdCuLaunchKernelNVX);
#endif

#ifdef VK_KHR_buffer_device_address
		VULKAN_FN(vkGetBufferDeviceAddressKHR);
#endif

#ifdef VK_KHR_dynamic_rendering
		VULKAN_FN(vkCmdBeginRenderingKHR);
		VULKAN_FN(vkCmdEndRenderingKHR);
#endif

#ifdef VK_KHR_external_memory_win32
		VULKAN_FN(vkGetMemoryWin32HandleKHR);
		VULKAN_FN(vkGetMemoryWin32HandlePropertiesKHR);
#endif

#ifdef VK_EXT_debug_utils
		VULKAN_FN(vkCmdBeginDebugUtilsLabelEXT);
		VULKAN_FN(vkCmdEndDebugUtilsLabelEXT);
		VULKAN_FN(vkCmdInsertDebugUtilsLabelEXT);
		VULKAN_FN(vkCreateDebugUtilsMessengerEXT);
		VULKAN_FN(vkDestroyDebugUtilsMessengerEXT);
		VULKAN_FN(vkSetDebugUtilsObjectNameEXT);
		VULKAN_FN(vkSetDebugUtilsObjectTagEXT);
#endif
	};

}  // namespace sce::vlt
#include "VltDebugUtil.h"

#include "VltDevice.h"

namespace sce::vlt
{
	PFN_vkSetDebugUtilsObjectNameEXT VltDebugUtil::pfnSetDebugUtilsObjectNameEXT = VK_NULL_HANDLE;
	PFN_vkSetDebugUtilsObjectTagEXT  VltDebugUtil::pfnSetDebugUtilsObjectTagEXT  = VK_NULL_HANDLE;

	PFN_vkCmdInsertDebugUtilsLabelEXT VltDebugUtil::pfnCmdInsertDebugUtilsLabelEXT = VK_NULL_HANDLE;
	PFN_vkCmdEndDebugUtilsLabelEXT    VltDebugUtil::pfnCmdEndDebugUtilsLabelEXT    = VK_NULL_HANDLE;
	PFN_vkCmdBeginDebugUtilsLabelEXT  VltDebugUtil::pfnCmdBeginDebugUtilsLabelEXT  = VK_NULL_HANDLE;

	VltDebugUtil::VltDebugUtil(VltDevice* device) :
		m_device(device)
	{
#ifdef VLT_DEBUG_LABEL
		setupDebugFunctions();
#endif  // VLT_DEBUG_LABEL
	}

	VltDebugUtil::~VltDebugUtil()
	{
	}

	void VltDebugUtil::setupDebugFunctions()
	{
		pfnSetDebugUtilsObjectNameEXT = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(vkGetDeviceProcAddr(m_device->handle(), "vkSetDebugUtilsObjectNameEXT"));
		pfnSetDebugUtilsObjectTagEXT  = reinterpret_cast<PFN_vkSetDebugUtilsObjectTagEXT>(vkGetDeviceProcAddr(m_device->handle(), "vkSetDebugUtilsObjectTagEXT"));

		pfnCmdBeginDebugUtilsLabelEXT  = reinterpret_cast<PFN_vkCmdBeginDebugUtilsLabelEXT>(vkGetDeviceProcAddr(m_device->handle(), "vkCmdBeginDebugUtilsLabelEXT"));
		pfnCmdEndDebugUtilsLabelEXT    = reinterpret_cast<PFN_vkCmdEndDebugUtilsLabelEXT>(vkGetDeviceProcAddr(m_device->handle(), "vkCmdEndDebugUtilsLabelEXT"));
		pfnCmdInsertDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdInsertDebugUtilsLabelEXT>(vkGetDeviceProcAddr(m_device->handle(), "vkCmdInsertDebugUtilsLabelEXT"));
	}

	void VltDebugUtil::setObjectName(VkDebugUtilsObjectNameInfoEXT* nameInfo)
	{
#ifdef VLT_DEBUG_LABEL
		pfnSetDebugUtilsObjectNameEXT(m_device->handle(), nameInfo);
#endif  // VLT_DEBUG_LABEL
	}

	void VltDebugUtil::setObjectTag(VkDebugUtilsObjectTagInfoEXT* tagInfo)
	{
#ifdef VLT_DEBUG_LABEL
		pfnSetDebugUtilsObjectTagEXT(m_device->handle(), tagInfo);
#endif  // VLT_DEBUG_LABEL
	}

	void VltDebugUtil::cmdBeginDebugUtilsLabel(VkCommandBuffer command, VkDebugUtilsLabelEXT* labelInfo)
	{
#ifdef VLT_DEBUG_LABEL
		pfnCmdBeginDebugUtilsLabelEXT(command, labelInfo);
#endif  // VLT_DEBUG_LABEL
	}

	void VltDebugUtil::cmdEndDebugUtilsLabel(VkCommandBuffer command)
	{
#ifdef VLT_DEBUG_LABEL
		pfnCmdEndDebugUtilsLabelEXT(command);
#endif  // VLT_DEBUG_LABEL
	}

	void VltDebugUtil::cmdInsertDebugUtilsLabel(VkCommandBuffer command, VkDebugUtilsLabelEXT* labelInfo)
	{
#ifdef VLT_DEBUG_LABEL
		pfnCmdInsertDebugUtilsLabelEXT(command, labelInfo);
#endif  // VLT_DEBUG_LABEL
	}

}  // namespace sce::vlt
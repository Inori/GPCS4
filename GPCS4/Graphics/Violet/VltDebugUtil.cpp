#include "VltDebugUtil.h"

#include "VltDevice.h"

namespace sce::vlt
{
	PFN_vkDebugMarkerSetObjectTagEXT  VltDebugUtil::pfnDebugMarkerSetObjectTag  = VK_NULL_HANDLE;
	PFN_vkDebugMarkerSetObjectNameEXT VltDebugUtil::pfnDebugMarkerSetObjectName = VK_NULL_HANDLE;
	PFN_vkCmdDebugMarkerBeginEXT      VltDebugUtil::pfnCmdDebugMarkerBegin      = VK_NULL_HANDLE;
	PFN_vkCmdDebugMarkerEndEXT        VltDebugUtil::pfnCmdDebugMarkerEnd        = VK_NULL_HANDLE;
	PFN_vkCmdDebugMarkerInsertEXT     VltDebugUtil::pfnCmdDebugMarkerInsert     = VK_NULL_HANDLE;

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
		pfnDebugMarkerSetObjectTag  = reinterpret_cast<PFN_vkDebugMarkerSetObjectTagEXT>(vkGetDeviceProcAddr(m_device->handle(), "vkDebugMarkerSetObjectTagEXT"));
		pfnDebugMarkerSetObjectName = reinterpret_cast<PFN_vkDebugMarkerSetObjectNameEXT>(vkGetDeviceProcAddr(m_device->handle(), "vkDebugMarkerSetObjectNameEXT"));
		pfnCmdDebugMarkerBegin      = reinterpret_cast<PFN_vkCmdDebugMarkerBeginEXT>(vkGetDeviceProcAddr(m_device->handle(), "vkCmdDebugMarkerBeginEXT"));
		pfnCmdDebugMarkerEnd        = reinterpret_cast<PFN_vkCmdDebugMarkerEndEXT>(vkGetDeviceProcAddr(m_device->handle(), "vkCmdDebugMarkerEndEXT"));
		pfnCmdDebugMarkerInsert     = reinterpret_cast<PFN_vkCmdDebugMarkerInsertEXT>(vkGetDeviceProcAddr(m_device->handle(), "vkCmdDebugMarkerInsertEXT"));

		pfnCmdBeginDebugUtilsLabelEXT  = reinterpret_cast<PFN_vkCmdBeginDebugUtilsLabelEXT>(vkGetDeviceProcAddr(m_device->handle(), "vkCmdBeginDebugUtilsLabelEXT"));
		pfnCmdEndDebugUtilsLabelEXT    = reinterpret_cast<PFN_vkCmdEndDebugUtilsLabelEXT>(vkGetDeviceProcAddr(m_device->handle(), "vkCmdEndDebugUtilsLabelEXT"));
		pfnCmdInsertDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdInsertDebugUtilsLabelEXT>(vkGetDeviceProcAddr(m_device->handle(), "vkCmdInsertDebugUtilsLabelEXT"));
	}

	void VltDebugUtil::setObjectName(uint64_t object, VkDebugReportObjectTypeEXT objType, const char* objName)
	{
#ifdef VLT_DEBUG_LABEL
		VkDebugMarkerObjectNameInfoEXT nameInfo = {};
		nameInfo.sType                          = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
		nameInfo.pNext                          = nullptr;
		nameInfo.object                         = object;
		nameInfo.objectType                     = objType;
		nameInfo.pObjectName                    = objName;
		pfnDebugMarkerSetObjectName(m_device->handle(), &nameInfo);
#endif  // VLT_DEBUG_LABEL
	}

	void VltDebugUtil::setObjectTag(uint64_t object, VkDebugReportObjectTypeEXT objType, void* tagData, size_t tagSize, uint64_t tagName /*= 0*/)
	{
#ifdef VLT_DEBUG_LABEL
		VkDebugMarkerObjectTagInfoEXT tagInfo = {};
		tagInfo.sType                         = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
		tagInfo.pNext                         = nullptr;
		tagInfo.object                        = object;
		tagInfo.objectType                    = objType;
		tagInfo.tagName                       = tagName;
		tagInfo.pTag                          = tagData;
		tagInfo.tagSize                       = tagSize;
		pfnDebugMarkerSetObjectTag(m_device->handle(), &tagInfo);
#endif  // VLT_DEBUG_LABEL
	}

	void VltDebugUtil::cmdMarkerBegin(VkCommandBuffer command, const char* marker)
	{
#ifdef VLT_DEBUG_LABEL
		VkDebugMarkerMarkerInfoEXT markerInfo = {};
		markerInfo.sType                      = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
		markerInfo.pNext                      = nullptr;
		markerInfo.pMarkerName                = marker;
		pfnCmdDebugMarkerBegin(command, &markerInfo);
#endif  // VLT_DEBUG_LABEL
	}

	void VltDebugUtil::cmdMarkerEnd(VkCommandBuffer command)
	{
#ifdef VLT_DEBUG_LABEL
		pfnCmdDebugMarkerEnd(command);
#endif  // VLT_DEBUG_LABEL
	}

	void VltDebugUtil::cmdMarkerInsert(VkCommandBuffer command, const char* marker)
	{
#ifdef VLT_DEBUG_LABEL
		VkDebugMarkerMarkerInfoEXT markerInfo = {};
		markerInfo.sType                      = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
		markerInfo.pNext                      = nullptr;
		markerInfo.pMarkerName                = marker;
		pfnCmdDebugMarkerInsert(command, &markerInfo);
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
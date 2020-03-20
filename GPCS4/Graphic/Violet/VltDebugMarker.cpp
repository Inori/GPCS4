#include "VltDebugMarker.h"
#include "VltDevice.h"

namespace vlt
{;



PFN_vkDebugMarkerSetObjectTagEXT VltDebugMarker::pfnDebugMarkerSetObjectTag = VK_NULL_HANDLE;
PFN_vkDebugMarkerSetObjectNameEXT VltDebugMarker::pfnDebugMarkerSetObjectName = VK_NULL_HANDLE;
PFN_vkCmdDebugMarkerBeginEXT VltDebugMarker::pfnCmdDebugMarkerBegin = VK_NULL_HANDLE;
PFN_vkCmdDebugMarkerEndEXT VltDebugMarker::pfnCmdDebugMarkerEnd = VK_NULL_HANDLE;
PFN_vkCmdDebugMarkerInsertEXT VltDebugMarker::pfnCmdDebugMarkerInsert = VK_NULL_HANDLE;

VltDebugMarker::VltDebugMarker(VltDevice* device):
	m_device(device)
{
#ifdef VLT_DEBUG_MARKER
	setupDebugFunctions();
#endif  // VLT_DEBUG_MARKER
}


VltDebugMarker::~VltDebugMarker()
{

}

void VltDebugMarker::setupDebugFunctions()
{
	pfnDebugMarkerSetObjectTag = reinterpret_cast<PFN_vkDebugMarkerSetObjectTagEXT>(vkGetDeviceProcAddr(*m_device, "vkDebugMarkerSetObjectTagEXT"));
	pfnDebugMarkerSetObjectName = reinterpret_cast<PFN_vkDebugMarkerSetObjectNameEXT>(vkGetDeviceProcAddr(*m_device, "vkDebugMarkerSetObjectNameEXT"));
	pfnCmdDebugMarkerBegin = reinterpret_cast<PFN_vkCmdDebugMarkerBeginEXT>(vkGetDeviceProcAddr(*m_device, "vkCmdDebugMarkerBeginEXT"));
	pfnCmdDebugMarkerEnd = reinterpret_cast<PFN_vkCmdDebugMarkerEndEXT>(vkGetDeviceProcAddr(*m_device, "vkCmdDebugMarkerEndEXT"));
	pfnCmdDebugMarkerInsert = reinterpret_cast<PFN_vkCmdDebugMarkerInsertEXT>(vkGetDeviceProcAddr(*m_device, "vkCmdDebugMarkerInsertEXT"));
}

void VltDebugMarker::setObjectName(uint64_t object, VkDebugReportObjectTypeEXT objType, const char* objName)
{
#ifdef VLT_DEBUG_MARKER
	VkDebugMarkerObjectNameInfoEXT nameInfo = {};
	nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
	nameInfo.pNext = nullptr;
	nameInfo.object = object;
	nameInfo.objectType = objType;
	nameInfo.pObjectName = objName;
	pfnDebugMarkerSetObjectName(*m_device, &nameInfo);
#endif  // VLT_DEBUG_MARKER
}

void VltDebugMarker::setObjectTag(uint64_t object, VkDebugReportObjectTypeEXT objType, 
	void* tagData, size_t tagSize, uint64_t tagName /*= 0*/)
{
#ifdef VLT_DEBUG_MARKER
	VkDebugMarkerObjectTagInfoEXT tagInfo = {};
	tagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
	tagInfo.pNext = nullptr;
	tagInfo.object = object;
	tagInfo.objectType = objType;
	tagInfo.tagName = tagName;
	tagInfo.pTag = tagData;
	tagInfo.tagSize = tagSize;
	pfnDebugMarkerSetObjectTag(*m_device, &tagInfo);
#endif  // VLT_DEBUG_MARKER
}

void VltDebugMarker::cmdMarkerBegin(VkCommandBuffer command, const char* marker)
{
#ifdef VLT_DEBUG_MARKER
	VkDebugMarkerMarkerInfoEXT markerInfo = {};
	markerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
	markerInfo.pNext = nullptr;
	markerInfo.pMarkerName = marker;
	pfnCmdDebugMarkerBegin(command, &markerInfo);
#endif  // VLT_DEBUG_MARKER
}

void VltDebugMarker::cmdMarkerEnd(VkCommandBuffer command)
{
#ifdef VLT_DEBUG_MARKER
	pfnCmdDebugMarkerEnd(command);
#endif  // VLT_DEBUG_MARKER
}

void VltDebugMarker::cmdMarkerInsert(VkCommandBuffer command, const char* marker)
{
#ifdef VLT_DEBUG_MARKER
	VkDebugMarkerMarkerInfoEXT markerInfo = {};
	markerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
	markerInfo.pNext = nullptr;
	markerInfo.pMarkerName = marker;
	pfnCmdDebugMarkerInsert(command, &markerInfo);
#endif  // VLT_DEBUG_MARKER
}



}  // namespace vlt
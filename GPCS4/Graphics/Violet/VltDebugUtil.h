#pragma once

#include "VltCommon.h"

namespace sce::vlt
{
	class VltDevice;

	class VltDebugUtil
	{
	public:
		VltDebugUtil(VltDevice* device);
		~VltDebugUtil();

		void setObjectName(
			uint64_t object, VkDebugReportObjectTypeEXT objType, const char* objName);

		void setObjectTag(
			uint64_t object, VkDebugReportObjectTypeEXT objType, void* tagData, size_t tagSize, uint64_t tagName = 0);

		void cmdMarkerBegin(
			VkCommandBuffer command, const char* marker);

		void cmdMarkerEnd(
			VkCommandBuffer command);

		void cmdMarkerInsert(
			VkCommandBuffer command, const char* marker);

		void cmdBeginDebugUtilsLabel(
			VkCommandBuffer command, VkDebugUtilsLabelEXT* labelInfo);

		void cmdEndDebugUtilsLabel(
			VkCommandBuffer command);

		void cmdInsertDebugUtilsLabel(
			VkCommandBuffer command, VkDebugUtilsLabelEXT* labelInfo);

	private:
		void setupDebugFunctions();

	private:
		VltDevice* m_device;

		static PFN_vkDebugMarkerSetObjectTagEXT  pfnDebugMarkerSetObjectTag;
		static PFN_vkDebugMarkerSetObjectNameEXT pfnDebugMarkerSetObjectName;
		static PFN_vkCmdDebugMarkerBeginEXT      pfnCmdDebugMarkerBegin;
		static PFN_vkCmdDebugMarkerEndEXT        pfnCmdDebugMarkerEnd;
		static PFN_vkCmdDebugMarkerInsertEXT     pfnCmdDebugMarkerInsert;

		static PFN_vkCmdBeginDebugUtilsLabelEXT  pfnCmdBeginDebugUtilsLabelEXT;
		static PFN_vkCmdEndDebugUtilsLabelEXT    pfnCmdEndDebugUtilsLabelEXT;
		static PFN_vkCmdInsertDebugUtilsLabelEXT pfnCmdInsertDebugUtilsLabelEXT;
	};
}  // namespace sce::vlt
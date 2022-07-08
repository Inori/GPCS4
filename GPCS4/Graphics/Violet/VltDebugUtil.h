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
			VkDebugUtilsObjectNameInfoEXT* nameInfo);

		void setObjectTag(
			VkDebugUtilsObjectTagInfoEXT* tagInfo);

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

		static PFN_vkSetDebugUtilsObjectNameEXT pfnSetDebugUtilsObjectNameEXT;
		static PFN_vkSetDebugUtilsObjectTagEXT  pfnSetDebugUtilsObjectTagEXT;

		static PFN_vkCmdBeginDebugUtilsLabelEXT  pfnCmdBeginDebugUtilsLabelEXT;
		static PFN_vkCmdEndDebugUtilsLabelEXT    pfnCmdEndDebugUtilsLabelEXT;
		static PFN_vkCmdInsertDebugUtilsLabelEXT pfnCmdInsertDebugUtilsLabelEXT;
	};
}  // namespace sce::vlt
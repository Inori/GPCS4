#pragma once

#include "VltCommon.h"

namespace sce::vlt
{
	class VltDevice;

	/**
     * \brief VLT command list
     * 
     * Stores a command buffer that a context can use to record Vulkan
     * commands. The command list shall also reference the resources
     * used by the recorded commands for automatic lifetime tracking.
     * When the command list has completed execution, resources that
     * are no longer used may get destroyed.
     */
	class VltCommandList : public RcObject
	{
	public:
		VltCommandList(VltDevice* device);
		virtual ~VltCommandList();

	private:
		VltDevice* m_device;
	};


}  // namespace sce::vlt
#pragma once

#include "VltCommon.h"

namespace sce::vlt
{
	class VltDevice;

	/**
     * \brief DXVk context
     * 
     * Tracks pipeline state and records command lists.
     * This is where the actual rendering commands are
     * recorded.
     */
	class VltContext : public RcObject
	{
	public:
		VltContext(VltDevice* device);
		virtual ~VltContext();

	private:
		VltDevice* m_device;
	};
}  // namespace sce::vlt
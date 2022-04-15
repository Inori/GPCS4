#pragma once

#include "VltBuffer.h"
#include "VltCommon.h"

#include <queue>

namespace sce::vlt
{
	class VltDevice;

	/**
     * \brief Staging data allocator
     *
     * Allocates buffer slices for resource uploads,
     * while trying to keep the number of allocations
     * but also the amount of allocated memory low.
     */
	class VltStagingDataAlloc
	{
		constexpr static VkDeviceSize MaxBufferSize  = 1 << 25;  // 32 MiB
		constexpr static uint32_t     MaxBufferCount = 2;

	public:
		VltStagingDataAlloc(VltDevice* device);

		~VltStagingDataAlloc();

		/**
         * \brief Alloctaes a staging buffer slice
         * 
         * \param [in] size Size of the allocation
		 * \param [in] align Alignment of the allocation
         * \returns Staging buffer slice
         */
		VltBufferSlice alloc(VkDeviceSize size, VkDeviceSize align);

		/**
         * \brief Deletes all staging buffers
         * 
         * Destroys allocated buffers and
         * releases all buffer memory.
         */
		void trim();

	private:
		Rc<VltBuffer> createBuffer(VkDeviceSize size);

	private:
		VltDevice*    m_device;
		Rc<VltBuffer> m_buffer;
		VkDeviceSize  m_offset = 0;

		std::queue<Rc<VltBuffer>> m_buffers;
	};

}  // namespace sce::vlt
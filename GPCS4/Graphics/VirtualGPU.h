#pragma once

#include "GPCS4Common.h"
#include "sce_types.h"
#include "SceVideoOut/sce_videoout_types.h"

#include <array>
#include <memory>

namespace sce
{
	namespace Gnm
	{
		enum GpuMode;
		class GnmLabelManager;
	}  // namespace Gnm

	class SceVideoOut;
	class SceGnmDriver;
	class SceResourceTracker;
	
	class VirtualGPU final
	{
		constexpr static uint32_t SceVideoOutPortBase = 0x1000;
		constexpr static uint32_t SceVideoOutCount    = 3;

	public:
		VirtualGPU();
		~VirtualGPU();

		/**
		 * \brief Implement sceVideoOutOpen
		 */
		int videoOutOpen(
			SceUserServiceUserId userId, int32_t type, int32_t index, const void* param);

		/**
		 * \brief Implement sceVideoOutClose
		 */
		int videoOutClose(int32_t handle);

		/**
		 * \brief Retrieve video out instance by handle
		 */
		SceVideoOut& videoOutGet(int32_t handle);

		/**
		 * \brief Get SceGnmDriver
		 */
		SceGnmDriver& gnmDriver();

		/**
		 * \brief Get GPU resource tracker.
		 */
		SceResourceTracker& resourceTracker();

		/**
		 * \brief Get GPU label manager.
		 */
		Gnm::GnmLabelManager& labelManager();

		/**
		 * \brief Global GPU mode.
		 * 
		 * Equalrant/implementation of 
		 * Gnm::GpuMode getGpuMode(void);
		 * 
		 */
		Gnm::GpuMode mode();

	private:

		// it's better to use std::unique_ptr here
		// but to prevent annoying errors of missing destructor
		// I use std::shared_ptr instead, that would be no performace
		// difference here.
		std::array<std::shared_ptr<SceVideoOut>, SceVideoOutCount>
			m_videoOutSlots = {};

		std::shared_ptr<SceGnmDriver> m_gnmDriver = nullptr;

		std::shared_ptr<SceResourceTracker>   m_tracker      = nullptr;
		std::shared_ptr<Gnm::GnmLabelManager> m_labelManager = nullptr;
	};

}  // namespace sce
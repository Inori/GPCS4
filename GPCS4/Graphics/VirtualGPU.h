#pragma once

#include "GPCS4Common.h"
#include "sce_types.h"
#include "SceVideoOut/sce_videoout_types.h"

#include <memory>
#include <array>

namespace sce
{

class SceVideoOut;
class SceGnmDriver;


class VirtualGPU final
{
	constexpr static uint32_t SceVideoOutPortBase = 0x1000;
	constexpr static uint32_t SceVideoOutCount    = 3;

public:
	VirtualGPU();
	~VirtualGPU();

	int videoOutOpen(
		SceUserServiceUserId userId, int32_t type, int32_t index, const void* param);

	int videoOutClose(int32_t handle);

	SceVideoOut& videoOutGet(int32_t handle);

	SceGnmDriver& gnmDriver();

private:
	std::array<std::unique_ptr<SceVideoOut>, SceVideoOutCount> 
		m_videoOutSlots = {};

	std::unique_ptr<SceGnmDriver> m_gnmDriver = nullptr;
};




}  // namespace sce
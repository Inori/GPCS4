#include "PlatVulkan.h"

#ifdef GPCS4_WINDOWS

// this acctually should be done using glfwGetRequiredInstanceExtensions
// but due to initialize order issue, we implement it here
std::vector<const char*> plat::vulkanGetRequiredInstanceExtensions()
{
	// clang-format off
	return std::vector<const char*>
		({ 
			"VK_KHR_win32_surface"
		});
	// clang-format on
}

#else

std::vector<const char*> plat::vulkanGetRequiredInstanceExtensions()
{
	// TODO:
}

#endif


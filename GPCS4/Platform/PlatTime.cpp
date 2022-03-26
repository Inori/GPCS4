#include "PlatTime.h"
#include <thread>
#include <chrono>

namespace plat
{


void MicroSleep(uint32_t ms)
{
	std::this_thread::sleep_for(
		std::chrono::microseconds(ms)
	);
}

}


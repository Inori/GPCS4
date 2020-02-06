#include "UtilTime.h"
#include <thread>
#include <chrono>

namespace UtilTime
{;


void MicroSleep(uint32_t ms)
{
	std::this_thread::sleep_for(
		std::chrono::microseconds(ms)
	);
}

}


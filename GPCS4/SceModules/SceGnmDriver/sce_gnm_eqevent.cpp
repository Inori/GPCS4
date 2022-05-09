#include "sce_gnmdriver.h"

LOG_CHANNEL(SceModules.SceDriver.GnmEQEvent);

int PS4API sceGnmAddEqEvent(SceKernelEqueue eq, Gnm::EqEventType id, void* udata)
{
	LOG_SCE_GRAPHIC("Not implemented");
	LOG_ASSERT(id == Gnm::kEqEventGfxEop, "TODO");
	return SCE_OK;
}


int PS4API sceGnmGetEqEventType(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDeleteEqEvent(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}

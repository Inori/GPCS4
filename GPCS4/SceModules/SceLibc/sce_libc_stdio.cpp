#include "sce_libc.h"

LOG_CHANNEL(SceModules.SceLibc.stdio);

// TODO:
// stdin stdout stderr shouldn't be functions
// these are global varables
// will be used as params for some file functions
int PS4API scec__Stderr(void)
{
	LOG_FIXME("Not implemented");
	
	return SCE_OK;
}

int PS4API scec__Stdin(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec__Stdout(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


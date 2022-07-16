#include "sce_libkernel.h"
#include "sce_kernel_tls.h"
#include "Emulator/TLSHandler.h"

LOG_CHANNEL(SceModules.SceLibkernel.tls);

void* PS4API __tls_get_addr(tls_index *ti)
{
	LOG_SCE_DUMMY_IMPL();
	auto tlsManager = TLSManager::instance();
	return tlsManager->tlsGetAddr(ti->ti_module, ti->ti_offset);
}
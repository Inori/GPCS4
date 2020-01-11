#include "sce_libkernel.h"
#include "sce_kernel_tls.h"


void* PS4API __tls_get_addr(tls_index *ti)
{
	LOG_SCE_DUMMY_IMPL();
	return calloc(1, 0x1000);
}
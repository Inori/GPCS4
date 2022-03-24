#include "VirtualCPU.h"
#include "tinydbr/memory_monitor.h"
#include "UtilString.h"

VirtualCPU::VirtualCPU()
{
	MonitorFlags flags = IgnoreCode | IgnoreStack | IgnoreRipRelative /* | SaveExtendedState*/ ;

	m_executor = std::make_unique<MemoryMonitor>(flags, &m_memController);
}

VirtualCPU::~VirtualCPU()
{
	m_executor->Unit();
}

// TODO:
// currently, repeated load is not supported, 
// it can only be called one time
void VirtualCPU::load(CodeBlock block)
{
	// Give it a dummy module name.
	auto shellcodeName = str::format("sce_module_%x", block.code);

	// Init module info.
	TargetModule virtualModule = {};
	virtualModule.name         = shellcodeName.c_str();
	virtualModule.is_main      = true;
	virtualModule.is_shellcode = true;
	virtualModule.code_sections.push_back({ block.code, block.size });

	// Set shellcode mode to true.
	Options options        = {};
	options.shellcode_mode = true;

	m_executor->Init({ virtualModule }, options);
}

MemoryAllocator& VirtualCPU::allocator()
{
	return m_memAllocator;
}

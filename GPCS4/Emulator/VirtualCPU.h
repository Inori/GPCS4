#pragma once

#include "GPCS4Common.h"
#include "Memory.h"

#include <memory>
#include <vector>

class MemoryMonitor;

struct CodeBlock
{
	void*  code = nullptr;
	size_t size = 0;
};

// This is our virtual CPU.
// The cpu is only used to execute instructions
// and control memory behaviors.
// Environment and thread management is outside of this class.

class VirtualCPU final
{
public:
	VirtualCPU();
	~VirtualCPU();

	// load but not execute.
	// the caller is responsible to execute the code.
	// once the code is executed after load,
	// the cpu will take control of instruction streams automatically.
	void load(CodeBlock block);

	// manage all memorys the target game may use.
	// except stack and image memory.
	MemoryAllocator& allocator();

private:
	std::unique_ptr<MemoryMonitor> m_executor;
	MemoryController               m_memController;
	MemoryAllocator                m_memAllocator;
};


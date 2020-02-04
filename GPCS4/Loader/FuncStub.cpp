#include "FuncStub.h"
#include "Platform/UtilDebug.h"

LOG_CHANNEL(Loader.FuncStub);

static void logFunc(const char *log) 
{
	LOG_TRACE(log); 
}

// Trap the debugger when an unresolved function is called.
static int PS4API trapDebugger()
{
	UtilDebug::debugBreakPoint();
	return -1;
}

// TODO: For safety sake, all the non-volatile registers should be saved, but I only save
// rcx, rdx, r8, r9, r10, r11 for convenience.
const std::vector<uint8_t> FuncStubGenerator::funcTemplate = {
	0x48, 0x83, 0xEC, 0x48,			// sub rsp, 0x48 
	0x48, 0x89, 0x4C, 0x24, 0x08,	// rcx 
	0x48, 0x89, 0x54, 0x24, 0x10,	// rdx
	0x4C, 0x89, 0x44, 0x24, 0x18,	// r8
	0x4C, 0x89, 0x4C, 0x24, 0x20,	// r9
	0x4C, 0x89, 0x54, 0x24, 0x28,	// r10
	0x4C, 0x89, 0x5C, 0x24, 0x30,	// r11

	0x48, 0xB9, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, // movabs rcx, _str
	0x48, 0xB8, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, // movabs rax, _logFunc
	0xFF, 0xD0, // call rax

	// restore argument registers
	0x48, 0x8B, 0x4C, 0x24, 0x08,
	0x48, 0x8B, 0x54, 0x24, 0x10,
	0x4C, 0x8B, 0x44, 0x24, 0x18,
	0x4C, 0x8B, 0x4C, 0x24, 0x20,
	0x4C, 0x8B, 0x54, 0x24, 0x28,
	0x4C, 0x8B, 0x5C, 0x24, 0x30,
	0x48, 0x83, 0xC4, 0x48, // add rsp,0x48

	0x48, 0xB8, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, //mov rax, _dest
	0xFF, 0xE0 // jmp rax
};

JitFunctionPool::JitFunctionPool(size_t funcSize, size_t funcNum) :
	m_funcSize{ util::alignRound(funcSize, (size_t)16) }, m_funcNum{ funcNum }, m_index{ 0 }
{
	m_totalSize = m_funcSize * funcNum;
	auto memory = UtilMemory::VMMapFlexible(nullptr, m_totalSize,
                                 UtilMemory::VMPF_CPU_READ | UtilMemory::VMPF_CPU_EXEC);

	m_memory.reset(reinterpret_cast<uint8_t *>(memory));
}

uint8_t *JitFunctionPool::newFunctionMemory()
{
	uint8_t *retPtr = nullptr;
	do
	{
		if (m_memory.get() == nullptr)
		{
			LOG_ERR("null pointer error");
			break;
		}

		if (m_index == m_funcNum)
		{
			LOG_ERR("OUT OF MEMORY");
			break;
		}

		retPtr = &m_memory.get()[m_funcSize * m_index];
		m_index += 1;

	} while (false);

	return retPtr;
}

uint8_t *JitFunctionPool::getFunctionMemory(size_t index)
{
	uint8_t *retPtr = nullptr;
	do
	{
		if (m_memory.get() == nullptr)
		{
			LOG_ERR("null pointer error");
			break;
		}

		if (index > m_index)
		{
			LOG_ERR("index out of range");
			break;
		}

		retPtr = &m_memory.get()[m_funcSize * index];
	} while (false);

	return retPtr;
}

bool FuncStubGenerator::attach(void *memory)
{
	bool retval = false;
	do
	{
		if (memory == nullptr)
		{
			LOG_ERR("null pointer error");
			break;
		}
		m_memory = reinterpret_cast<uint8_t *>(memory);
		memcpy(m_memory, funcTemplate.data(), funcTemplate.size());
		retval = true;
	} while (false);

	return retval;
}

size_t FuncStubGenerator::size() const { return funcTemplate.size(); }

void FuncStubGenerator::patchLogString(const char *logString)
{
	patch(0x24, reinterpret_cast<uint64_t>(logString));
}

void FuncStubGenerator::patchLogFunction(void (*logFunctionPtr)(const char *))
{
	patch(0x2e, reinterpret_cast<uint64_t>(logFunctionPtr));
}

void FuncStubGenerator::patchDestPointer(const void *dest)
{
	patch(0x5c, reinterpret_cast<uint64_t>(dest));
}

FuncStubManager::FuncStubManager(JitFunctionPool *pool, FuncStubGenerator *stub)
	: m_pool{pool}, m_stub{stub}
{
}

void *FuncStubManager::generate(std::string const &message, void *dest)
{
	void *retPtr = nullptr;
	do
	{
		auto index = m_messageList.size();
		m_messageList.push_back(message);

		auto &msg   = m_messageList.at(index);
		auto msgPtr = msg.c_str();

		auto funcMem = m_pool->newFunctionMemory();

		if (funcMem == nullptr)
		{
			break;
		}

		auto ret = m_stub->attach(funcMem);
		if (!ret)
		{
			break;
		}

		m_stub->patchLogString(msgPtr);
		m_stub->patchLogFunction(logFunc);
		m_stub->patchDestPointer(dest);

		retPtr = funcMem;
	} while (false);

	return retPtr;
}

void *FuncStubManager::generateUnknown(std::string const &message)
{
	return generate(message, (void*)trapDebugger);
}

FuncStubManager *GetFuncStubManager() 
{ 
	static FuncStubGenerator generator = {};
	static JitFunctionPool pool        = {generator.size(), 5000};
	static FuncStubManager manager     = { &pool, &generator };
	
	return &manager;
}

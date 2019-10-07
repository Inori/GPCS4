#pragma once

#include "GPCS4Common.h"
#include "Platform/PlatformUtils.h"

#include <vector>
#include <string>
#include <cstdint>

class JitFunctionPool
{
public:
	JitFunctionPool(size_t funcSize, size_t funcNum);
	uint8_t *newFunctionMemory();
	uint8_t *getFunctionMemory(size_t index);

private:
	UtilMemory::memory_uptr m_memory;
	const size_t m_funcSize;
	size_t m_funcNum;
	size_t m_totalSize;
	size_t m_index;
};

class FuncStubGenerator
{
public:
	FuncStubGenerator() = default;
	bool attach(void *memory);
	void patchLogString(const char *logString);
	void patchLogFunction(void (*logFunctionPtr)(const char *));
	void patchDestPointer(const void *destPointer);
	size_t size() const;

private:
	template <typename T>
	void patch(size_t offset, T value)
    {
		auto ptr = reinterpret_cast<T *>(&m_memory[offset]);
		*ptr     = value;
	}

	uint8_t *m_memory;
	const static std::vector<uint8_t> funcTemplate;
};

class FuncStubManager
{
public:
	FuncStubManager(JitFunctionPool *pool, FuncStubGenerator *funcStub);
	void *generate(std::string const &message, void *dest);
	void *generate(std::string const &message);
private:
	std::vector<std::string> m_messageList;
	JitFunctionPool *m_pool;
	FuncStubGenerator *m_stub;
};

FuncStubManager *GetFuncStubManager();

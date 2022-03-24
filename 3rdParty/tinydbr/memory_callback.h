#pragma once

// Library users should inherit this interface
class MemoryCallback
{
public:
	virtual ~MemoryCallback();

	// memory read callback
	// this will be called before the read instruction,
	// so you can prepare the memory for the target program to read
	virtual void OnMemoryRead(void* address, size_t size) = 0;

	// memory write callback
	// this will be called after the write instruction
	// so you can get the memory content been written by the target program
	virtual void OnMemoryWrite(void* address, size_t size) = 0;
};

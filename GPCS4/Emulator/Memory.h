#pragma once

#include "GPCS4Common.h"
#include "UtilSingleton.h"


class MemoryController : public Singleton<MemoryController>
{
	friend class Singleton<MemoryController>;

public:


private:
	MemoryController();
	~MemoryController();
};


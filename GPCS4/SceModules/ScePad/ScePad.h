#pragma once

#include "GPCS4Common.h"
#include "sce_types.h"
#include "sce_pad_types.h"

class ScePad
{
public:
	ScePad(SceUserServiceUserId userId, int32_t type, int32_t index);
	~ScePad();

	int32_t type() const;

	int read(ScePadData* data, int32_t num);

	int readState(ScePadData* data);
private:
	int32_t m_type;
	int32_t m_index;
};


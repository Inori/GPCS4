#pragma once

#include "GcnCommon.h"

#include <vector>

namespace sce::gcn
{
	class GcnModule
	{
	public:
		GcnModule(
			const std::vector<uint8_t>& code);
		~GcnModule();

	private:
	};

}  // namespace sce::gcn
#pragma once

#include "GveCommon.h"

namespace gve
{;
	
class GveDevice;

class GvePresenter
{
public:
	GvePresenter(GveDevice* device);
	~GvePresenter();

private:
	GveDevice* m_device;
};



}  // namespace gve
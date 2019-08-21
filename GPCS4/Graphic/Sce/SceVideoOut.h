#pragma once

#include "GPCS4Common.h"
#include "../Pssl/PsslShaderFileBinary.h"
#include "../Gnm/GnmSharpBuffer.h"

// copy-paste code, quick and dirty implementation, just a test
// need to fully reconstruct, of course :)

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include <array>
#include <optional>

namespace sce 
{;


class SceVideoOut
{
public:
	SceVideoOut();
	~SceVideoOut();

private:

};

} // sce



#pragma once

#include "SceCommon.h"
#include "SceResourceTracker.h"
#include "Gnm/GnmLabelManager.h"
#include "Gnm/GnmShader.h"

namespace sce
{
	namespace vlt
	{
		class VltDevice;
	}

	class SceObjects
	{
	public:
		SceObjects(vlt::VltDevice* device) :
			m_labels(device)
		{
		}

		SceResourceTracker& resourceTracker()
		{
			return m_tracker;
		}

		Gnm::GnmLabelManager& labelManager()
		{
			return m_labels;
		}

		Gnm::GnmShaderModuleSet& shaderModuleSet()
		{
			return m_modules;
		}

	private:
		SceResourceTracker      m_tracker;
		Gnm::GnmLabelManager    m_labels;
		Gnm::GnmShaderModuleSet m_modules;
	};

}  // namespace sce
#include "GnmShader.h"
#include "Violet/VltShader.h"
#include "Gcn/GcnShaderMeta.h"

#include <fstream>
#include <fmt/format.h>

LOG_CHANNEL(Graphic.Gnm.GnmShader);

using namespace sce::gcn;
using namespace sce::vlt;

namespace sce::Gnm
{
	GnmShader::GnmShader(const VltShaderKey& key,
						 const void*         code):
		m_key(key),
		m_module(reinterpret_cast<const uint8_t*>(code))
	{
		const std::string name = key.toString();
		LOG_DEBUG("Compiling shader %s", name.c_str());
	}

	GnmShader::~GnmShader()
	{
	}

	Rc<VltShader> GnmShader::compile(
		const GcnModuleInfo& moduleInfo,
		const GcnShaderMeta& meta)
	{
		if (m_shader == nullptr)
		{
			m_shader = m_module.compile(meta, moduleInfo);
			m_shader->setShaderKey(m_key);

			std::ofstream fout(
				fmt::format("shaders/{}", m_shader->debugName()), 
				std::ofstream::binary);
			m_shader->dump(fout);
		}
		return m_shader;
	}

	GnmShaderModuleSet::GnmShaderModuleSet()
	{
	}

	GnmShaderModuleSet::~GnmShaderModuleSet()
	{
	}

	GnmShader GnmShaderModuleSet::getShaderModule(const vlt::VltShaderKey& key,
												  const void*              code)
	{
		// Use the shader's unique key for the lookup
		{
			std::unique_lock<std::mutex> lock(m_mutex);

			auto entry = m_modules.find(key);
			if (entry != m_modules.end())
			{
				return entry->second;
			}
		}

		// This shader has not been compiled yet, so we have to create a
		// new module. This takes a while, so we won't lock the structure.
		GnmShader shader = GnmShader(key, code);

		// Insert the new module into the lookup table. If another thread
		// has compiled the same shader in the meantime, we should return
		// that object instead and discard the newly created module.
		{
			std::unique_lock<std::mutex> lock(m_mutex);

			auto status = m_modules.insert({ key, shader });
			if (!status.second)
			{
				return status.first->second;
			}
		}

		return shader;
	}



}  // namespace sce::Gnm
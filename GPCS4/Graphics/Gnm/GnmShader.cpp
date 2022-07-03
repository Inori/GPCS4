#include "GnmShader.h"
#include "Violet/VltShader.h"
#include "Gcn/GcnModule.h"
#include "Gcn/GcnShaderMeta.h"


LOG_CHANNEL(Graphic.Gnm.GnmShader);

using namespace sce::gcn;
using namespace sce::vlt;

namespace sce::Gnm
{
	GnmShader::GnmShader() :
		m_shader(nullptr)
	{
	}

	GnmShader::GnmShader(const vlt::VltShaderKey*  key,
						 const gcn::GcnModuleInfo* moduleInfo,
						 const GcnShaderMeta&      meta,
						 const void*               code)
	{
		const std::string name = key->toString();
		LOG_DEBUG("Compiling shader %s", name.c_str());

		GcnModule module(
			reinterpret_cast<const uint8_t*>(code));

		m_resources = module.getResourceTable();

		m_shader = module.compile(meta, *moduleInfo);
		m_shader->setShaderKey(*key);
	}

	GnmShader::~GnmShader()
	{
	}

	GnmShaderModuleSet::GnmShaderModuleSet()
	{
	}

	GnmShaderModuleSet::~GnmShaderModuleSet()
	{
	}

	GnmShader GnmShaderModuleSet::getShaderModule(const VltShaderKey*  key,
												  const GcnModuleInfo* moduleInfo,
												  const GcnShaderMeta& meta,
												  const void*          code)
	{
		// Use the shader's unique key for the lookup
		{
			std::unique_lock<std::mutex> lock(m_mutex);

			auto entry = m_modules.find(*key);
			if (entry != m_modules.end())
			{
				return entry->second;
			}
		}

		// This shader has not been compiled yet, so we have to create a
		// new module. This takes a while, so we won't lock the structure.
		GnmShader shader = GnmShader(key, moduleInfo, meta, code);

		// Insert the new module into the lookup table. If another thread
		// has compiled the same shader in the meantime, we should return
		// that object instead and discard the newly created module.
		{
			std::unique_lock<std::mutex> lock(m_mutex);

			auto status = m_modules.insert({ *key, shader });
			if (!status.second)
			{
				return status.first->second;
			}
		}

		return shader;
	}



}  // namespace sce::Gnm
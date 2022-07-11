#pragma once

#include "GnmCommon.h"
#include "Gcn/GcnModule.h"
#include "Violet/VltShader.h"

#include <array>
#include <mutex>
#include <unordered_map>


namespace sce::Gnm
{
	/**
	 * \brief Common shader object
	 *
	 * Stores the compiled SPIR-V shader and the
	 * hash of the original gcn shader, which can be
	 * used to identify the shader.
	 */
	class GnmShader
	{
	public:
		GnmShader(const vlt::VltShaderKey& key,
				  const void*              code);
		~GnmShader();

		vlt::Rc<vlt::VltShader>
		compile(const gcn::GcnModuleInfo& moduleInfo,
				const gcn::GcnShaderMeta& meta);

		const gcn::GcnShaderResourceTable&
		getResources() const
		{
			return m_module.getResourceTable();
		}

	private:
		vlt::VltShaderKey       m_key;
		vlt::Rc<vlt::VltShader> m_shader;
		gcn::GcnModule          m_module;
	};

	/**
	 * \brief Shader module set
	 *
	 * Some applications may compile the same shader multiple
	 * times, so we should cache the resulting shader modules
	 * and reuse them rather than creating new ones. This
	 * class is thread-safe.
	 */
	class GnmShaderModuleSet
	{

	public:
		GnmShaderModuleSet();
		~GnmShaderModuleSet();

		GnmShader getShaderModule(
			const vlt::VltShaderKey& key,
			const void*              code);

	private:
		std::mutex m_mutex;

		std::unordered_map<
			vlt::VltShaderKey,
			GnmShader,
			vlt::VltHash,
			vlt::VltEq> m_modules;
	};

}  // namespace sce::Gnm
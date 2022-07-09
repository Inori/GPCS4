#pragma once

#include "GPCS4Common.h"
#include "GnmSampler.h"
#include "Violet/VltSampler.h"

#include <unordered_map>


namespace sce::Gnm
{
	struct GnmSamplerEq
	{
		size_t operator()(const Sampler& a, const Sampler& b) const
		{
			return a == b;
		}
	};

	struct GnmSamplerHash
	{
		size_t operator()(const Sampler& object) const;
	};

	/**
	 * \brief Sampler cache
	 * 
	 */
	class GnmSamplerCache
	{
	public:
		GnmSamplerCache();
		~GnmSamplerCache();

		vlt::Rc<vlt::VltSampler> getSampler(
			const Sampler& ssharp);

		void addSampler(
			const Sampler&                  ssharp,
			const vlt::Rc<vlt::VltSampler>& sampler);

	private:
		std::unordered_map<
			Sampler,
			vlt::Rc<vlt::VltSampler>,
			GnmSamplerHash,
			GnmSamplerEq> m_samplers;
	};


}  // namespace sce::Gnm
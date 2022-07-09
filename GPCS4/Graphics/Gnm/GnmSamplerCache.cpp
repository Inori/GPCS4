#include "GnmSamplerCache.h"
#include "Violet/VltHash.h"

using namespace sce::vlt;

namespace sce::Gnm
{
	size_t GnmSamplerHash::operator()(const Sampler& object) const
	{
		VltHashState state;
		state.add(object.m_regs[0]);
		state.add(object.m_regs[1]);
		state.add(object.m_regs[2]);
		state.add(object.m_regs[3]);
		return state;
	}

	GnmSamplerCache::GnmSamplerCache()
	{
	}

	GnmSamplerCache::~GnmSamplerCache()
	{
	}

	Rc<VltSampler> GnmSamplerCache::getSampler(const Sampler& ssharp)
	{
		Rc<VltSampler> result = nullptr;
		
		auto iter = m_samplers.find(ssharp);
		if (iter != m_samplers.end())
		{
			result = iter->second;
		}

		return result;
	}

	void GnmSamplerCache::addSampler(
		const Sampler&        ssharp,
		const Rc<VltSampler>& sampler)
	{
		m_samplers.emplace(ssharp, sampler);
	}

}  // namespace sce::Gnm
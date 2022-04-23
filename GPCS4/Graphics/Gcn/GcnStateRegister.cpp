#include "GcnStateRegister.h"

#include "GcnCompiler.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{

	GcnStateRegister::GcnStateRegister(
		GcnCompiler* compiler,
		uint64_t     initValue) :
		m_compiler(compiler)
	{
		GcnRegisterInfo info;
		info.type.ctype   = GcnScalarType::Uint32;
		info.type.ccount  = 1;
		info.type.alength = 0;
		info.sclass       = spv::StorageClassPrivate;

		m_low.type.ctype  = info.type.ctype;
		m_low.type.ccount = info.type.ccount;
		m_low.id          = m_compiler->emitNewVariable(info);

		m_high.type = m_low.type;
		m_high.id   = m_compiler->emitNewVariable(info);

		uint32_t lowValue  = static_cast<uint32_t>(initValue & 0xFFFFFFFF);
		uint32_t highValue = static_cast<uint32_t>((initValue >> 32) & 0xFFFFFFFF);

		auto& module = m_compiler->m_module;

		module.opStore(
			m_low.id,
			module.constu32(lowValue));
		module.opStore(
			m_high.id,
			module.constu32(highValue));
	}

	GcnStateRegister::~GcnStateRegister()
	{
	}

	GcnRegisterValuePair GcnStateRegister::emitLoad(
		const GcnRegMask& mask)
	{
		LOG_ASSERT(mask.popCount() <= 2, "error mask for state register.");
	
		GcnRegisterValuePair result = {};
		if (mask.popCount() == 1)
		{
			if (mask[0])
			{
				// low 32 bits
				result.low = m_compiler->emitValueLoad(m_low);
			}
			else
			{
				// high 32 bits
				result.low = m_compiler->emitValueLoad(m_high);
			}
		}
		else
		{
			// all 64 bits
			result.low = m_compiler->emitValueLoad(m_low);
			result.high = m_compiler->emitValueLoad(m_high);
		}
		return result;
	}

	void GcnStateRegister::emitStore(
		const GcnRegisterValuePair& value,
		const GcnRegMask&           mask)
	{
		LOG_ASSERT(mask.popCount() <= 2, "error mask for state register.");

		if (mask.popCount() == 1)
		{
			if (mask[0])
			{
				// low 32 bits
				m_compiler->emitValueStore(m_low, value.low, GcnRegMask::select(0));
			}
			else
			{
				// high 32 bits
				m_compiler->emitValueStore(m_high, value.low, GcnRegMask::select(0));
			}
		}
		else
		{
			// all 64 bits
			m_compiler->emitValueStore(m_low, value.low, GcnRegMask::select(0));
			m_compiler->emitValueStore(m_high, value.high, GcnRegMask::select(0));
		}
	}
}  // namespace sce::gcn
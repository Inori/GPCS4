#include "GcnStateRegister.h"

#include "GcnCompiler.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{

	GcnStateRegister::GcnStateRegister(
		GcnCompiler* compiler,
		const char*  name) :
		m_compiler(compiler),
		m_name(name)
	{

	}

	GcnStateRegister::~GcnStateRegister()
	{
	}

	void GcnStateRegister::create()
	{
		GcnRegisterInfo info;
		info.type.ctype   = GcnScalarType::Uint32;
		info.type.ccount  = 1;
		info.type.alength = 0;
		info.sclass       = spv::StorageClassPrivate;

		m_low.type.ctype  = info.type.ctype;
		m_low.type.ccount = info.type.ccount;
		m_low.id          = m_compiler->emitNewVariable(info);
		m_compiler->m_module.setDebugName(m_low.id,
										  util::str::formatex(m_name, "_lo").c_str());

		m_high.type = m_low.type;
		m_high.id   = m_compiler->emitNewVariable(info);
		m_compiler->m_module.setDebugName(m_high.id,
										  util::str::formatex(m_name, "_hi").c_str());

		info.type.ctype = GcnScalarType::Bool;
		m_zflag         = m_compiler->emitNewVariable(info);
		m_compiler->m_module.setDebugName(m_zflag,
										  util::str::formatex(m_name, "z").c_str());

		m_created = true;
	}

	void GcnStateRegister::init(uint64_t value)
	{
		if (!m_created)
		{
			create();
		}

		uint32_t lowValue  = static_cast<uint32_t>(value & 0xFFFFFFFF);
		uint32_t highValue = static_cast<uint32_t>((value >> 32) & 0xFFFFFFFF);

		auto& module = m_compiler->m_module;

		module.opStore(
			m_low.id,
			module.constu32(lowValue));
		module.opStore(
			m_high.id,
			module.constu32(highValue));
		module.opStore(
			m_zflag,
			module.constBool(static_cast<bool>(value & 1)));
	}

	GcnRegisterValuePair GcnStateRegister::emitLoad(
		const GcnRegMask& mask)
	{
		LOG_ASSERT(mask.popCount() <= 2, "error mask for state register.");
	
		if (!m_created)
		{
			create();
		}

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

		if (!m_created)
		{
			create();
		}

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

		updateZflag();
	}

	uint32_t GcnStateRegister::zflag()
	{
		auto& module = m_compiler->m_module;
		return module.opLoad(module.defBoolType(),
							 m_zflag);
	}

	void GcnStateRegister::updateZflag()
	{
		auto& module = m_compiler->m_module;

		const uint32_t uintTypeId = m_compiler->getVectorTypeId(m_low.type);
		const uint32_t boolTypeId = module.defBoolType();

		// We only need to test the lower part,
		// in fact, only the LSB bit makes sense.
		uint32_t valueId     = module.opLoad(uintTypeId, m_low.id);
		uint32_t conditionId = module.opIEqual(boolTypeId,
											   valueId,
											   module.constu32(0));
		module.opStore(m_zflag, conditionId);
	}

}  // namespace sce::gcn
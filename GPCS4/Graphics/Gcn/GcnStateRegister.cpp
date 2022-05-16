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

	void GcnStateRegister::init(uint32_t lowId, uint32_t highId)
	{
		if (!m_created)
		{
			create();
		}

		auto& module = m_compiler->m_module;

		module.opStore(
			m_low.id,
			lowId);
		module.opStore(
			m_high.id,
			highId);

		updateZflag();
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
				// Note that we store result in low part
				// to uniform some operations.
				result.low = m_compiler->emitValueLoad(m_high);
			}
		}
		else
		{
			// all 64 bits
			result.low  = m_compiler->emitValueLoad(m_low);
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

		uint32_t low         = module.opLoad(uintTypeId, m_low.id);
		uint32_t high        = module.opLoad(uintTypeId, m_high.id);
		uint32_t conditionId = module.opLogicalAnd(boolTypeId,
												   module.opIEqual(boolTypeId,
																   low,
																   module.constu32(0)),
												   module.opIEqual(boolTypeId,
																   high,
																   module.constu32(0)));
		module.opStore(m_zflag, conditionId);
	}



}  // namespace sce::gcn
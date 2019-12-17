#include "GCNStateRegister.h"
#include "GCNCompiler.h"
#include "../SpirV/SpirvModule.h"

namespace pssl
{;
	

SpirvRegisterU64::SpirvRegisterU64(GCNCompiler* compiler, const std::string& name /* = nullptr */) :
	m_compiler(compiler),
	m_module(&m_compiler->m_module),
	m_name(name)
{
	// All lazy initialization
}

SpirvRegisterU64::~SpirvRegisterU64()
{
}

SpirvRegisterPointer SpirvRegisterU64::low()
{
	if (m_low.id == InvalidSpvId)
	{
		m_low = mapAccessPtr(RegType::REG64_LOW);
	}
	return m_low;
}

SpirvRegisterPointer SpirvRegisterU64::high()
{
	if (m_high.id == InvalidSpvId)
	{
		m_high = mapAccessPtr(RegType::REG64_HIGH);
	}
	return m_high;
}

SpirvRegisterPointer SpirvRegisterU64::value()
{
	if (m_value.id == InvalidSpvId)
	{
		m_value = mapAccessPtr(RegType::REG64_FULL);
	}
	return m_value;
}


SpirvRegisterPointer SpirvRegisterU64::createU64Value(const std::string& name)
{
	SpirvRegisterPointer result;
	result.type.ctype   = SpirvScalarType::Uint64;
	result.type.ccount = 1;
	result.id           = m_compiler->emitNewVariable({ result.type, spv::StorageClass::StorageClassPrivate }, name);
	return result;
}

SpirvRegisterPointer SpirvRegisterU64::mapAccessPtr(RegType type)
{
	SpirvRegisterPointer u64Value;
	if (m_value.id == InvalidSpvId)
	{
		u64Value = createU64Value(m_name);
	}

	if (m_vec2Ptr.id == InvalidSpvId)
	{
		m_vec2Ptr.type.ctype            = SpirvScalarType::Uint32;
		m_vec2Ptr.type.ccount           = 2;

		const uint32_t vec2U32PtrTypeId = m_compiler->getPointerTypeId(
			{ m_vec2Ptr.type, spv::StorageClass::StorageClassPrivate });

		m_vec2Ptr.id = m_module->opBitcast(vec2U32PtrTypeId, u64Value.id);
	}

	SpirvRegisterPointer result;
	if (type != RegType::REG64_FULL)
	{
		result.type.ctype           = SpirvScalarType::Uint32;
		result.type.ccount          = 1;

		const uint32_t resultTypeId = m_compiler->getPointerTypeId({ result.type, spv::StorageClass::StorageClassPrivate });
		uint32_t index              = type == RegType::REG64_LOW ? 0 : 1;
		result.id                   = m_module->opAccessChain(resultTypeId, m_vec2Ptr.id, 1, &index);
	}
	else
	{
		result = u64Value;
	}

	return result;
}

}  // namespace pssl
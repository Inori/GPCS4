#include "GCNStateRegister.h"
#include "GCNCompiler.h"
#include "../SpirV/SpirvModule.h"

namespace pssl
{;
	

SpirvRegisterU64::SpirvRegisterU64(GCNCompiler* compiler,
								   const std::string& name,
								   std::optional<uint32_t> initValue) :
	m_compiler(compiler),
	m_module(&m_compiler->m_module),
	m_name(name),
	m_initValue(initValue)
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
		std::string name = m_name + "_lo";
		m_module->setDebugName(m_low.id, name.c_str());
	}
	return m_low;
}

SpirvRegisterPointer SpirvRegisterU64::high()
{
	if (m_high.id == InvalidSpvId)
	{
		m_high = mapAccessPtr(RegType::REG64_HIGH);
		std::string name = m_name + "_hi";
		m_module->setDebugName(m_high.id, name.c_str());
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


pssl::SpirvRegisterPointer SpirvRegisterU64::createU64Value(const std::string& name, std::optional<uint32_t> initValue)
{
	SpirvRegisterPointer result;
	result.type.ctype  = SpirvScalarType::Uint64;
	result.type.ccount = 1;
	result.id          = m_compiler->emitNewVariable(
		{ result.type, spv::StorageClass::StorageClassPrivate }, 
		name, 
		initValue);
	return result;
}

SpirvRegisterPointer SpirvRegisterU64::castToVec2(SpirvRegisterPointer u64Val)
{
	SpirvRegisterPointer result;
	result.type.ctype = SpirvScalarType::Uint32;
	result.type.ccount = 2;

	const uint32_t vec2U32PtrTypeId = m_compiler->getPointerTypeId(
		{ result.type, spv::StorageClass::StorageClassPrivate });

	result.id = m_module->opBitcast(vec2U32PtrTypeId, u64Val.id);
	return result;
}

SpirvRegisterPointer SpirvRegisterU64::mapAccessPtr(RegType type)
{
	if (m_value.id == InvalidSpvId)
	{
		m_value = createU64Value(m_name, m_initValue);
	}

	SpirvRegisterPointer result;
	if (type != RegType::REG64_FULL)
	{
		if (m_vec2Ptr.id == InvalidSpvId)
		{
			m_vec2Ptr = castToVec2(m_value);
		}

		result.type.ctype           = SpirvScalarType::Uint32;
		result.type.ccount          = 1;

		const uint32_t resultTypeId = m_compiler->getPointerTypeId({ result.type, spv::StorageClass::StorageClassPrivate });
		uint32_t index              = type == RegType::REG64_LOW ? 
									  m_module->constu32(0) : m_module->constu32(1);
		result.id                   = m_module->opAccessChain(resultTypeId, m_vec2Ptr.id, 1, &index);
	}
	else
	{
		result = m_value;
	}

	return result;
}

}  // namespace pssl
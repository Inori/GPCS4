#include "GCNStateRegister.h"
#include "GCNCompiler.h"

#include "../SpirV/SpirvModule.h"

namespace pssl
{;

SpirvRegisterValue GcnStateRegister::load(SpirvModule& module) const
{
	module.enableCapability(spv::CapabilityInt64);
	// Define type ids
	uint32_t uintTypeId   = module.defIntType(32, 0);
	uint32_t uint2TypeId  = module.defVectorType(uintTypeId, 2);
	uint32_t uint64TypdId = module.defIntType(64, 0);

	// Load both low and high part
	uint32_t lowId  = module.opLoad(uintTypeId, lo.id);
	uint32_t highId = module.opLoad(uintTypeId, hi.id);

	// Concat low and high part, build a uint2 vector
	std::array<uint32_t, 2> ids = { { highId, lowId } };
	uint32_t uint2Id = module.opCompositeConstruct(
		uint2TypeId,
		ids.size(), ids.data());

	// Cast the uint2 vector to Uint64
	uint32_t uint64Id = module.opBitcast(
		uint64TypdId,
		uint2Id);

	SpirvRegisterValue result;
	result.type.ctype  = SpirvScalarType::Uint64;
	result.type.ccount = 1;
	result.id          = uint64Id;
	return result;
}

}  // namespace pssl
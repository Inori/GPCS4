#include "GnmGpuAddressInternal.h"
#include "GnmGpuAddress.h"

using namespace sce;
using namespace sce::GpuAddress;

#include <cstring>

LOG_CHANNEL("GpuAddress");

int32_t sce::GpuAddress::computeSwizzledBufferSize(Gnm::GpuMode targetGpuMode, uint64_t *outSizeBytes, uint32_t elemStride, uint32_t numElements, Gnm::BufferSwizzleElementSize swizzleSize, Gnm::BufferSwizzleStride swizzleStride)
{
	SCE_GNM_UNUSED(targetGpuMode);
	SCE_GNM_ASSERT_MSG_RETURN(outSizeBytes, kStatusInvalidArgument, "outSizeBytes must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN( (uint32_t)swizzleSize <= Gnm::kBufferSwizzleElementSize16, kStatusInvalidArgument, "swizzleSize (%d) is not a valid enum value.", swizzleSize);
	SCE_GNM_ASSERT_MSG_RETURN( (uint32_t)swizzleStride <= Gnm::kBufferSwizzleStride64, kStatusInvalidArgument, "swizzleStride (%d) is not a valid enum value.", swizzleStride);
	uint64_t actualElemSize = 2ULL << swizzleSize;
	uint64_t actualIndexStride = 8ULL << swizzleStride;

	// It looks like swizzleElemSize must be a multiple of elemStride
	SCE_GNM_ASSERT_MSG_RETURN( (elemStride % actualElemSize) == 0, kStatusInvalidArgument, "elemStride (%u) must be a multiple of 2<<swizzleSize (%d).", elemStride, swizzleSize);

	uint64_t paddedElemStride = (elemStride + (actualElemSize-1)) & ~(actualElemSize-1);
	uint64_t paddedNumElements = (numElements + (actualIndexStride-1)) & ~(actualIndexStride-1);
	*outSizeBytes = paddedElemStride*paddedNumElements;
	return kStatusSuccess;
}
int32_t sce::GpuAddress::computeSwizzledBufferSize(uint64_t *outSizeBytes, uint32_t elementSizeInBytes, uint32_t numElements, Gnm::BufferSwizzleElementSize swizzleSize, Gnm::BufferSwizzleStride swizzleStride) // DEPRECATED
{
	return computeSwizzledBufferSize(Gnm::kGpuModeBase, outSizeBytes, elementSizeInBytes, numElements, swizzleSize, swizzleStride);
}

int32_t sce::GpuAddress::swizzleBufferData(Gnm::GpuMode targetGpuMode, void *outSwizzledData, const void *linearData, uint32_t elemStride, uint32_t numElements, Gnm::BufferSwizzleElementSize swizzleSize, Gnm::BufferSwizzleStride swizzleStride)
{
	SCE_GNM_ASSERT_MSG_RETURN(outSwizzledData != NULL, kStatusInvalidArgument, "outSwizzledData must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(linearData != NULL, kStatusInvalidArgument, "linearData must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN( (uint32_t)swizzleSize <= Gnm::kBufferSwizzleElementSize16, kStatusInvalidArgument, "swizzleSize (%d) is not a valid enum value.", swizzleSize);
	SCE_GNM_ASSERT_MSG_RETURN( (uint32_t)swizzleStride <= Gnm::kBufferSwizzleStride64, kStatusInvalidArgument, "swizzleStride (%d) is not a valid enum value.", swizzleStride);
	uint32_t actualElemSize = 2 << swizzleSize;
	uint32_t actualIndexStride = 8 << swizzleStride;

	SCE_GNM_ASSERT_MSG_RETURN( (elemStride % actualElemSize) == 0, kStatusInvalidArgument, "elemStride (%u) must be a multiple of 2<<swizzleSize (%d).", elemStride, swizzleSize);

	uint8_t *swizzledBytes = (uint8_t*)outSwizzledData;
	const uint8_t *linearBytes = (const uint8_t*)linearData;
	uint32_t linearBufferSize = elemStride*numElements;
	uint64_t swizzledBufferSize = 0;
	int32_t status = computeSwizzledBufferSize(targetGpuMode, &swizzledBufferSize, elemStride, numElements, swizzleSize, swizzleStride);
	if (status != kStatusSuccess)
		return status;
#ifdef SCE_GNM_DEBUG
	memset(outSwizzledData, 0xCD, swizzledBufferSize);
#endif
	for(uint32_t iRecord=0; iRecord<numElements; ++iRecord)
	{
		for(uint32_t iOffset=0; iOffset<elemStride; ++iOffset)
		{
			const uint8_t *pLinear = linearBytes + iRecord*elemStride + iOffset;
			uint8_t *pSwizzled = swizzledBytes +
				((iRecord / actualIndexStride) * elemStride + (iOffset / actualElemSize) * actualElemSize) * actualIndexStride +
				(iRecord % actualIndexStride) * actualElemSize +
				(iOffset % actualElemSize);
			SCE_GNM_ASSERT_MSG_RETURN(pLinear >= linearBytes && pLinear < linearBytes+linearBufferSize, kStatusInternalTilingError, "linear offset is outside buffer bounds");
			SCE_GNM_ASSERT_MSG_RETURN(pSwizzled >= swizzledBytes && pSwizzled < swizzledBytes+swizzledBufferSize, kStatusInternalTilingError, "swizzled offset is outside buffer bounds");
			*pSwizzled = *pLinear;
		}
	}
	return kStatusSuccess;
}
int32_t sce::GpuAddress::swizzleBufferData(void *outSwizzledData, const void *linearData, uint32_t elemStride, uint32_t numElements, Gnm::BufferSwizzleElementSize swizzleSize, Gnm::BufferSwizzleStride swizzleStride) // DEPRECATED
{
	return swizzleBufferData(Gnm::kGpuModeBase, outSwizzledData, linearData, elemStride, numElements, swizzleSize, swizzleStride);
}

int32_t sce::GpuAddress::deswizzleBufferData(Gnm::GpuMode targetGpuMode, void *outLinearData, const void *swizzledData, uint32_t elemStride, uint32_t numElements, Gnm::BufferSwizzleElementSize swizzleSize, Gnm::BufferSwizzleStride swizzleStride)
{
	SCE_GNM_ASSERT_MSG_RETURN(outLinearData != NULL, kStatusInvalidArgument, "outLinearData must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN(swizzledData != NULL, kStatusInvalidArgument, "swizzledData must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN( (uint32_t)swizzleSize <= Gnm::kBufferSwizzleElementSize16, kStatusInvalidArgument, "swizzleSize (%d) is not a valid enum value.", swizzleSize);
	SCE_GNM_ASSERT_MSG_RETURN( (uint32_t)swizzleStride <= Gnm::kBufferSwizzleStride64, kStatusInvalidArgument, "swizzleStride (%d) is not a valid enum value.", swizzleStride);
	uint32_t actualElemSize = 2 << swizzleSize;
	uint32_t actualIndexStride = 8 << swizzleStride;

	SCE_GNM_ASSERT_MSG_RETURN( (elemStride % actualElemSize) == 0, kStatusInvalidArgument, "elemStride (%u) must be a multiple of 2<<swizzleSize (%d).", elemStride, swizzleSize);

	const uint8_t *swizzledBytes = (const uint8_t*)swizzledData;
	uint8_t *linearBytes = (uint8_t*)outLinearData;
	uint32_t linearBufferSize = elemStride*numElements;
	uint64_t swizzledBufferSize = 0;
	int32_t status = computeSwizzledBufferSize(targetGpuMode, &swizzledBufferSize, elemStride, numElements, swizzleSize, swizzleStride);
	if (status != kStatusSuccess)
		return status;
#ifdef SCE_GNM_DEBUG
	memset(outLinearData, 0xCD, linearBufferSize);
#endif
	for(uint32_t iRecord=0; iRecord<numElements; ++iRecord)
	{
		for(uint32_t iOffset=0; iOffset<elemStride; ++iOffset)
		{
			uint8_t *pLinear = linearBytes + iRecord*elemStride + iOffset;
			const uint8_t *pSwizzled = swizzledBytes +
				((iRecord / actualIndexStride) * elemStride + (iOffset / actualElemSize) * actualElemSize) * actualIndexStride +
				(iRecord % actualIndexStride) * actualElemSize +
				(iOffset % actualElemSize);
			SCE_GNM_ASSERT_MSG_RETURN(pLinear >= linearBytes && pLinear < linearBytes+linearBufferSize, kStatusInternalTilingError, "linear offset is outside buffer bounds");
			SCE_GNM_ASSERT_MSG_RETURN(pSwizzled >= swizzledBytes && pSwizzled < swizzledBytes+swizzledBufferSize, kStatusInternalTilingError, "swizzled offset is outside buffer bounds");
			*pLinear = *pSwizzled;
		}
	}
	return kStatusSuccess;
}
int32_t sce::GpuAddress::deswizzleBufferData(void *outLinearData, const void *swizzledData, uint32_t elemStride, uint32_t numElements, Gnm::BufferSwizzleElementSize swizzleSize, Gnm::BufferSwizzleStride swizzleStride) // DEPRECATED
{
	return deswizzleBufferData(Gnm::kGpuModeBase, outLinearData, swizzledData, elemStride, numElements, swizzleSize, swizzleStride);
}

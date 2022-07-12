#include "GcnCompiler.h"
#include "GcnInstructionUtil.h"
#include "UtilVector.h"

using namespace sce::vlt;

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

namespace sce::gcn
{
	void GcnCompiler::emitVectorMemory(const GcnShaderInstruction& ins)
	{
		auto opClass = ins.opClass;
		switch (opClass)
		{
			case GcnInstClass::VectorMemBufNoFmt:
				this->emitVectorMemBufNoFmt(ins);
				break;
			case GcnInstClass::VectorMemBufFmt:
				this->emitVectorMemBufFmt(ins);
				break;
			case GcnInstClass::VectorMemBufAtomic:
				this->emitVectorMemBufAtomic(ins);
				break;
			case GcnInstClass::VectorMemImgNoSmp:
				this->emitVectorMemImgNoSmp(ins);
				break;
			case GcnInstClass::VectorMemImgSmp:
				this->emitVectorMemImgSmp(ins);
				break;
			case GcnInstClass::VectorMemImgUt:
				this->emitVectorMemImgUt(ins);
				break;
			case GcnInstClass::VectorMemL1Cache:
				this->emitVectorMemL1Cache(ins);
				break;
		}
	}

	void GcnCompiler::emitVectorMemBufNoFmt(const GcnShaderInstruction& ins)
	{
		emitVectorMemBuffer(ins);
	}

	void GcnCompiler::emitVectorMemBufFmt(const GcnShaderInstruction& ins)
	{
		emitVectorMemBuffer(ins);
	}

	void GcnCompiler::emitVectorMemBufAtomic(const GcnShaderInstruction& ins)
	{
		auto src     = emitRegisterLoad(ins.src[1]);
		auto ptrList = emitGetBufferComponentPtr(ins, false);

		LOG_ASSERT(ins.control.mubuf.slc == 0, "TODO: support GLC and SLC.");

		GcnRegisterValuePair dst = {};
		dst.low.type.ctype       = getDestinationType(ins.src[1].type);
		dst.low.type.ccount      = 1;
		dst.high.type            = dst.low.type;

		const uint32_t typeId = getScalarTypeId(dst.low.type.ctype);

		// should we use device scope?
		uint32_t scope     = spv::ScopeDevice;
		uint32_t semantics = spv::MemorySemanticsUniformMemoryMask |
							 spv::MemorySemanticsAcquireReleaseMask;

		const uint32_t scopeId     = m_module.constu32(scope);
		const uint32_t semanticsId = m_module.constu32(semantics);

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::BUFFER_ATOMIC_ADD:
				dst.low.id = m_module.opAtomicIAdd(typeId,
												   ptrList[0].id,
												   scopeId, semanticsId,
												   src.low.id);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

		bool saveOriginal = ins.control.mubuf.glc != 0;
		if (saveOriginal)
		{
			emitRegisterStore(ins.src[1], dst);
		}
	}

	void GcnCompiler::emitVectorMemImgNoSmp(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::IMAGE_LOAD_MIP:
				emitStorageImageLoad(ins);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::emitVectorMemImgSmp(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::IMAGE_SAMPLE:
			case GcnOpcode::IMAGE_SAMPLE_L:
			case GcnOpcode::IMAGE_SAMPLE_LZ_O:
			case GcnOpcode::IMAGE_SAMPLE_LZ:
			case GcnOpcode::IMAGE_SAMPLE_C:
				emitTextureSample(ins);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::emitVectorMemImgUt(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::IMAGE_GET_RESINFO:
				this->emitQueryTextureInfo(ins);
				break;
			case GcnOpcode::IMAGE_GET_LOD:
				this->emitQueryTextureLod(ins);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::emitVectorMemL1Cache(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorMemBuffer(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::BUFFER_LOAD_FORMAT_X:
			case GcnOpcode::BUFFER_LOAD_FORMAT_XY:
			case GcnOpcode::BUFFER_LOAD_FORMAT_XYZ:
			case GcnOpcode::BUFFER_LOAD_FORMAT_XYZW:
			case GcnOpcode::TBUFFER_LOAD_FORMAT_X:
			case GcnOpcode::TBUFFER_LOAD_FORMAT_XY:
			case GcnOpcode::TBUFFER_LOAD_FORMAT_XYZ:
			case GcnOpcode::TBUFFER_LOAD_FORMAT_XYZW:
				emitBufferLoadStoreFmt(ins, true);
				break;
			case GcnOpcode::BUFFER_STORE_FORMAT_X:
			case GcnOpcode::BUFFER_STORE_FORMAT_XY:
			case GcnOpcode::BUFFER_STORE_FORMAT_XYZ:
			case GcnOpcode::BUFFER_STORE_FORMAT_XYZW:
			case GcnOpcode::TBUFFER_STORE_FORMAT_X:
			case GcnOpcode::TBUFFER_STORE_FORMAT_XY:
			case GcnOpcode::TBUFFER_STORE_FORMAT_XYZ:
			case GcnOpcode::TBUFFER_STORE_FORMAT_XYZW:
				emitBufferLoadStoreFmt(ins, false);
				break;
			case GcnOpcode::BUFFER_LOAD_UBYTE:
			case GcnOpcode::BUFFER_LOAD_SBYTE:
			case GcnOpcode::BUFFER_LOAD_USHORT:
			case GcnOpcode::BUFFER_LOAD_SSHORT:
			case GcnOpcode::BUFFER_LOAD_DWORD:
			case GcnOpcode::BUFFER_LOAD_DWORDX2:
			case GcnOpcode::BUFFER_LOAD_DWORDX4:
			case GcnOpcode::BUFFER_LOAD_DWORDX3:
				emitBufferLoadStoreNoFmt(ins, true);
				break;
			case GcnOpcode::BUFFER_STORE_BYTE:
			case GcnOpcode::BUFFER_STORE_SHORT:
			case GcnOpcode::BUFFER_STORE_DWORD:
			case GcnOpcode::BUFFER_STORE_DWORDX2:
			case GcnOpcode::BUFFER_STORE_DWORDX4:
			case GcnOpcode::BUFFER_STORE_DWORDX3:
				emitBufferLoadStoreNoFmt(ins, false);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	GcnRegisterValue GcnCompiler::emitQueryTextureSize(const GcnShaderInstruction& ins)
	{
		const GcnInstOperand& textureReg = ins.src[2];
		const uint32_t        textureId  = textureReg.code * 4;
		const GcnTexture&     info       = m_textures.at(textureId);

		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Uint32;
		result.type.ccount = getTexSizeDim(info.imageInfo);

		if (info.imageInfo.ms == 0 && info.imageInfo.sampled == 1)
		{
			auto lod  = emitRegisterLoad(ins.src[0]);

			result.id = m_module.opImageQuerySizeLod(
				getVectorTypeId(result.type),
				m_module.opLoad(info.imageTypeId, info.varId),
				lod.low.id);
		}
		else
		{
			result.id = m_module.opImageQuerySize(
				getVectorTypeId(result.type),
				m_module.opLoad(info.imageTypeId, info.varId));
		}

		return result;
	}

	GcnRegisterValue GcnCompiler::emitQueryTextureLevels(const GcnShaderInstruction& ins)
	{
		const GcnInstOperand& textureReg = ins.src[2];
		const uint32_t        textureId  = textureReg.code * 4;
		const GcnTexture&     info       = m_textures.at(textureId);

		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Uint32;
		result.type.ccount = 1;

		if (info.imageInfo.sampled == 1)
		{
			result.id = m_module.opImageQueryLevels(
				getVectorTypeId(result.type),
				m_module.opLoad(info.imageTypeId, info.varId));
		}
		else
		{
			// Report one LOD in case of UAVs
			result.id = m_module.constu32(1);
		}
		return result;
	}

	void GcnCompiler::emitQueryTextureInfo(const GcnShaderInstruction& ins)
	{
		GcnImageResFlags flags = GcnImageResFlags(ins.control.mimg.dmask);

		GcnRegisterValue textureSize  = emitQueryTextureSize(ins);
		GcnRegisterValue textureLevel = {}; 
		if (flags.test(GcnImageResComponent::MipCount))
		{
			textureLevel = emitQueryTextureLevels(ins);
		}

		auto     vdata = ins.src[1];
		uint32_t index = vdata.code;
		if (flags.test(GcnImageResComponent::Width))
		{
			vdata.code = index++;
			auto value = emitRegisterExtract(textureSize, GcnRegMask::select(0));
			emitVgprStore(vdata, value);
		}

		if (flags.test(GcnImageResComponent::Height))
		{
			vdata.code = index++;
			auto value = emitRegisterExtract(textureSize, GcnRegMask::select(1));
			emitVgprStore(vdata, value);
		}

		if (flags.test(GcnImageResComponent::Depth))
		{
			vdata.code = index++;
			auto value = emitRegisterExtract(textureSize, GcnRegMask::select(2));
			emitVgprStore(vdata, value);
		}

		if (flags.test(GcnImageResComponent::MipCount))
		{
			vdata.code = index++;
			emitVgprStore(vdata, textureLevel);
		}
	}

	void GcnCompiler::emitQueryTextureLod(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	GcnRegisterValue GcnCompiler::emitCalcBufferAddress(
		const GcnShaderInstruction& ins)
	{
		bool     idxen     = false;
		bool     offen     = false;
		uint32_t optOffset = 0;
		if (ins.encoding == GcnInstEncoding::MUBUF)
		{
			idxen     = ins.control.mubuf.idxen;
			offen     = ins.control.mubuf.offen;
			optOffset = ins.control.mubuf.offset;
		}
		else
		{
			idxen     = ins.control.mtbuf.idxen;
			offen     = ins.control.mtbuf.offen;
			optOffset = ins.control.mtbuf.offset;
		}

		const uint32_t zero       = m_module.constu32(0);
		auto           bufferInfo = getBufferType(ins.src[2]);

		const uint32_t typdId = getScalarTypeId(GcnScalarType::Uint32);

		auto soff = emitRegisterLoad(ins.src[3]);
		// sV#.base is zero in our case.
		uint32_t base  = soff.low.id;
		uint32_t index = idxen ? emitRegisterLoad(ins.src[0]).low.id : zero;

		GcnInstOperand offsetReg = ins.src[0];
		offsetReg.code += static_cast<uint32_t>(idxen);

		uint32_t offset = offen ? emitRegisterLoad(offsetReg).low.id : zero;
		offset          = m_module.opIAdd(typdId, offset, m_module.constu32(optOffset));

		uint32_t stride = m_module.constu32(bufferInfo.buffer.stride);

		LOG_ASSERT(bufferInfo.buffer.isSwizzle == false, "TODO: support swizzle buffer.");

		// Note the returned address is in bytes.
		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Uint32;
		result.type.ccount = 1;

		result.id = m_module.opIAdd(typdId,
									m_module.opIAdd(typdId, base, offset),
									m_module.opIMul(typdId, index, stride));
		return result;
	}

	std::vector<GcnRegisterPointer>
	GcnCompiler::emitGetBufferComponentPtr(const GcnShaderInstruction& ins,
										   bool                        isFormat)
	{
		auto op = ins.opcode;

		auto bufferInfo = getBufferType(ins.src[2]);

		Gnm::BufferFormat      dfmt;
		Gnm::BufferChannelType nfmt;
		uint32_t               size = 0;

		if (ins.encoding == GcnInstEncoding::MUBUF)
		{
			dfmt = bufferInfo.buffer.dfmt;
			nfmt = bufferInfo.buffer.nfmt;
			size = ins.control.mubuf.size;
		}
		else
		{
			dfmt = (Gnm::BufferFormat)ins.control.mtbuf.dfmt;
			nfmt = (Gnm::BufferChannelType)ins.control.mtbuf.nfmt;
		}

		GcnRegisterInfo info;
		info.type.ctype   = GcnScalarType::Uint32;
		info.type.ccount  = 1;
		info.type.alength = 0;
		info.sclass       = spv::StorageClassUniform;

		uint32_t ptrTypeId  = getPointerTypeId(info);
		uint32_t uintTypeId = getScalarTypeId(GcnScalarType::Uint32);

		uint32_t dataCount = 0;
		if (isFormat)
		{
			auto bufferFormat = getBufferFormat(dfmt, nfmt);
			dataCount         = bufferFormat.sizeInBytes < 4 ? 1 : bufferFormat.sizeInBytes / 4;
		}
		else
		{
			LOG_ASSERT(size != 0, "error instruction size.");
			dataCount = size > 4 ? size / 4 : 1;
		}

		LOG_ASSERT(nfmt == Gnm::kBufferChannelTypeFloat ||
					   nfmt == Gnm::kBufferChannelTypeUInt ||
					   nfmt == Gnm::kBufferChannelTypeSInt,
				   "TODO: support encoded channel type.");

		auto address = emitCalcBufferAddress(ins);

		std::vector<GcnRegisterPointer> ptrList;
		if (bufferInfo.isSsbo)
		{
			ptrList = emitStorageBufferAccess(bufferInfo.varId,
											  address.id,
											  dataCount);
		}
		else
		{
			ptrList = emitUniformBufferAccess(bufferInfo.varId,
											  address.id,
											  dataCount);
		}
		return ptrList;
	}

	void GcnCompiler::emitBufferLoadStoreNoFmt(const GcnShaderInstruction& ins,
											   bool                        isLoad)
	{
		auto     op         = ins.opcode;
		auto     bufferInfo = getBufferType(ins.src[2]);
		uint32_t size       = ins.control.mubuf.size;

		bool isSigned = op == GcnOpcode::BUFFER_LOAD_SBYTE ||
						op == GcnOpcode::BUFFER_LOAD_SSHORT;

		GcnScalarType dataType   = bufferInfo.isSsbo ? GcnScalarType::Uint32 : GcnScalarType::Float32;
		uint32_t      dataTypeId = getScalarTypeId(dataType);

		auto ptrList = emitGetBufferComponentPtr(ins, false);

		uint32_t vgprCount = size > 4 ? size / 4 : 1;
		for (uint32_t i = 0; i != vgprCount; ++i)
		{
			const auto&    ptr = ptrList[i];
			GcnInstOperand reg = ins.src[1];
			reg.code += i;

			if (isLoad)
			{
				uint32_t         dataId = m_module.opLoad(dataTypeId,
														  ptr.id);
				GcnRegisterValue value;
				value.type.ctype  = dataType;
				value.type.ccount = 1;

				if (size < 4)
				{
					uint32_t uintTypeId = getScalarTypeId(GcnScalarType::Uint32);
					uint32_t sintTypeId = getScalarTypeId(GcnScalarType::Sint32);

					uint32_t src = dataId;
					if (isSigned)
					{
						src = m_module.opBitcast(sintTypeId, src);
					}
					else if (dataType == GcnScalarType::Float32)
					{
						src = m_module.opBitcast(uintTypeId, src);
					}

					value.id = isSigned ? m_module.opBitFieldSExtract(sintTypeId,
																	  src,
																	  m_module.constu32(0),
																	  m_module.constu32(8 * size))
										: m_module.opBitFieldUExtract(uintTypeId,
																	  src,
																	  m_module.constu32(0),
																	  m_module.constu32(8 * size));
				}
				else
				{
					value.id = dataId;
				}

				emitVgprStore(reg, value);
			}
			else
			{
				auto value = emitVgprLoad(reg);
				if (size < 4)
				{
					LOG_ASSERT(false, "support byte and ushort store.");
				}
				else
				{
					value = emitRegisterBitcast(value, dataType);
				}
				m_module.opStore(ptr.id, value.id);
			}
		}
	}

	void GcnCompiler::emitBufferLoadStoreFmt(const GcnShaderInstruction& ins, bool isLoad)
	{
		auto bufferInfo = getBufferType(ins.src[2]);

		uint32_t               count = 0;
		Gnm::BufferFormat      dfmt;
		Gnm::BufferChannelType nfmt;

		if (ins.encoding == GcnInstEncoding::MUBUF)
		{
			count = ins.control.mubuf.count;
			dfmt  = bufferInfo.buffer.dfmt;
			nfmt  = bufferInfo.buffer.nfmt;
		}
		else
		{
			count = ins.control.mtbuf.count;
			dfmt  = (Gnm::BufferFormat)ins.control.mtbuf.dfmt;
			nfmt  = (Gnm::BufferChannelType)ins.control.mtbuf.nfmt;
		}

		auto bufferFormat = getBufferFormat(dfmt, nfmt);

		GcnScalarType dataType   = bufferInfo.isSsbo ? GcnScalarType::Uint32 : GcnScalarType::Float32;
		uint32_t      dataTypeId = getScalarTypeId(dataType);

		auto     dataPtr   = emitGetBufferComponentPtr(ins, true);
		uint32_t vgprCount = count;
		for (uint32_t c = 0; c != bufferFormat.channelCount; ++c)
		{
			if (isLoad)
			{
				switch (dfmt)
				{
					case Gnm::kBufferFormatInvalid:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat8:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat16:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat8_8:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat32:
					{
						GcnInstOperand reg = ins.src[1];

						uint32_t itemId = m_module.opLoad(dataTypeId, dataPtr[c].id);

						GcnRegisterValue value;
						value.type.ctype  = dataType;
						value.type.ccount = 1;
						value.id          = itemId;

						emitVgprStore(reg, value);
					}
					break;
					case Gnm::kBufferFormat16_16:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat10_11_11:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat11_11_10:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat10_10_10_2:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat2_10_10_10:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat8_8_8_8:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat32_32:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat16_16_16_16:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat32_32_32:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat32_32_32_32:
					{
						if (c >= vgprCount)
						{
							// discard extra channel data
							break;
						}

						GcnInstOperand reg = ins.src[1];
						reg.code += c;

						uint32_t itemId = m_module.opLoad(dataTypeId, dataPtr[c].id);

						GcnRegisterValue value;
						value.type.ctype  = dataType;
						value.type.ccount = 1;
						value.id          = itemId;

						emitVgprStore(reg, value);
					}
					break;
				}
			}
			else
			{
				switch (dfmt)
				{
					case Gnm::kBufferFormatInvalid:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat8:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat16:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat8_8:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat32:
					{
						GcnInstOperand reg   = ins.src[1];
						auto           value = emitVgprLoad(reg);
						value                = emitRegisterBitcast(value, dataType);
						m_module.opStore(dataPtr[c].id, value.id);
					}
					break;
					case Gnm::kBufferFormat16_16:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat10_11_11:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat11_11_10:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat10_10_10_2:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat2_10_10_10:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat8_8_8_8:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat32_32:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat16_16_16_16:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat32_32_32:
						LOG_GCN_UNHANDLED_INST();
						break;
					case Gnm::kBufferFormat32_32_32_32:
					{
						uint32_t valueId = 0;
						if (c >= vgprCount)
						{
							valueId = m_module.constu32(0.0);
						}
						else
						{
							GcnInstOperand reg = ins.src[1];
							reg.code += c;

							auto value = emitVgprLoad(reg);
							value      = emitRegisterBitcast(value, dataType);
							valueId    = value.id;
						}

						m_module.opStore(dataPtr[c].id, valueId);
					}
					break;
				}
			}
		}

		// TODO:
		// Zero the extra vgpr
		if (isLoad && vgprCount > bufferFormat.channelCount)
		{
			uint32_t zeroCount = vgprCount - bufferFormat.channelCount;
		}
	}

	void GcnCompiler::emitTextureSample(const GcnShaderInstruction& ins)
	{
		auto                  mimg        = gcnInstructionAs<GcnShaderInstMIMG>(ins);
		const GcnInstOperand& texCoordReg = mimg.vaddr;
		const GcnInstOperand& textureReg  = mimg.srsrc;
		const GcnInstOperand& samplerReg  = mimg.ssamp;
		auto                  flags       = GcnMimgModifierFlags(mimg.control.mod);

		// Texture and sampler register IDs
		// These registers are 4-GPR aligned, so multiplied by 4
		const uint32_t textureId = textureReg.code * 4;
		const uint32_t samplerId = samplerReg.code * 4;

		// Image type, which stores the image dimensions etc.
		const GcnImageInfo imageType = m_textures.at(textureId).imageInfo;

		// Load the texture coordinates. SPIR-V allows these
		// to be float4 even if not all components are used.
		GcnRegisterValue coord = emitLoadTexCoord(ins);

		auto op             = ins.opcode;
		bool isDepthCompare = flags.test(GcnMimgModifier::Pcf);

		const GcnRegisterValue referenceValue = isDepthCompare
													? emitLoadAddrComponent(GcnImageAddrComponent::Zpcf, ins)
													: GcnRegisterValue();

		// LOD for certain sample operations
		const bool hasLod = flags.test(GcnMimgModifier::Lod);

		const GcnRegisterValue lod = hasLod
										 ? emitLoadAddrComponent(GcnImageAddrComponent::Lod, ins)
										 : GcnRegisterValue();

		// Accumulate additional image operands. These are
		// not part of the actual operand token in SPIR-V.
		SpirvImageOperands imageOperands = {};

		if (flags.test(GcnMimgModifier::Offset))
		{
			m_module.enableCapability(spv::CapabilityImageGatherExtended);

			auto offset = emitLoadTexOffset(ins);

			imageOperands.flags |= spv::ImageOperandsOffsetMask;
			imageOperands.sConstOffset = offset.id;
		}

		// Combine the texture and the sampler into a sampled image
		const uint32_t sampledImageId = emitLoadSampledImage(m_textures.at(textureId),
															 m_samplers.at(samplerId),
															 isDepthCompare);

		// Sampling an image always returns a four-component
		// vector, whereas depth-compare ops return a scalar.
		GcnRegisterValue result = {};
		result.type.ctype       = m_textures.at(textureId).sampledType;
		result.type.ccount      = isDepthCompare ? 1 : 4;

		switch (op)
		{
			// Simple image sample operation
			case GcnOpcode::IMAGE_SAMPLE:
			{
				result.id = m_module.opImageSampleImplicitLod(
					getVectorTypeId(result.type),
					sampledImageId, coord.id,
					imageOperands);
			}
				break;
			case GcnOpcode::IMAGE_SAMPLE_L:
			{
				imageOperands.flags |= spv::ImageOperandsLodMask;
				imageOperands.sLod = lod.id;

				result.id = m_module.opImageSampleExplicitLod(
					getVectorTypeId(result.type), sampledImageId, coord.id,
					imageOperands);
			}
				break;
			case GcnOpcode::IMAGE_SAMPLE_LZ:
			case GcnOpcode::IMAGE_SAMPLE_LZ_O:
			{
				imageOperands.flags |= spv::ImageOperandsLodMask;
				imageOperands.sLod = m_module.constf32(0.0);

				result.id = m_module.opImageSampleExplicitLod(
					getVectorTypeId(result.type), sampledImageId, coord.id,
					imageOperands);
			}
				break;
			case GcnOpcode::IMAGE_SAMPLE_C:
			{
				result.id = m_module.opImageSampleDrefImplicitLod(
					getVectorTypeId(result.type), sampledImageId, coord.id,
					referenceValue.id, imageOperands);
			}
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}

		auto colorMask = GcnRegMask(mimg.control.dmask);
		result         = emitRegisterExtract(result, colorMask);
		emitVgprArrayStore(mimg.vdata,
						   result,
						   colorMask);
	}

	void GcnCompiler::emitStorageImageLoad(const GcnShaderInstruction& ins)
	{
		auto mimg = gcnInstructionAs<GcnShaderInstMIMG>(ins);

		const uint32_t    registerId = mimg.srsrc.code << 2;
		const GcnTexture& typeInfo   = m_textures.at(registerId);

		// Load texture coordinates
		GcnRegisterValue coord = emitLoadTexCoord(ins);

		// Additional image operands. This will store
		// the LOD and other information if present.
		SpirvImageOperands imageOperands;

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::IMAGE_LOAD_MIP:
			{
				GcnRegisterValue imageLod = 
					emitLoadAddrComponent(GcnImageAddrComponent::Lod, ins);

				imageOperands.flags |= spv::ImageOperandsLodMask;
				imageOperands.sLod = imageLod.id;
			}
			break;
		}

		// Load source value from the storage image.
		GcnRegisterValue result;
		result.type.ctype  = typeInfo.sampledType;
		result.type.ccount = 4;
		result.id          = m_module.opImageFetch(
					 getVectorTypeId(result.type),
					 m_module.opLoad(typeInfo.imageTypeId, typeInfo.varId),
					 coord.id, imageOperands);

		// Apply component swizzle and mask
		auto colorMask = GcnRegMask(mimg.control.dmask);
		result         = emitRegisterExtract(result, colorMask);
		emitVgprArrayStore(mimg.vdata,
						   result,
						   colorMask);
	}

	GcnRegisterValue GcnCompiler::emitCalcTexCoord(
		GcnRegisterValue    coordVector,
		const GcnImageInfo& imageInfo)
	{
		const uint32_t dim = getTexCoordDim(imageInfo);

		if (dim != coordVector.type.ccount)
		{
			coordVector = emitRegisterExtract(
				coordVector, GcnRegMask::firstN(dim));
		}

		return coordVector;
	}

	GcnRegisterValue GcnCompiler::emitLoadTexCoord(
		const GcnShaderInstruction& ins)
	{
		GcnInstOperand addrReg = ins.src[0];

		const GcnImageInfo imageInfo = getImageInfo(ins);

		uint32_t dim        = getTexCoordDim(imageInfo);
		uint32_t coordIndex = calcAddrComponentIndex(
			GcnImageAddrComponent::X, ins);

		addrReg.code += coordIndex;
		GcnRegisterValue coord = emitVgprArrayLoad(
			addrReg, GcnRegMask::firstN(dim));

		auto result = emitCalcTexCoord(coord, imageInfo);
		// Some non-sampling image instructions use
		// integer coordinate offset, we need to do a cast.
		result = emitRegisterBitcast(result, addrReg.type);

		if (imageInfo.dim == spv::DimCube)
		{
			// Why do we need recover?
			// See comments in emitCubeCalculate.
			result = emitRecoverCubeCoord(result);
		}
		return result;
	}

	GcnRegisterValue GcnCompiler::emitLoadTexOffset(const GcnShaderInstruction& ins)
	{
		const uint32_t typeId  = getScalarTypeId(GcnScalarType::Uint32);
		auto           offsets = emitLoadAddrComponent(GcnImageAddrComponent::Offsets, ins);

		const GcnImageInfo imageInfo = getImageInfo(ins);
		uint32_t           dim       = getTexCoordDim(imageInfo);

		util::static_vector<uint32_t, 3> components;
		for (uint32_t i = 0; i != dim; ++i)
		{
			uint32_t offsetId = m_module.opBitFieldUExtract(typeId,
															offsets.id,
															m_module.constu32(i * 8),
															m_module.constu32(6));
			components.push_back(offsetId);
		}

		GcnRegisterValue result = {};
		result.type.ctype       = GcnScalarType::Uint32;
		result.type.ccount      = components.size();
		result.id               = m_module.opCompositeConstruct(getVectorTypeId(result.type),
																components.size(), components.data());
		return result;
	}

	GcnRegisterValue GcnCompiler::emitRecoverCubeCoord(
		const GcnRegisterValue& coord)
	{
		LOG_ASSERT(coord.type.ccount == 3, "cube coordinate must be vec3.");
		auto s = emitRegisterExtract(coord, GcnRegMask::select(0));
		auto t = emitRegisterExtract(coord, GcnRegMask::select(1));
		auto z = emitRegisterExtract(coord, GcnRegMask::select(2));

		const uint32_t typeId = getScalarTypeId(GcnScalarType::Float32);

		// We need to fix x and y coordinate,
		// because the s and t coordinate will be scaled and plus 1.5
		// by v_madak_f32.
		// We already force the scale value to be 1.0 when handling v_cubema_f32,
		// here we subtract 1.5 to recover the original value. 
		auto x = m_module.opFSub(typeId, s.id, m_module.constf32(1.5));
		auto y = m_module.opFSub(typeId, t.id, m_module.constf32(1.5));

		std::array<uint32_t, 3> direction = { x, y, z.id };

		GcnRegisterValue result;
		result.type = coord.type;
		result.id   = m_module.opCompositeConstruct(getVectorTypeId(result.type),
													direction.size(),
													direction.data());
		return result;
	}

	GcnRegisterValue GcnCompiler::emitLoadAddrComponent(
		GcnImageAddrComponent       component,
		const GcnShaderInstruction& ins)
	{
		const GcnInstOperand& addrReg    = ins.src[0];
		const GcnInstOperand& textureReg = ins.src[2];
		auto                  flags      = GcnMimgModifierFlags(ins.control.mimg.mod);
	
		// These registers are 4-GPR aligned, so multiplied by 4
		const uint32_t textureId = textureReg.code * 4;
	
		// Image type, which stores the image dimensions etc.
		const GcnImageInfo imageInfo = m_textures.at(textureId).imageInfo;

		uint32_t index = calcAddrComponentIndex(component, ins);
		auto     reg   = addrReg;
		reg.code += index;

		GcnScalarType type = GcnScalarType::Float32;
		if (isImageAccessNoSampling(ins))
		{
			// Non-sampling instructions always use 
			// integer components
			type = GcnScalarType::Uint32;
		}
		else
		{
			auto type = flags.test(GcnMimgModifier::Offset)
							? GcnScalarType::Uint32
							: GcnScalarType::Float32;
		}
		return emitRegisterBitcast(emitVgprLoad(reg), type);
	}

	uint32_t GcnCompiler::emitLoadSampledImage(
		const GcnTexture& textureResource,
		const GcnSampler& samplerResource,
		bool              isDepthCompare)
	{
		const uint32_t sampledImageType = isDepthCompare
											  ? m_module.defSampledImageType(textureResource.depthTypeId)
											  : m_module.defSampledImageType(textureResource.colorTypeId);

		return m_module.opSampledImage(sampledImageType,
									   m_module.opLoad(textureResource.imageTypeId, textureResource.varId),
									   m_module.opLoad(samplerResource.typeId, samplerResource.varId));
	}

	
	GcnBufferInfo GcnCompiler::getBufferType(
		const GcnInstOperand& reg)
	{
		uint32_t regIdx = reg.code << 2;

		GcnBufferMeta* meta = nullptr;
		switch (m_programInfo.type())
		{
			case GcnProgramType::VertexShader: meta = &m_meta.vs.bufferInfos[regIdx]; break;
			case GcnProgramType::PixelShader: meta = &m_meta.ps.bufferInfos[regIdx]; break;
			case GcnProgramType::ComputeShader: meta = &m_meta.cs.bufferInfos[regIdx]; break;
			case GcnProgramType::GeometryShader: meta = &m_meta.gs.bufferInfos[regIdx]; break;
			case GcnProgramType::HullShader: meta = &m_meta.hs.bufferInfos[regIdx]; break;
			case GcnProgramType::DomainShader: meta = &m_meta.ds.bufferInfos[regIdx]; break;
		}

		auto& buffer = m_buffers[regIdx];

		GcnBufferInfo result = {};
		result.varId         = buffer.varId;
		result.isSsbo        = buffer.asSsbo;
		result.buffer        = *meta;
		result.image         = GcnImageInfo();

		return result;
	}

	GcnImageInfo GcnCompiler::getImageType(
		Gnm::TextureType textureType,
		bool             isUav,
		bool             isDepth) const
	{
		uint32_t     depth    = isDepth ? 1u : 0u;
		uint32_t     sampled  = isUav ? 2u : 1u;
		GcnImageInfo typeInfo = [textureType, depth, sampled]() -> GcnImageInfo
		{
			switch (textureType)
			{
				case Gnm::kTextureType1d:
					return { spv::Dim1D, depth, 0, 0, sampled, VK_IMAGE_VIEW_TYPE_1D };
				case Gnm::kTextureType2d:
					return { spv::Dim2D, depth, 0, 0, sampled, VK_IMAGE_VIEW_TYPE_2D };
				case Gnm::kTextureType3d:
					return { spv::Dim3D, depth, 0, 0, sampled, VK_IMAGE_VIEW_TYPE_3D };
				case Gnm::kTextureTypeCubemap:
					return { spv::DimCube, depth, 0, 0, sampled, VK_IMAGE_VIEW_TYPE_CUBE };
				case Gnm::kTextureType1dArray:
					return { spv::Dim1D, depth, 1, 0, sampled, VK_IMAGE_VIEW_TYPE_1D_ARRAY };
				case Gnm::kTextureType2dArray:
					return { spv::Dim2D, depth, 1, 0, sampled, VK_IMAGE_VIEW_TYPE_2D_ARRAY };
				case Gnm::kTextureType2dMsaa:
					return { spv::Dim2D, depth, 0, 1, sampled, VK_IMAGE_VIEW_TYPE_2D };
				case Gnm::kTextureType2dArrayMsaa:
					return { spv::Dim2D, depth, 1, 1, sampled, VK_IMAGE_VIEW_TYPE_2D_ARRAY };
				default:
					Logger::exception(util::str::formatex("GcnCompiler: Unsupported resource type: ", textureType));
			}
		}();

		return typeInfo;
	}

	GcnImageInfo GcnCompiler::getImageInfo(const GcnShaderInstruction& ins) const
	{
		const GcnInstOperand& textureReg = ins.src[2];
		const uint32_t        textureId  = textureReg.code * 4;
		GcnImageInfo          imageInfo  = m_textures.at(textureId).imageInfo;
		return imageInfo;
	}

	uint32_t GcnCompiler::getTexSizeDim(const GcnImageInfo& imageType) const
	{
		switch (imageType.dim)
		{
			case spv::DimBuffer: return 1 + imageType.array;
			case spv::Dim1D: return 1 + imageType.array;
			case spv::Dim2D: return 2 + imageType.array;
			case spv::Dim3D: return 3 + imageType.array;
			case spv::DimCube: return 2 + imageType.array;
			default: Logger::exception("DxbcCompiler: getTexLayerDim: Unsupported image dimension");
		}
	}

	uint32_t GcnCompiler::getTexLayerDim(const GcnImageInfo& imageType) const
	{
		switch (imageType.dim)
		{
			case spv::DimBuffer: return 1;
			case spv::Dim1D: return 1;
			case spv::Dim2D: return 2;
			case spv::Dim3D: return 3;
			case spv::DimCube: return 3;
			default: Logger::exception("DxbcCompiler: getTexLayerDim: Unsupported image dimension");
		}
	}

	uint32_t GcnCompiler::getTexCoordDim(const GcnImageInfo& imageType) const
	{
		return getTexLayerDim(imageType) + imageType.array;
	}

	uint32_t GcnCompiler::calcAddrComponentIndex(
		GcnImageAddrComponent       component,
		const GcnShaderInstruction& ins)
	{
		int32_t index      = -1;
		auto    flags      = GcnMimgModifierFlags(ins.control.mimg.mod);
		auto    imageInfo  = getImageInfo(ins);
		auto    dim        = imageInfo.dim;
		auto    msaa       = imageInfo.ms;
		bool    noSampling = isImageAccessNoSampling(ins);

		// clang-format off
		switch (component)
		{
			case GcnImageAddrComponent::Clamp:
				if (flags.test(GcnMimgModifier::LodClamp)) ++index;
				[[fallthrough]];
			case GcnImageAddrComponent::Lod:
				if (flags.test(GcnMimgModifier::Lod)) ++index;
				[[fallthrough]];
			case GcnImageAddrComponent::FragId:
				if (noSampling &&
					dim == spv::Dim2D &&
					msaa != 0) ++index;
				[[fallthrough]];
			case GcnImageAddrComponent::FaceId:
				if (dim == spv::DimCube) ++index;
				[[fallthrough]];
			case GcnImageAddrComponent::Slice:
				if (ins.control.mimg.da != 0 &&
					dim != spv::DimCube) ++index;
				[[fallthrough]];
			case GcnImageAddrComponent::Z:
				if (dim == spv::Dim3D) ++index;
				[[fallthrough]];
			case GcnImageAddrComponent::Y:
				if (dim == spv::Dim2D ||
					dim == spv::Dim3D ||
					dim == spv::DimCube) ++index;
				[[fallthrough]];
			case GcnImageAddrComponent::X:
				++index;
				[[fallthrough]];
			case GcnImageAddrComponent::DzDv:
				if (flags.any(GcnMimgModifier::Derivative, 
							  GcnMimgModifier::CoarseDerivative) &&
					dim == spv::Dim3D) ++index;
				[[fallthrough]];
			case GcnImageAddrComponent::DyDv:
				if (flags.any(GcnMimgModifier::Derivative, 
							  GcnMimgModifier::CoarseDerivative) &&
				    (dim == spv::Dim2D ||
					 dim == spv::Dim3D ||
					 dim == spv::DimCube)) ++index;
				[[fallthrough]];
			case GcnImageAddrComponent::DxDv:
				if (flags.any(GcnMimgModifier::Derivative, 
							  GcnMimgModifier::CoarseDerivative)) ++index;
				[[fallthrough]];
			case GcnImageAddrComponent::DzDh:
				if (flags.any(GcnMimgModifier::Derivative, 
							  GcnMimgModifier::CoarseDerivative) &&
					dim == spv::Dim3D) ++index;
				[[fallthrough]];
			case GcnImageAddrComponent::DyDh:
				if (flags.any(GcnMimgModifier::Derivative, 
							  GcnMimgModifier::CoarseDerivative) &&
				    (dim == spv::Dim2D ||
					 dim == spv::Dim3D ||
					 dim == spv::DimCube)) ++index;
				[[fallthrough]];
			case GcnImageAddrComponent::DxDh:
				if (flags.any(GcnMimgModifier::Derivative, 
							  GcnMimgModifier::CoarseDerivative)) ++index;
				[[fallthrough]];
			case GcnImageAddrComponent::Zpcf:
				if (flags.test(GcnMimgModifier::Pcf)) ++index;
				[[fallthrough]];
			case GcnImageAddrComponent::Bias:
				if (flags.test(GcnMimgModifier::LodBias)) ++index;
				[[fallthrough]];
			case GcnImageAddrComponent::Offsets:
				if (flags.test(GcnMimgModifier::Offset)) ++index;
		}
		// clang-format on

		LOG_ASSERT(index != -1, "Get vaddr component failed.");
		return static_cast<uint32_t>(index);
	}

	GcnBufferFormat GcnCompiler::getBufferFormat(
		Gnm::BufferFormat dfmt, Gnm::BufferChannelType nfmt)
	{
		GcnBufferFormat format;
		switch (dfmt)
		{
			case Gnm::kBufferFormatInvalid:
			{
				format.sizeInBytes  = 0;
				format.channelCount = 0;
			}
			break;
			case Gnm::kBufferFormat8:
			{
				format.sizeInBytes  = 1;
				format.channelCount = 1;
			}
			break;
			case Gnm::kBufferFormat16:
			{
				format.sizeInBytes  = 2;
				format.channelCount = 1;
			}
			break;
			case Gnm::kBufferFormat8_8:
			{
				format.sizeInBytes  = 2;
				format.channelCount = 2;
			}
			break;
			case Gnm::kBufferFormat32:
			{
				format.sizeInBytes  = 4;
				format.channelCount = 1;
			}
			break;
			case Gnm::kBufferFormat16_16:
			{
				format.sizeInBytes  = 4;
				format.channelCount = 2;
			}
			break;
			case Gnm::kBufferFormat10_11_11:
			case Gnm::kBufferFormat11_11_10:
			{
				format.sizeInBytes  = 4;
				format.channelCount = 3;
			}
			break;
			case Gnm::kBufferFormat10_10_10_2:
			case Gnm::kBufferFormat2_10_10_10:
			case Gnm::kBufferFormat8_8_8_8:
			{
				format.sizeInBytes  = 4;
				format.channelCount = 4;
			}
			break;
			case Gnm::kBufferFormat32_32:
			{
				format.sizeInBytes  = 8;
				format.channelCount = 2;
			}
			break;
			case Gnm::kBufferFormat16_16_16_16:
			{
				format.sizeInBytes  = 8;
				format.channelCount = 4;
			}
			break;
			case Gnm::kBufferFormat32_32_32:
			{
				format.sizeInBytes  = 12;
				format.channelCount = 3;
			}
			break;
			case Gnm::kBufferFormat32_32_32_32:
			{
				format.sizeInBytes  = 16;
				format.channelCount = 4;
			}
			break;
			default:
				LOG_ASSERT(false, "error dfmt passed.");
				break;
		}

		switch (nfmt)
		{
			case Gnm::kBufferChannelTypeUNorm:
			case Gnm::kBufferChannelTypeUScaled:
			case Gnm::kBufferChannelTypeUInt:
				format.channelType = GcnScalarType::Uint32;
				break;
			case Gnm::kBufferChannelTypeSNorm:
			case Gnm::kBufferChannelTypeSNormNoZero:
			case Gnm::kBufferChannelTypeSScaled:
			case Gnm::kBufferChannelTypeSInt:
				format.channelType = GcnScalarType::Sint32;
				break;
			case Gnm::kBufferChannelTypeFloat:
				format.channelType = GcnScalarType::Float32;
				break;
			default:
				LOG_ASSERT(false, "error nfmt passed.");
				break;
		}
		return format;
	}

}  // namespace sce::gcn
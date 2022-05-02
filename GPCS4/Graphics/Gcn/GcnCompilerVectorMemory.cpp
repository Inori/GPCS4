#include "GcnCompiler.h"

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

	GcnRegisterValue GcnCompiler::emitCalcBufferAddress(
		const GcnShaderInstruction& ins)
	{

		bool idxen = false;
		bool offen = false;
		if (ins.encoding == GcnInstEncoding::MUBUF)
		{
			idxen = ins.control.mubuf.idxen;
			offen = ins.control.mubuf.offen;
		}
		else
		{
			idxen = ins.control.mtbuf.idxen;
			offen = ins.control.mtbuf.offen;
		}

		const uint32_t zero       = m_module.constu32(0);
		auto           bufferInfo = getBufferType(ins.src[2]);

		auto soff = emitRegisterLoad(ins.src[3]);
		// sV#.base is zero in our case.
		uint32_t base = soff.low.id;
		uint32_t index = idxen ? 
			emitRegisterLoad(ins.src[0]).low.id : zero;

		GcnInstOperand offsetReg = ins.src[0];
		offsetReg.code += static_cast<uint32_t>(idxen);

		uint32_t offset = offen ? 
			emitRegisterLoad(offsetReg).low.id : zero;
		uint32_t stride = m_module.constu32(bufferInfo.buffer.stride);

		LOG_ASSERT(bufferInfo.buffer.isSwizzle == false, "TODO: support swizzle buffer.");
		
		// Note the returned address is in bytes.
		GcnRegisterValue result;
		result.type.ctype = GcnScalarType::Uint32;
		result.type.ccount = 1;

		const uint32_t typdId = getVectorTypeId(result.type);

		result.id = m_module.opIAdd(typdId,
									m_module.opIAdd(typdId, base, offset),
									m_module.opIMul(typdId, index, stride));
		return result;
	}

	
	std::vector<GcnRegisterPointer>
		GcnCompiler::emitGetResourceBufferPtr(const GcnShaderInstruction& ins)
	{
		auto op = ins.opcode;

		auto bufferInfo = getBufferType(ins.src[2]);

		Gnm::BufferFormat      dfmt;
		Gnm::BufferChannelType nfmt;

		if (ins.encoding == GcnInstEncoding::MUBUF)
		{
			dfmt  = bufferInfo.buffer.dfmt;
			nfmt  = bufferInfo.buffer.nfmt;
		}
		else
		{
			dfmt  = (Gnm::BufferFormat)ins.control.mtbuf.dfmt;
			nfmt  = (Gnm::BufferChannelType)ins.control.mtbuf.nfmt;
		}

		auto bufferFormat = getBufferFormat(dfmt, nfmt);

		GcnRegisterInfo info;
		info.type.ctype   = GcnScalarType::Uint32;
		info.type.ccount  = 1;
		info.type.alength = 0;
		info.sclass       = spv::StorageClassUniform;

		uint32_t ptrTypeId     = getPointerTypeId(info);
		uint32_t uintTypeId    = getScalarTypeId(GcnScalarType::Uint32);

		uint32_t dataCount = bufferFormat.sizeInBytes < 4 ? 1 : bufferFormat.sizeInBytes / 4;

		LOG_ASSERT(nfmt == Gnm::kBufferChannelTypeFloat ||
					   nfmt == Gnm::kBufferChannelTypeUInt ||
					   nfmt == Gnm::kBufferChannelTypeSInt,
				   "TODO: support encoded channel type.");

		auto address = emitCalcBufferAddress(ins);

		std::vector<GcnRegisterPointer> ptrList;
		if (bufferInfo.isSsbo)
		{
			ptrList = emitGetStorageBufferPtr(bufferInfo.varId,
											  address.id,
											  dataCount);
		}
		else
		{
			ptrList = emitGetUniformBufferPtr(bufferInfo.varId,
											  address.id,
											  dataCount);
		}
		return ptrList;
	}

	void GcnCompiler::emitBufferLoadStoreNoFmt(const GcnShaderInstruction& ins,
										  bool                        isLoad)
	{
		/*
		auto op = ins.opcode;

		auto bufferInfo = getBufferType(ins.src[2]);

		uint32_t               count = 0;
		uint32_t               size  = 0;
		Gnm::BufferFormat      dfmt;
		Gnm::BufferChannelType nfmt;

		if (ins.encoding == GcnInstEncoding::MUBUF)
		{
			count = ins.control.mubuf.count;
			size  = ins.control.mubuf.size;
			dfmt  = bufferInfo.buffer.dfmt;
			nfmt  = bufferInfo.buffer.nfmt;
		}
		else
		{
			count = ins.control.mtbuf.count;
			size  = ins.control.mtbuf.size;
			dfmt  = (Gnm::BufferFormat)ins.control.mtbuf.dfmt;
			nfmt  = (Gnm::BufferChannelType)ins.control.mtbuf.nfmt;
		}

		bool isFormat = (op >= GcnOpcode::BUFFER_LOAD_FORMAT_X &&
						 op <= GcnOpcode::BUFFER_STORE_FORMAT_XYZW) ||
						(op >= GcnOpcode::TBUFFER_LOAD_FORMAT_X &&
						 op <= GcnOpcode::TBUFFER_STORE_FORMAT_XYZW);

		GcnRegisterInfo info;
		info.type.ctype   = GcnScalarType::Float32;
		info.type.ccount  = 1;
		info.type.alength = 0;
		info.sclass       = spv::StorageClassUniform;

		uint32_t uintTypeId = getScalarTypeId(GcnScalarType::Uint32);
		uint32_t fpTypeId   = getScalarTypeId(GcnScalarType::Float32);
		uint32_t ptrTypeId  = getPointerTypeId(info);
		*/
		LOG_GCN_UNHANDLED_INST();
		//// In bytes
		//uint32_t formatDataSize = 
		//uint32_t dataSize = isFormat ? getBufferFormat(dfmt, nfmt)
		//							 : (size > 4 ? 4 : size);
		//LOG_ASSERT(dataSize == 4, "TODO: support non 4 bytes types.");

		//auto address = emitCalcBufferAddress(ins);
		//for (uint32_t i = 0; i != count ; ++i)
		//{
		//	uint32_t addressId = address.id;
		//	uint32_t offsetId  = m_module.opIAdd(uintTypeId,
		//										 addressId,
		//										 m_module.constu32(i * dataSize));
		//	// Storage buffers are uint arrays
		//	uint32_t uintIdx   = m_module.opUDiv(uintTypeId,
		//										 offsetId,
		//										 m_module.constu32(sizeof(uint32_t)));

		//	const std::array<uint32_t, 2> indices = { { m_module.consti32(0), uintIdx } };

		//	uint32_t componentPtr = m_module.opAccessChain(ptrTypeId,
		//												   bufferInfo.varId,
		//												   indices.size(), indices.data());

		//	GcnInstOperand reg = ins.src[1];
		//	reg.code += i;

		//	// TODO:
		//	// Support channel format conversion
		//	if (isLoad)
		//	{
		//		uint32_t         itemId = m_module.opLoad(fpTypeId,
		//												  componentPtr);
		//		GcnRegisterValue value;
		//		value.type.ctype   = GcnScalarType::Float32;
		//		value.type.ccount  = 1;
		//		value.id           = itemId;

		//		emitVgprStore(reg, value);
		//	}
		//	else
		//	{
		//		auto value = emitVgprLoad(reg);
		//		m_module.opStore(componentPtr, value.id);
		//	}
		//}

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

		GcnScalarType dataType   = bufferInfo.isSsbo ? 
			GcnScalarType::Uint32 : GcnScalarType::Float32;
		uint32_t      dataTypeId = getScalarTypeId(dataType);
	
		auto     dataPtr   = emitGetResourceBufferPtr(ins);
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
			default:
				LOG_GCN_UNHANDLED_INST();
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

	void GcnCompiler::emitVectorMemImgNoSmp(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorMemImgSmp(const GcnShaderInstruction& ins)
	{
		auto mimg = gcnInstructionAs<GcnShaderInstMIMG>(ins);

		auto op = ins.opcode;
		switch (op)
		{
			case GcnOpcode::IMAGE_SAMPLE:
				emitTextureSample(ins);
				break;
			default:
				LOG_GCN_UNHANDLED_INST();
				break;
		}
	}

	void GcnCompiler::emitVectorMemImgUt(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

	void GcnCompiler::emitVectorMemL1Cache(const GcnShaderInstruction& ins)
	{
		LOG_GCN_UNHANDLED_INST();
	}

}  // namespace sce::gcn
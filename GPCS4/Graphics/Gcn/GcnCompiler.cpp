#include "GcnCompiler.h"
#include "GcnHeader.h"
#include "GcnUtil.h"
#include "PlatFile.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

using namespace sce::vlt;

namespace sce::gcn
{
	constexpr uint32_t PerVertex_Position = 0;
	constexpr uint32_t PerVertex_CullDist = 1;
	constexpr uint32_t PerVertex_ClipDist = 2;

	GcnCompiler::GcnCompiler(
		const std::string&     fileName,
		const GcnProgramInfo&  programInfo,
		const GcnHeader&       header,
		const GcnShaderMeta&   meta,
		const GcnAnalysisInfo& analysis) :
		m_programInfo(programInfo),
		m_header(&header),
		m_meta(meta),
		m_analysis(&analysis),
		m_module(spvVersion(1, 3))
	{
		// Declare an entry point ID. We'll need it during the
		// initialization phase where the execution mode is set.
		m_entryPointId = m_module.allocateId();

		// Set the shader name so that we recognize it in renderdoc
		m_module.setDebugSource(
			spv::SourceLanguageUnknown, 0,
			m_module.addDebugString(fileName.c_str()),
			nullptr);

		// Set the memory model. This is the same for all shaders.
		m_module.setMemoryModel(
			spv::AddressingModelLogical,
			spv::MemoryModelGLSL450);

		this->emitInit();
	}

	GcnCompiler::~GcnCompiler()
	{
	}

	void GcnCompiler::processInstruction(
		const GcnShaderInstruction& ins)
	{
		compileInstruction(ins);

		updateProgramCounter(ins);
	}
	
	void GcnCompiler::compileInstruction(
		const GcnShaderInstruction& ins)
	{
		auto catecory = ins.category;
		switch (catecory)
		{
		case GcnInstCategory::ScalarALU:
			this->emitScalarALU(ins);
			break;
		case GcnInstCategory::ScalarMemory:
			this->emitScalarMemory(ins);
			break;
		case GcnInstCategory::VectorALU:
			this->emitVectorALU(ins);
			break;
		case GcnInstCategory::VectorMemory:
			this->emitVectorMemory(ins);
			break;
		case GcnInstCategory::FlowControl:
			this->emitFlowControl(ins);
			break;
		case GcnInstCategory::DataShare:
			this->emitDataShare(ins);
			break;
		case GcnInstCategory::VectorInterpolation:
			this->emitVectorInterpolation(ins);
			break;
		case GcnInstCategory::Export:
			this->emitExport(ins);
			break;
		case GcnInstCategory::DebugProfile:
			this->emitDebugProfile(ins);
			break;
		case GcnInstCategory::Undefined:
			LOG_GCN_UNHANDLED_INST();
			break;
		}
	}

	Rc<VltShader> GcnCompiler::finalize()
	{
		// Depending on the shader type, this will prepare
		// input registers, call various shader functions
		// and write back the output registers.

		// clang-format off
		switch (m_programInfo.type()) 
		{
		  case GcnProgramType::VertexShader:   this->emitVsFinalize(); break;
		  case GcnProgramType::HullShader:     this->emitHsFinalize(); break;
		  case GcnProgramType::DomainShader:   this->emitDsFinalize(); break;
		  case GcnProgramType::GeometryShader: this->emitGsFinalize(); break;
		  case GcnProgramType::PixelShader:    this->emitPsFinalize(); break;
		  case GcnProgramType::ComputeShader:  this->emitCsFinalize(); break;
		}
		// clang-format on

		// Declare the entry point, we now have all the
		// information we need, including the interfaces
		m_module.addEntryPoint(m_entryPointId,
							   m_programInfo.executionModel(), "main",
							   m_entryPointInterfaces.size(),
							   m_entryPointInterfaces.data());
		m_module.setDebugName(m_entryPointId, "main");

		// Options is not used currently, pass a dummy value.
		VltShaderOptions shaderOptions = {};

		// Create the shader module object
		return new VltShader(
			m_programInfo.shaderStage(),
			m_resourceSlots,
			m_interfaceSlots,
			m_module.compile(),
			shaderOptions,
			std::move(m_immConstData));
	}

	void GcnCompiler::emitInit()
	{
		// Set up common capabilities for all shaders
		m_module.enableCapability(spv::CapabilityShader);
		m_module.enableCapability(spv::CapabilityImageQuery);
    
		// Initialize the shader module with capabilities
		// etc. Each shader type has its own peculiarities.

		// clang-format off
		switch (m_programInfo.type()) 
		{
		  case GcnProgramType::VertexShader:   emitVsInit(); break;
		  case GcnProgramType::HullShader:     emitHsInit(); break;
		  case GcnProgramType::DomainShader:   emitDsInit(); break;
		  case GcnProgramType::GeometryShader: emitGsInit(); break;
		  case GcnProgramType::PixelShader:    emitPsInit(); break;
		  case GcnProgramType::ComputeShader:  emitCsInit(); break;
		}
		// clang-format on

	}

	void GcnCompiler::emitFunctionBegin(
		uint32_t entryPoint, 
		uint32_t returnType, 
		uint32_t funcType)
	{
		this->emitFunctionEnd();

		m_module.functionBegin(
			returnType, entryPoint, funcType,
			spv::FunctionControlMaskNone);

		m_insideFunction = true;
	}

	void GcnCompiler::emitFunctionEnd()
	{
		if (m_insideFunction)
		{
			m_module.opReturn();
			m_module.functionEnd();
		}

		m_insideFunction = false;
	}

	void GcnCompiler::emitFunctionLabel()
	{
		m_module.opLabel(m_module.allocateId());
	}

	void GcnCompiler::emitMainFunctionBegin()
	{
		this->emitFunctionBegin(
			m_entryPointId,
			m_module.defVoidType(),
			m_module.defFunctionType(
				m_module.defVoidType(), 0, nullptr));
		this->emitFunctionLabel();
	}

	void GcnCompiler::emitVsInit()
	{
		m_module.enableCapability(spv::CapabilityClipDistance);
		m_module.enableCapability(spv::CapabilityCullDistance);
		m_module.enableCapability(spv::CapabilityDrawParameters);

		m_module.enableExtension("SPV_KHR_shader_draw_parameters");

		// Declare the per-vertex output block. This is where
		// the vertex shader will write the vertex position.
		const uint32_t perVertexStruct  = this->getPerVertexBlockId();
		const uint32_t perVertexPointer = m_module.defPointerType(
			perVertexStruct, spv::StorageClassOutput);

		m_perVertexOut = m_module.newVar(
			perVertexPointer, spv::StorageClassOutput);
		m_entryPointInterfaces.push_back(m_perVertexOut);
		m_module.setDebugName(m_perVertexOut, "vs_vertex_out");
	}

	void GcnCompiler::emitHsInit()
	{
	}

	void GcnCompiler::emitDsInit()
	{
	}

	void GcnCompiler::emitGsInit()
	{
	}

	void GcnCompiler::emitPsInit()
	{
	}

	void GcnCompiler::emitCsInit()
	{
	}

	void GcnCompiler::emitVsFinalize()
	{
		this->emitMainFunctionBegin();
		this->emitInputFetch();
		m_module.opFunctionCall(
			m_module.defVoidType(),
			m_vs.functionId, 0, nullptr);
		this->emitOutputSetup();
		this->emitFunctionEnd();
	}

	void GcnCompiler::emitHsFinalize()
	{
	}

	void GcnCompiler::emitDsFinalize()
	{
	}

	void GcnCompiler::emitGsFinalize()
	{
	}

	void GcnCompiler::emitPsFinalize()
	{
	}

	void GcnCompiler::emitCsFinalize()
	{
	}

	void GcnCompiler::emitInputFetch()
	{
		// Emulate fetch shader
	}

	void GcnCompiler::emitOutputSetup()
	{
	}

	 GcnRegisterValue GcnCompiler::emitBuildConstVecf32(
		float              x,
		float              y,
		float              z,
		float              w,
		const GcnRegMask& writeMask)
	{
		// TODO refactor these functions into one single template
		std::array<uint32_t, 4> ids            = { 0, 0, 0, 0 };
		uint32_t                componentIndex = 0;

		if (writeMask[0])
			ids[componentIndex++] = m_module.constf32(x);
		if (writeMask[1])
			ids[componentIndex++] = m_module.constf32(y);
		if (writeMask[2])
			ids[componentIndex++] = m_module.constf32(z);
		if (writeMask[3])
			ids[componentIndex++] = m_module.constf32(w);

		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Float32;
		result.type.ccount = componentIndex;
		result.id          = componentIndex > 1
								 ? m_module.constComposite(
									   getVectorTypeId(result.type),
									   componentIndex, ids.data())
								 : ids[0];
		return result;
	}

	GcnRegisterValue GcnCompiler::emitBuildConstVecu32(
		uint32_t           x,
		uint32_t           y,
		uint32_t           z,
		uint32_t           w,
		const GcnRegMask& writeMask)
	{
		std::array<uint32_t, 4> ids            = { 0, 0, 0, 0 };
		uint32_t                componentIndex = 0;

		if (writeMask[0])
			ids[componentIndex++] = m_module.constu32(x);
		if (writeMask[1])
			ids[componentIndex++] = m_module.constu32(y);
		if (writeMask[2])
			ids[componentIndex++] = m_module.constu32(z);
		if (writeMask[3])
			ids[componentIndex++] = m_module.constu32(w);

		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Uint32;
		result.type.ccount = componentIndex;
		result.id          = componentIndex > 1
								 ? m_module.constComposite(
									   getVectorTypeId(result.type),
									   componentIndex, ids.data())
								 : ids[0];
		return result;
	}

	GcnRegisterValue GcnCompiler::emitBuildConstVeci32(
		int32_t            x,
		int32_t            y,
		int32_t            z,
		int32_t            w,
		const GcnRegMask& writeMask)
	{
		std::array<uint32_t, 4> ids            = { 0, 0, 0, 0 };
		uint32_t                componentIndex = 0;

		if (writeMask[0])
			ids[componentIndex++] = m_module.consti32(x);
		if (writeMask[1])
			ids[componentIndex++] = m_module.consti32(y);
		if (writeMask[2])
			ids[componentIndex++] = m_module.consti32(z);
		if (writeMask[3])
			ids[componentIndex++] = m_module.consti32(w);

		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Sint32;
		result.type.ccount = componentIndex;
		result.id          = componentIndex > 1
								 ? m_module.constComposite(
									   getVectorTypeId(result.type),
									   componentIndex, ids.data())
								 : ids[0];
		return result;
	}

	GcnRegisterValue GcnCompiler::emitBuildConstVecf64(
		double             xy,
		double             zw,
		const GcnRegMask& writeMask)
	{
		std::array<uint32_t, 2> ids            = { 0, 0 };
		uint32_t                componentIndex = 0;

		if (writeMask[0] && writeMask[1])
			ids[componentIndex++] = m_module.constf64(xy);
		if (writeMask[2] && writeMask[3])
			ids[componentIndex++] = m_module.constf64(zw);

		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Float64;
		result.type.ccount = componentIndex;
		result.id          = componentIndex > 1
								 ? m_module.constComposite(
									   getVectorTypeId(result.type),
									   componentIndex, ids.data())
								 : ids[0];
		return result;
	}
  

	GcnRegisterValue GcnCompiler::emitRegisterBitcast(
		GcnRegisterValue srcValue,
		GcnScalarType    dstType)
	{
		GcnScalarType srcType = srcValue.type.ctype;

		if (srcType == dstType)
			return srcValue;

		GcnRegisterValue result;
		result.type.ctype  = dstType;
		result.type.ccount = srcValue.type.ccount;

		if (isDoubleType(srcType))
			result.type.ccount *= 2;
		if (isDoubleType(dstType))
			result.type.ccount /= 2;

		result.id = m_module.opBitcast(
			getVectorTypeId(result.type),
			srcValue.id);
		return result;
	}

	GcnRegisterValue GcnCompiler::emitRegisterSwizzle(
		GcnRegisterValue value,
		GcnRegSwizzle    swizzle,
		GcnRegMask       writeMask)
	{
		if (value.type.ccount == 1)
			return emitRegisterExtend(value, writeMask.popCount());

		std::array<uint32_t, 4> indices;

		uint32_t dstIndex = 0;

		for (uint32_t i = 0; i < 4; i++)
		{
			if (writeMask[i])
				indices[dstIndex++] = swizzle[i];
		}

		// If the swizzle combined with the mask can be reduced
		// to a no-op, we don't need to insert any instructions.
		bool isIdentitySwizzle = dstIndex == value.type.ccount;

		for (uint32_t i = 0; i < dstIndex && isIdentitySwizzle; i++)
			isIdentitySwizzle &= indices[i] == i;

		if (isIdentitySwizzle)
			return value;

		// Use OpCompositeExtract if the resulting vector contains
		// only one component, and OpVectorShuffle if it is a vector.
		GcnRegisterValue result;
		result.type.ctype  = value.type.ctype;
		result.type.ccount = dstIndex;

		const uint32_t typeId = getVectorTypeId(result.type);

		if (dstIndex == 1)
		{
			result.id = m_module.opCompositeExtract(
				typeId, value.id, 1, indices.data());
		}
		else
		{
			result.id = m_module.opVectorShuffle(
				typeId, value.id, value.id,
				dstIndex, indices.data());
		}

		return result;
	}

	GcnRegisterValue GcnCompiler::emitRegisterExtract(
		GcnRegisterValue value,
		GcnRegMask       mask)
	{
		return emitRegisterSwizzle(value,
								   GcnRegSwizzle(0, 1, 2, 3), mask);
	}

	GcnRegisterValue GcnCompiler::emitRegisterInsert(
		GcnRegisterValue dstValue,
		GcnRegisterValue srcValue,
		GcnRegMask       srcMask)
	{
		GcnRegisterValue result;
		result.type = dstValue.type;

		const uint32_t typeId = getVectorTypeId(result.type);

		if (srcMask.popCount() == 0)
		{
			// Nothing to do if the insertion mask is empty
			result.id = dstValue.id;
		}
		else if (dstValue.type.ccount == 1)
		{
			// Both values are scalar, so the first component
			// of the write mask decides which one to take.
			result.id = srcMask[0] ? srcValue.id : dstValue.id;
		}
		else if (srcValue.type.ccount == 1)
		{
			// The source value is scalar. Since OpVectorShuffle
			// requires both arguments to be vectors, we have to
			// use OpCompositeInsert to modify the vector instead.
			const uint32_t componentId = srcMask.firstSet();

			result.id = m_module.opCompositeInsert(typeId,
												   srcValue.id, dstValue.id, 1, &componentId);
		}
		else
		{
			// Both arguments are vectors. We can determine which
			// components to take from which vector and use the
			// OpVectorShuffle instruction.
			std::array<uint32_t, 4> components;
			uint32_t                srcComponentId = dstValue.type.ccount;

			for (uint32_t i = 0; i < dstValue.type.ccount; i++)
				components.at(i) = srcMask[i] ? srcComponentId++ : i;

			result.id = m_module.opVectorShuffle(
				typeId, dstValue.id, srcValue.id,
				dstValue.type.ccount, components.data());
		}

		return result;
	}

	GcnRegisterValue GcnCompiler::emitRegisterConcat(
		GcnRegisterValue value1,
		GcnRegisterValue value2)
	{
		std::array<uint32_t, 2> ids = { { value1.id, value2.id } };

		GcnRegisterValue result;
		result.type.ctype  = value1.type.ctype;
		result.type.ccount = value1.type.ccount + value2.type.ccount;
		result.id          = m_module.opCompositeConstruct(
					 getVectorTypeId(result.type),
					 ids.size(), ids.data());
		return result;
	}

	GcnRegisterValue GcnCompiler::emitRegisterExtend(
		GcnRegisterValue value,
		uint32_t          size)
	{
		if (size == 1)
			return value;

		std::array<uint32_t, 4> ids = { {
			value.id,
			value.id,
			value.id,
			value.id,
		} };

		GcnRegisterValue result;
		result.type.ctype  = value.type.ctype;
		result.type.ccount = size;
		result.id          = m_module.opCompositeConstruct(
					 getVectorTypeId(result.type),
					 size, ids.data());
		return result;
	}

	GcnRegisterValue GcnCompiler::emitRegisterAbsolute(
		GcnRegisterValue value)
	{
		const uint32_t typeId = getVectorTypeId(value.type);

		switch (value.type.ctype)
		{
		case GcnScalarType::Float32:
			value.id = m_module.opFAbs(typeId, value.id);
			break;
		case GcnScalarType::Sint32:
			value.id = m_module.opSAbs(typeId, value.id);
			break;
		default:
			Logger::warn("GcnCompiler: Cannot get absolute value for given type");
		}

		return value;
	}

	GcnRegisterValue GcnCompiler::emitRegisterNegate(
		GcnRegisterValue value)
	{
		const uint32_t typeId = getVectorTypeId(value.type);

		switch (value.type.ctype)
		{
		case GcnScalarType::Float32:
			value.id = m_module.opFNegate(typeId, value.id);
			break;
		case GcnScalarType::Float64:
			value.id = m_module.opFNegate(typeId, value.id);
			break;
		case GcnScalarType::Sint32:
			value.id = m_module.opSNegate(typeId, value.id);
			break;
		case GcnScalarType::Sint64:
			value.id = m_module.opSNegate(typeId, value.id);
			break;
		default:
			Logger::warn("GcnCompiler: Cannot negate given type");
		}

		return value;
	}

	GcnRegisterValue GcnCompiler::emitRegisterZeroTest(
		GcnRegisterValue value,
		GcnZeroTest      test)
	{
		GcnRegisterValue result;
		result.type.ctype  = GcnScalarType::Bool;
		result.type.ccount = 1;

		const uint32_t zeroId = m_module.constu32(0u);
		const uint32_t typeId = getVectorTypeId(result.type);

		result.id = test == GcnZeroTest::TestZ
						? m_module.opIEqual(typeId, value.id, zeroId)
						: m_module.opINotEqual(typeId, value.id, zeroId);
		return result;
	}

	GcnRegisterValue GcnCompiler::emitRegisterMaskBits(
		GcnRegisterValue value,
		uint32_t          mask)
	{
		GcnRegisterValue maskVector = emitBuildConstVecu32(
			mask, mask, mask, mask, GcnRegMask::firstN(value.type.ccount));

		GcnRegisterValue result;
		result.type = value.type;
		result.id   = m_module.opBitwiseAnd(
			  getVectorTypeId(result.type),
			  value.id, maskVector.id);
		return result;
	}

	//GcnRegisterValue GcnCompiler::emitSrcOperandModifiers(
	//	GcnRegisterValue value,
	//	GcnRegModifiers  modifiers)
	//{
	//	if (modifiers.test(GcnRegModifier::Abs))
	//		value = emitRegisterAbsolute(value);

	//	if (modifiers.test(GcnRegModifier::Neg))
	//		value = emitRegisterNegate(value);
	//	return value;
	//}

	//GcnRegisterValue GcnCompiler::emitDstOperandModifiers(
	//	GcnRegisterValue value,
	//	GcnOpModifiers   modifiers)
	//{
	//	const uint32_t typeId = getVectorTypeId(value.type);

	//	if (value.type.ctype == GcnScalarType::Float32)
	//	{
	//		// Saturating only makes sense on floats
	//		if (modifiers.saturate)
	//		{
	//			const GcnRegMask       mask = GcnRegMask::firstN(value.type.ccount);
	//			const GcnRegisterValue vec0 = emitBuildConstVecf32(0.0f, 0.0f, 0.0f, 0.0f, mask);
	//			const GcnRegisterValue vec1 = emitBuildConstVecf32(1.0f, 1.0f, 1.0f, 1.0f, mask);

	//			value.id = m_module.opNClamp(typeId, value.id, vec0.id, vec1.id);
	//		}
	//	}

	//	return value;
	//}

	uint32_t GcnCompiler::getScalarTypeId(GcnScalarType type)
	{
		if (type == GcnScalarType::Float64)
			m_module.enableCapability(spv::CapabilityFloat64);

		if (type == GcnScalarType::Sint64 || type == GcnScalarType::Uint64)
			m_module.enableCapability(spv::CapabilityInt64);

		switch (type)
		{
		case GcnScalarType::Uint32:
			return m_module.defIntType(32, 0);
		case GcnScalarType::Uint64:
			return m_module.defIntType(64, 0);
		case GcnScalarType::Sint32:
			return m_module.defIntType(32, 1);
		case GcnScalarType::Sint64:
			return m_module.defIntType(64, 1);
		case GcnScalarType::Float32:
			return m_module.defFloatType(32);
		case GcnScalarType::Float64:
			return m_module.defFloatType(64);
		case GcnScalarType::Bool:
			return m_module.defBoolType();
		}

		LOG_ASSERT(false, "GcnCompiler: Invalid scalar type");
	}

	uint32_t GcnCompiler::getVectorTypeId(const GcnVectorType& type)
	{
		uint32_t typeId = this->getScalarTypeId(type.ctype);

		if (type.ccount > 1)
			typeId = m_module.defVectorType(typeId, type.ccount);

		return typeId;
	}

	uint32_t GcnCompiler::getArrayTypeId(const GcnArrayType& type)
	{
		GcnVectorType vtype;
		vtype.ctype  = type.ctype;
		vtype.ccount = type.ccount;

		uint32_t typeId = this->getVectorTypeId(vtype);

		if (type.alength != 0)
		{
			typeId = m_module.defArrayType(typeId,
										   m_module.constu32(type.alength));
		}

		return typeId;
	}

	uint32_t GcnCompiler::getPointerTypeId(const GcnRegisterInfo& type)
	{
		return m_module.defPointerType(
			this->getArrayTypeId(type.type),
			type.sclass);
	}

	uint32_t GcnCompiler::getPerVertexBlockId()
	{
		uint32_t t_f32    = m_module.defFloatType(32);
		uint32_t t_f32_v4 = m_module.defVectorType(t_f32, 4);
		//     uint32_t t_f32_a4 = m_module.defArrayType(t_f32, m_module.constu32(4));

		std::array<uint32_t, 1> members;
		members[PerVertex_Position] = t_f32_v4;
		//     members[PerVertex_CullDist] = t_f32_a4;
		//     members[PerVertex_ClipDist] = t_f32_a4;

		uint32_t typeId = m_module.defStructTypeUnique(
			members.size(), members.data());

		m_module.memberDecorateBuiltIn(typeId, PerVertex_Position, spv::BuiltInPosition);
		//     m_module.memberDecorateBuiltIn(typeId, PerVertex_CullDist, spv::BuiltInCullDistance);
		//     m_module.memberDecorateBuiltIn(typeId, PerVertex_ClipDist, spv::BuiltInClipDistance);
		m_module.decorateBlock(typeId);

		m_module.setDebugName(typeId, "s_per_vertex");
		m_module.setDebugMemberName(typeId, PerVertex_Position, "position");
		//     m_module.setDebugMemberName(typeId, PerVertex_CullDist, "cull_dist");
		//     m_module.setDebugMemberName(typeId, PerVertex_ClipDist, "clip_dist");
		return typeId;
	}

	bool GcnCompiler::isDoubleType(
		GcnScalarType type) const
	{
		return type == GcnScalarType::Sint64 ||
			   type == GcnScalarType::Uint64 ||
			   type == GcnScalarType::Float64;
	}

}  // namespace sce::gcn
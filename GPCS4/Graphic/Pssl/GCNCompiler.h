#pragma once

#include "GPCS4Common.h"
#include "PsslProgramInfo.h"
#include "PsslFetchShader.h"
#include "GCNInstruction.h"
#include "GCNAnalyzer.h"
#include "GCNDecoder.h"
#include "GCNEnums.h"

#include "GCNParser/SMRDInstruction.h"
#include "GCNParser/SOPPInstruction.h"
#include "GCNParser/SOPCInstruction.h"
#include "GCNParser/SOP1Instruction.h"
#include "GCNParser/SOPKInstruction.h"
#include "GCNParser/SOP2Instruction.h"
#include "GCNParser/VINTRPInstruction.h"
#include "GCNParser/DSInstruction.h"
#include "GCNParser/MUBUFInstruction.h"
#include "GCNParser/MTBUFInstruction.h"
#include "GCNParser/MIMGInstruction.h"
#include "GCNParser/EXPInstruction.h"
#include "GCNParser/VOPInstruction.h"

#include "../Gve/GveShader.h"
#include "../SpirV/SpirvModule.h"

#include <optional>
#include <map>
#include <array>

namespace pssl
{;


constexpr size_t GcnMaxSgprCount = 104;
constexpr size_t GcnMaxVgprCount = 256;


/**
 * \brief Vector type
 *
 * Convenience struct that stores a scalar
 * type and a component count. The compiler
 * can use this to generate SPIR-V types.
 */
struct SpirvVectorType 
{
	SpirvVectorType():
		ctype(SpirvScalarType::Unknown), ccount(0)
	{}
	SpirvVectorType(SpirvScalarType type, uint32_t count):
		ctype(type), ccount(count)
	{}

	SpirvScalarType   ctype;
	uint32_t          ccount;
};


/**
 * \brief Array type
 *
 * Convenience struct that stores a scalar type, a
 * component count and an array size. An array of
 * length 0 will be evaluated to a vector type. The
 * compiler can use this to generate SPIR-V types.
 */
struct SpirvArrayType 
{
	SpirvArrayType():
		alength(0)
	{}

	SpirvArrayType(const SpirvVectorType& type, uint32_t alen):
		vtype(type), alength(alen)
	{}

	SpirvArrayType(SpirvScalarType type, uint32_t count, uint32_t alen):
		vtype(type, count), alength(alen)
	{}

	SpirvVectorType    vtype;
	uint32_t           alength;
};


/**
 * \brief Register info
 *
 * Stores the array type of a register and
 * its storage class. The compiler can use
 * this to generate SPIR-V pointer types.
 */
struct SpirvRegisterInfo 
{
	SpirvRegisterInfo():
		sclass(spv::StorageClassMax)
	{}

	SpirvRegisterInfo(const SpirvVectorType& type, spv::StorageClass cls):
		atype(type, 0), sclass(cls)
	{}

	SpirvRegisterInfo(const SpirvArrayType& type, spv::StorageClass cls) :
		atype(type), sclass(cls)
	{}

	SpirvRegisterInfo(SpirvScalarType type, 
		uint32_t count, uint32_t alen, 
		spv::StorageClass cls):
		atype(type, count, alen),
		sclass(cls)
	{}

	SpirvArrayType     atype;
	spv::StorageClass  sclass;
};


/**
 * \brief Register value
 *
 * Stores a vector type and a SPIR-V ID that
 * represents an intermediate value. This is
 * used to track the type of such values.
 */
struct SpirvRegisterValue 
{
	SpirvRegisterValue():
		id(0)
	{}
	SpirvRegisterValue(SpirvVectorType vType, uint32_t spvId):
		type(vType), id(spvId)
	{}
	SpirvRegisterValue(SpirvScalarType sType, uint32_t count, uint32_t spvId) :
		type(sType, count), id(spvId)
	{}

	SpirvVectorType   type;
	uint32_t          id;
};


/**
 * \brief Register pointer
 *
 * Stores a vector type and a SPIR-V ID that
 * represents a pointer to such a vector. This
 * can be used to load registers conveniently.
 */
struct SpirvRegisterPointer
{
	SpirvRegisterPointer():
		id(0)
	{}
	SpirvRegisterPointer(SpirvVectorType vType, uint32_t spvId):
		type(vType), id(spvId)
	{}
	SpirvRegisterPointer(SpirvScalarType sType, uint32_t count, uint32_t spvId) :
		type(sType, count), id(spvId)
	{}

	SpirvVectorType   type;
	uint32_t          id;
};

/**
 * \brief Literal Constant
 *
 * A single literal constant
 * 
 */
struct SpirvLiteralConstant
{
	SpirvLiteralConstant():
		literalConst(0)
	{}
	SpirvLiteralConstant(SpirvScalarType sType, uint32_t value):
		type(sType), literalConst(value)
	{}

	SpirvScalarType type;
	// Note: this may need to cast to proper type 
	// depending on the type member
	uint32_t literalConst;
};

/**
 * \brief GPR array
 *
 * SGPR or VGPR array/group
 * e.g. s[8:11], v[4:6] etc.
 * Useful when access #V #T #S buffers
 */
struct SpirvGprArray
{
	SpirvGprType type;
	uint32_t startIndex;
	uint32_t count;
};


/**
 * \brief Sampler binding
 *
 * Stores a sampler variable that can be
 * used together with a texture resource.
 */
struct SpirvSampler 
{
	uint32_t varId = 0;
	uint32_t typeId = 0;
};

/**
 * \brief Image type information
 */
struct SpirvImageInfo 
{
	spv::Dim        dim		= spv::Dim1D;
	uint32_t        array	= 0;
	uint32_t        ms		= 0;
	uint32_t        sampled = 0;
	spv::ImageFormat format = spv::ImageFormatUnknown;
};


/**
 * \brief Shader resource binding
 *
 * Stores a resource variable
 * and associated type IDs.
 */
struct SpirvTexture
{
	SpirvImageInfo    imageInfo;
	uint32_t          varId = 0;
	uint32_t          imageTypeId = 0;
};


/**
 * \brief Sharp buffer resource.
 *
 * V# T# or S# buffer input to the shader
 */
struct GcnResourceBuffer
{
	GcnResourceBuffer(SpirvResourceType rType, ShaderInputUsageType uType, PsslShaderResource& resource):
		resType(rType), usageType(uType), res(resource)
	{}

	SpirvResourceType resType;
	ShaderInputUsageType usageType;
	PsslShaderResource res;
};



struct GcnStateRegister
{
	// local data share
	uint64_t lds = 0;
	// exec mask
	union
	{
		struct
		{
			uint32_t exec_lo;
			uint32_t exec_hi;
		};
		uint64_t exec = 0;
	};
	// vector condition code
	union
	{
		struct  
		{
			uint32_t vcc_lo;
			uint32_t vcc_hi;
		};
		uint64_t vcc = 0;
	};
	// mode register
	uint32_t mode = 0;
	// memory descriptor register
	uint32_t m0 = 0;
};

/**
 * \brief Vertex shader-specific structure
 */
struct GcnCompilerVsPart 
{
	spv::Id mainFunctionId = 0;
	spv::Id fsFunctionId = 0;
	// semantic -- spirv id
	std::map<uint32_t, SpirvRegisterPointer> vsInputs;
	// exp target -- spirv id
	std::map<uint32_t, SpirvRegisterPointer> vsOutputs;

	// Uniform Buffer Object Id
	// TODO:
	// Currently I see only one block of uniform buffer memory being used
	// even if multiple uniform variables declared.
	// So I only declare one member variable.
	// If multiple blocks of uniform buffer are found in the future
	// change this to a std::vector or something convinient.
	uint32_t m_uboId = 0;
};


/**
 * \brief Pixel shader-specific structure
 */
struct GcnCompilerPsPart 
{
	spv::Id functionId = 0;
	// attr index -- spirv id
	std::map<uint32_t, SpirvRegisterPointer> psInputs;
	// exp target -- spirv id
	std::map<uint32_t, SpirvRegisterPointer> psOutputs;
	// start register index -- sampler
	std::array<SpirvSampler, GcnMaxSgprCount> samplers;
	// start register index -- texture
	std::array<SpirvTexture, GcnMaxSgprCount> textures;
};


/**
 * \brief Compute shader-specific structure
 */
struct GcnCompilerCsPart 
{
	spv::Id functionId = 0;

	uint32_t workgroupSizeX = 0;
	uint32_t workgroupSizeY = 0;
	uint32_t workgroupSizeZ = 0;

	uint32_t builtinGlobalInvocationId = 0;
	uint32_t builtinLocalInvocationId = 0;
	uint32_t builtinLocalInvocationIndex = 0;
	uint32_t builtinWorkgroupId = 0;
};

/**
 * \brief Shader input information
 * 
 * Convenience struct to prevent too many parameters
 * in GCNCompiler's constructor.
 *
 */

struct GcnShaderInput
{
	std::vector<GcnResourceBuffer> resourceBuffer;
	std::optional<std::vector<VertexInputSemantic>> vsInputSemantics;
	std::optional<std::vector<PixelInputSemantic>> psInputSemantics;
};


class GCNCompiler
{
public:

	GCNCompiler(
		const PsslProgramInfo& progInfo,
		const GcnAnalysisInfo& analysis,
		const GcnShaderInput& shaderInput);
	~GCNCompiler();

	void processInstruction(GCNInstruction& ins);

	RcPtr<gve::GveShader> finalize();

private:

	void emitInit();
	/////////////////////////////////
	// Shader initialization methods
	void emitVsInit();
	void emitHsInit();
	void emitDsInit();
	void emitGsInit();
	void emitPsInit();
	void emitCsInit();

	///////////////////////////////
	// Shader finalization methods
	void emitVsFinalize();
	void emitHsFinalize();
	void emitDsFinalize();
	void emitGsFinalize();
	void emitPsFinalize();
	void emitCsFinalize();

	void emitFunctionBegin(
		uint32_t                entryPoint,
		uint32_t                returnType,
		uint32_t                funcType);

	void emitFunctionEnd();

	void emitMainFunctionBegin();

	void emitFunctionLabel();

	void emitDclVertexInput();
	void emitDclVertexOutput();
	void emitEmuFetchShader();

	void emitDclPixelInput();
	void emitDclPixelOutput();

	void emitGprInitialize();
	// For all shader types
	void emitDclUniformBuffer();
	void emitDclImmConstBuffer(const GcnResourceBuffer& res, uint32_t index);
	void emitDclImmSampler(const GcnResourceBuffer& res, uint32_t index);
	void emitDclImmResource(const GcnResourceBuffer& res, uint32_t index);

	/////////////////////////////////////////////////////////
	SpirvRegisterValue emitValueLoad(const SpirvRegisterPointer& reg);
	SpirvRegisterValue emitSgprLoad(uint32_t index);
	SpirvRegisterValue emitVgprLoad(uint32_t index);

	void emitValueStore(
		const SpirvRegisterPointer &ptr,
		const SpirvRegisterValue &src,
		const GcnRegMask &writeMask);
	void emitSgprStore(uint32_t dstIdx, const SpirvRegisterValue& srcReg);
	void emitSgprArrayStore(uint32_t startIdx, const SpirvRegisterValue* values, uint32_t count);
	void emitVgprStore(uint32_t dstIdx, const SpirvRegisterValue& srcReg);
	void emitVgprArrayStore(uint32_t startIdx, const SpirvRegisterValue* values, uint32_t count);
	// Store a vector to continuous vgprs
	void emitVgprVectorStore(uint32_t startIdx, const SpirvRegisterValue& srcVec, const GcnRegMask& writeMask);
	

	/////////////////////////////////////////
	// Operands manipulation methods
	SpirvRegisterValue emitLoadScalarOperand(uint32_t srcOperand, uint32_t regIndex, uint32_t literalConst = 0);
	SpirvRegisterValue emitLoadVectorOperand(uint32_t index);

	void emitStoreScalarOperand(uint32_t dstOperand, uint32_t regIndex, const SpirvRegisterValue& srcReg);
	void emitStoreVectorOperand(uint32_t dstIndex, const SpirvRegisterValue& srcReg);

	SpirvRegisterValue emitInlineConstantFloat(Instruction::OperandSRC src);
	SpirvRegisterValue emitInlineConstantInteger(Instruction::OperandSRC src);

	/////////////////////////////////////////
	// Hardware state register manipulation methods
	void emitStoreVCC(const SpirvRegisterValue& vccValueReg, bool isVccHi);
	void emitStoreM0(const SpirvRegisterValue& m0ValueReg);

	///////////////////////////////////////
	// Image register manipulation methods
	uint32_t emitLoadSampledImage(
		const SpirvTexture&     textureResource,
		const SpirvSampler&     samplerResource);

	SpirvRegisterValue emitPackFloat16(const SpirvRegisterValue& v2floatVec);
	SpirvRegisterValue emitUnpackFloat16(const SpirvRegisterValue& uiVec);

	///////////////////////////////
	// Variable definition methods
	uint32_t emitNewVariable(
		const SpirvRegisterInfo& info,
		const std::string& name = "");

	uint32_t emitNewBuiltinVariable(
		const SpirvRegisterInfo& info,
		spv::BuiltIn	         builtIn,
		const char*              name);

	////////////////////////////////////////////////
	// Constant building methods. These are used to
	// generate constant vectors that store the same
	// value in each component.
	SpirvRegisterValue emitBuildConstVecf32(
		float                   x,
		float                   y,
		float                   z,
		float                   w,
		const GcnRegMask&       writeMask);

	SpirvRegisterValue emitBuildConstVecu32(
		uint32_t                x,
		uint32_t                y,
		uint32_t                z,
		uint32_t                w,
		const GcnRegMask&       writeMask);

	SpirvRegisterValue emitBuildConstVeci32(
		int32_t                 x,
		int32_t                 y,
		int32_t                 z,
		int32_t                 w,
		const GcnRegMask&       writeMask);

	SpirvRegisterValue emitBuildConstVecf64(
		double                  xy,
		double                  zw,
		const GcnRegMask&       writeMask);

	/////////////////////////////////////////
	// Generic register manipulation methods
	SpirvRegisterValue emitRegisterBitcast(
		SpirvRegisterValue       srcValue,
		SpirvScalarType          dstType);

	SpirvRegisterValue emitRegisterSwizzle(
		SpirvRegisterValue     value,
		GcnRegSwizzle          swizzle,
		GcnRegMask             writeMask);

	SpirvRegisterValue emitRegisterExtract(
		SpirvRegisterValue     value,
		GcnRegMask             mask);

	SpirvRegisterValue emitRegisterInsert(
		SpirvRegisterValue     dstValue,
		SpirvRegisterValue     srcValue,
		GcnRegMask             srcMask);

	SpirvRegisterValue emitRegisterConcat(
		SpirvRegisterValue       value1,
		SpirvRegisterValue       value2);

	SpirvRegisterValue emitRegisterExtend(
		SpirvRegisterValue      value,
		uint32_t                size);

	SpirvRegisterValue emitRegisterAbsolute(
		SpirvRegisterValue       value);

	SpirvRegisterValue emitRegisterNegate(
		SpirvRegisterValue       value);

	SpirvRegisterValue emitRegisterZeroTest(
		SpirvRegisterValue       value,
		SpirvZeroTest            test);

	SpirvRegisterValue emitRegisterMaskBits(
		SpirvRegisterValue       value,
		uint32_t                mask);

	// load a vector's composite,
	// 0 - x, 1 - y, 2 - z, 3 - w
	SpirvRegisterValue emitRegisterComponentLoad(
		const SpirvRegisterPointer&		srcVec,
		uint32_t						compIndex,
		spv::StorageClass				storageClass = spv::StorageClassPrivate);

	/////////////////////////////////////////////////////////
	// Category handlers
	void emitScalarALU(GCNInstruction& ins);
	void emitScalarMemory(GCNInstruction& ins);
	void emitVectorALU(GCNInstruction& ins);
	void emitVectorMemory(GCNInstruction& ins);
	void emitFlowControl(GCNInstruction& ins);
	void emitDataShare(GCNInstruction& ins);
	void emitVectorInterpolation(GCNInstruction& ins);
	void emitExport(GCNInstruction& ins);
	void emitDebugProfile(GCNInstruction& ins);

	// ScalarALU
	void emitScalarArith(GCNInstruction& ins);
	void emitScalarAbs(GCNInstruction& ins);
	void emitScalarMov(GCNInstruction& ins);
	void emitScalarCmp(GCNInstruction& ins);
	void emitScalarSelect(GCNInstruction& ins);
	void emitScalarBitLogic(GCNInstruction& ins);
	void emitScalarBitManip(GCNInstruction& ins);
	void emitScalarBitField(GCNInstruction& ins);
	void emitScalarConv(GCNInstruction& ins);
	void emitScalarExecMask(GCNInstruction& ins);
	void emitScalarQuadMask(GCNInstruction& ins);

	// VectorALU
	void emitVectorRegMov(GCNInstruction& ins);
	void emitVectorLane(GCNInstruction& ins);
	void emitVectorBitLogic(GCNInstruction& ins);
	void emitVectorBitField32(GCNInstruction& ins);
	void emitVectorThreadMask(GCNInstruction& ins);
	void emitVectorBitField64(GCNInstruction& ins);
	void emitVectorFpArith32(GCNInstruction& ins);
	void emitVectorFpRound32(GCNInstruction& ins);
	void emitVectorFpField32(GCNInstruction& ins);
	void emitVectorFpTran32(GCNInstruction& ins);
	void emitVectorFpCmp32(GCNInstruction& ins);
	void emitVectorFpArith64(GCNInstruction& ins);
	void emitVectorFpRound64(GCNInstruction& ins);
	void emitVectorFpField64(GCNInstruction& ins);
	void emitVectorFpTran64(GCNInstruction& ins);
	void emitVectorFpCmp64(GCNInstruction& ins);
	void emitVectorIntArith32(GCNInstruction& ins);
	void emitVectorIntArith64(GCNInstruction& ins);
	void emitVectorIntCmp32(GCNInstruction& ins);
	void emitVectorIntCmp64(GCNInstruction& ins);
	void emitVectorConv(GCNInstruction& ins);
	void emitVectorFpGraph32(GCNInstruction& ins);
	void emitVectorIntGraph(GCNInstruction& ins);
	void emitVectorMisc(GCNInstruction& ins);

	// FlowControl
	void emitScalarProgFlow(GCNInstruction& ins);
	void emitScalarProgFlowPC(GCNInstruction& ins);

	void emitScalarSync(GCNInstruction& ins);
	void emitScalarWait(GCNInstruction& ins);
	void emitScalarCache(GCNInstruction& ins);
	void emitScalarPrior(GCNInstruction& ins);
	void emitScalarRegAccess(GCNInstruction& ins);
	void emitScalarMsg(GCNInstruction& ins);

	// ScalarMemory
	void emitScalarMemRd(GCNInstruction& ins);
	void emitScalarMemUt(GCNInstruction& ins);

	// VectorMemory
	void emitVectorMemBufNoFmt(GCNInstruction& ins);
	void emitVectorMemBufFmt(GCNInstruction& ins);
	void emitVectorMemImgNoSmp(GCNInstruction& ins);
	void emitVectorMemImgSmp(GCNInstruction& ins);
	void emitVectorMemImgUt(GCNInstruction& ins);
	void emitVectorMemL1Cache(GCNInstruction& ins);

	// DataShare
	void emitDsIdxRd(GCNInstruction& ins);
	void emitDsIdxWr(GCNInstruction& ins);
	void emitDsIdxWrXchg(GCNInstruction& ins);
	void emitDsIdxCondXchg(GCNInstruction& ins);
	void emitDsIdxWrap(GCNInstruction& ins);
	void emitDsAtomicArith32(GCNInstruction& ins);
	void emitDsAtomicArith64(GCNInstruction& ins);
	void emitDsAtomicMinMax32(GCNInstruction& ins);
	void emitDsAtomicMinMax64(GCNInstruction& ins);
	void emitDsAtomicCmpSt32(GCNInstruction& ins);
	void emitDsAtomicCmpSt64(GCNInstruction& ins);
	void emitDsAtomicLogic32(GCNInstruction& ins);
	void emitDsAtomicLogic64(GCNInstruction& ins);
	void emitDsAppendCon(GCNInstruction& ins);
	void emitDsDataShareUt(GCNInstruction& ins);
	void emitDsDataShareMisc(GCNInstruction& ins);
	void emitGdsSync(GCNInstruction& ins);
	void emitGdsOrdCnt(GCNInstruction& ins);

	// VectorInterpolation
	void emitVectorInterpFpCache(GCNInstruction& ins);

	// Export
	void emitExp(GCNInstruction& ins);
	void emitExpVS(GCNInstruction& ins);
	void emitExpPS(GCNInstruction& ins);

	// DebugProfile
	void emitDbgProf(GCNInstruction& ins);

	/////////////////////////////////////////////////////////

	///////////////////////////
	// Type definition methods
	uint32_t getPerVertexBlockId();

	uint32_t getScalarTypeId(
		SpirvScalarType type);

	uint32_t getVectorTypeId(
		const SpirvVectorType& type);

	uint32_t getArrayTypeId(
		const SpirvArrayType& type);

	uint32_t getPointerTypeId(
		const SpirvRegisterInfo& type);

	////////////////
	// Misc methods

	bool isDoubleWordType(
		SpirvScalarType type) const;

	// Convenient when used with opcodes with may have
	// different encodings. e.g. V_MAC_F32 [VOP2|VOP3]
	uint32_t getVopOpcode(GCNInstruction& ins);

	void getVopOperands(
		GCNInstruction& ins,
		uint32_t* vdst, uint32_t* vdstRidx,
		uint32_t* src0, uint32_t* src0Ridx,
		uint32_t* src1 = nullptr, uint32_t* src1Ridx = nullptr,
		uint32_t* src2 = nullptr, uint32_t* src2Ridx = nullptr,
		uint32_t* sdst = nullptr, uint32_t* sdstRidx = nullptr);


	// Convenience function to dynamic cast instruction types
	template<typename InsType>
	inline
		typename std::enable_if<std::is_base_of<Instruction, InsType>::value, const InsType*>::type
		asInst(const GCNInstruction& ins)
	{
		return dynamic_cast<const InsType*>(ins.instruction.get());
	}

private:

	PsslProgramInfo m_programInfo;

	SpirvModule m_module;

	// Global analyze information
	const GcnAnalysisInfo* m_analysis;

	GcnShaderInput m_shaderInput;
	///////////////////////////////////////////////////
	// Entry point description - we'll need to declare
	// the function ID and all input/output variables.
	std::vector<uint32_t> m_entryPointInterfaces;
	uint32_t              m_entryPointId = 0;

	////////////////////////////////////////////////////
	// Per-vertex input and output blocks. Depending on
	// the shader stage, these may be declared as arrays.
	uint32_t m_perVertexIn = 0;
	uint32_t m_perVertexOut = 0;

	//////////////////////////////////////////////
	// Function state tracking. Required in order
	// to properly end functions in some cases.
	bool m_insideFunction = false;


	///////////////////////////////////
	// Shader-specific data structures
	GcnCompilerVsPart m_vs;
	GcnCompilerPsPart m_ps;
	GcnCompilerCsPart m_cs;

	///////////////////////////////////
	// State registers
	GcnStateRegister m_stateRegs;

	///////////////////////////////////
	// Gcn register to spir-v variable map
	// gcn register index -- spirv register
	std::map<uint32_t, SpirvRegisterPointer> m_sgprs;
	std::map<uint32_t, SpirvRegisterPointer> m_vgprs;

	///////////////////////////////////
	// Resources

	// spir-v id to literal constant value table
	std::map<uint32_t, SpirvLiteralConstant> m_constValueTable;

};



} // namespace pssl

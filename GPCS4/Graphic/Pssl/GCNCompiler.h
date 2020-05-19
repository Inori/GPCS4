#pragma once

#include "GCNAnalyzer.h"
#include "GCNDecoder.h"
#include "GCNEnums.h"
#include "GCNInstruction.h"
#include "GCNParser/DSInstruction.h"
#include "GCNParser/EXPInstruction.h"
#include "GCNParser/MIMGInstruction.h"
#include "GCNParser/MTBUFInstruction.h"
#include "GCNParser/MUBUFInstruction.h"
#include "GCNParser/SMRDInstruction.h"
#include "GCNParser/SOP1Instruction.h"
#include "GCNParser/SOP2Instruction.h"
#include "GCNParser/SOPCInstruction.h"
#include "GCNParser/SOPKInstruction.h"
#include "GCNParser/SOPPInstruction.h"
#include "GCNParser/VINTRPInstruction.h"
#include "GCNParser/VOPInstruction.h"
#include "GCNSpirvTypes.h"
#include "GCNStateRegister.h"
#include "PsslCommon.h"
#include "PsslFetchShader.h"
#include "PsslProgramInfo.h"
#include "PsslShaderFileBinary.h"
#include "PsslShaderStructure.h"

#include "../SpirV/SpirvModule.h"
#include "../Violet/VltPipelineLayout.h"
#include "../Violet/VltShader.h"

#include <array>
#include <map>
#include <optional>

namespace pssl
{;


constexpr size_t GcnMaxSgprCount = 104;
constexpr size_t GcnMaxVgprCount = 256;

/**
 * \brief Vertex shader-specific structure
 */
struct GcnCompilerVsPart
{
	uint32_t mainFunctionId = 0;
	uint32_t fsFunctionId   = 0;

	uint32_t builtinVertexId     = 0;
	uint32_t builtinInstanceId   = 0;
	uint32_t builtinBaseVertex   = 0;
	uint32_t builtinBaseInstance = 0;

	// semantic -- spirv id
	std::map<uint32_t, SpirvRegisterPointer> vsInputs;
	// exp target -- spirv id
	std::map<uint32_t, SpirvRegisterPointer> vsOutputs;
};

/**
 * \brief Pixel shader-specific structure
 */
struct GcnCompilerPsPart
{
	uint32_t functionId = 0;

	uint32_t builtinFragCoord     = 0;
	uint32_t builtinDepth         = 0;
	uint32_t builtinStencilRef    = 0;
	uint32_t builtinIsFrontFace   = 0;
	uint32_t builtinSampleId      = 0;
	uint32_t builtinSampleMaskIn  = 0;
	uint32_t builtinSampleMaskOut = 0;
	uint32_t builtinLayer         = 0;
	uint32_t builtinViewportId    = 0;

	uint32_t builtinLaneId = 0;
	uint32_t killState     = 0;

	uint32_t specRsSampleCount = 0;
	// attr index -- spirv id
	std::map<uint32_t, SpirvRegisterPointer> psInputs;
	// exp target -- spirv id
	std::map<uint32_t, SpirvRegisterPointer> psOutputs;
};

/**
 * \brief Compute shader-specific structure
 */
struct GcnCompilerCsPart
{
	uint32_t functionId = 0;

	uint32_t workgroupSizeX = 0;
	uint32_t workgroupSizeY = 0;
	uint32_t workgroupSizeZ = 0;

	uint32_t builtinGlobalInvocationId   = 0;
	uint32_t builtinLocalInvocationId    = 0;
	uint32_t builtinLocalInvocationIndex = 0;
	uint32_t builtinWorkgroupId          = 0;

	SpirvRegisterPointer lds;
};


class GCNCompiler : public GCNInstructionIterator
{
	friend class SpirvRegisterU64;

public:
	GCNCompiler(
		const PsslProgramInfo& progInfo,
		const GcnAnalysisInfo& analysis,
		const GcnShaderInput&  shaderInput);
	~GCNCompiler();

	virtual void processInstruction(GCNInstruction& ins);

	RcPtr<vlt::VltShader> finalize();

private:
	void compileInstruction(GCNInstruction& ins);

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
		uint32_t entryPoint,
		uint32_t returnType,
		uint32_t funcType);

	void emitFunctionEnd();

	void emitMainFunctionBegin();

	void emitFunctionLabel();

	// VS
	void emitDclVertexInput();
	void emitDclVertexOutput();
	void emitEmuFetchShader();
	// PS
	void emitDclPixelInput();
	void emitDclPixelOutput();
	// CS
	void emitDclThreadGroup();
	void emitDclThreadGroupSharedMemory();

	// TODO:
	// For SGPRs and some VGPRs maybe,
	// we should use specialization constants
	void emitGprInitializeVS();
	void emitGprInitializePS();
	void emitGprInitializeCS();

	// For all shader types
	void emitInitUserDataRegisters(uint32_t count);
	void emitDclStateRegisters();
	
	void emitDclShaderResource(const GcnShaderResourceInstance& res);
	void emitDclShaderResourceUD();
	void emitDclShaderResourceEUD(uint32_t dstRegIndex, uint32_t eudOffsetDw);

	void emitDclImmConstBuffer(const GcnShaderResourceInstance& res);
	void emitDclImmSampler(const GcnShaderResourceInstance& res);
	void emitDclImmBuffer(const GcnShaderResourceInstance& res);
	void emitDclImmTexture(const GcnShaderResourceInstance& res);
	void emitDclImmResource(const GcnShaderResourceInstance& res);

	

	SpirvRegisterValue emitVsSystemValueLoad(
		SpirvSystemValue sv,
		GcnRegMask       mask);

	SpirvRegisterValue emitPsSystemValueLoad(
		SpirvSystemValue sv,
		GcnRegMask       mask);

	SpirvRegisterValue emitCsSystemValueLoad(
		SpirvSystemValue sv,
		GcnRegMask       mask);

	/////////////////////////////////////////////////////////
	SpirvRegisterValue emitValueLoad(
		const SpirvRegisterPointer& reg);

	void emitValueStore(
		const SpirvRegisterPointer& ptr,
		const SpirvRegisterValue&   src,
		const GcnRegMask&           writeMask);

	template <SpirvGprType GprType>
	SpirvRegisterValue emitGprLoad(
		uint32_t        index,
		SpirvScalarType dstType);

	template <SpirvGprType GprType>
	void emitGprStore(
		uint32_t                  index,
		const SpirvRegisterValue& value);

	template <SpirvGprType GprType>
	SpirvRegisterPointer emitGprCreate(
		uint32_t                index,
		SpirvScalarType         type,
		std::optional<uint32_t> initId = std::nullopt);

	SpirvRegisterPointer emitVgprCreate(
		uint32_t                index,
		SpirvScalarType         type,
		std::optional<uint32_t> initId = std::nullopt);

	SpirvRegisterPointer emitSgprCreate(
		uint32_t                index,
		SpirvScalarType         type,
		std::optional<uint32_t> initId = std::nullopt);

	void emitSgprArrayStore(
		uint32_t                  startIdx,
		const SpirvRegisterValue* values,
		uint32_t                  count);
	void emitVgprArrayStore(
		uint32_t                  startIdx,
		const SpirvRegisterValue* values,
		uint32_t                  count);
	// Store a vector to continuous vgprs
	void emitVgprVectorStore(
		uint32_t                  startIdx,
		const SpirvRegisterValue& srcVec,
		const GcnRegMask&         writeMask);

	// Load/Store sgpr pairs as Uint64 type
	// e.g. s[2:3]
	SpirvRegisterValue emitSgprPairLoad(
		uint32_t firstIndex);

	void emitSgprPairStore(
		uint32_t                  firstIndex,
		const SpirvRegisterValue& srcReg);

	SpirvRegisterValue emitLiteralConstLoad(
		uint32_t        value,
		SpirvScalarType dstType);

	SpirvRegisterValue emitStateRegisterLoad(
		const GcnStateRegister& reg,
		SpirvScalarType         dstType,
		const std::string&      name = "");

	void emitStateRegisterStore(const GcnStateRegister&   reg,
								const SpirvRegisterValue& src);
	/////////////////////////////////////////
	// Operands manipulation methods
	SpirvRegisterValue emitLoadScalarOperand(
		uint32_t        srcOperand,
		uint32_t        regIndex,
		SpirvScalarType dstType,
		uint32_t        literalConst = 0);
	SpirvRegisterValue emitLoadVectorOperand(
		uint32_t        index,
		SpirvScalarType dstType);

	void emitStoreScalarOperand(
		uint32_t                  dstOperand,
		uint32_t                  regIndex,
		const SpirvRegisterValue& srcReg);
	void emitStoreVectorOperand(
		uint32_t                  dstIndex,
		const SpirvRegisterValue& srcReg);

	SpirvRegisterValue emitInlineConstantFloat(
		Instruction::OperandSRC src);
	SpirvRegisterValue emitInlineConstantInteger(
		Instruction::OperandSRC src,
		uint32_t                regIndex,
		SpirvScalarType         dstType);

	///////////////////////////////////////
	// Image register manipulation methods
	uint32_t emitLoadSampledImage(
		const SpirvTexture& textureResource,
		const SpirvSampler& samplerResource);

	SpirvRegisterValue emitPackFloat16(const SpirvRegisterValue& v2floatVec);
	SpirvRegisterValue emitUnpackFloat16(const SpirvRegisterValue& uiVec);

	///////////////////////////////
	// Variable definition methods
	uint32_t emitNewVariable(
		const SpirvRegisterInfo& info,
		const std::string&       name        = "",
		std::optional<uint32_t>  initValueId = std::nullopt);

	uint32_t emitNewBuiltinVariable(
		const SpirvRegisterInfo& info,
		spv::BuiltIn             builtIn,
		const char*              name);

	///////////////////////////
	// Control Flow methods
	void emitBranchLabelTry();

	///////////////////////////
	// VOP3 modifiers
	void emitVop3InputModifier(
		const GCNInstruction&                                          ins,
		const std::vector<std::reference_wrapper<SpirvRegisterValue>>& values);

	SpirvRegisterValue emitVop3OutputModifier(
		const GCNInstruction&     ins,
		const SpirvRegisterValue& value);

	SpirvRegisterValue emitLoadVopSrc1(
		const GCNInstruction& ins,
		uint32_t              srcOperand,
		uint32_t              regIndex,
		SpirvScalarType       dstType);

	////////////////////////////////
	// Pointer manipulation methods
	SpirvRegisterPointer emitArrayAccess(
		SpirvRegisterPointer pointer,
		spv::StorageClass    sclass,
		uint32_t             index);

	SpirvRegisterPointer emitVectorAccess(
		SpirvRegisterPointer pointer,
		spv::StorageClass    sclass,
		GcnRegMask           mask);

	SpirvRegisterValue emitVectorLoad(
		SpirvRegisterPointer pointer,
		spv::StorageClass    sclass,
		GcnRegMask           mask);

	void emitVectorStore(
		SpirvRegisterPointer pointer,
		spv::StorageClass    sclass,
		GcnRegMask           mask,
		SpirvRegisterValue   value);

	////////////////////////////////////////////////
	// Constant building methods. These are used to
	// generate constant vectors that store the same
	// value in each component.
	SpirvRegisterValue emitBuildConstVecf32(
		float             x,
		float             y,
		float             z,
		float             w,
		const GcnRegMask& writeMask);

	SpirvRegisterValue emitBuildConstVecu32(
		uint32_t          x,
		uint32_t          y,
		uint32_t          z,
		uint32_t          w,
		const GcnRegMask& writeMask);

	SpirvRegisterValue emitBuildConstVeci32(
		int32_t           x,
		int32_t           y,
		int32_t           z,
		int32_t           w,
		const GcnRegMask& writeMask);

	SpirvRegisterValue emitBuildConstVecf64(
		double            xy,
		double            zw,
		const GcnRegMask& writeMask);

	/////////////////////////////////////////
	// Generic register manipulation methods
	SpirvRegisterValue emitRegisterBitcast(
		SpirvRegisterValue srcValue,
		SpirvScalarType    dstType);

	SpirvRegisterValue emitRegisterSwizzle(
		SpirvRegisterValue value,
		GcnRegSwizzle      swizzle,
		GcnRegMask         writeMask);

	SpirvRegisterValue emitRegisterExtract(
		SpirvRegisterValue value,
		GcnRegMask         mask);

	SpirvRegisterValue emitRegisterInsert(
		SpirvRegisterValue dstValue,
		SpirvRegisterValue srcValue,
		GcnRegMask         srcMask);

	SpirvRegisterValue emitRegisterConcat(
		SpirvRegisterValue value1,
		SpirvRegisterValue value2);

	SpirvRegisterValue emitRegisterExtend(
		SpirvRegisterValue value,
		uint32_t           size);

	SpirvRegisterValue emitRegisterAbsolute(
		SpirvRegisterValue value);

	SpirvRegisterValue emitRegisterNegate(
		SpirvRegisterValue value);

	SpirvRegisterValue emitRegisterZeroTest(
		SpirvRegisterValue value,
		SpirvZeroTest      test);

	SpirvRegisterValue emitRegisterMaskBits(
		SpirvRegisterValue value,
		uint32_t           mask);


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

	// DebugProfile
	void emitDbgProf(GCNInstruction& ins);

	// Extra dispatch functions
	void emitVectorMemBufFmtUntyped(GCNInstruction& ins);
	void emitVectorMemBufFmtTyped(GCNInstruction& ins);

	void emitScalarProgFlowPC(GCNInstruction& ins);
	void emitScalarProgFlowBranch(GCNInstruction& ins);

	SpirvRegisterValue emitExpSrcLoadCompr(GCNInstruction& ins);
	SpirvRegisterValue emitExpSrcLoadNoCompr(GCNInstruction& ins);
	void               emitExpVS(GCNInstruction& ins);
	void               emitExpPS(GCNInstruction& ins);

	void emitScalarMemBufferLoad(
		uint32_t bufferId,
		uint32_t dstRegStart,
		uint32_t dstRegCount,
		bool     imm,
		uint32_t offset,
		uint32_t literal);

	void emitVectorMemBufferLoad(GCNInstruction& ins);
	void emitVectorMemBufferStore(GCNInstruction& ins);

	void emitScalarMovSOP1(GCNInstruction& ins);
	void emitScalarMovSOPK(GCNInstruction& ins);

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

	uint32_t findResourceBufferId(uint32_t regIndex);

	bool isVop3Encoding(
		const GCNInstruction& ins);

	SpirvScalarType getScalarType(
		Instruction::OperandType operandType);

	// Convenient when used with opcodes with may have
	// different encodings. e.g. V_MAC_F32 [VOP2|VOP3]
	uint32_t getVopOpcode(
		GCNInstruction& ins);

	void getVopOperands(
		GCNInstruction& ins,
		uint32_t*       vdst,
		uint32_t*       vdstRidx,
		uint32_t*       src0,
		uint32_t*       src0Ridx,
		uint32_t*       src1     = nullptr,
		uint32_t*       src1Ridx = nullptr,
		uint32_t*       src2     = nullptr,
		uint32_t*       src2Ridx = nullptr,
		uint32_t*       sdst     = nullptr,
		uint32_t*       sdstRidx = nullptr);

	uint32_t getSopOpcode(const GCNInstruction& ins);

	void getSopOperands(
		const GCNInstruction& ins,
		uint32_t*             sdst,
		uint32_t*             sdstRidx,
		uint32_t*             src0,
		uint32_t*             src0Ridx,
		uint32_t*             src1     = nullptr,
		uint32_t*             src1Ridx = nullptr,
		int16_t*              imm      = nullptr);

	const char* getTypeName(SpirvScalarType type);

private:
	////////////////////////////////////////////////////
	// Constructor inputs

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
	uint32_t m_perVertexIn  = 0;
	uint32_t m_perVertexOut = 0;

	uint32_t m_clipDistances = 0;
	uint32_t m_cullDistances = 0;

	uint32_t m_primitiveIdIn  = 0;
	uint32_t m_primitiveIdOut = 0;

	//////////////////////////////////////////////
	// Function state tracking. Required in order
	// to properly end functions in some cases.
	bool m_insideFunction = false;

	// Control Flow Block state tracking.
	// A block start with OpLabel and 
	// end with termination instruction.
	// See spir-v manual for details.
	bool m_insideBlock = false;

	///////////////////////////////////
	// Shader-specific data structures
	GcnCompilerVsPart m_vs;
	GcnCompilerPsPart m_ps;
	GcnCompilerCsPart m_cs;

	//////////////////////////////////////////////////////
	// Shader resource variables. These provide access to
	// constant buffers, samplers, textures, and storage buffers.
	std::array<SpirvConstantBuffer, GcnMaxSgprCount> m_constantBuffers;
	std::array<SpirvSampler, GcnMaxSgprCount>        m_samplers;
	std::array<SpirvTexture, GcnMaxSgprCount>        m_textures;
	std::array<SpirvRegularBuffer, GcnMaxSgprCount>  m_regularBuffers;

	///////////////////////////////////
	// State registers
	GcnStateRegisters m_stateRegs;

	///////////////////////////////////
	// Gcn register to spir-v variable map
	std::array<SpirvRegisterPointer, GcnMaxSgprCount> m_sgprs;
	std::array<SpirvRegisterPointer, GcnMaxVgprCount> m_vgprs;

	///////////////////////////////////
	// Resources

	// Used to record shader resource this shader declared using InputUsageSlot
	std::vector<vlt::VltResourceSlot> m_resourceSlots;

	///////////////////////////////////
	// Control flow
	std::unordered_map<uint32_t, uint32_t> m_branchLabels;
};


}  // namespace pssl

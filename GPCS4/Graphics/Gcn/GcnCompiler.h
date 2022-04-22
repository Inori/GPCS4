#pragma once

#include "GcnCommon.h"
#include "GcnProgramInfo.h"
#include "GcnInstructionIterator.h"
#include "GcnShaderMeta.h"
#include "GcnDecoder.h"
#include "GcnCompilerDefs.h"

#include "SpirV/SpirvModule.h"
#include "Violet/VltRc.h"
#include "Violet/VltPipeLayout.h"
#include "Violet/VltShader.h"

#include <vector>

namespace sce::Gnm
{
	enum TextureType;
}  // namespace sce::Gnm

namespace sce::gcn
{

	class GcnHeader;
	struct GcnShaderInstruction;
	struct GcnAnalysisInfo;
	struct GcnShaderResource;


	/**
	 * \brief Shader recompiler
	 * 
	 * Recompile GCN instructions into Spir-V byte code.
	 * Produce VltShader for Violet.
	 */
	class GcnCompiler : public GcnInstructionIterator
	{
	public:
		GcnCompiler(
			const std::string&     fileName,
			const GcnProgramInfo&  programInfo,
			const GcnHeader&       header,
			const GcnShaderMeta&   meta,
			const GcnAnalysisInfo& analysis);
		virtual ~GcnCompiler();

		/**
         * \brief Processes a single instruction
         * \param [in] ins The instruction
         */
		virtual void processInstruction(
			const GcnShaderInstruction& ins) override;

		/**
         * \brief Finalizes the shader
         * \returns The final shader object
         */
		vlt::Rc<vlt::VltShader> finalize();

	private:
		void compileInstruction(
			const GcnShaderInstruction& ins);
		/////////////////////////////////////////////////////////
		// Category handlers
		void emitScalarALU(const GcnShaderInstruction& ins);
		void emitScalarMemory(const GcnShaderInstruction& ins);
		void emitVectorALU(const GcnShaderInstruction& ins);
		void emitVectorMemory(const GcnShaderInstruction& ins);
		void emitFlowControl(const GcnShaderInstruction& ins);
		void emitDataShare(const GcnShaderInstruction& ins);
		void emitVectorInterpolation(const GcnShaderInstruction& ins);
		void emitExport(const GcnShaderInstruction& ins);
		void emitDebugProfile(const GcnShaderInstruction& ins);

		//////////////////////////////////////
		// Common function definition methods
		void emitInit();

		void emitFunctionBegin(
			uint32_t entryPoint,
			uint32_t returnType,
			uint32_t funcType);

		void emitFunctionEnd();

		void emitFunctionLabel();

		void emitMainFunctionBegin();

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

		////////////////////////////
		// Input/output preparation
		void emitInputSetup();
		void emitFetchInput();

		void emitOutputSetup();
		/////////////////////////////////////////////////////
		// Shader interface and metadata declaration methods
		void emitDclInputSlots();
		void emitDclVertexInput();
		void emitDclInput(
			const VertexInputSemantic& sema);

		void emitDclBuffer(
			const GcnShaderResource& res);
		void emitDclTexture(
			const GcnShaderResource& res);
		void emitDclSampler(
			const GcnShaderResource& res);
		///////////////////////////////
		// Variable definition methods
		uint32_t emitNewVariable(
			const GcnRegisterInfo& info);

		uint32_t emitNewBuiltinVariable(
			const GcnRegisterInfo& info,
			spv::BuiltIn           builtIn,
			const char*            name);

		///////////////////////////////
		// SGPR/VGPR load/store methods
		GcnRegisterValue emitVgprLoad(
			const GcnInstOperand& reg);
		GcnRegisterValue emitSgprLoad(
			const GcnInstOperand& reg);
		GcnRegisterValue emitVgprArrayLoad(
			const GcnInstOperand& start,
			uint32_t              count);
		GcnRegisterValue emitSgprArrayLoad(
			const GcnInstOperand& start,
			uint32_t              count);

		void emitVgprStore(
			const GcnInstOperand&   reg,
			const GcnRegisterValue& value);
		void emitSgprStore(
			const GcnInstOperand&   reg,
			const GcnRegisterValue& value);
		void emitVgprArrayStore(
			const GcnInstOperand&   start,
			uint32_t                count,
			const GcnRegisterValue& value);
		void emitSgprArrayStore(
			const GcnInstOperand&   start,
			uint32_t                count,
			const GcnRegisterValue& value);

		//////////////////////////////
		// Operand load/store methods
		GcnRegisterValue emitValueLoad(
			GcnRegisterPointer ptr);

		void emitValueStore(
			GcnRegisterPointer ptr,
			GcnRegisterValue   value,
			GcnRegMask         writeMask);

		GcnRegisterValue emitRegisterLoad(
			const GcnInstOperand& reg,
			GcnRegMask            writeMask);

		void emitRegisterStore(
			const GcnInstOperand& reg,
			GcnRegisterValue      value);

		GcnRegisterPointer emitCompositeAccess(
			GcnRegisterPointer pointer,
			spv::StorageClass  sclass,
			uint32_t           index);

		////////////////////////////////////////////////
		// Constant building methods. These are used to
		// generate constant vectors that store the same
		// value in each component.
		GcnRegisterValue emitBuildConstVecf32(
			float              x,
			float              y,
			float              z,
			float              w,
			const GcnRegMask& writeMask);

		GcnRegisterValue emitBuildConstVecu32(
			uint32_t           x,
			uint32_t           y,
			uint32_t           z,
			uint32_t           w,
			const GcnRegMask& writeMask);

		GcnRegisterValue emitBuildConstVeci32(
			int32_t            x,
			int32_t            y,
			int32_t            z,
			int32_t            w,
			const GcnRegMask& writeMask);

		GcnRegisterValue emitBuildConstVecf64(
			double             xy,
			double             zw,
			const GcnRegMask& writeMask);
		/////////////////////////////////////////
		// Generic register manipulation methods
		GcnRegisterValue emitRegisterBitcast(
			GcnRegisterValue srcValue,
			GcnScalarType    dstType);

		GcnRegisterValue emitRegisterSwizzle(
			GcnRegisterValue value,
			GcnRegSwizzle    swizzle,
			GcnRegMask       writeMask);

		GcnRegisterValue emitRegisterExtract(
			GcnRegisterValue value,
			GcnRegMask       mask);

		GcnRegisterValue emitRegisterInsert(
			GcnRegisterValue dstValue,
			GcnRegisterValue srcValue,
			GcnRegMask       srcMask);

		GcnRegisterValue emitRegisterConcat(
			GcnRegisterValue value1,
			GcnRegisterValue value2);

		GcnRegisterValue emitRegisterExtend(
			GcnRegisterValue value,
			uint32_t          size);

		GcnRegisterValue emitRegisterAbsolute(
			GcnRegisterValue value);

		GcnRegisterValue emitRegisterNegate(
			GcnRegisterValue value);

		GcnRegisterValue emitRegisterZeroTest(
			GcnRegisterValue value,
			GcnZeroTest      test);

		GcnRegisterValue emitRegisterMaskBits(
			GcnRegisterValue value,
			uint32_t          mask);

		//GcnRegisterValue emitSrcOperandModifiers(
		//	GcnRegisterValue value,
		//	GcnRegModifiers  modifiers);

		//GcnRegisterValue emitDstOperandModifiers(
		//	GcnRegisterValue value,
		//	GcnOpModifiers   modifiers);

		///////////////////////////
		// Type definition methods
		uint32_t getScalarTypeId(
			GcnScalarType type);

		uint32_t getVectorTypeId(
			const GcnVectorType& type);

		uint32_t getArrayTypeId(
			const GcnArrayType& type);

		uint32_t getPointerTypeId(
			const GcnRegisterInfo& type);

		uint32_t getPerVertexBlockId();


		///////////////////////////
		//
		bool isDoubleType(
			GcnScalarType type) const;

		uint32_t getUserSgprCount() const;

		bool hasFetchShader() const;

		std::pair<const VertexInputSemantic*, uint32_t>
			getSemanticTable();

		const std::array<GcnTextureInfo, 128>&
			getTextureInfoTable();

		GcnImageInfo getImageType(
			Gnm::TextureType textureType,
			bool             isStorage,
			bool             isDepth) const;

	private:
		GcnProgramInfo         m_programInfo;
		const GcnHeader*       m_header;
		GcnShaderMeta          m_meta;
		const GcnAnalysisInfo* m_analysis;
		SpirvModule            m_module;

		///////////////////////////////////////////////////
		// Entry point description - we'll need to declare
		// the function ID and all input/output variables.
		uint32_t              m_entryPointId = 0;
		std::vector<uint32_t> m_entryPointInterfaces;

		///////////////////////////////////////////////////
		// Shader input interfaces
		std::array<
			GcnRegisterPointer, 
			GcnMaxInterfaceRegs> m_inputs;

		///////////////////////////////////////////////////
		// VGPR/SGPR registers
		std::array<
			GcnRegisterPointer,
			GcnMaxVGPR> m_vgprs = {};
		std::array<
			GcnRegisterPointer,
			GcnMaxSGPR> m_sgprs = {};

		//////////////////////////////////////////////////////
		// Shader resource variables. These provide access to
		// buffers, samplers and textures.
		std::array<GcnBuffer, 16>   m_buffers;
		std::array<GcnSampler, 16>  m_samplers;
		std::array<GcnTexture, 128> m_textures;

		//////////////////////////////////////////////
		// Function state tracking. Required in order
		// to properly end functions in some cases.
		bool m_insideFunction = false;
		///////////////////////////////////////////////////////////
		// An array stores up to 16 user data registers.
		uint32_t m_vUserDataArray = 0;

		////////////////////////////////////////////////////
		// Per-vertex input and output blocks. Depending on
		// the shader stage, these may be declared as arrays.
		uint32_t m_perVertexIn  = 0;
		uint32_t m_perVertexOut = 0;

		///////////////////////////////////////////////////////
		// Resource slot description for the shader.
		std::vector<vlt::VltResourceSlot> m_resourceSlots;
		////////////////////////////////////////////
		// Inter-stage shader interface slots. Also
		// covers vertex input and fragment output.
		vlt::VltInterfaceSlots m_interfaceSlots;
		//////////////////////////////////////////////////
		// Immediate constant buffer. If defined, this is
		// an array of four-component uint32 vectors.
		uint32_t                m_immConstBuf = 0;
		vlt::VltShaderConstData m_immConstData;

		///////////////////////////////////
		// Shader-specific data structures
		GcnCompilerVsPart m_vs;
		GcnCompilerPsPart m_ps;
		GcnCompilerCsPart m_cs;
	};

}  // namespace sce::gcn
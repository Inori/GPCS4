#pragma once

#include "GcnCommon.h"
#include "GcnProgramInfo.h"
#include "GcnInstructionIterator.h"
#include "GcnShaderMeta.h"
#include "GcnDecoder.h"

#include "SpirV/SpirvModule.h"
#include "Violet/VltRc.h"
#include "Violet/VltPipeLayout.h"
#include "Violet/VltShader.h"

#include <vector>


namespace sce::gcn
{

	class GcnHeader;
	struct GcnShaderInstruction;
	struct GcnAnalysisInfo;

	enum class GcnZeroTest : uint32_t
	{
		TestZ  = 0,
		TestNz = 1,
	};

	/**
     * \brief Scalar value type
     * 
     * Enumerates possible register component
     * types. Scalar types are represented as
     * a one-component vector type.
     */
	enum class GcnScalarType : uint32_t
	{
		Uint32  = 0,
		Uint64  = 1,
		Sint32  = 2,
		Sint64  = 3,
		Float32 = 4,
		Float64 = 5,
		Bool    = 6,
	};

	/**
	 * \brief Vector type
	 *
	 * Convenience struct that stores a scalar
	 * type and a component count. The compiler
	 * can use this to generate SPIR-V types.
	 */
	struct GcnVectorType
	{
		GcnScalarType ctype;
		uint32_t      ccount;
	};

	/**
	 * \brief Array type
	 *
	 * Convenience struct that stores a scalar type, a
	 * component count and an array size. An array of
	 * length 0 will be evaluated to a vector type. The
	 * compiler can use this to generate SPIR-V types.
	 */
	struct GcnArrayType
	{
		GcnScalarType ctype;
		uint32_t      ccount;
		uint32_t      alength;
	};

	/**
	 * \brief Register info
	 *
	 * Stores the array type of a register and
	 * its storage class. The compiler can use
	 * this to generate SPIR-V pointer types.
	 */
	struct GcnRegisterInfo
	{
		GcnArrayType      type;
		spv::StorageClass sclass;
	};

	/**
	 * \brief Register value
	 *
	 * Stores a vector type and a SPIR-V ID that
	 * represents an intermediate value. This is
	 * used to track the type of such values.
	 */
	struct GcnRegisterValue
	{
		GcnVectorType type;
		uint32_t      id;
	};

	/**
	 * \brief Register pointer
	 *
	 * Stores a vector type and a SPIR-V ID that
	 * represents a pointer to such a vector. This
	 * can be used to load registers conveniently.
	 */
	struct GcnRegisterPointer
	{
		GcnVectorType type;
		uint32_t      id;
	};

	/**
	 * \brief Vertex shader-specific structure
	 */
	struct GcnCompilerVsPart
	{
		uint32_t functionId = 0;

		uint32_t builtinVertexId     = 0;
		uint32_t builtinInstanceId   = 0;
		uint32_t builtinBaseVertex   = 0;
		uint32_t builtinBaseInstance = 0;
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
	};

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

	private:
		////////////////////////////
		// Input/output preparation
		void emitInputFetch();

		void emitOutputSetup();
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

		//////////////////////////////////////////////
		// Function state tracking. Required in order
		// to properly end functions in some cases.
		bool m_insideFunction = false;
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
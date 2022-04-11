#include "GcnCompiler.h"
#include "GcnHeader.h"
#include "GcnDecoder.h"

LOG_CHANNEL(Graphic.Gcn.GcnCompiler);

using namespace sce::vlt;

namespace sce::gcn
{
	GcnCompiler::GcnCompiler(
		const std::string&     fileName,
		const GcnProgramInfo&  programInfo,
		const GcnHeader&       header,
		const GcnAnalysisInfo& analysis) :
		m_programInfo(programInfo),
		m_header(&header),
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

	void GcnCompiler::emitFunctionBegin(uint32_t entryPoint, uint32_t returnType, uint32_t funcType)
	{
	}

	void GcnCompiler::emitFunctionEnd()
	{
	}

	void GcnCompiler::emitFunctionLabel()
	{
	}

	void GcnCompiler::emitMainFunctionBegin()
	{
	}

	void GcnCompiler::emitVsInit()
	{
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

}  // namespace sce::gcn
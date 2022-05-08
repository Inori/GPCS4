#include "GnmCommandBuffer.h"

#include "Emulator.h"
#include "PlatProcess.h"
#include "VirtualGPU.h"
#include "GnmRenderState.h"
#include "Gcn/GcnShaderRegField.h"
#include "Sce/SceGpuQueue.h"
#include "Violet/VltCmdList.h"
#include "Violet/VltDevice.h"

using namespace sce::vlt;
using namespace sce::gcn;

namespace sce::Gnm
{

	GnmCommandBuffer::GnmCommandBuffer(vlt::VltDevice* device) :
		m_device(device),
		m_factory(device)
	{
	}

	GnmCommandBuffer::~GnmCommandBuffer()
	{
	}

	Rc<VltCommandList> GnmCommandBuffer::finalize()
	{
		return m_context->endRecording();
	}

	void GnmCommandBuffer::initializeDefaultHardwareState()
	{
		m_tracker      = &(GPU().resourceTracker());
		m_labelManager = &(GPU().labelManager());
	}

	void GnmCommandBuffer::emuWriteGpuLabel(EventWriteSource selector, void* label, uint64_t value)
	{
		do
		{
			if (!label)
			{
				break;
			}

			if (selector == kEventWriteSource32BitsImmediate)
			{
				*(uint32_t*)label = value;
			}
			else if (selector == kEventWriteSource64BitsImmediate)
			{
				*(uint64_t*)label = value;
			}
			else
			{
				*(uint64_t*)label = plat::GetProcessTimeCounter();
			}

		} while (false);
	}

	void GnmCommandBuffer::setCsShader(
		GnmShaderContext&            ctx,
		const gcn::CsStageRegisters* computeData,
		uint32_t                     shaderModifier)
	{
		ctx.code = computeData->getCodeAddress();

		ctx.meta.cs.computeNumThreadX = computeData->computeNumThreadX;
		ctx.meta.cs.computeNumThreadY = computeData->computeNumThreadY;
		ctx.meta.cs.computeNumThreadZ = computeData->computeNumThreadZ;

		const COMPUTE_PGM_RSRC2* rsrc2   = reinterpret_cast<const COMPUTE_PGM_RSRC2*>(&computeData->computePgmRsrc2);
		ctx.meta.cs.userSgprCount        = rsrc2->user_sgpr;
		ctx.meta.cs.enableTgidX          = rsrc2->tgid_x_en;
		ctx.meta.cs.enableTgidY          = rsrc2->tgid_y_en;
		ctx.meta.cs.enableTgidZ          = rsrc2->tgid_z_en;
		ctx.meta.cs.enableTgSize         = rsrc2->tg_size_en;
		ctx.meta.cs.threadIdInGroupCount = rsrc2->tidig_comp_cnt;
		// LDS is allocated in 128 dword blocks on PS4
		constexpr uint32_t LDSAlignShift = 9;
		ctx.meta.cs.ldsSize              = rsrc2->lds_size << LDSAlignShift;
	}

}  // namespace sce::Gnm
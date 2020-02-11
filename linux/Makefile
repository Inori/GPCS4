CFLAGS = -Wall -Wunused -fexceptions -Winvalid-pch -std=c++2a \
	 -g -O0 -DDEBUG -DGPCS4_DEBUG -DGPCS4_LINUX
TARGET = GPCS4.elf
LDFLAGS = \
	rtaudio.dir/RtAudio.cpp.o \
	rtaudio.dir/rtaudio_c.cpp.o \
	$(shell pkg-config --libs vulkan) \
	$(shell pkg-config --libs alsa) \
	$(shell pkg-config --libs glfw3) \
	-lpthread

CC = clang++
LD = clang++

INCDIR = \
	../3rdParty \
	../GPCS4	\
	../GPCS4/Algorithm	\
	../GPCS4/Common	\
	../GPCS4/Debug	\
	../GPCS4/Debug/GPCS4.tlog	\
	../GPCS4/Emulator	\
	../GPCS4/Graphic	\
	../GPCS4/Graphic/Gnm	\
	../GPCS4/Graphic/Gve	\
	../GPCS4/Graphic/Pssl	\
	../GPCS4/Graphic/Pssl/GCNParser	\
	../GPCS4/Graphic/Sce	\
	../GPCS4/Graphic/SpirV	\
	../GPCS4/Loader	\
	../GPCS4/Platform	\
	../GPCS4/SceModules	\
	../GPCS4/SceModules/SceHttp	\
	../GPCS4/SceModules/SceNpManager	\
	../GPCS4/SceModules/SceAjm	\
	../GPCS4/SceModules/SceAppContentUtil	\
	../GPCS4/SceModules/SceAudio3d	\
	../GPCS4/SceModules/SceAudioOut	\
	../GPCS4/SceModules/SceCommonDialog	\
	../GPCS4/SceModules/SceErrorDialog	\
	../GPCS4/SceModules/SceFiber	\
	../GPCS4/SceModules/SceFios2	\
	../GPCS4/SceModules/SceGameLiveStreaming	\
	../GPCS4/SceModules/SceGnmDriver	\
	../GPCS4/SceModules/SceIme	\
	../GPCS4/SceModules/SceInvitationDialog	\
	../GPCS4/SceModules/SceJobManager	\
	../GPCS4/SceModules/SceJson	\
	../GPCS4/SceModules/SceLibc	\
	../GPCS4/SceModules/SceLibkernel	\
	../GPCS4/SceModules/SceMouse	\
	../GPCS4/SceModules/SceMsgDialog	\
	../GPCS4/SceModules/SceNet	\
	../GPCS4/SceModules/SceNetCtl	\
	../GPCS4/SceModules/SceNpCommerce	\
	../GPCS4/SceModules/SceNpCommon	\
	../GPCS4/SceModules/SceNpMatching2	\
	../GPCS4/SceModules/SceNpProfileDialog	\
	../GPCS4/SceModules/SceNpScore	\
	../GPCS4/SceModules/SceNpSignaling	\
	../GPCS4/SceModules/SceNpTrophy	\
	../GPCS4/SceModules/SceNpTus	\
	../GPCS4/SceModules/SceNpWebApi	\
	../GPCS4/SceModules/ScePad	\
	../GPCS4/SceModules/ScePlayGo	\
	../GPCS4/SceModules/ScePlayGoDialog	\
	../GPCS4/SceModules/SceRtc	\
	../GPCS4/SceModules/SceRudp	\
	../GPCS4/SceModules/SceSaveData	\
	../GPCS4/SceModules/SceSaveDataDialog	\
	../GPCS4/SceModules/SceScreenShot	\
	../GPCS4/SceModules/SceSsl	\
	../GPCS4/SceModules/SceSysmodule	\
	../GPCS4/SceModules/SceSystemService	\
	../GPCS4/SceModules/SceUserService	\
	../GPCS4/SceModules/SceVideoOut	\
	../GPCS4/SceModules/SceVideoRecording	\
	../GPCS4/Util	\

SOURCE = \
	../GPCS4/SceModules/SceHttp/sce_http.cpp	\
	../GPCS4/SceModules/SceHttp/sce_http_export.cpp	\
	../GPCS4/SceModules/SceNpManager/sce_npmanager.cpp	\
	../GPCS4/SceModules/SceNpManager/sce_npmanager_export.cpp	\
	../GPCS4/SceModules/SceAjm/sce_ajm.cpp	\
	../GPCS4/SceModules/SceAjm/sce_ajm_export.cpp	\
	../GPCS4/SceModules/SceAppContentUtil/sce_appcontentutil.cpp	\
	../GPCS4/SceModules/SceAppContentUtil/sce_appcontentutil_export.cpp	\
	../GPCS4/SceModules/SceAudio3d/sce_audio3d.cpp	\
	../GPCS4/SceModules/SceAudio3d/sce_audio3d_export.cpp	\
	../GPCS4/SceModules/SceAudioOut/AudioOut.cpp	\
	../GPCS4/SceModules/SceAudioOut/sce_audioout.cpp	\
	../GPCS4/SceModules/SceAudioOut/sce_audioout_export.cpp	\
	../GPCS4/SceModules/SceCommonDialog/sce_commondialog.cpp	\
	../GPCS4/SceModules/SceCommonDialog/sce_commondialog_export.cpp	\
	../GPCS4/SceModules/SceErrorDialog/sce_errordialog.cpp	\
	../GPCS4/SceModules/SceErrorDialog/sce_errordialog_export.cpp	\
	../GPCS4/SceModules/SceFiber/sce_fiber.cpp	\
	../GPCS4/SceModules/SceFiber/sce_fiber_export.cpp	\
	../GPCS4/SceModules/SceFios2/sce_fios2.cpp	\
	../GPCS4/SceModules/SceFios2/sce_fios2_export.cpp	\
	../GPCS4/SceModules/SceGameLiveStreaming/sce_gamelivestreaming.cpp	\
	../GPCS4/SceModules/SceGameLiveStreaming/sce_gamelivestreaming_export.cpp	\
	../GPCS4/SceModules/SceGnmDriver/sce_gnmdriver.cpp	\
	../GPCS4/SceModules/SceGnmDriver/sce_gnmdriver_export.cpp	\
	../GPCS4/SceModules/SceGnmDriver/sce_gnm_cmd.cpp	\
	../GPCS4/SceModules/SceGnmDriver/sce_gnm_constant.cpp	\
	../GPCS4/SceModules/SceGnmDriver/sce_gnm_dispatch.cpp	\
	../GPCS4/SceModules/SceGnmDriver/sce_gnm_draw.cpp	\
	../GPCS4/SceModules/SceGnmDriver/sce_gnm_eqevent.cpp	\
	../GPCS4/SceModules/SceGnmDriver/sce_gnm_submit.cpp	\
	../GPCS4/SceModules/SceIme/sce_ime.cpp	\
	../GPCS4/SceModules/SceIme/sce_ime_export.cpp	\
	../GPCS4/SceModules/SceInvitationDialog/sce_invitationdialog.cpp	\
	../GPCS4/SceModules/SceInvitationDialog/sce_invitationdialog_export.cpp	\
	../GPCS4/SceModules/SceJobManager/sce_jobmanager.cpp	\
	../GPCS4/SceModules/SceJobManager/sce_jobmanager_export.cpp	\
	../GPCS4/SceModules/SceJson/sce_json.cpp	\
	../GPCS4/SceModules/SceJson/sce_json_export.cpp	\
	../GPCS4/SceModules/SceLibc/sce_libc.cpp	\
	../GPCS4/SceModules/SceLibc/sce_libc_cxa.cpp	\
	../GPCS4/SceModules/SceLibc/sce_libc_export.cpp	\
	../GPCS4/SceModules/SceLibc/sce_libc_file.cpp	\
	../GPCS4/SceModules/SceLibc/sce_libc_math.cpp	\
	../GPCS4/SceModules/SceLibc/sce_libc_memory.cpp	\
	../GPCS4/SceModules/SceLibc/sce_libc_sce.cpp	\
	../GPCS4/SceModules/SceLibc/sce_libc_stdio.cpp	\
	../GPCS4/SceModules/SceLibc/sce_libc_stdlib.cpp	\
	../GPCS4/SceModules/SceLibc/sce_libc_string.cpp	\
	../GPCS4/SceModules/SceLibkernel/SceEventFlag.cpp	\
	../GPCS4/SceModules/SceLibkernel/SceSemaphore.cpp	\
	../GPCS4/SceModules/SceLibkernel/sce_kernel_eventflag.cpp	\
	../GPCS4/SceModules/SceLibkernel/sce_kernel_eventqueue.cpp	\
	../GPCS4/SceModules/SceLibkernel/sce_kernel_file_stub.cpp	\
	../GPCS4/SceModules/SceLibkernel/sce_kernel_memory.cpp	\
	../GPCS4/SceModules/SceLibkernel/sce_kernel_pthread.cpp	\
	../GPCS4/SceModules/SceLibkernel/sce_kernel_scepthread.cpp	\
	../GPCS4/SceModules/SceLibkernel/sce_kernel_semaphore.cpp	\
	../GPCS4/SceModules/SceLibkernel/sce_kernel_time.cpp	\
	../GPCS4/SceModules/SceLibkernel/sce_kernel_tls.cpp	\
	../GPCS4/SceModules/SceLibkernel/sce_libkernel_export.cpp	\
	../GPCS4/SceModules/SceMouse/sce_mouse.cpp	\
	../GPCS4/SceModules/SceMouse/sce_mouse_export.cpp	\
	../GPCS4/SceModules/SceMsgDialog/sce_msgdialog.cpp	\
	../GPCS4/SceModules/SceMsgDialog/sce_msgdialog_export.cpp	\
	../GPCS4/SceModules/SceNet/sce_net.cpp	\
	../GPCS4/SceModules/SceNet/sce_net_export.cpp	\
	../GPCS4/SceModules/SceNetCtl/sce_netctl.cpp	\
	../GPCS4/SceModules/SceNetCtl/sce_netctl_export.cpp	\
	../GPCS4/SceModules/SceNpCommerce/sce_npcommerce.cpp	\
	../GPCS4/SceModules/SceNpCommerce/sce_npcommerce_export.cpp	\
	../GPCS4/SceModules/SceNpCommon/sce_npcommon.cpp	\
	../GPCS4/SceModules/SceNpCommon/sce_npcommon_export.cpp	\
	../GPCS4/SceModules/SceNpMatching2/sce_npmatching2.cpp	\
	../GPCS4/SceModules/SceNpMatching2/sce_npmatching2_export.cpp	\
	../GPCS4/SceModules/SceNpProfileDialog/sce_npprofiledialog.cpp	\
	../GPCS4/SceModules/SceNpProfileDialog/sce_npprofiledialog_export.cpp	\
	../GPCS4/SceModules/SceNpScore/sce_npscore.cpp	\
	../GPCS4/SceModules/SceNpScore/sce_npscore_export.cpp	\
	../GPCS4/SceModules/SceNpSignaling/sce_npsignaling.cpp	\
	../GPCS4/SceModules/SceNpSignaling/sce_npsignaling_export.cpp	\
	../GPCS4/SceModules/SceNpTrophy/sce_nptrophy.cpp	\
	../GPCS4/SceModules/SceNpTrophy/sce_nptrophy_export.cpp	\
	../GPCS4/SceModules/SceNpTus/sce_nptus.cpp	\
	../GPCS4/SceModules/SceNpTus/sce_nptus_export.cpp	\
	../GPCS4/SceModules/SceNpWebApi/sce_npwebapi.cpp	\
	../GPCS4/SceModules/SceNpWebApi/sce_npwebapi_export.cpp	\
	../GPCS4/SceModules/ScePad/ScePad.cpp	\
	../GPCS4/SceModules/ScePad/sce_pad.cpp	\
	../GPCS4/SceModules/ScePad/sce_pad_export.cpp	\
	../GPCS4/SceModules/ScePlayGo/sce_playgo.cpp	\
	../GPCS4/SceModules/ScePlayGo/sce_playgo_export.cpp	\
	../GPCS4/SceModules/ScePlayGoDialog/sce_playgodialog.cpp	\
	../GPCS4/SceModules/ScePlayGoDialog/sce_playgodialog_export.cpp	\
	../GPCS4/SceModules/SceRtc/sce_rtc.cpp	\
	../GPCS4/SceModules/SceRtc/sce_rtc_export.cpp	\
	../GPCS4/SceModules/SceRudp/sce_rudp.cpp	\
	../GPCS4/SceModules/SceRudp/sce_rudp_export.cpp	\
	../GPCS4/SceModules/SceSaveData/sce_savedata.cpp	\
	../GPCS4/SceModules/SceSaveData/sce_savedata_export.cpp	\
	../GPCS4/SceModules/SceSaveDataDialog/sce_savedatadialog.cpp	\
	../GPCS4/SceModules/SceSaveDataDialog/sce_savedatadialog_export.cpp	\
	../GPCS4/SceModules/SceScreenShot/sce_screenshot.cpp	\
	../GPCS4/SceModules/SceScreenShot/sce_screenshot_export.cpp	\
	../GPCS4/SceModules/SceSsl/sce_ssl.cpp	\
	../GPCS4/SceModules/SceSsl/sce_ssl_export.cpp	\
	../GPCS4/SceModules/SceSysmodule/sce_sysmodule.cpp	\
	../GPCS4/SceModules/SceSysmodule/sce_sysmodule_export.cpp	\
	../GPCS4/SceModules/SceSystemService/sce_systemservice.cpp	\
	../GPCS4/SceModules/SceSystemService/sce_systemservice_export.cpp	\
	../GPCS4/SceModules/SceUserService/sce_userservice.cpp	\
	../GPCS4/SceModules/SceUserService/sce_userservice_export.cpp	\
	../GPCS4/SceModules/SceVideoOut/sce_videoout.cpp	\
	../GPCS4/SceModules/SceVideoOut/sce_videoout_export.cpp	\
	../GPCS4/SceModules/SceVideoRecording/sce_videorecording.cpp	\
	../GPCS4/SceModules/SceVideoRecording/sce_videorecording_export.cpp	\
	../GPCS4/Algorithm/MurmurHash2.cpp	\
	../GPCS4/Algorithm/Sha1Hash.cpp	\
	../GPCS4/Common/GPCS4Log.cpp	\
	../GPCS4/Emulator/Emulator.cpp	\
	../GPCS4/Emulator/GameThread.cpp	\
	../GPCS4/Emulator/Linker.cpp	\
	../GPCS4/Emulator/Module.cpp	\
	../GPCS4/Emulator/ModuleManger.cpp	\
	../GPCS4/Emulator/PolicyManager.cpp	\
	../GPCS4/Emulator/RegisterModules.cpp	\
	../GPCS4/Emulator/SceModuleSystem.cpp	\
	../GPCS4/Emulator/SymbolManager.cpp	\
	../GPCS4/Emulator/TLSHandler.cpp	\
	../GPCS4/GPCS4Main.cpp	\
	../GPCS4/Graphic/Gnm/GnmCmdStream.cpp	\
	../GPCS4/Graphic/Gnm/GnmCommandBuffer.cpp	\
	../GPCS4/Graphic/Gnm/GnmCommandBufferDispatch.cpp	\
	../GPCS4/Graphic/Gnm/GnmCommandBufferDraw.cpp	\
	../GPCS4/Graphic/Gnm/GnmCommandBufferDummy.cpp	\
	../GPCS4/Graphic/Gnm/GnmConvertor.cpp	\
	../GPCS4/Graphic/Gnm/GnmDataFormat.cpp	\
	../GPCS4/Graphic/Gnm/GnmGpuAddress.cpp	\
	../GPCS4/Graphic/Gnm/GnmGpuAddressTool.cpp	\
	../GPCS4/Graphic/Gnm/GnmOpCode.cpp	\
	../GPCS4/Graphic/Gnm/GnmTiler.cpp	\
	../GPCS4/Graphic/GraphicShared.cpp	\
	../GPCS4/Graphic/Gve/GveBuffer.cpp	\
	../GPCS4/Graphic/Gve/GveCmdList.cpp	\
	../GPCS4/Graphic/Gve/GveContext.cpp	\
	../GPCS4/Graphic/Gve/GveDebugMarker.cpp	\
	../GPCS4/Graphic/Gve/GveDescriptor.cpp	\
	../GPCS4/Graphic/Gve/GveDevice.cpp	\
	../GPCS4/Graphic/Gve/GveExtension.cpp	\
	../GPCS4/Graphic/Gve/GveFrameBuffer.cpp	\
	../GPCS4/Graphic/Gve/GveGpuResource.cpp	\
	../GPCS4/Graphic/Gve/GveGraphicsPipeline.cpp	\
	../GPCS4/Graphic/Gve/GveImage.cpp	\
	../GPCS4/Graphic/Gve/GveInstance.cpp	\
	../GPCS4/Graphic/Gve/GveLifetime.cpp	\
	../GPCS4/Graphic/Gve/GveMemory.cpp	\
	../GPCS4/Graphic/Gve/GvePhysicalDevice.cpp	\
	../GPCS4/Graphic/Gve/GvePipelineLayout.cpp	\
	../GPCS4/Graphic/Gve/GvePipelineManager.cpp	\
	../GPCS4/Graphic/Gve/GvePipelineState.cpp	\
	../GPCS4/Graphic/Gve/GvePresenter.cpp	\
	../GPCS4/Graphic/Gve/GveRenderPass.cpp	\
	../GPCS4/Graphic/Gve/GveResourceObjects.cpp	\
	../GPCS4/Graphic/Gve/GveSampler.cpp	\
	../GPCS4/Graphic/Gve/GveShader.cpp	\
	../GPCS4/Graphic/Gve/GveSharpResourceManager.cpp	\
	../GPCS4/Graphic/Gve/GveStaging.cpp	\
	../GPCS4/Graphic/Gve/GveSwapChain.cpp	\
	../GPCS4/Graphic/Gve/GveUtil.cpp	\
	../GPCS4/Graphic/Pssl/GCNCompilerScalarALU.cpp	\
	../GPCS4/Graphic/Pssl/GCNAnalyzer.cpp	\
	../GPCS4/Graphic/Pssl/GCNCompiler.cpp	\
	../GPCS4/Graphic/Pssl/GCNCompilerDataShare.cpp	\
	../GPCS4/Graphic/Pssl/GCNCompilerDebugProfile.cpp	\
	../GPCS4/Graphic/Pssl/GCNCompilerExport.cpp	\
	../GPCS4/Graphic/Pssl/GCNCompilerFlowControl.cpp	\
	../GPCS4/Graphic/Pssl/GCNCompilerScalarMemory.cpp	\
	../GPCS4/Graphic/Pssl/GCNCompilerVectorALU.cpp	\
	../GPCS4/Graphic/Pssl/GCNCompilerVectorInterpolation.cpp	\
	../GPCS4/Graphic/Pssl/GCNCompilerVectorMemory.cpp	\
	../GPCS4/Graphic/Pssl/GCNDecoder.cpp	\
	../GPCS4/Graphic/Pssl/GCNInstructionIterator.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/GCNInstructionDefs.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/GCNParser.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/Instruction.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/ParserSI.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/ParserSIDS.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/ParserSIEXP.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/ParserSIMIMG.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/ParserSIMTBUF.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/ParserSIMUBUF.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/ParserSISMRD.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/ParserSISOP1.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/ParserSISOP2.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/ParserSISOPC.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/ParserSISOPK.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/ParserSISOPP.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/ParserSIVINTRP.cpp	\
	../GPCS4/Graphic/Pssl/GCNParser/ParserSIVOP.cpp	\
	../GPCS4/Graphic/Pssl/GCNStateRegister.cpp	\
	../GPCS4/Graphic/Pssl/PsslFetchShader.cpp	\
	../GPCS4/Graphic/Pssl/PsslKey.cpp	\
	../GPCS4/Graphic/Pssl/PsslProgramInfo.cpp	\
	../GPCS4/Graphic/Pssl/PsslPsUsageTable.cpp	\
	../GPCS4/Graphic/Pssl/PsslSbReader.cpp	\
	../GPCS4/Graphic/Pssl/PsslShaderModule.cpp	\
	../GPCS4/Graphic/Sce/SceGnmDriver.cpp	\
	../GPCS4/Graphic/Sce/SceVideoOut.cpp	\
	../GPCS4/Graphic/SpirV/SpirvCodeBuffer.cpp	\
	../GPCS4/Graphic/SpirV/SpirvCompression.cpp	\
	../GPCS4/Graphic/SpirV/SpirvModule.cpp	\
	../GPCS4/ImportLibs.cpp	\
	../GPCS4/Loader/EbootObject.cpp	\
	../GPCS4/Loader/ELFMapper.cpp	\
	../GPCS4/Loader/FuncStub.cpp	\
	../GPCS4/Loader/ModuleLoader.cpp	\
	../GPCS4/Platform/UtilDebug.cpp	\
	../GPCS4/Platform/UtilFile.cpp	\
	../GPCS4/Platform/UtilHardware.cpp	\
	../GPCS4/Platform/UtilMemory.cpp	\
	../GPCS4/Platform/UtilPath.cpp	\
	../GPCS4/Platform/UtilProcess.cpp	\
	../GPCS4/Platform/UtilString.cpp	\
	../GPCS4/Platform/UtilThread.cpp	\
	../GPCS4/Platform/UtilTime.cpp	\
#	../GPCS4/SceModules/SceLibkernel/sce_pthread_common.cpp	\
#	../GPCS4/SceModules/SceLibkernel/sce_libkernel.cpp	\
#	The pthread4w is DIFFERENT from linux, okay...
	
INCARG = $(patsubst %, -I%, $(INCDIR))
OBJDIR = $(patsubst ../%, obj/%, $(INCDIR))
OBJECT = $(patsubst ../%.cpp, obj/%.o, $(SOURCE))

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(LD) -o $@ $^ $(LDFLAGS)

obj/%.o: ../%.cpp $(OBJDIR)
	$(CC) $(CFLAGS) $(INCARG) -c -o $@ $<

$(OBJDIR):
	-mkdir -p $@

.PHONY: all objdir

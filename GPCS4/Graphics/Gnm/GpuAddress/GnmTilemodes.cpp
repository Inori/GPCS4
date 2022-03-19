#include "GnmGpuAddress.h"

using namespace sce;
using namespace sce::GpuAddress;

#include "GnmGpuAddressInternal.h"
#include "GnmRegsinfo.h"
#include "GnmRegsinfoPrivate.h"

LOG_CHANNEL("GpuAddress");

typedef union TileModeReg
{
	struct
	{
		uint32_t reserved0        : 2;
		uint32_t arrayMode        : 4; ///< Gnm::ArrayMode
		uint32_t pipeCfg          : 5; ///< Gnm::PipeConfig
		uint32_t tileSplit        : 3; ///< Gnm::TileSplit
		uint32_t reserved1        : 8;
		uint32_t microTileModeNew : 3; ///< Gnm::MicroTileMode
		uint32_t sampleSplit      : 2; ///< Gnm::SampleSplit
		uint32_t altPipeConfig    : 5; ///< NEO ONLY
	} m_bits;
	uint32_t m_reg;
} TileModeReg;

typedef union MacroTileModeReg
{
	struct
	{
		uint32_t bankWidth          : 2; ///< Gnm::BankWidth
		uint32_t bankHeight         : 2; ///< Gnm::BankHeight
		uint32_t macroTileAspect    : 2; ///< Gnm::MacroTileAspect
		uint32_t numBanks           : 2; ///< Gnm::NumBanks
		uint32_t altBankHeight      : 2; ///< NEO ONLY
		uint32_t altMacroTileAspect : 2; ///< NEO ONLY
		uint32_t altNumBanks        : 2; ///< NEO ONLY
		uint32_t reserved0          : 18;
	} m_bits;
	uint32_t m_reg;
} MacroTileModeReg;

static const uint32_t g_tileModeDefaultsAsInts[32] =
{
	0x90800310,	// GB_TILE_MODE0  0x00 kTileModeDepth_2dThin_64	      am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Depth    ss=1
	0x90800B10,	// GB_TILE_MODE1  0x01 kTileModeDepth_2dThin_128   	  am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts= 128  mtm=Depth    ss=1
	0x90801310,	// GB_TILE_MODE2  0x02 kTileModeDepth_2dThin_256   	  am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts= 256  mtm=Depth    ss=1
	0x90801B10,	// GB_TILE_MODE3  0x03 kTileModeDepth_2dThin_512   	  am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts= 512  mtm=Depth    ss=1
	0x90802310,	// GB_TILE_MODE4  0x04 kTileModeDepth_2dThin_1K		  am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=1024  mtm=Depth    ss=1
	0x90800308,	// GB_TILE_MODE5  0x05 kTileModeDepth_1dThin       	  am=1dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Depth    ss=1
	0x90801318,	// GB_TILE_MODE6  0x06 kTileModeDepth_2dThinPrt_256	  am=2dTiledThinPrt   pipe/alt=P8_32x32_16x16/P16  ts= 256  mtm=Depth    ss=1
	0x90802318,	// GB_TILE_MODE7  0x07 kTileModeDepth_2dThinPrt_1K 	  am=2dTiledThinPrt   pipe/alt=P8_32x32_16x16/P16  ts=1024  mtm=Depth    ss=1
	0x90000304,	// GB_TILE_MODE8  0x08 kTileModeDisplay_LinearAligned am=LinearAligned    pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Display  ss=1
	0x90000308,	// GB_TILE_MODE9  0x09 kTileModeDisplay_1dThin        am=1dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Display  ss=1
	0x92000310,	// GB_TILE_MODE10 0x0A kTileModeDisplay_2dThin        am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Display  ss=2
	0x92000294,	// GB_TILE_MODE11 0x0B kTileModeDisplay_ThinPrt       am=TiledThinPrt     pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Display  ss=2
	0x92000318,	// GB_TILE_MODE12 0x0C kTileModeDisplay_2dThinPrt     am=2dTiledThinPrt   pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Display  ss=2
	0x90400308,	// GB_TILE_MODE13 0x0D kTileModeThin_1dThin   		  am=1dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thin     ss=1
	0x92400310,	// GB_TILE_MODE14 0x0E kTileModeThin_2dThin   		  am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thin     ss=2
	0x924002B0,	// GB_TILE_MODE15 0x0F kTileModeThin_3dThin   		  am=3dTiledThin      pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Thin     ss=2
	0x92400294,	// GB_TILE_MODE16 0x10 kTileModeThin_ThinPrt  		  am=TiledThinPrt     pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Thin     ss=2
	0x92400318,	// GB_TILE_MODE17 0x11 kTileModeThin_2dThinPrt		  am=2dTiledThinPrt   pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thin     ss=2
	0x9240032C,	// GB_TILE_MODE18 0x12 kTileModeThin_3dThinPrt		  am=3dTiledThinPrt   pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thin     ss=2
	0x9100030C,	// GB_TILE_MODE19 0x13 kTileModeThick_1dThick   	  am=1dTiledThick     pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thick    ss=1
	0x9100031C,	// GB_TILE_MODE20 0x14 kTileModeThick_2dThick   	  am=2dTiledThick     pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thick    ss=1
	0x910002B4,	// GB_TILE_MODE21 0x15 kTileModeThick_3dThick   	  am=3dTiledThick     pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Thick    ss=1
	0x910002A4,	// GB_TILE_MODE22 0x16 kTileModeThick_ThickPrt  	  am=TiledThickPrt    pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Thick    ss=1
	0x91000328,	// GB_TILE_MODE23 0x17 kTileModeThick_2dThickPrt	  am=2dTiledThickPrt  pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thick    ss=1
	0x910002BC,	// GB_TILE_MODE24 0x18 kTileModeThick_3dThickPrt	  am=3dTiledThickPrt  pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Thick    ss=1
	0x91000320,	// GB_TILE_MODE25 0x19 kTileModeThick_2dXThick  	  am=2dTiledXThick    pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thick    ss=1
	0x910002B8,	// GB_TILE_MODE26 0x1A kTileModeThick_3dXThick  	  am=3dTiledXThick    pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Thick    ss=1
	0x90C00308,	// GB_TILE_MODE27 0x1B kTileModeRotated_1dThin   	  am=1dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Rotated  ss=1
	0x92C00310,	// GB_TILE_MODE28 0x1C kTileModeRotated_2dThin   	  am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Rotated  ss=2
	0x92C00294,	// GB_TILE_MODE29 0x1D kTileModeRotated_ThinPrt  	  am=TiledThinPrt     pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Rotated  ss=2
	0x92C00318,	// GB_TILE_MODE30 0x1E kTileModeRotated_2dThinPrt	  am=2dTiledThinPrt   pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Rotated  ss=2
	0x00000000, // GB_TILE_MODE31 0x1F kTileModeDisplay_LinearGeneral am=LinearGeneral    pipe/alt=P2            / P2  ts=  64  mtm=Display  ss=1
};


static const uint32_t g_macroTileModeDefaultsAsInts[16] =
{
	0x26E8,	// GB_MACROTILE_MODE0  0x00 kMacroTileMode_1x4_16      bankWidth=1 bankHeight=4 macroTileAspect=4 numBanks=16 altBankHeight=4 altNumBanks= 8 altMacroTileAspect=2
	0x26D4,	// GB_MACROTILE_MODE1  0x01 kMacroTileMode_1x2_16      bankWidth=1 bankHeight=2 macroTileAspect=2 numBanks=16 altBankHeight=4 altNumBanks= 8 altMacroTileAspect=2
	0x21D0,	// GB_MACROTILE_MODE2  0x02 kMacroTileMode_1x1_16      bankWidth=1 bankHeight=1 macroTileAspect=2 numBanks=16 altBankHeight=2 altNumBanks= 8 altMacroTileAspect=1
	0x21D0,	// GB_MACROTILE_MODE3  0x03 kMacroTileMode_1x1_16_dup  bankWidth=1 bankHeight=1 macroTileAspect=2 numBanks=16 altBankHeight=2 altNumBanks= 8 altMacroTileAspect=1
	0x2080,	// GB_MACROTILE_MODE4  0x04 kMacroTileMode_1x1_8       bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 8 altBankHeight=1 altNumBanks= 8 altMacroTileAspect=1
	0x2040,	// GB_MACROTILE_MODE5  0x05 kMacroTileMode_1x1_4       bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 4 altBankHeight=1 altNumBanks= 8 altMacroTileAspect=1
	0x1000,	// GB_MACROTILE_MODE6  0x06 kMacroTileMode_1x1_2       bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 2 altBankHeight=1 altNumBanks= 4 altMacroTileAspect=1
	0x0000,	// GB_MACROTILE_MODE7  0x07 kMacroTileMode_1x1_2_dup   bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 2 altBankHeight=1 altNumBanks= 2 altMacroTileAspect=1
	0x36EC,	// GB_MACROTILE_MODE8  0x08 kMacroTileMode_1x8_16      bankWidth=1 bankHeight=8 macroTileAspect=4 numBanks=16 altBankHeight=4 altNumBanks=16 altMacroTileAspect=2
	0x26E8,	// GB_MACROTILE_MODE9  0x09 kMacroTileMode_1x4_16_dup  bankWidth=1 bankHeight=4 macroTileAspect=4 numBanks=16 altBankHeight=4 altNumBanks= 8 altMacroTileAspect=2
	0x21D4,	// GB_MACROTILE_MODE10 0x0A kMacroTileMode_1x2_16_dup  bankWidth=1 bankHeight=2 macroTileAspect=2 numBanks=16 altBankHeight=2 altNumBanks= 8 altMacroTileAspect=1
	0x20D0,	// GB_MACROTILE_MODE11 0x0B kMacroTileMode_1x1_16_dup2 bankWidth=1 bankHeight=1 macroTileAspect=2 numBanks=16 altBankHeight=1 altNumBanks= 8 altMacroTileAspect=1
	0x1080,	// GB_MACROTILE_MODE12 0x0C kMacroTileMode_1x1_8_dup   bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 8 altBankHeight=1 altNumBanks= 4 altMacroTileAspect=1
	0x1040,	// GB_MACROTILE_MODE13 0x0D kMacroTileMode_1x1_4_dup   bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 4 altBankHeight=1 altNumBanks= 4 altMacroTileAspect=1
	0x0000,	// GB_MACROTILE_MODE14 0x0E kMacroTileMode_1x1_2_dup2  bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 2 altBankHeight=1 altNumBanks= 2 altMacroTileAspect=1
	0x0000,	// GB_MACROTILE_MODE15 0x0F kMacroTileMode_1x1_2_dup3  bankWidth=1 bankHeight=1 macroTileAspect=1 numBanks= 2 altBankHeight=1 altNumBanks= 2 altMacroTileAspect=1
};

int32_t sce::GpuAddress::getMicroTileMode(Gnm::MicroTileMode *outMicroTileMode, Gnm::TileMode tileMode)
{
	SCE_GNM_ASSERT_MSG_RETURN(outMicroTileMode, kStatusInvalidArgument, "outMicroTileMode must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN((uint32_t)tileMode < 0x20, kStatusInvalidArgument, "tileMode (0x%02X) does not correspond to a valid TileMode enum entry.", tileMode);
	*outMicroTileMode = (Gnm::MicroTileMode)SCE_GNM_GET_FIELD(g_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, MICRO_TILE_MODE_NEW);
	return kStatusSuccess;
}
int32_t sce::GpuAddress::getArrayMode(Gnm::ArrayMode *outArrayMode, Gnm::TileMode tileMode)
{
	SCE_GNM_ASSERT_MSG_RETURN(outArrayMode, kStatusInvalidArgument, "outArrayMode must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN((uint32_t)tileMode < 0x20, kStatusInvalidArgument, "tileMode (0x%02X) does not correspond to a valid TileMode enum entry.", tileMode);
	*outArrayMode = (Gnm::ArrayMode)SCE_GNM_GET_FIELD(g_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, ARRAY_MODE);
	return kStatusSuccess;
}
int32_t sce::GpuAddress::getPipeConfig(Gnm::PipeConfig *outPipeConfig, Gnm::TileMode tileMode)
{
	SCE_GNM_ASSERT_MSG_RETURN(outPipeConfig, kStatusInvalidArgument, "outPipeCongig must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN((uint32_t)tileMode < 0x20, kStatusInvalidArgument, "tileMode (0x%02X) does not correspond to a valid TileMode enum entry.", tileMode);
	*outPipeConfig = (Gnm::PipeConfig)SCE_GNM_GET_FIELD(g_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, PIPE_CONFIG);
	return kStatusSuccess;
}
int32_t sce::GpuAddress::getTileSplit(Gnm::TileSplit *outTileSplit, Gnm::TileMode tileMode)
{
	SCE_GNM_ASSERT_MSG_RETURN(outTileSplit, kStatusInvalidArgument, "outTileSplit must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN((uint32_t)tileMode < 0x20, kStatusInvalidArgument, "tileMode (0x%02X) does not correspond to a valid TileMode enum entry.", tileMode);
	*outTileSplit = (Gnm::TileSplit)SCE_GNM_GET_FIELD(g_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, TILE_SPLIT);
	return kStatusSuccess;
}

int32_t sce::GpuAddress::getSampleSplit(Gnm::SampleSplit *outSampleSplit, Gnm::TileMode tileMode)
{
	SCE_GNM_ASSERT_MSG_RETURN(outSampleSplit, kStatusInvalidArgument, "outSampleSplit must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN((uint32_t)tileMode < 0x20, kStatusInvalidArgument, "tileMode (0x%02X) does not correspond to a valid TileMode enum entry.", tileMode);
	*outSampleSplit = (Gnm::SampleSplit)SCE_GNM_GET_FIELD(g_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, SAMPLE_SPLIT);
	return kStatusSuccess;
}
int32_t sce::GpuAddress::getBankWidth(Gnm::BankWidth *outBankWidth, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel)
{
	SCE_GNM_ASSERT_MSG_RETURN(outBankWidth, kStatusInvalidArgument, "outBankWidth must not be NULL.");
	Gnm::MacroTileMode macroTileMode;
	int32_t status = computeSurfaceMacroTileMode(&macroTileMode, tileMode, bitsPerElement, numFragmentsPerPixel);
	if (status == kStatusSuccess)
		*outBankWidth = (Gnm::BankWidth)SCE_GNM_GET_FIELD(g_macroTileModeDefaultsAsInts[macroTileMode], GB_MACROTILE_MODE0, BANK_WIDTH);
	return status;
}
int32_t sce::GpuAddress::getBankHeight(Gnm::BankHeight *outBankHeight, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel)
{
	SCE_GNM_ASSERT_MSG_RETURN(outBankHeight, kStatusInvalidArgument, "outBankHeight must not be NULL.");
	Gnm::MacroTileMode macroTileMode;
	int32_t status = computeSurfaceMacroTileMode(&macroTileMode, tileMode, bitsPerElement, numFragmentsPerPixel);
	if (status == kStatusSuccess)
		*outBankHeight = (Gnm::BankHeight)SCE_GNM_GET_FIELD(g_macroTileModeDefaultsAsInts[macroTileMode], GB_MACROTILE_MODE0, BANK_HEIGHT);
	return status;
}
int32_t sce::GpuAddress::getMacroTileAspect(Gnm::MacroTileAspect *outMacroTileAspect, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel)
{
	SCE_GNM_ASSERT_MSG_RETURN(outMacroTileAspect, kStatusInvalidArgument, "outMacroTileAspect must not be NULL.");
	Gnm::MacroTileMode macroTileMode;
	int32_t status = computeSurfaceMacroTileMode(&macroTileMode, tileMode, bitsPerElement, numFragmentsPerPixel);
	if (status == kStatusSuccess)
		*outMacroTileAspect = (Gnm::MacroTileAspect)SCE_GNM_GET_FIELD(g_macroTileModeDefaultsAsInts[macroTileMode], GB_MACROTILE_MODE0, MACRO_TILE_ASPECT);
	return status;
}
int32_t sce::GpuAddress::getNumBanks(Gnm::NumBanks *outNumBanks, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel)
{
	SCE_GNM_ASSERT_MSG_RETURN(outNumBanks, kStatusInvalidArgument, "outNumBanks must not be NULL.");
	Gnm::MacroTileMode macroTileMode;
	int32_t status = computeSurfaceMacroTileMode(&macroTileMode, tileMode, bitsPerElement, numFragmentsPerPixel);
	if (status == kStatusSuccess)
		*outNumBanks = (Gnm::NumBanks)SCE_GNM_GET_FIELD(g_macroTileModeDefaultsAsInts[macroTileMode], GB_MACROTILE_MODE0, NUM_BANKS);
	return status;
}
int32_t sce::GpuAddress::getAllMacroTileData(Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel,
	Gnm::BankWidth *outBankWidth, Gnm::BankHeight *outBankHeight, Gnm::MacroTileAspect *outMacroTileAspect,
	Gnm::NumBanks *outNumBanks)
{
	Gnm::MacroTileMode macroTileMode;
	int32_t status = computeSurfaceMacroTileMode(&macroTileMode, tileMode, bitsPerElement, numFragmentsPerPixel);
	if (status == kStatusSuccess)
	{
		uint32_t mtmReg = g_macroTileModeDefaultsAsInts[macroTileMode];
		if (outBankWidth)
			*outBankWidth = (Gnm::BankWidth)SCE_GNM_GET_FIELD(mtmReg, GB_MACROTILE_MODE0, BANK_WIDTH);
		if (outBankHeight)
			*outBankHeight = (Gnm::BankHeight)SCE_GNM_GET_FIELD(mtmReg, GB_MACROTILE_MODE0, BANK_HEIGHT);
		if (outMacroTileAspect)
			*outMacroTileAspect = (Gnm::MacroTileAspect)SCE_GNM_GET_FIELD(mtmReg, GB_MACROTILE_MODE0, MACRO_TILE_ASPECT);
		if (outNumBanks)
			*outNumBanks = (Gnm::NumBanks)SCE_GNM_GET_FIELD(mtmReg, GB_MACROTILE_MODE0, NUM_BANKS);
	}
	return status;
}

int32_t sce::GpuAddress::getAltPipeConfig(Gnm::PipeConfig *outAltPipeConfig, Gnm::TileMode tileMode)
{
	SCE_GNM_ASSERT_MSG_RETURN(outAltPipeConfig, kStatusInvalidArgument, "outPipeCongig must not be NULL.");
	SCE_GNM_ASSERT_MSG_RETURN((uint32_t)tileMode < 0x20, kStatusInvalidArgument, "tileMode (0x%02X) does not correspond to a valid TileMode enum entry.", tileMode);
	*outAltPipeConfig = (Gnm::PipeConfig)SCE_GNM_GET_FIELD(g_tileModeDefaultsAsInts[tileMode], GB_TILE_MODE0, ALT_PIPE_CONFIG);
	return kStatusSuccess;
}
int32_t sce::GpuAddress::getAltBankHeight(Gnm::BankHeight *outAltBankHeight, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel)
{
	SCE_GNM_ASSERT_MSG_RETURN(outAltBankHeight, kStatusInvalidArgument, "outBankHeight must not be NULL.");
	Gnm::MacroTileMode macroTileMode;
	int32_t status = computeSurfaceMacroTileMode(&macroTileMode, tileMode, bitsPerElement, numFragmentsPerPixel);
	if (status == kStatusSuccess)
		*outAltBankHeight = (Gnm::BankHeight)SCE_GNM_GET_FIELD(g_macroTileModeDefaultsAsInts[macroTileMode], GB_MACROTILE_MODE0, ALT_BANK_HEIGHT);
	return status;
}
int32_t sce::GpuAddress::getAltMacroTileAspect(Gnm::MacroTileAspect *outMacroTileAspect, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel)
{
	SCE_GNM_ASSERT_MSG_RETURN(outMacroTileAspect, kStatusInvalidArgument, "outMacroTileAspect must not be NULL.");
	Gnm::MacroTileMode macroTileMode;
	int32_t status = computeSurfaceMacroTileMode(&macroTileMode, tileMode, bitsPerElement, numFragmentsPerPixel);
	if (status == kStatusSuccess)
		*outMacroTileAspect = (Gnm::MacroTileAspect)SCE_GNM_GET_FIELD(g_macroTileModeDefaultsAsInts[macroTileMode], GB_MACROTILE_MODE0, ALT_MACRO_TILE_ASPECT);
	return status;
}
int32_t sce::GpuAddress::getAltNumBanks(Gnm::NumBanks *outAltNumBanks, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel)
{
	SCE_GNM_ASSERT_MSG_RETURN(outAltNumBanks, kStatusInvalidArgument, "outNumBanks must not be NULL.");
	Gnm::MacroTileMode macroTileMode;
	int32_t status = computeSurfaceMacroTileMode(&macroTileMode, tileMode, bitsPerElement, numFragmentsPerPixel);
	if (status == kStatusSuccess)
		*outAltNumBanks = (Gnm::NumBanks)SCE_GNM_GET_FIELD(g_macroTileModeDefaultsAsInts[macroTileMode], GB_MACROTILE_MODE0, ALT_NUM_BANKS);
	return status;
}
int32_t sce::GpuAddress::getAllAltMacroTileData(Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel,
	Gnm::BankWidth *outBankWidth, Gnm::BankHeight *outAltBankHeight, Gnm::MacroTileAspect *outAltMacroTileAspect,
	Gnm::NumBanks *outAltNumBanks)
{
	Gnm::MacroTileMode macroTileMode;
	int32_t status = computeSurfaceMacroTileMode(&macroTileMode, tileMode, bitsPerElement, numFragmentsPerPixel);
	if (status == kStatusSuccess)
	{
		uint32_t mtmReg = g_macroTileModeDefaultsAsInts[macroTileMode];
		if (outBankWidth)
			*outBankWidth = (Gnm::BankWidth)SCE_GNM_GET_FIELD(mtmReg, GB_MACROTILE_MODE0, BANK_WIDTH);
		if (outAltBankHeight)
			*outAltBankHeight = (Gnm::BankHeight)SCE_GNM_GET_FIELD(mtmReg, GB_MACROTILE_MODE0, ALT_BANK_HEIGHT);
		if (outAltMacroTileAspect)
			*outAltMacroTileAspect = (Gnm::MacroTileAspect)SCE_GNM_GET_FIELD(mtmReg, GB_MACROTILE_MODE0, ALT_MACRO_TILE_ASPECT);
		if (outAltNumBanks)
			*outAltNumBanks = (Gnm::NumBanks)SCE_GNM_GET_FIELD(mtmReg, GB_MACROTILE_MODE0, ALT_NUM_BANKS);
	}
	return status;
}

//==============================================================================
// Copyright (c) 2010-2016 Advanced Micro Devices, Inc. All rights reserved.
/// \author AMD Developer Tools Team
/// \file
/// \brief  Device info table
//==============================================================================

#ifndef _DEVICE_INFO_H_
#define _DEVICE_INFO_H_

#ifndef _WIN32
    #include <cstddef>      // Required by GCC size_t
#endif

#define REVISION_ID_ANY 0xFFFFFFFF ///< ignore revision id when looking up device Id

enum GDT_HW_ASIC_TYPE
{
    GDT_ASIC_TYPE_NONE = -1,  ///< undefined asic
    GDT_TAHITI_PRO = 0,       ///< TAHITI GPU PRO
    GDT_TAHITI_XT,            ///< TAHITI GPU XT
    GDT_PITCAIRN_PRO,         ///< PITCAIRN GPU PRO
    GDT_PITCAIRN_XT,          ///< PITCAIRN GPU XT
    GDT_CAPEVERDE_PRO,        ///< CAPE VERDE PRO GPU
    GDT_CAPEVERDE_XT,         ///< CAPE VERDE XT GPU
    GDT_OLAND,                ///< OLAND GPU (mobile is MARS)
    GDT_HAINAN,               ///< HAINAN GPU
    GDT_BONAIRE,              ///< BONAIRE GPU (mobile is SATURN)
    GDT_HAWAII,               ///< HAWAII GPU
    GDT_KALINDI,              ///< KB APU
    GDT_SPECTRE,              ///< KV APU SPECTRE
    GDT_SPECTRE_SL,           ///< KV APU SPECTRE SL
    GDT_SPECTRE_LITE,         ///< KV APU SPECTRE LITE
    GDT_SPOOKY,               ///< KV APU SPOOKY
    GDT_ICELAND,              ///< ICELAND GPU
    GDT_TONGA,                ///< TONGA GPU
    GDT_CARRIZO,              ///< CZ APU
    GDT_CARRIZO_EMB,          ///< CZ APU EMBEDDED
    GDT_FIJI,                 ///< FIJI GPU
    GDT_STONEY,               ///< STONEY APU
    GDT_ELLESMERE,            ///< ELLESMERE GPU
    GDT_BAFFIN,               ///< BAFFIN GPU
    GDT_GFX8_0_4,             ///< GFX8_0_4 GPU
    GDT_VEGAM1,               ///< VegaM GPU
    GDT_VEGAM2,               ///< VegaM GPU
    GDT_GFX9_0_0,             ///< GFX9_0_0 GPU
    GDT_GFX9_0_2,             ///< GFX9_0_2 APU
    GDT_GFX9_0_6,             ///< GFX9_0_6 GPU
    GDT_GFX10_1_0,            ///< GFX10_1_0 GPU
    GDT_GFX10_1_0_XL,         ///< GFX10_1_0_XL GPU
    GDT_ASIC_PLACEHOLDER_1,   ///< Placeholder for future ASIC
    GDT_ASIC_PLACEHOLDER_2,   ///< Placeholder for future ASIC
    GDT_ASIC_PLACEHOLDER_3,   ///< Placeholder for future ASIC
    GDT_ASIC_PLACEHOLDER_4,   ///< Placeholder for future ASIC
    GDT_ASIC_PLACEHOLDER_5,   ///< Placeholder for future ASIC
    GDT_LAST                  ///< last
};


//List of known GCN versions :
//
//CLRX Version	AMD Version			Example devices
//GCN 1.0			GCN 1			Pitcairn(HD 7850), Tahiti(HD 7970), Cape verde
//GCN 1.1			GCN 2			Bonaire(R7 260), Hawaii(R9 290)
//GCN 1.2			GCN 3,GCN 4		Tonga(R9 285), Fiji(Fury X), Ellesmere(RX 480), Baffin
//GCN 1.4			GCN 5			VEGA(GFX900) (RX VEGA 64)
//GCN 1.4.1			GCN 5,VEGA20	VEGA20(GFX906) (VEGA with Deep Learning extensions)
//
//List of architecture names :
//
//CLRX Version		CLRX names
//GCN 1.0			GCN1.0, GFX6, SI
//GCN 1.1			GCN1.1, GFX7, CI
//GCN 1.2			GCN1.2, GFX8, VI
//GCN 1.4			GCN1.4, GFX9, VEGA
//GCN 1.4.1			GCN1.4.1, GFX906, VEGA20

//------------------------------------------------------------------------------------
// This is used for counter set selection
//------------------------------------------------------------------------------------
enum GDT_HW_GENERATION
{
    GDT_HW_GENERATION_NONE,                                           ///< undefined hw generation
    GDT_HW_GENERATION_NVIDIA,                                         ///< nvidia GPU
    GDT_HW_GENERATION_INTEL,                                          ///< intel GPU
    GDT_HW_GENERATION_SOUTHERNISLAND,                                 ///< GFX IP 6
    GDT_HW_GENERATION_FIRST_AMD = GDT_HW_GENERATION_SOUTHERNISLAND,   ///< first AMD generation
    GDT_HW_GENERATION_SEAISLAND,                                      ///< GFX IP 7
    GDT_HW_GENERATION_VOLCANICISLAND,                                 ///< GFX IP 8
    GDT_HW_GENERATION_GFX9,                                           ///< GFX IP 9
    GDT_HW_GENERATION_GFX10,                                          ///< GFX IP 10
    GDT_HW_GENERATION_LAST
};

//------------------------------------------------------------------------------------
// Card info
//------------------------------------------------------------------------------------
struct GDT_GfxCardInfo
{
    GDT_HW_ASIC_TYPE  m_asicType;          ///< ASIC type, which is also the index to gs_deviceInfo table
    size_t            m_deviceID;          ///< numeric device id
    size_t            m_revID;             ///< numeric revision id
    GDT_HW_GENERATION m_generation;        ///< hardware generation
    bool              m_bAPU;              ///< flag indicating whether or not the device is an APU
    const char*       m_szCALName;         ///< CAL name
    const char*       m_szMarketingName;   ///< marketing style device name
};

//------------------------------------------------------------------------------------
// Device info
//------------------------------------------------------------------------------------
struct GDT_DeviceInfo
{
    size_t m_nNumShaderEngines;  ///< Number of shader engines
    size_t m_nMaxWavePerSIMD;    ///< Number of wave slots per SIMD
    size_t m_suClocksPrim;       ///< Number of clocks it takes to process a primitive
    size_t m_nNumSQMaxCounters;  ///< Max number of SQ counters
    size_t m_nNumPrimPipes;      ///< Number of primitive pipes
    size_t m_nWaveSize;          ///< Wavefront size
    size_t m_nNumSHPerSE;        ///< Number of shader array per Shader Engine
    size_t m_nNumCUPerSH;        ///< Number of compute unit per Shader Array
    size_t m_nNumSIMDPerCU;      ///< Number of SIMDs per Compute unit

    bool  m_deviceInfoValid;     ///< Flag indicating whether or not this device info is valid (as opposed to placeholder data)

    /// Get total number of SIMDs
    size_t numberSIMDs() const
    {
        return m_nNumSIMDPerCU * numberCUs();
    }

    /// Get total number of shader arrays
    size_t numberSHs() const
    {
        return m_nNumSHPerSE * m_nNumShaderEngines;
    }

    /// Get total number of compute units
    size_t numberCUs() const
    {
        return numberSHs() * m_nNumCUPerSH;
    }
};

#endif // _DEVICE_INFO_H_

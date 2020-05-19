#include "GnmDataFormatCodec.h"
#include "GnmFloatPoint.h"

#include <algorithm>

LOG_CHANNEL(Graphic.Gnm.GnmDataFormat);

namespace GpuAddress
{;

union Channel 
{
	enum
	{
		kBias = 127
	};

	class
	{
	public:
		uint32_t m : 23; 
		uint32_t e : 8;
		uint32_t s : 1; 
	} b;

	uint32_t u; 
	int32_t  i; 
	float    f; 
};

class SurfaceFormatInfo
{
public:
	SurfaceFormat m_format;
	uint8_t       m_bitsPerChannel[4];
};

#define DEFINE_SURFACEFORMATINFO(S, X, Y, Z, W) \
	{                                           \
		(S),                                    \
		{                                       \
			(X), (Y), (Z), (W)                  \
		}                                       \
	}

static const SurfaceFormatInfo g_surfaceFormatInfo[kSurfaceFormat1Reversed + 1] = 
{
	/* 0x00 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormatInvalid, 0, 0, 0, 0),
	/* 0x01 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat8, 8, 0, 0, 0),
	/* 0x02 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat16, 16, 0, 0, 0),
	/* 0x03 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat8_8, 8, 8, 0, 0),
	/* 0x04 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat32, 32, 0, 0, 0),
	/* 0x05 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat16_16, 16, 16, 0, 0),
	/* 0x06 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat10_11_11, 11, 11, 10, 0),
	/* 0x07 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat11_11_10, 10, 11, 11, 0),
	/* 0x08 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat10_10_10_2, 2, 10, 10, 10),
	/* 0x09 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat2_10_10_10, 10, 10, 10, 2),
	/* 0x0a */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat8_8_8_8, 8, 8, 8, 8),
	/* 0x0b */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat32_32, 32, 32, 0, 0),
	/* 0x0c */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat16_16_16_16, 16, 16, 16, 16),
	/* 0x0d */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat32_32_32, 32, 32, 32, 0),
	/* 0x0e */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat32_32_32_32, 32, 32, 32, 32),
	/* 0x0f */ {},
	/* 0x10 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat5_6_5, 5, 6, 5, 0),
	/* 0x11 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat1_5_5_5, 5, 5, 5, 1),
	/* 0x12 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat5_5_5_1, 1, 5, 5, 5),
	/* 0x13 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat4_4_4_4, 4, 4, 4, 4),
	/* 0x14 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat8_24, 24, 8, 0, 0),
	/* 0x15 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat24_8, 8, 24, 0, 0),
	/* 0x16 */ {},
	/* 0x17 */ {},
	/* 0x18 */ {},
	/* 0x19 */ {},
	/* 0x1a */ {},
	/* 0x1b */ {},
	/* 0x1c */ {},
	/* 0x1d */ {},
	/* 0x1e */ {},
	/* 0x1f */ {},
	/* 0x20 */ {},
	/* 0x21 */ {},
	/* 0x22 */ {},
	/* 0x23 */ {},
	/* 0x24 */ {},
	/* 0x25 */ {},
	/* 0x26 */ {},
	/* 0x27 */ {},
	/* 0x28 */ {},
	/* 0x29 */ {},
	/* 0x2a */ {},
	/* 0x2b */ {},

	/* 0x2c */ {},
	/* 0x2d */ {},
	/* 0x2e */ {},
	/* 0x2f */ {},
	/* 0x30 */ {},
	/* 0x31 */ {},
	/* 0x32 */ {},
	/* 0x33 */ {},
	/* 0x34 */ {},
	/* 0x35 */ {},
	/* 0x36 */ {},
	/* 0x37 */ {},
	/* 0x38 */ {},

	/* 0x39 */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat4_4, 4, 4, 0, 0),
	/* 0x3a */ DEFINE_SURFACEFORMATINFO(kSurfaceFormat6_5_5, 5, 5, 6, 0),
	/* 0x3b */ {},
	/* 0x3c */ {},
};

struct Floats
{
	uint8_t m_signBits;
	uint8_t m_exponentBits;
	uint8_t m_mantissaBits;
};

static const Floats floats[] = 
{
	{},  // 0
	{},  // 1
	{},  // 2
	{},  // 3

	{},  // 4
	{},  // 5
	{},  // 6
	{},  // 7

	{},           // 8
	{},           // 9
	{ 0, 5, 5 },  // 10
	{ 0, 5, 6 },  // 11

	{},  // 12
	{},  // 13
	{},  // 14
	{},  // 15

	{ 1, 5, 10 },  // 16
};

const uint32_t lookup_8_srgb[256] = 
{
	0x00000000,  // 0 0.000000000
	0x399f0000,  // 1 0.000303268
	0x3a1f0000,  // 2 0.000606537
	0x3a6f0000,  // 3 0.000911713
	0x3a9f0000,  // 4 0.001213074
	0x3ac70000,  // 5 0.001518250
	0x3aef0000,  // 6 0.001823425
	0x3b0b0000,  // 7 0.002120972
	0x3b1f0000,  // 8 0.002426147
	0x3b330000,  // 9 0.002731323
	0x3b470000,  // 10 0.003036499
	0x3b5b0000,  // 11 0.003341675
	0x3b710000,  // 12 0.003677368
	0x3b840000,  // 13 0.004028320
	0x3b900000,  // 14 0.004394531
	0x3b9d0000,  // 15 0.004791260
	0x3baa0000,  // 16 0.005187988
	0x3bb80000,  // 17 0.005615234
	0x3bc60000,  // 18 0.006042480
	0x3bd50000,  // 19 0.006500244
	0x3be50000,  // 20 0.006988525
	0x3bf60000,  // 21 0.007507324
	0x3c030000,  // 22 0.007995605
	0x3c0c0000,  // 23 0.008544922
	0x3c160000,  // 24 0.009155273
	0x3c1f0000,  // 25 0.009704590
	0x3c290000,  // 26 0.010314941
	0x3c340000,  // 27 0.010986328
	0x3c3e0000,  // 28 0.011596680
	0x3c490000,  // 29 0.012268066
	0x3c550000,  // 30 0.013000488
	0x3c600000,  // 31 0.013671875
	0x3c6d0000,  // 32 0.014465332
	0x3c790000,  // 33 0.015197754
	0x3c830000,  // 34 0.015991211
	0x3c8a0000,  // 35 0.016845703
	0x3c910000,  // 36 0.017700195
	0x3c980000,  // 37 0.018554688
	0x3c9f0000,  // 38 0.019409180
	0x3ca60000,  // 39 0.020263672
	0x3cae0000,  // 40 0.021240234
	0x3cb60000,  // 41 0.022216797
	0x3cbe0000,  // 42 0.023193359
	0x3cc60000,  // 43 0.024169922
	0x3cce0000,  // 44 0.025146484
	0x3cd70000,  // 45 0.026245117
	0x3ce00000,  // 46 0.027343750
	0x3ce90000,  // 47 0.028442383
	0x3cf20000,  // 48 0.029541016
	0x3cfc0000,  // 49 0.030761719
	0x3d030000,  // 50 0.031982422
	0x3d080000,  // 51 0.033203125
	0x3d0d0000,  // 52 0.034423828
	0x3d120000,  // 53 0.035644531
	0x3d170000,  // 54 0.036865234
	0x3d1c0000,  // 55 0.038085938
	0x3d220000,  // 56 0.039550781
	0x3d280000,  // 57 0.041015625
	0x3d2d0000,  // 58 0.042236328
	0x3d330000,  // 59 0.043701172
	0x3d390000,  // 60 0.045166016
	0x3d3f0000,  // 61 0.046630859
	0x3d450000,  // 62 0.048095703
	0x3d4c0000,  // 63 0.049804688
	0x3d520000,  // 64 0.051269531
	0x3d590000,  // 65 0.052978516
	0x3d5f0000,  // 66 0.054443359
	0x3d660000,  // 67 0.056152344
	0x3d6d0000,  // 68 0.057861328
	0x3d740000,  // 69 0.059570313
	0x3d7b0000,  // 70 0.061279297
	0x3d810000,  // 71 0.062988281
	0x3d850000,  // 72 0.064941406
	0x3d880000,  // 73 0.066406250
	0x3d8c0000,  // 74 0.068359375
	0x3d900000,  // 75 0.070312500
	0x3d940000,  // 76 0.072265625
	0x3d980000,  // 77 0.074218750
	0x3d9c0000,  // 78 0.076171875
	0x3da00000,  // 79 0.078125000
	0x3da40000,  // 80 0.080078125
	0x3da90000,  // 81 0.082519531
	0x3dad0000,  // 82 0.084472656
	0x3db10000,  // 83 0.086425781
	0x3db60000,  // 84 0.088867188
	0x3dba0000,  // 85 0.090820313
	0x3dbf0000,  // 86 0.093261719
	0x3dc30000,  // 87 0.095214844
	0x3dc80000,  // 88 0.097656250
	0x3dcd0000,  // 89 0.100097656
	0x3dd10000,  // 90 0.102050781
	0x3dd60000,  // 91 0.104492188
	0x3ddb0000,  // 92 0.106933594
	0x3de00000,  // 93 0.109375000
	0x3de50000,  // 94 0.111816406
	0x3dea0000,  // 95 0.114257813
	0x3df00000,  // 96 0.117187500
	0x3df50000,  // 97 0.119628906
	0x3dfa0000,  // 98 0.122070313
	0x3e000000,  // 99 0.125000000
	0x3e020000,  // 100 0.126953125
	0x3e050000,  // 101 0.129882813
	0x3e080000,  // 102 0.132812500
	0x3e0b0000,  // 103 0.135742188
	0x3e0e0000,  // 104 0.138671875
	0x3e110000,  // 105 0.141601563
	0x3e140000,  // 106 0.144531250
	0x3e170000,  // 107 0.147460938
	0x3e1a0000,  // 108 0.150390625
	0x3e1d0000,  // 109 0.153320313
	0x3e200000,  // 110 0.156250000
	0x3e230000,  // 111 0.159179688
	0x3e260000,  // 112 0.162109375
	0x3e290000,  // 113 0.165039063
	0x3e2c0000,  // 114 0.167968750
	0x3e300000,  // 115 0.171875000
	0x3e330000,  // 116 0.174804688
	0x3e360000,  // 117 0.177734375
	0x3e3a0000,  // 118 0.181640625
	0x3e3d0000,  // 119 0.184570313
	0x3e400000,  // 120 0.187500000
	0x3e440000,  // 121 0.191406250
	0x3e470000,  // 122 0.194335938
	0x3e4b0000,  // 123 0.198242188
	0x3e4e0000,  // 124 0.201171875
	0x3e520000,  // 125 0.205078125
	0x3e560000,  // 126 0.208984375
	0x3e590000,  // 127 0.211914063
	0x3e5d0000,  // 128 0.215820313
	0x3e610000,  // 129 0.219726563
	0x3e650000,  // 130 0.223632813
	0x3e680000,  // 131 0.226562500
	0x3e6c0000,  // 132 0.230468750
	0x3e700000,  // 133 0.234375000
	0x3e740000,  // 134 0.238281250
	0x3e780000,  // 135 0.242187500
	0x3e7c0000,  // 136 0.246093750
	0x3e800000,  // 137 0.250000000
	0x3e820000,  // 138 0.253906250
	0x3e840000,  // 139 0.257812500
	0x3e860000,  // 140 0.261718750
	0x3e880000,  // 141 0.265625000
	0x3e8a0000,  // 142 0.269531250
	0x3e8d0000,  // 143 0.275390625
	0x3e8f0000,  // 144 0.279296875
	0x3e910000,  // 145 0.283203125
	0x3e930000,  // 146 0.287109375
	0x3e950000,  // 147 0.291015625
	0x3e980000,  // 148 0.296875000
	0x3e9a0000,  // 149 0.300781250
	0x3e9c0000,  // 150 0.304687500
	0x3e9e0000,  // 151 0.308593750
	0x3ea10000,  // 152 0.314453125
	0x3ea30000,  // 153 0.318359375
	0x3ea50000,  // 154 0.322265625
	0x3ea80000,  // 155 0.328125000
	0x3eaa0000,  // 156 0.332031250
	0x3ead0000,  // 157 0.337890625
	0x3eaf0000,  // 158 0.341796875
	0x3eb20000,  // 159 0.347656250
	0x3eb40000,  // 160 0.351562500
	0x3eb60000,  // 161 0.355468750
	0x3eb90000,  // 162 0.361328125
	0x3ebc0000,  // 163 0.367187500
	0x3ebe0000,  // 164 0.371093750
	0x3ec10000,  // 165 0.376953125
	0x3ec30000,  // 166 0.380859375
	0x3ec60000,  // 167 0.386718750
	0x3ec80000,  // 168 0.390625000
	0x3ecb0000,  // 169 0.396484375
	0x3ece0000,  // 170 0.402343750
	0x3ed10000,  // 171 0.408203125
	0x3ed30000,  // 172 0.412109375
	0x3ed60000,  // 173 0.417968750
	0x3ed90000,  // 174 0.423828125
	0x3edb0000,  // 175 0.427734375
	0x3ede0000,  // 176 0.433593750
	0x3ee10000,  // 177 0.439453125
	0x3ee40000,  // 178 0.445312500
	0x3ee70000,  // 179 0.451171875
	0x3eea0000,  // 180 0.457031250
	0x3eed0000,  // 181 0.462890625
	0x3ef00000,  // 182 0.468750000
	0x3ef20000,  // 183 0.472656250
	0x3ef50000,  // 184 0.478515625
	0x3ef80000,  // 185 0.484375000
	0x3efb0000,  // 186 0.490234375
	0x3efe0000,  // 187 0.496093750
	0x3f010000,  // 188 0.503906250
	0x3f020000,  // 189 0.507812500
	0x3f040000,  // 190 0.515625000
	0x3f050000,  // 191 0.519531250
	0x3f070000,  // 192 0.527343750
	0x3f090000,  // 193 0.535156250
	0x3f0a0000,  // 194 0.539062500
	0x3f0c0000,  // 195 0.546875000
	0x3f0d0000,  // 196 0.550781250
	0x3f0f0000,  // 197 0.558593750
	0x3f110000,  // 198 0.566406250
	0x3f120000,  // 199 0.570312500
	0x3f140000,  // 200 0.578125000
	0x3f160000,  // 201 0.585937500
	0x3f170000,  // 202 0.589843750
	0x3f190000,  // 203 0.597656250
	0x3f1b0000,  // 204 0.605468750
	0x3f1c0000,  // 205 0.609375000
	0x3f1e0000,  // 206 0.617187500
	0x3f200000,  // 207 0.625000000
	0x3f210000,  // 208 0.628906250
	0x3f230000,  // 209 0.636718750
	0x3f250000,  // 210 0.644531250
	0x3f270000,  // 211 0.652343750
	0x3f290000,  // 212 0.660156250
	0x3f2a0000,  // 213 0.664062500
	0x3f2c0000,  // 214 0.671875000
	0x3f2e0000,  // 215 0.679687500
	0x3f300000,  // 216 0.687500000
	0x3f320000,  // 217 0.695312500
	0x3f330000,  // 218 0.699218750
	0x3f350000,  // 219 0.707031250
	0x3f370000,  // 220 0.714843750
	0x3f390000,  // 221 0.722656250
	0x3f3b0000,  // 222 0.730468750
	0x3f3d0000,  // 223 0.738281250
	0x3f3f0000,  // 224 0.746093750
	0x3f410000,  // 225 0.753906250
	0x3f430000,  // 226 0.761718750
	0x3f450000,  // 227 0.769531250
	0x3f470000,  // 228 0.777343750
	0x3f490000,  // 229 0.785156250
	0x3f4b0000,  // 230 0.792968750
	0x3f4d0000,  // 231 0.800781250
	0x3f4f0000,  // 232 0.808593750
	0x3f510000,  // 233 0.816406250
	0x3f530000,  // 234 0.824218750
	0x3f550000,  // 235 0.832031250
	0x3f570000,  // 236 0.839843750
	0x3f590000,  // 237 0.847656250
	0x3f5b0000,  // 238 0.855468750
	0x3f5d0000,  // 239 0.863281250
	0x3f5f0000,  // 240 0.871093750
	0x3f610000,  // 241 0.878906250
	0x3f630000,  // 242 0.886718750
	0x3f650000,  // 243 0.894531250
	0x3f680000,  // 244 0.906250000
	0x3f6a0000,  // 245 0.914062500
	0x3f6c0000,  // 246 0.921875000
	0x3f6e0000,  // 247 0.929687500
	0x3f700000,  // 248 0.937500000
	0x3f730000,  // 249 0.949218750
	0x3f750000,  // 250 0.957031250
	0x3f770000,  // 251 0.964843750
	0x3f790000,  // 252 0.972656250
	0x3f7b0000,  // 253 0.980468750
	0x3f7e0000,  // 254 0.992187500
	0x3f800000,  // 255 1.000000000
};

inline bool isSupported(SurfaceFormat surfaceFormat)
{
	switch (surfaceFormat)
	{
	case kSurfaceFormat32:
	case kSurfaceFormat32_32:
	case kSurfaceFormat32_32_32_32:
	case kSurfaceFormatBc1:
	case kSurfaceFormatBc2:
	case kSurfaceFormatBc3:
	case kSurfaceFormatBc4:
	case kSurfaceFormatBc5:
	case kSurfaceFormatBc6:
	case kSurfaceFormatBc7:
	case kSurfaceFormat1:
	case kSurfaceFormat1Reversed:
	case kSurfaceFormat5_9_9_9:
		return false;
	default:
		return true;
	}
}

TextureChannelType getRealChannelType(TextureChannelType purportedChannelType, int count)
{
	if (count >= 6)
		return purportedChannelType;
	switch (purportedChannelType)
	{
	case kTextureChannelTypeUBScaled:
	case kTextureChannelTypeSScaled:
	case kTextureChannelTypeUScaled:
		return kTextureChannelTypeUScaled;
	case kTextureChannelTypeUBInt:
	case kTextureChannelTypeSInt:
	case kTextureChannelTypeUInt:
		return kTextureChannelTypeUInt;
	default:
		return kTextureChannelTypeUNorm;
	}
}

static int32_t signExtend(int32_t value, uint32_t bits)
{
	value <<= 32 - bits;
	value >>= 32 - bits;
	return value;
}

uint64_t extractBitfield(const void* pointer, uint64_t offset, uint64_t count)
{
	LOG_ASSERT(nullptr != pointer, "error bit pointer");
	LOG_ASSERT(count <= 64, "error bit count");

	const void* p = static_cast<const uint8_t*>(pointer) + (offset >> 3);
	offset &= 7;
	const uint64_t mask = (uint64_t(1) << count) - 1;
	switch ((offset + count + 7) / 8)
	{
	case 1:
		return (*static_cast<const uint8_t*>(p) >> offset) & mask;
	case 2:
		return (*static_cast<const uint16_t*>(p) >> offset) & mask;
	case 3:
	case 4:
		return (*static_cast<const uint32_t*>(p) >> offset) & mask;
	case 5:
	case 6:
	case 7:
	case 8:
		return (*static_cast<const uint64_t*>(p) >> offset) & mask;
	}
	return 0;
}

void dataFormatDecoder(Reg32* __restrict destination, const uint32_t* __restrict source, const DataFormat dataFormat)
{
	DataFormat dataFmt = dataFormat;

	if (false == isSupported(dataFmt.getSurfaceFormat()))
		return;
	if (kTextureChannelTypeSrgb == dataFmt.getTextureChannelType())
	{
		switch (dataFmt.getTotalBytesPerElement())
		{
		case 1:
			dataFmt.m_bits.m_surfaceFormat = kSurfaceFormat8;
		case 2:
			dataFmt.m_bits.m_surfaceFormat = kSurfaceFormat8_8;
		default:
			dataFmt.m_bits.m_surfaceFormat = kSurfaceFormat8_8_8_8;
			break;
		}
	}

	const auto           numComponents     = (kSurfaceFormat5_9_9_9 == dataFmt.getSurfaceFormat()) ? 3 : dataFmt.getNumComponents();
	const TextureChannel textureChannel[4] = 
	{
		static_cast<TextureChannel>(dataFmt.m_bits.m_channelX),
		static_cast<TextureChannel>(dataFmt.m_bits.m_channelY),
		static_cast<TextureChannel>(dataFmt.m_bits.m_channelZ),
		static_cast<TextureChannel>(dataFmt.m_bits.m_channelW),
	};
	const bool     constantValuesAreInteger = kTextureChannelTypeUInt == dataFmt.getTextureChannelType() || kTextureChannelTypeSInt == dataFmt.getTextureChannelType();
	const uint32_t constantValue[2]         = { 0x00000000, constantValuesAreInteger ? 0x00000001U : 0x3f800000U };
	const auto&    info                     = g_surfaceFormatInfo[dataFmt.getSurfaceFormat()];


	struct TexelChannel
	{
		uint32_t           m_offset;
		uint32_t           m_count;
		TextureChannelType m_type;
	};

	TexelChannel channel[4];
	unsigned     bitOffset = 0;
	for (auto c = 0; c < 4; ++c)
	{
		channel[c].m_offset = bitOffset;
		channel[c].m_count  = info.m_bitsPerChannel[c];
		channel[c].m_type   = dataFmt.getTextureChannelType();
		channel[c].m_type   = getRealChannelType(channel[c].m_type, channel[c].m_count);
		bitOffset += channel[c].m_count;
	}

	uint32_t encoded[4] = {};
	for (auto c = 0U; c < dataFmt.getNumComponents(); ++c)
	{
		encoded[c] = static_cast<uint32_t>(extractBitfield(source, channel[c].m_offset, channel[c].m_count));
	}

	if (kSurfaceFormat5_9_9_9 == dataFmt.getSurfaceFormat())
	{
		for (auto c = 0; c < 3; ++c)
		{
			encoded[c] |= encoded[3] << 9;
		}
	}

	Channel decoded[4] = {};

	for (auto c = 0U; c < numComponents; ++c)
	{
		const uint32_t bitCount         = (kSurfaceFormat5_9_9_9 == dataFmt.getSurfaceFormat()) ? 14 : channel[c].m_count;
		const int32_t  unsignedToSigned = -(1LL << (bitCount - 1LL));
		const int32_t  mask             = ~((1LL << bitCount) - 1LL);
		Channel        u;
		u.u = encoded[c];
		switch (channel[c].m_type)
		{
		case kTextureChannelTypeUBInt:
			decoded[c].i = u.i + unsignedToSigned;
			break;
		case kTextureChannelTypeSInt:
			decoded[c].i = signExtend(u.i, bitCount);
			break;
		case kTextureChannelTypeUInt:
			decoded[c].u = u.u;
			break;
		case kTextureChannelTypeUBScaled:
			decoded[c].f = static_cast<float>(u.i + unsignedToSigned);
			break;
		case kTextureChannelTypeSScaled:
			decoded[c].f = static_cast<float>(signExtend(u.i, bitCount));
			break;
		case kTextureChannelTypeUScaled:
			decoded[c].f = static_cast<float>(u.u);
			break;
		case kTextureChannelTypeUBNorm:
			u.i += unsignedToSigned;
			u.i &= mask;
		case kTextureChannelTypeSNorm:
			decoded[c].f = std::max((float)signExtend(u.u, bitCount) / ((1 << (bitCount - 1)) - 1), -1.0f);
			break;
		case kTextureChannelTypeUBNormNoZero:
			u.i += unsignedToSigned;
			u.i &= mask;
		case kTextureChannelTypeSNormNoZero:
			decoded[c].f = (((float)signExtend(u.u, bitCount) + (1 << (bitCount - 1))) / ((1 << bitCount) - 1)) * 2.0 - 1.0;
			break;
		case kTextureChannelTypeUNorm:
			decoded[c].f = (float)u.u / ((1 << bitCount) - 1);
			break;
		case kTextureChannelTypeSrgb:
			if (3 == c)
				decoded[c].f = (float)u.u / ((1 << bitCount) - 1);
			else
			{
				const auto shift     = std::max(0, (int32_t)bitCount - 8);
				const auto unclamped = u.u >> shift;
				const auto clamped   = std::max(0U, std::min(255U, unclamped));
				decoded[c].u         = lookup_8_srgb[clamped];
			}
			break;
		case kTextureChannelTypeFloat:
			if (bitCount == 32)
				decoded[c].f = u.f;
			else
				decoded[c].f = unpackFloat(u.u, floats[bitCount].m_signBits, floats[bitCount].m_exponentBits, floats[bitCount].m_mantissaBits);
			break;
		}
	}

	for (auto c = 0; c < 4; ++c)
	{
		switch (textureChannel[c])
		{
		case kTextureChannelConstant0:
			destination[c].u = constantValue[0];
			break;
		case kTextureChannelConstant1:
			destination[c].u = constantValue[1];
			break;
		case kTextureChannelX:
			destination[c].u = decoded[0].u;
			break;
		case kTextureChannelY:
			destination[c].u = decoded[1].u;
			break;
		case kTextureChannelZ:
			destination[c].u = decoded[2].u;
			break;
		case kTextureChannelW:
			destination[c].u = decoded[3].u;
			break;
		default:
			destination[c].u = 0;
			break;
		}
	}

}
	
}  // namespace datafmt



#include "GnmCommandProxy.h"

LOG_CHANNEL(Graphic.Gnm.GnmCommandProxy);

namespace sce::Gnm
{

	GnmCommandProxy::GnmCommandProxy()
	{
	}

	GnmCommandProxy::~GnmCommandProxy()
	{
	}

	uint32_t GnmCommandProxy::sub_7FFC805A03D0(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   sub_7FFC805A9580(a1, a2, 1, 0);

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: sub_7FFC805A03D");
			// m_cb->sub_7FFC805A03D();
			count = kPacketCountSub_7FFC805A03D0;
		}

		return count;
	}

	uint32_t GnmCommandProxy::prepareFlip2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   sub_7FFC805A93D0(a1, a2, 1);

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: prepareFlip");
			// m_cb->sub_7FFC805A03F();
			count = kPacketCountPrepareFlip2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setVsharpInUserData(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   unsigned int v6; // [rsp+20h] [rbp-98h] BYREF
		//   unsigned int v7; // [rsp+24h] [rbp-94h] BYREF
		//   __int64 v8; // [rsp+28h] [rbp-90h]
		//   __int16 a2; // [rsp+30h] [rbp-88h] BYREF
		//   int a2_4; // [rsp+34h] [rbp-84h]
		//   int v11; // [rsp+38h] [rbp-80h]
		//   unsigned __int16 v12; // [rsp+3Ch] [rbp-7Ch]
		//   char v13[16]; // [rsp+40h] [rbp-78h] BYREF
		//   char v14[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 32i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 4096
		//     && (v11 & 0xFF00) == 30208
		//     && (a2_4 & 0xFFFF0FFF) == 1752498180
		//     && (v11 & 0x3FFF0000) == 0x40000 )
		//   {
		//     if ( (unsigned __int8)sub_7FFC80575550((unsigned int)v12 + 11264, &v6, &v7) )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setVsharpInUserData");
		//       v3 = sub_7FFC80564220((__int64)v14, v6);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stage", (__int64)"ShaderStage", v3);
		//       sub_7FFC805644A0((__int64)v14);
		//       v4 = sub_7FFC80564220((__int64)v14, v7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"startUserDataSlot", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v14);
		//       v5 = sub_7FFC80564280(v14, v13, 4i64);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"buffer", (__int64)"ResourceVsharp", v5);
		//       sub_7FFC805644A0((__int64)v14);
		//       set_packet_count((uint64_t)rdx0, 2u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setVsharpInUserData");
			// m_cb->setVsharpInUserData();
			count = kPacketCountSetVsharpInUserData;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setTsharpInUserData(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   unsigned int v6; // [rsp+20h] [rbp-A8h] BYREF
		//   unsigned int v7; // [rsp+24h] [rbp-A4h] BYREF
		//   __int64 v8; // [rsp+28h] [rbp-A0h]
		//   __int16 a2; // [rsp+30h] [rbp-98h] BYREF
		//   int a2_4; // [rsp+34h] [rbp-94h]
		//   int v11; // [rsp+38h] [rbp-90h]
		//   unsigned __int16 v12; // [rsp+3Ch] [rbp-8Ch]
		//   char v13[32]; // [rsp+40h] [rbp-88h] BYREF
		//   char v14[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 48i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 4096
		//     && (v11 & 0xFF00) == 30208
		//     && (a2_4 & 0xFFFF0FFF) == 1752498181
		//     && (v11 & 0x3FFF0000) == 0x80000 )
		//   {
		//     if ( (unsigned __int8)sub_7FFC80575550((unsigned int)v12 + 11264, &v6, &v7) )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setTsharpInUserData");
		//       v3 = sub_7FFC80564220((__int64)v14, v6);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stage", (__int64)"ShaderStage", v3);
		//       sub_7FFC805644A0((__int64)v14);
		//       v4 = sub_7FFC80564220((__int64)v14, v7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"startUserDataSlot", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v14);
		//       v5 = sub_7FFC80564280(v14, v13, 8i64);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"tex", (__int64)"ResourceTsharp", v5);
		//       sub_7FFC805644A0((__int64)v14);
		//       set_packet_count((uint64_t)rdx0, 2u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setTsharpInUserData");
			// m_cb->setTsharpInUserData();
			count = kPacketCountSetTsharpInUserData;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setSsharpInUserData(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   unsigned int v6; // [rsp+20h] [rbp-98h] BYREF
		//   unsigned int v7; // [rsp+24h] [rbp-94h] BYREF
		//   __int64 v8; // [rsp+28h] [rbp-90h]
		//   __int16 a2; // [rsp+30h] [rbp-88h] BYREF
		//   int a2_4; // [rsp+34h] [rbp-84h]
		//   int v11; // [rsp+38h] [rbp-80h]
		//   unsigned __int16 v12; // [rsp+3Ch] [rbp-7Ch]
		//   char v13[16]; // [rsp+40h] [rbp-78h] BYREF
		//   char v14[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 32i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 4096
		//     && (v11 & 0xFF00) == 30208
		//     && (a2_4 & 0xFFFF0FFF) == 1752498182
		//     && (v11 & 0x3FFF0000) == 0x40000 )
		//   {
		//     if ( (unsigned __int8)sub_7FFC80575550((unsigned int)v12 + 11264, &v6, &v7) )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setSsharpInUserData");
		//       v3 = sub_7FFC80564220((__int64)v14, v6);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stage", (__int64)"ShaderStage", v3);
		//       sub_7FFC805644A0((__int64)v14);
		//       v4 = sub_7FFC80564220((__int64)v14, v7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"startUserDataSlot", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v14);
		//       v5 = sub_7FFC80564280(v14, v13, 4i64);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"sampler", (__int64)"ResourceSsharp", v5);
		//       sub_7FFC805644A0((__int64)v14);
		//       set_packet_count((uint64_t)rdx0, 2u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setSsharpInUserData");
			// m_cb->setSsharpInUserData();
			count = kPacketCountSetSsharpInUserData;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setUserDataRegion(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned __int64 v4; // rdi
		//   __int64 v5; // rbx
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   unsigned int v10; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v11; // [rsp+24h] [rbp-84h] BYREF
		//   __int64 v12; // [rsp+28h] [rbp-80h]
		//   int v13[2]; // [rsp+30h] [rbp-78h] BYREF
		//   unsigned int v14; // [rsp+38h] [rbp-70h]
		//   unsigned __int16 v15; // [rsp+3Ch] [rbp-6Ch]
		//   char v16[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v12 = -2i64;
		//   if ( GetOpCodeType(a1, (__int64)v13, 16i64, *((_DWORD *)a1 + 2))
		//     && (v13[0] & 0xFF00) == 4096
		//     && (v13[1] & 0xFFFF0FFF) == 1752498189
		//     && (v14 & 0xFF00) == 30208 )
		//   {
		//     if ( (unsigned __int8)sub_7FFC80575550((unsigned int)v15 + 11264, &v10, &v11) )
		//     {
		//       v4 = ((unsigned __int64)(unsigned int)get_packet_size(v14) - 8) >> 2;
		//       v5 = sub_7FFC80594650(*a1, a1[1]);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         a2,
		//         (struct Concurrency::details::SchedulerNode *)"setUserDataRegion");
		//       v6 = sub_7FFC80564220((__int64)v16, v10);
		//       sub_7FFC805A8FC0((__int64)a2, (__int64)"stage", (__int64)"ShaderStage", v6);
		//       sub_7FFC805644A0((__int64)v16);
		//       v7 = sub_7FFC80564220((__int64)v16, v11);
		//       sub_7FFC805A8FC0((__int64)a2, (__int64)"startUserDataSlot", (__int64)"uint32_t", v7);
		//       sub_7FFC805644A0((__int64)v16);
		//       v8 = sub_7FFC80564430(v16, v5 + 16);
		//       sub_7FFC805A8FC0((__int64)a2, (__int64)"userData", (__int64)"uint32_t*", v8);
		//       sub_7FFC805644A0((__int64)v16);
		//       v9 = sub_7FFC80564220((__int64)v16, v4);
		//       sub_7FFC805A8FC0((__int64)a2, (__int64)"numDwords", (__int64)"uint32_t", v9);
		//       sub_7FFC805644A0((__int64)v16);
		//       set_packet_count((uint64_t)a2, 2u);
		//       *((_BYTE *)a2 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setUserDataRegion");
			// m_cb->setUserDataRegion();
			count = kPacketCountSetUserDataRegion;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPointerInUserData(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   unsigned int v6; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v7; // [rsp+24h] [rbp-84h] BYREF
		//   __int64 v8; // [rsp+28h] [rbp-80h]
		//   int a2; // [rsp+30h] [rbp-78h] BYREF
		//   unsigned __int16 a2_4; // [rsp+34h] [rbp-74h]
		//   __int64 v11; // [rsp+38h] [rbp-70h]
		//   char v12[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && (a2 & 0x3FFF0000) == 0x20000 )
		//   {
		//     if ( (unsigned __int8)sub_7FFC80575550((unsigned int)a2_4 + 11264, &v6, &v7) )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setPointerInUserData");
		//       v3 = sub_7FFC80564220((__int64)v12, v6);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stage", (__int64)"ShaderStage", v3);
		//       sub_7FFC805644A0((__int64)v12);
		//       v4 = sub_7FFC80564220((__int64)v12, v7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"startUserDataSlot", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v12);
		//       v5 = sub_7FFC80564430(v12, v11);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"gpuAddr", (__int64)"void*", v5);
		//       sub_7FFC805644A0((__int64)v12);
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPointerInUserData");
			// m_cb->setPointerInUserData();
			count = kPacketCountSetPointerInUserData;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setUserData(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   unsigned int v6; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v7; // [rsp+24h] [rbp-84h] BYREF
		//   __int64 v8; // [rsp+28h] [rbp-80h]
		//   int a2; // [rsp+30h] [rbp-78h] BYREF
		//   unsigned __int16 a2_4; // [rsp+34h] [rbp-74h]
		//   unsigned int v11; // [rsp+38h] [rbp-70h]
		//   char v12[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && (a2 & 0x3FFF0000) == 0x10000 )
		//   {
		//     if ( (unsigned __int8)sub_7FFC80575550((unsigned int)a2_4 + 11264, &v6, &v7) )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setUserData");
		//       v3 = sub_7FFC80564220((__int64)v12, v6);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stage", (__int64)"ShaderStage", v3);
		//       sub_7FFC805644A0((__int64)v12);
		//       v4 = sub_7FFC80564220((__int64)v12, v7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"userDataSlot", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v12);
		//       v5 = sub_7FFC80564220((__int64)v12, v11);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"data", (__int64)"uint32_t", v5);
		//       sub_7FFC805644A0((__int64)v12);
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setUserData");
			// m_cb->setUserData();
			count = kPacketCountSetUserData;
		}

		return count;
	}

	uint32_t GnmCommandProxy::prefetchIntoL2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int16 a2; // [rsp+28h] [rbp-90h] BYREF
		//   int a2_4; // [rsp+2Ch] [rbp-8Ch]
		//   __int64 v7; // [rsp+30h] [rbp-88h]
		//   __int64 v8; // [rsp+38h] [rbp-80h]
		//   unsigned int v9; // [rsp+40h] [rbp-78h]
		//   char v10[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 20480 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"dmaData");
		//     if ( (a2_4 & 0x300000 | ((v9 & 0x8000000 | (v9 >> 1) & 0x10000000) >> 5)) >> 20 == 12 && v8 == 197164 )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"prefetchIntoL2");
		//       v3 = sub_7FFC80564430(v10, v7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dataAddr", (__int64)"void*", v3);
		//       sub_7FFC805644A0((__int64)v10);
		//       v4 = sub_7FFC80564220((__int64)v10, v9 & 0x1FFFFF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"sizeInBytes", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: prefetchIntoL");
			// m_cb->prefetchIntoL();
			count = kPacketCountPrefetchIntoL2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::requestVideoFlip(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int16 a2; // [rsp+20h] [rbp-38h] BYREF
		//   int a2_4; // [rsp+24h] [rbp-34h]
		//   int v5; // [rsp+34h] [rbp-24h]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 14080
		//     && (a2_4 & 0xF00) == 1280
		//     && (a2_4 & 0x10000) == 0
		//     && (a2_4 & 0x100000) == 0
		//     && (a2_4 & 0xC0000000) == 0
		//     && (v5 & 0xFF00) == 4096
		//     && (v5 & 0x3FFF0000) == 589824 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"requestVideoFlip");
		//     set_packet_count((uint64_t)rdx0, 2u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: requestVideoFlip");
			// m_cb->requestVideoFlip();
			count = kPacketCountRequestVideoFlip;
		}

		return count;
	}

	uint32_t GnmCommandProxy::initializeDefaultHardwareState(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 a2; // [rsp+20h] [rbp-58h] BYREF
		//   int v6; // [rsp+2Ch] [rbp-4Ch]
		//   int v7; // [rsp+34h] [rbp-44h]
		//   int v8; // [rsp+50h] [rbp-28h]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 60i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 10240
		//     && (v6 & 0xFF00) == 4608
		//     && (v7 & 0xFF00) == 22528
		//     && (v8 & 0xFF00) == 30208 )
		//   {
		//     v4 = sub_7FFC805A9130((_QWORD *)a1);
		//     set_packet_count((uint64_t)rdx0, v4 + 1);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"initializeDefaultHardwareState");
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: initializeDefaultHardwareState");
			// m_cb->initializeDefaultHardwareState();
			count = kPacketCountInitializeDefaultHardwareState;
		}

		return count;
	}

	uint32_t GnmCommandProxy::initializeToDefaultContextState(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 a2; // [rsp+20h] [rbp-68h] BYREF
		//   int v6; // [rsp+2Ch] [rbp-5Ch]
		//   int v7; // [rsp+34h] [rbp-54h]
		//   __int16 v8; // [rsp+38h] [rbp-50h]
		//   int v9; // [rsp+40h] [rbp-48h]
		//   __int16 v10; // [rsp+44h] [rbp-44h]
		//   int v11; // [rsp+4Ch] [rbp-3Ch]
		//   __int16 v12; // [rsp+50h] [rbp-38h]
		//   int v13; // [rsp+58h] [rbp-30h]
		//   __int16 v14; // [rsp+5Ch] [rbp-2Ch]
		//   int v15; // [rsp+64h] [rbp-24h]
		//   __int16 v16; // [rsp+68h] [rbp-20h]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 80i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 10240
		//     && (v6 & 0xFF00) == 4608
		//     && (v7 & 0xFF00) == 26880
		//     && v8 == 761
		//     && (v9 & 0xFF00) == 26880
		//     && v10 == 642
		//     && (v11 & 0xFF00) == 26880
		//     && v12 == 640
		//     && (v13 & 0xFF00) == 26880
		//     && v14 == 641
		//     && (v15 & 0xFF00) == 26880
		//     && v16 == 516 )
		//   {
		//     v4 = sub_7FFC805A9130((_QWORD *)a1, 16);
		//     set_packet_count((uint64_t)rdx0, v4 + 1);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"initializeToDefaultContextState");
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: initializeToDefaultContextState");
			// m_cb->initializeToDefaultContextState();
			count = kPacketCountInitializeToDefaultContextState;
		}

		return count;
	}

	uint32_t GnmCommandProxy::initializeToDefaultContextState2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 a2; // [rsp+20h] [rbp-48h] BYREF
		//   int v6; // [rsp+2Ch] [rbp-3Ch]
		//   int v7; // [rsp+34h] [rbp-34h]
		//   int v8; // [rsp+3Ch] [rbp-2Ch]
		//   __int16 v9; // [rsp+40h] [rbp-28h]
		//   int v10; // [rsp+48h] [rbp-20h]
		//   __int16 v11; // [rsp+4Ch] [rbp-1Ch]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 52i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 10240
		//     && (v6 & 0xFF00) == 4608
		//     && (v7 & 0xFF00) == 12032
		//     && (v8 & 0xFF00) == 26880
		//     && v9 == 258
		//     && (v10 & 0xFF00) == 26880
		//     && v11 == 514 )
		//   {
		//     v4 = sub_7FFC805A9130((_QWORD *)a1, 16);
		//     set_packet_count((uint64_t)rdx0, v4 + 1);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"initializeToDefaultContextState");
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: initializeToDefaultContextState");
			// m_cb->initializeToDefaultContextState();
			count = kPacketCountInitializeToDefaultContextState2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::initializeToDefaultContextState3(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 a2; // [rsp+20h] [rbp-38h] BYREF
		//   int v6; // [rsp+2Ch] [rbp-2Ch]
		//   int v7; // [rsp+34h] [rbp-24h]
		//   int v8; // [rsp+3Ch] [rbp-1Ch]
		//   __int16 v9; // [rsp+40h] [rbp-18h]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 40i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 10240
		//     && (v6 & 0xFF00) == 4608
		//     && (v7 & 0xFF00) == 12032
		//     && (v8 & 0xFF00) == 26880
		//     && v9 == 514 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"initializeToDefaultContextState");
		//     v4 = sub_7FFC805A9130((_QWORD *)a1, 16);
		//     set_packet_count((uint64_t)rdx0, v4 + 1);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: initializeToDefaultContextState");
			// m_cb->initializeToDefaultContextState();
			count = kPacketCountInitializeToDefaultContextState3;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setupEsGsRingRegisters(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 683 )
		//   {
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     v3 = sub_7FFC80564220((__int64)v7, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"maxExportVertexSizeInDword", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setupEsGsRingRegisters");
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setupEsGsRingRegisters");
			// m_cb->setupEsGsRingRegisters();
			count = kPacketCountSetupEsGsRingRegisters;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setupGsVsRingRegisters(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   _WORD a2[26]; // [rsp+30h] [rbp-59h] BYREF
		//   unsigned int v9; // [rsp+64h] [rbp-25h]
		//   int v10; // [rsp+68h] [rbp-21h]
		//   __int16 v11; // [rsp+6Ch] [rbp-1Dh]
		//   char v12[80]; // [rsp+80h] [rbp-9h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)a2, 68i64, *(_DWORD *)(a1 + 8))
		//     && (a2[0] & 0xFF00) == 26880
		//     && a2[2] == 727
		//     && (a2[12] & 0xFF00) == 26880
		//     && a2[14] == 664
		//     && (a2[22] & 0xFF00) == 26880
		//     && a2[24] == 718
		//     && (v10 & 0xFF00) == 26880
		//     && v11 == 684 )
		//   {
		//     set_packet_count((uint64_t)rdx0, 4u);
		//     v3 = sub_7FFC80564220((__int64)v12, a2[4] & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"vertexSizePerStreamInDword[0]", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v12);
		//     v4 = sub_7FFC80564220((__int64)v12, a2[6] & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"vertexSizePerStreamInDword[1]", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v12);
		//     v5 = sub_7FFC80564220((__int64)v12, a2[8] & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"vertexSizePerStreamInDword[2]", (__int64)"uint32_t", v5);
		//     sub_7FFC805644A0((__int64)v12);
		//     v6 = sub_7FFC80564220((__int64)v12, a2[10] & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"vertexSizePerStreamInDword[3]", (__int64)"uint32_t", v6);
		//     sub_7FFC805644A0((__int64)v12);
		//     v7 = sub_7FFC80564220((__int64)v12, v9);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"maxOutputVertexCount", (__int64)"uint32_t", v7);
		//     sub_7FFC805644A0((__int64)v12);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setupGsVsRingRegisters");
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setupGsVsRingRegisters");
			// m_cb->setupGsVsRingRegisters();
			count = kPacketCountSetupGsVsRingRegisters;
		}

		return count;
	}

	uint32_t GnmCommandProxy::flushStreamout(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 a2; // [rsp+20h] [rbp-78h] BYREF
		//   int v4; // [rsp+28h] [rbp-70h]
		//   int v5; // [rsp+44h] [rbp-54h]
		//   __int16 v6; // [rsp+48h] [rbp-50h]
		//   int v7; // [rsp+50h] [rbp-48h]
		//   int v8; // [rsp+58h] [rbp-40h]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 84i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 17920
		//     && (v4 & 0xFF00) == 22528
		//     && (v5 & 0xFF00) == 30976
		//     && v6 == 63
		//     && (v7 & 0xFF00) == 17920
		//     && (v8 & 0xFF00) == 15360 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"flushStreamout");
		//     set_packet_count((uint64_t)rdx0, 5u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: flushStreamout");
			// m_cb->flushStreamout();
			count = kPacketCountFlushStreamout;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setStreamoutBufferDimensions(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v3; // edx
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int v8; // [rsp+30h] [rbp-78h]
		//   unsigned int v9; // [rsp+34h] [rbp-74h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 0x6900 )
		//   {
		//     v3 = WORD2(a2) - 692;
		//     if ( v3 <= 0xC )
		//     {
		//       v4 = sub_7FFC80564220((__int64)v10, v3 >> 2);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"bufferId", (__int64)"StreamoutBufferId", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       v5 = sub_7FFC80564220((__int64)v10, v8);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"bufferSizeInDW", (__int64)"uint32_t", v5);
		//       sub_7FFC805644A0((__int64)v10);
		//       v6 = sub_7FFC80564220((__int64)v10, v9);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"bufferStrideInDW", (__int64)"uint32_t", v6);
		//       sub_7FFC805644A0((__int64)v10);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setStreamoutBufferDimensions");
		//       *((_BYTE *)rdx0 + 24) = 1;
		//       set_packet_count((uint64_t)rdx0, 1u);
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setStreamoutBufferDimensions");
			// m_cb->setStreamoutBufferDimensions();
			count = kPacketCountSetStreamoutBufferDimensions;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setStreamoutMapping(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 0x6900
		//     && WORD2(a2) == 0x2E6 )
		//   {
		//     v3 = sub_7FFC80564220((__int64)v7, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"mapping", (__int64)"StreamoutBufferMapping", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setStreamoutMapping");
		//     *((_BYTE *)rdx0 + 24) = 1;
		//     set_packet_count((uint64_t)rdx0, 1u);
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setStreamoutMapping");
			// m_cb->setStreamoutMapping();
			count = kPacketCountSetStreamoutMapping;
		}

		return count;
	}

	uint32_t GnmCommandProxy::writeStreamoutBufferUpdate(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int16 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v10; // [rsp+30h] [rbp-78h]
		//   unsigned __int16 v11; // [rsp+34h] [rbp-74h]
		//   unsigned int v12; // [rsp+38h] [rbp-70h]
		//   unsigned __int16 v13; // [rsp+3Ch] [rbp-6Ch]
		//   char v14[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 24i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 13312 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"writeStreamoutBufferUpdate");
		//     v3 = sub_7FFC80564220((__int64)v14, (a2_4 >> 8) & 3);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"buffer", (__int64)"StreamoutBufferId", v3);
		//     sub_7FFC805644A0((__int64)v14);
		//     v4 = sub_7FFC80564220((__int64)v14, (unsigned __int8)a2_4 >> 3);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"sourceSelect", (__int64)"StreamoutBufferUpdateWrite", v4);
		//     sub_7FFC805644A0((__int64)v14);
		//     v5 = sub_7FFC80564220((__int64)v14, a2_4 & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"updateMemory", (__int64)"StreamoutBufferUpdateSaveFilledSize", v5);
		//     sub_7FFC805644A0((__int64)v14);
		//     v6 = sub_7FFC80564430(v14, v10 | ((unsigned __int64)v11 << 32));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dstAddr", (__int64)"void*", v6);
		//     sub_7FFC805644A0((__int64)v14);
		//     v7 = sub_7FFC80564430(v14, v12 | ((unsigned __int64)v13 << 32));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"srcAddrOrImm", (__int64)"uint64_t", v7);
		//     sub_7FFC805644A0((__int64)v14);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//     set_packet_count((uint64_t)rdx0, 1u);
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: writeStreamoutBufferUpdate");
			// m_cb->writeStreamoutBufferUpdate();
			count = kPacketCountWriteStreamoutBufferUpdate;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setComputeShaderControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v9; // [rsp+28h] [rbp-80h]
		//   __int64 v10; // [rsp+30h] [rbp-78h]
		//   char v11[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v10 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 30208 && WORD2(a2) == 533 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setComputeShaderControl");
		//       v4 = sub_7FFC80564220((__int64)v11, v9 & 0x3FF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"wavesPerSh", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v11);
		//       v5 = sub_7FFC80564220((__int64)v11, (unsigned __int16)v9 >> 12);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"threadgroupsPerCu", (__int64)"uint32_t", v5);
		//       sub_7FFC805644A0((__int64)v11);
		//       v6 = sub_7FFC80564220((__int64)v11, HIWORD(v9) & 0x3F);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"lockThreshold", (__int64)"uint32_t", v6);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v11);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setComputeShaderControl");
			// m_cb->setComputeShaderControl();
			count = kPacketCountSetComputeShaderControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setGraphicsShaderControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   __int64 v10; // rax
		//   __int64 v11; // rax
		//   __int64 v12; // rax
		//   __int64 v13; // rax
		//   __int64 v14; // rax
		//   __int64 v15; // rax
		//   __int64 v16; // rax
		//   __int64 v17; // rax
		//   __int64 v18; // rax
		//   __int64 v19; // rax
		//   __int64 v20; // rax
		//   __int64 v21; // rax
		//   __int64 a2; // [rsp+20h] [rbp-79h] BYREF
		//   unsigned int v24; // [rsp+28h] [rbp-71h]
		//   int v25; // [rsp+2Ch] [rbp-6Dh]
		//   __int16 v26; // [rsp+30h] [rbp-69h]
		//   unsigned int v27; // [rsp+34h] [rbp-65h]
		//   int v28; // [rsp+38h] [rbp-61h]
		//   __int16 v29; // [rsp+3Ch] [rbp-5Dh]
		//   unsigned int v30; // [rsp+40h] [rbp-59h]
		//   int v31; // [rsp+44h] [rbp-55h]
		//   __int16 v32; // [rsp+48h] [rbp-51h]
		//   unsigned int v33; // [rsp+4Ch] [rbp-4Dh]
		//   int v34; // [rsp+50h] [rbp-49h]
		//   __int16 v35; // [rsp+54h] [rbp-45h]
		//   unsigned int v36; // [rsp+58h] [rbp-41h]
		//   int v37; // [rsp+5Ch] [rbp-3Dh]
		//   __int16 v38; // [rsp+60h] [rbp-39h]
		//   unsigned int v39; // [rsp+64h] [rbp-35h]
		//   int v40; // [rsp+68h] [rbp-31h]
		//   __int16 v41; // [rsp+6Ch] [rbp-2Dh]
		//   unsigned int v42; // [rsp+70h] [rbp-29h]
		//   __int64 v43; // [rsp+80h] [rbp-19h]
		//   char v44[80]; // [rsp+90h] [rbp-9h] BYREF
		//
		//   v43 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 84i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 30208 && WORD2(a2) == 7 )
		//     {
		//       v3 = v25 & 0xFF00;
		//       if ( v3 == 30208 && v26 == 70 )
		//       {
		//         v3 = v28 & 0xFF00;
		//         if ( v3 == 30208 && v29 == 135 )
		//         {
		//           v3 = v31 & 0xFF00;
		//           if ( v3 == 30208 && v32 == 199 )
		//           {
		//             v3 = v34 & 0xFF00;
		//             if ( v3 == 30208 && v35 == 263 )
		//             {
		//               v3 = v37 & 0xFF00;
		//               if ( v3 == 30208 && v38 == 327 )
		//               {
		//                 v3 = v40 & 0xFF00;
		//                 if ( v3 == 30208 && v41 == 71 )
		//                 {
		//                   set_packet_count((uint64_t)rdx0, 7u);
		//                   Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//                     rdx0,
		//                     (struct Concurrency::details::SchedulerNode *)"setGraphicsShaderControl");
		//                   v4 = sub_7FFC80564220((__int64)v44, (unsigned __int16)v24);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Ps.cuMask", (__int64)"uint16_t", v4);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v5 = sub_7FFC80564220((__int64)v44, HIWORD(v24) & 0x3F);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Ps.waveLimitBy16", (__int64)"uint32_t", v5);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v6 = sub_7FFC80564220((__int64)v44, (v24 >> 22) & 0xF);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Ps.lockThresholdBy4", (__int64)"uint32_t", v6);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v7 = sub_7FFC80564220((__int64)v44, (unsigned __int16)v27);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Vs.cuMask", (__int64)"uint16_t", v7);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v8 = sub_7FFC80564220((__int64)v44, HIWORD(v27) & 0x3F);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Vs.waveLimitBy16", (__int64)"uint32_t", v8);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v9 = sub_7FFC80564220((__int64)v44, (v27 >> 22) & 0xF);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Vs.lockThresholdBy4", (__int64)"uint32_t", v9);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v10 = sub_7FFC80564220((__int64)v44, (unsigned __int16)v30);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Gs.cuMask", (__int64)"uint16_t", v10);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v11 = sub_7FFC80564220((__int64)v44, HIWORD(v30) & 0x3F);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Gs.waveLimitBy16", (__int64)"uint32_t", v11);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v12 = sub_7FFC80564220((__int64)v44, (v30 >> 22) & 0xF);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Gs.lockThresholdBy4", (__int64)"uint32_t", v12);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v13 = sub_7FFC80564220((__int64)v44, (unsigned __int16)v33);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Es.cuMask", (__int64)"uint16_t", v13);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v14 = sub_7FFC80564220((__int64)v44, HIWORD(v33) & 0x3F);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Es.waveLimitBy16", (__int64)"uint32_t", v14);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v15 = sub_7FFC80564220((__int64)v44, (v33 >> 22) & 0xF);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Es.lockThresholdBy4", (__int64)"uint32_t", v15);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v16 = sub_7FFC80564220((__int64)v44, v36 & 0x3F);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Hs.waveLimitBy16", (__int64)"uint32_t", v16);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v17 = sub_7FFC80564220((__int64)v44, (v36 >> 6) & 0xF);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Hs.lockThresholdBy4", (__int64)"uint32_t", v17);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v18 = sub_7FFC80564220((__int64)v44, (unsigned __int16)v39);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Ls.cuMask", (__int64)"uint16_t", v18);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v19 = sub_7FFC80564220((__int64)v44, HIWORD(v39) & 0x3F);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Ls.waveLimitBy16", (__int64)"uint32_t", v19);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v20 = sub_7FFC80564220((__int64)v44, (v39 >> 22) & 0xF);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.Ls.lockThresholdBy4", (__int64)"uint32_t", v20);
		//                   sub_7FFC805644A0((__int64)v44);
		//                   v21 = sub_7FFC80564220((__int64)v44, v42);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"control.m_regVsLateAlloc", (__int64)"uint32_t", v21);
		//                   LOBYTE(v3) = sub_7FFC805644A0((__int64)v44);
		//                   *((_BYTE *)rdx0 + 24) = 1;
		//                 }
		//               }
		//             }
		//           }
		//         }
		//       }
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setGraphicsShaderControl");
			// m_cb->setGraphicsShaderControl();
			count = kPacketCountSetGraphicsShaderControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setGraphicsShaderControl2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v10; // [rsp+28h] [rbp-80h]
		//   __int64 v11; // [rsp+30h] [rbp-78h]
		//   char v12[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v11 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 30208 && WORD2(a2) == 7 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setGraphicsShaderControl");
		//       v4 = sub_7FFC80564220((__int64)v12, 1u);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stage", (__int64)"ShaderStage", v4);
		//       sub_7FFC805644A0((__int64)v12);
		//       v5 = sub_7FFC80564220((__int64)v12, (unsigned __int16)v10);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cuMask", (__int64)"uint16_t", v5);
		//       sub_7FFC805644A0((__int64)v12);
		//       v6 = sub_7FFC80564220((__int64)v12, HIWORD(v10) & 0x3F);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"waveLimitBy16", (__int64)"uint32_t", v6);
		//       sub_7FFC805644A0((__int64)v12);
		//       v7 = sub_7FFC80564220((__int64)v12, (v10 >> 22) & 0xF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"lockThresholdBy4", (__int64)"uint32_t", v7);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v12);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setGraphicsShaderControl");
			// m_cb->setGraphicsShaderControl();
			count = kPacketCountSetGraphicsShaderControl2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setGraphicsShaderControl3(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v10; // [rsp+28h] [rbp-80h]
		//   __int64 v11; // [rsp+30h] [rbp-78h]
		//   char v12[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v11 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 30208 && WORD2(a2) == 70 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setGraphicsShaderControl");
		//       v4 = sub_7FFC80564220((__int64)v12, 2u);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stage", (__int64)"ShaderStage", v4);
		//       sub_7FFC805644A0((__int64)v12);
		//       v5 = sub_7FFC80564220((__int64)v12, (unsigned __int16)v10);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cuMask", (__int64)"uint16_t", v5);
		//       sub_7FFC805644A0((__int64)v12);
		//       v6 = sub_7FFC80564220((__int64)v12, HIWORD(v10) & 0x3F);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"waveLimitBy16", (__int64)"uint32_t", v6);
		//       sub_7FFC805644A0((__int64)v12);
		//       v7 = sub_7FFC80564220((__int64)v12, (v10 >> 22) & 0xF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"lockThresholdBy4", (__int64)"uint32_t", v7);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v12);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setGraphicsShaderControl");
			// m_cb->setGraphicsShaderControl();
			count = kPacketCountSetGraphicsShaderControl3;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setGraphicsShaderControl4(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v10; // [rsp+28h] [rbp-80h]
		//   __int64 v11; // [rsp+30h] [rbp-78h]
		//   char v12[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v11 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 30208 && WORD2(a2) == 263 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setGraphicsShaderControl");
		//       v4 = sub_7FFC80564220((__int64)v12, 5u);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stage", (__int64)"ShaderStage", v4);
		//       sub_7FFC805644A0((__int64)v12);
		//       v5 = sub_7FFC80564220((__int64)v12, 0);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cuMask", (__int64)"uint16_t", v5);
		//       sub_7FFC805644A0((__int64)v12);
		//       v6 = sub_7FFC80564220((__int64)v12, v10 & 0x3F);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"waveLimitBy16", (__int64)"uint32_t", v6);
		//       sub_7FFC805644A0((__int64)v12);
		//       v7 = sub_7FFC80564220((__int64)v12, (v10 >> 6) & 0xF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"lockThresholdBy4", (__int64)"uint32_t", v7);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v12);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setGraphicsShaderControl");
			// m_cb->setGraphicsShaderControl();
			count = kPacketCountSetGraphicsShaderControl4;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setGraphicsShaderControl5(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v10; // [rsp+28h] [rbp-80h]
		//   __int64 v11; // [rsp+30h] [rbp-78h]
		//   char v12[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v11 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 0x7600 && WORD2(a2) == 327 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setGraphicsShaderControl");
		//       v4 = sub_7FFC80564220((__int64)v12, 6u);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stage", (__int64)"ShaderStage", v4);
		//       sub_7FFC805644A0((__int64)v12);
		//       v5 = sub_7FFC80564220((__int64)v12, (unsigned __int16)v10);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cuMask", (__int64)"uint16_t", v5);
		//       sub_7FFC805644A0((__int64)v12);
		//       v6 = sub_7FFC80564220((__int64)v12, HIWORD(v10) & 0x3F);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"waveLimitBy16", (__int64)"uint32_t", v6);
		//       sub_7FFC805644A0((__int64)v12);
		//       v7 = sub_7FFC80564220((__int64)v12, (v10 >> 22) & 0xF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"lockThresholdBy4", (__int64)"uint32_t", v7);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v12);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setGraphicsShaderControl");
			// m_cb->setGraphicsShaderControl();
			count = kPacketCountSetGraphicsShaderControl5;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setGraphicsShaderControl6(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v10; // [rsp+28h] [rbp-80h]
		//   __int64 v11; // [rsp+30h] [rbp-78h]
		//   char v12[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v11 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 30208 && WORD2(a2) == 135 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setGraphicsShaderControl");
		//       v4 = sub_7FFC80564220((__int64)v12, 3u);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stage", (__int64)"ShaderStage", v4);
		//       sub_7FFC805644A0((__int64)v12);
		//       v5 = sub_7FFC80564220((__int64)v12, (unsigned __int16)v10);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cuMask", (__int64)"uint16_t", v5);
		//       sub_7FFC805644A0((__int64)v12);
		//       v6 = sub_7FFC80564220((__int64)v12, HIWORD(v10) & 0x3F);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"waveLimitBy16", (__int64)"uint32_t", v6);
		//       sub_7FFC805644A0((__int64)v12);
		//       v7 = sub_7FFC80564220((__int64)v12, (v10 >> 22) & 0xF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"lockThresholdBy4", (__int64)"uint32_t", v7);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v12);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setGraphicsShaderControl");
			// m_cb->setGraphicsShaderControl();
			count = kPacketCountSetGraphicsShaderControl6;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setGraphicsShaderControl7(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v10; // [rsp+28h] [rbp-80h]
		//   __int64 v11; // [rsp+30h] [rbp-78h]
		//   char v12[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v11 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 30208 && WORD2(a2) == 199 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setGraphicsShaderControl");
		//       v4 = sub_7FFC80564220((__int64)v12, 4u);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stage", (__int64)"ShaderStage", v4);
		//       sub_7FFC805644A0((__int64)v12);
		//       v5 = sub_7FFC80564220((__int64)v12, (unsigned __int16)v10);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cuMask", (__int64)"uint16_t", v5);
		//       sub_7FFC805644A0((__int64)v12);
		//       v6 = sub_7FFC80564220((__int64)v12, HIWORD(v10) & 0x3F);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"waveLimitBy16", (__int64)"uint32_t", v6);
		//       sub_7FFC805644A0((__int64)v12);
		//       v7 = sub_7FFC80564220((__int64)v12, (v10 >> 22) & 0xF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"lockThresholdBy4", (__int64)"uint32_t", v7);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v12);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setGraphicsShaderControl");
			// m_cb->setGraphicsShaderControl();
			count = kPacketCountSetGraphicsShaderControl7;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setComputeResourceManagement(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   unsigned int v4; // ebx
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v10; // [rsp+28h] [rbp-80h]
		//   int v11; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned __int16 v12; // [rsp+30h] [rbp-78h]
		//   int v13; // [rsp+34h] [rbp-74h]
		//   __int64 v14; // [rsp+38h] [rbp-70h]
		//   char v15[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v14 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 24i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 30208 )
		//     {
		//       LOBYTE(v3) = v11;
		//       if ( (v11 & 0xFF00) == 30208 && (a2 & 0x3FFF0000) == 0x10000 )
		//       {
		//         LOBYTE(v3) = 0;
		//         if ( (v11 & 0x3FFF0000) == 0x10000 )
		//         {
		//           LOBYTE(v3) = v12;
		//           if ( a2_4 == 534 )
		//           {
		//             v4 = 0;
		//           }
		//           else
		//           {
		//             if ( a2_4 != 535 )
		//               return v3;
		//             v4 = 1;
		//           }
		//           v3 = v12 - a2_4;
		//           if ( v3 == 3 )
		//           {
		//             LOBYTE(v3) = v13;
		//             if ( v10 == v13 )
		//             {
		//               Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//                 rdx0,
		//                 (struct Concurrency::details::SchedulerNode *)"setComputeResourceManagement");
		//               set_packet_count((uint64_t)rdx0, 2u);
		//               v5 = sub_7FFC80564220((__int64)v15, v4);
		//               sub_7FFC805A8FC0((__int64)rdx0, (__int64)"engine", (__int64)"ShaderEngine", v5);
		//               sub_7FFC805644A0((__int64)v15);
		//               v6 = sub_7FFC80564220((__int64)v15, v10);
		//               sub_7FFC805A8FC0((__int64)rdx0, (__int64)"mask", (__int64)"uint16_t", v6);
		//               LOBYTE(v3) = sub_7FFC805644A0((__int64)v15);
		//               *((_BYTE *)rdx0 + 24) = 1;
		//             }
		//           }
		//         }
		//       }
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setComputeResourceManagement");
			// m_cb->setComputeResourceManagement();
			count = kPacketCountSetComputeResourceManagement;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setComputeResourceManagement2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int packet_size; // eax
		//   int v4; // ebx
		//   unsigned int v5; // ebx
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   int a2; // [rsp+28h] [rbp-90h] BYREF
		//   __int16 a2_4; // [rsp+2Ch] [rbp-8Ch]
		//   unsigned int v11; // [rsp+30h] [rbp-88h]
		//   unsigned int v12; // [rsp+34h] [rbp-84h]
		//   char v13[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   LOBYTE(packet_size) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)packet_size )
		//   {
		//     v4 = a2;
		//     packet_size = a2 & 0xFF00;
		//     if ( packet_size == 30208 )
		//     {
		//       packet_size = v12 & 0xFF00;
		//       if ( packet_size == 4096 )
		//       {
		//         packet_size = get_packet_size(v12);
		//         if ( packet_size == 3 && (v4 & 0x3FFF0000) == 0x10000 )
		//         {
		//           LOBYTE(packet_size) = a2_4 - 22;
		//           if ( a2_4 == 534 )
		//           {
		//             v5 = 0;
		//             goto LABEL_10;
		//           }
		//           if ( a2_4 == 535 )
		//           {
		//             v5 = 1;
		// LABEL_10:
		//             Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//               rdx0,
		//               (struct Concurrency::details::SchedulerNode *)"setComputeResourceManagement");
		//             set_packet_count((uint64_t)rdx0, 2u);
		//             v6 = sub_7FFC80564220((__int64)v13, v5);
		//             sub_7FFC805A8FC0((__int64)rdx0, (__int64)"engine", (__int64)"ShaderEngine", v6);
		//             sub_7FFC805644A0((__int64)v13);
		//             v7 = sub_7FFC80564220((__int64)v13, v11);
		//             sub_7FFC805A8FC0((__int64)rdx0, (__int64)"mask", (__int64)"uint16_t", v7);
		//             LOBYTE(packet_size) = sub_7FFC805644A0((__int64)v13);
		//             *((_BYTE *)rdx0 + 24) = 1;
		//           }
		//         }
		//       }
		//     }
		//   }
		//   return packet_size;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setComputeResourceManagement");
			// m_cb->setComputeResourceManagement();
			count = kPacketCountSetComputeResourceManagement2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setComputeResourceManagementForNeo(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   char result; // al
		//   unsigned int v4; // ebx
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v9; // [rsp+28h] [rbp-80h]
		//   __int64 v10; // [rsp+30h] [rbp-78h]
		//   char v11[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v10 = -2i64;
		//   result = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( result )
		//   {
		//     result = a2;
		//     if ( (a2 & 0xFF00) == 30208 )
		//     {
		//       result = 0;
		//       if ( (a2 & 0x3FFF0000) == 0x10000 )
		//       {
		//         if ( a2_4 == 534 )
		//         {
		//           v4 = 0;
		//         }
		//         else if ( a2_4 == 535 )
		//         {
		//           v4 = 1;
		//         }
		//         else
		//         {
		//           result = a2_4 - 25;
		//           if ( a2_4 == 537 )
		//           {
		//             v4 = 2;
		//           }
		//           else
		//           {
		//             if ( a2_4 != 538 )
		//               return result;
		//             v4 = 3;
		//           }
		//         }
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"setComputeResourceManagementForNeo");
		//         *((_BYTE *)rdx0 + 26) = 1;
		//         set_packet_count((uint64_t)rdx0, 1u);
		//         v5 = sub_7FFC80564220((__int64)v11, v4);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"engine", (__int64)"ShaderEngine", v5);
		//         sub_7FFC805644A0((__int64)v11);
		//         v6 = sub_7FFC80564220((__int64)v11, v9);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"mask", (__int64)"uint16_t", v6);
		//         result = sub_7FFC805644A0((__int64)v11);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//     }
		//   }
		//   return result;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setComputeResourceManagementForNeo");
			// m_cb->setComputeResourceManagementForNeo();
			count = kPacketCountSetComputeResourceManagementForNeo;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setComputeResourceManagementForBase(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   char result; // al
		//   unsigned int v4; // ebx
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v9; // [rsp+28h] [rbp-80h]
		//   __int64 v10; // [rsp+30h] [rbp-78h]
		//   char v11[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v10 = -2i64;
		//   result = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( result )
		//   {
		//     result = a2;
		//     if ( (a2 & 0xFF00) == 30208 )
		//     {
		//       result = 0;
		//       if ( (a2 & 0x3FFF0000) == 0x10000 )
		//       {
		//         result = a2_4 - 22;
		//         if ( a2_4 == 534 )
		//         {
		//           v4 = 0;
		//         }
		//         else
		//         {
		//           if ( a2_4 != 535 )
		//             return result;
		//           v4 = 1;
		//         }
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"setComputeResourceManagementForBase");
		//         set_packet_count((uint64_t)rdx0, 1u);
		//         v5 = sub_7FFC80564220((__int64)v11, v4);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"engine", (__int64)"ShaderEngine", v5);
		//         sub_7FFC805644A0((__int64)v11);
		//         v6 = sub_7FFC80564220((__int64)v11, v9);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"mask", (__int64)"uint16_t", v6);
		//         result = sub_7FFC805644A0((__int64)v11);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//     }
		//   }
		//   return result;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setComputeResourceManagementForBase");
			// m_cb->setComputeResourceManagementForBase();
			count = kPacketCountSetComputeResourceManagementForBase;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setGraphicsScratchSize(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v8; // [rsp+28h] [rbp-80h]
		//   __int64 v9; // [rsp+30h] [rbp-78h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 0x6900 && WORD2(a2) == 0x1BA )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setGraphicsScratchSize");
		//       v4 = sub_7FFC80564220((__int64)v10, v8 & 0xFFF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"maxNumWaves", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       v5 = sub_7FFC80564220((__int64)v10, (v8 >> 12) & 0x1FFF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"num1KByteChunksPerWave", (__int64)"uint32_t", v5);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v10);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setGraphicsScratchSize");
			// m_cb->setGraphicsScratchSize();
			count = kPacketCountSetGraphicsScratchSize;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setComputeScratchSize(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v8; // [rsp+28h] [rbp-80h]
		//   __int64 v9; // [rsp+30h] [rbp-78h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 30208 && WORD2(a2) == 536 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setComputeScratchSize");
		//       v4 = sub_7FFC80564220((__int64)v10, v8 & 0xFFF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"maxNumWaves", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       v5 = sub_7FFC80564220((__int64)v10, (v8 >> 12) & 0x1FFF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"num1KByteChunksPerWave", (__int64)"uint32_t", v5);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v10);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setComputeScratchSize");
			// m_cb->setComputeScratchSize();
			count = kPacketCountSetComputeScratchSize;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setViewportTransformControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v7; // [rsp+28h] [rbp-80h]
		//   __int64 v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 0x6900 && WORD2(a2) == 0x206 )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setViewportTransformControl");
		//       v4 = sub_7FFC80564220((__int64)v9, v7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"vportControl", (__int64)"ViewportTransformControl", v4);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v9);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setViewportTransformControl");
			// m_cb->setViewportTransformControl();
			count = kPacketCountSetViewportTransformControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setClipControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v7; // [rsp+28h] [rbp-80h]
		//   __int64 v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 0x6900 && WORD2(a2) == 0x204 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setClipControl");
		//       v4 = sub_7FFC80564220((__int64)v9, v7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"reg", (__int64)"ClipControl", v4);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v9);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setClipControl");
			// m_cb->setClipControl();
			count = kPacketCountSetClipControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setUserClipPlane(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v3; // edx
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 a2; // [rsp+30h] [rbp-88h] BYREF
		//   char v10[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 24i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 26880 )
		//   {
		//     v3 = WORD2(a2) - 367;
		//     if ( v3 <= 0x14 )
		//     {
		//       v4 = sub_7FFC80564220((__int64)v10, v3 >> 2);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"clipPlane", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       v5 = sub_7FFC80564250(v10);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"x", (__int64)"float", v5);
		//       sub_7FFC805644A0((__int64)v10);
		//       v6 = sub_7FFC80564250(v10);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"y", (__int64)"float", v6);
		//       sub_7FFC805644A0((__int64)v10);
		//       v7 = sub_7FFC80564250(v10);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"z", (__int64)"float", v7);
		//       sub_7FFC805644A0((__int64)v10);
		//       v8 = sub_7FFC80564250(v10);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)L"w", (__int64)"float", v8);
		//       sub_7FFC805644A0((__int64)v10);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setUserClipPlane");
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setUserClipPlane");
			// m_cb->setUserClipPlane();
			count = kPacketCountSetUserClipPlane;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setClipRectangle(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   int a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned __int16 a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v10; // [rsp+30h] [rbp-78h]
		//   unsigned int v11; // [rsp+34h] [rbp-74h]
		//   char v12[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && (unsigned int)a2_4 - 132 <= 6
		//     && (a2 & 0x3FFF0000) == 0x20000 )
		//   {
		//     v3 = sub_7FFC80564220((__int64)v12, ((unsigned int)a2_4 - 132) >> 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"rectId", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v12);
		//     v4 = sub_7FFC80564220((__int64)v12, v10 & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"left", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v12);
		//     v5 = sub_7FFC80564220((__int64)v12, HIWORD(v10) & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"top", (__int64)"uint32_t", v5);
		//     sub_7FFC805644A0((__int64)v12);
		//     v6 = sub_7FFC80564220((__int64)v12, v11 & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"right", (__int64)"uint32_t", v6);
		//     sub_7FFC805644A0((__int64)v12);
		//     v7 = sub_7FFC80564220((__int64)v12, HIWORD(v11) & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"bottom", (__int64)"uint32_t", v7);
		//     sub_7FFC805644A0((__int64)v12);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setClipRectangle");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setClipRectangle");
			// m_cb->setClipRectangle();
			count = kPacketCountSetClipRectangle;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setClipRectangleRule(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   _WORD a2[8]; // [rsp+20h] [rbp-88h] BYREF
		//   __int64 v7; // [rsp+30h] [rbp-78h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     v3 = a2[0] & 0xFF00;
		//     if ( v3 == 26880 && a2[2] == 131 )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setClipRectangleRule");
		//       v4 = sub_7FFC80564220((__int64)v8, a2[4]);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"clipRule", (__int64)"uint16_t", v4);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v8);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setClipRectangleRule");
			// m_cb->setClipRectangleRule();
			count = kPacketCountSetClipRectangleRule;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPrimitiveSetup(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v7; // [rsp+28h] [rbp-80h]
		//   __int64 v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 517 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setPrimitiveSetup");
		//       v4 = sub_7FFC80564220((__int64)v9, v7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"reg", (__int64)"PrimitiveSetup", v4);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v9);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPrimitiveSetup");
			// m_cb->setPrimitiveSetup();
			count = kPacketCountSetPrimitiveSetup;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPrimitiveResetIndexEnable(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v7; // [rsp+28h] [rbp-80h]
		//   __int64 v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 677 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setPrimitiveResetIndexEnable");
		//       v4 = sub_7FFC80564220((__int64)v9, v7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"enable", (__int64)"bool", v4);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v9);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPrimitiveResetIndexEnable");
			// m_cb->setPrimitiveResetIndexEnable();
			count = kPacketCountSetPrimitiveResetIndexEnable;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPrimitiveResetIndex(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v7; // [rsp+28h] [rbp-80h]
		//   __int64 v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 259 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setPrimitiveResetIndex");
		//       v4 = sub_7FFC80564220((__int64)v9, v7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"resetIndex", (__int64)"uint32_t", v4);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v9);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPrimitiveResetIndex");
			// m_cb->setPrimitiveResetIndex();
			count = kPacketCountSetPrimitiveResetIndex;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setVertexQuantization(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v9; // [rsp+28h] [rbp-80h]
		//   __int64 v10; // [rsp+30h] [rbp-78h]
		//   char v11[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v10 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 761 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setVertexQuantization");
		//       v4 = sub_7FFC80564220((__int64)v11, (v9 >> 3) & 7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"quantizeMode", (__int64)"VertexQuantizationMode", v4);
		//       sub_7FFC805644A0((__int64)v11);
		//       v5 = sub_7FFC80564220((__int64)v11, (v9 >> 1) & 3);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"roundMode", (__int64)"VertexQuantizationRoundMode", v5);
		//       sub_7FFC805644A0((__int64)v11);
		//       v6 = sub_7FFC80564220((__int64)v11, v9 & 1);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"centerMode", (__int64)"VertexQuantizationCenterMode", v6);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v11);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setVertexQuantization");
			// m_cb->setVertexQuantization();
			count = kPacketCountSetVertexQuantization;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setWindowOffset(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v8; // [rsp+28h] [rbp-80h]
		//   __int64 v9; // [rsp+30h] [rbp-78h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 128 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setWindowOffset");
		//       v4 = sub_7FFC80564220((__int64)v10, (unsigned __int16)v8);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"offsetX", (__int64)"int16_t", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       v5 = sub_7FFC80564220((__int64)v10, HIWORD(v8));
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"offsetY", (__int64)"int16_t", v5);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v10);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setWindowOffset");
			// m_cb->setWindowOffset();
			count = kPacketCountSetWindowOffset;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setScreenScissor(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   int a2; // [rsp+28h] [rbp-80h] BYREF
		//   __int16 a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v9; // [rsp+30h] [rbp-78h]
		//   unsigned int v10; // [rsp+34h] [rbp-74h]
		//   char v11[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 12
		//     && (a2 & 0x3FFF0000) == 0x20000 )
		//   {
		//     v3 = sub_7FFC805641F0(v11, (unsigned __int16)v9);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"left", (__int64)"int32_t", v3);
		//     sub_7FFC805644A0((__int64)v11);
		//     v4 = sub_7FFC805641F0(v11, HIWORD(v9));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"top", (__int64)"int32_t", v4);
		//     sub_7FFC805644A0((__int64)v11);
		//     v5 = sub_7FFC805641F0(v11, (unsigned __int16)v10);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"right", (__int64)"int32_t", v5);
		//     sub_7FFC805644A0((__int64)v11);
		//     v6 = sub_7FFC805641F0(v11, HIWORD(v10));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"bottom", (__int64)"int32_t", v6);
		//     sub_7FFC805644A0((__int64)v11);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setScreenScissor");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setScreenScissor");
			// m_cb->setScreenScissor();
			count = kPacketCountSetScreenScissor;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setWindowScissor(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   int a2; // [rsp+28h] [rbp-80h] BYREF
		//   __int16 a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   int v10; // [rsp+30h] [rbp-78h]
		//   __int16 v11; // [rsp+34h] [rbp-74h]
		//   __int16 v12; // [rsp+36h] [rbp-72h]
		//   char v13[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 129
		//     && (a2 & 0x3FFF0000) == 0x20000 )
		//   {
		//     v3 = sub_7FFC805641F0(v13, v10 & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"left", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v13);
		//     v4 = sub_7FFC805641F0(v13, HIWORD(v10) & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"top", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v13);
		//     v5 = sub_7FFC805641F0(v13, v11 & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"right", (__int64)"uint32_t", v5);
		//     sub_7FFC805644A0((__int64)v13);
		//     v6 = sub_7FFC805641F0(v13, v12 & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"bottom", (__int64)"uint32_t", v6);
		//     sub_7FFC805644A0((__int64)v13);
		//     v7 = sub_7FFC80564220((__int64)v13, v10 >= 0);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"windowOffsetEnable", (__int64)"WindowOffsetMode", v7);
		//     sub_7FFC805644A0((__int64)v13);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setWindowScissor");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setWindowScissor");
			// m_cb->setWindowScissor();
			count = kPacketCountSetWindowScissor;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setGenericScissor(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   int a2; // [rsp+28h] [rbp-80h] BYREF
		//   __int16 a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   int v10; // [rsp+30h] [rbp-78h]
		//   __int16 v11; // [rsp+34h] [rbp-74h]
		//   __int16 v12; // [rsp+36h] [rbp-72h]
		//   char v13[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 144
		//     && (a2 & 0x3FFF0000) == 0x20000 )
		//   {
		//     v3 = sub_7FFC805641F0(v13, v10 & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"left", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v13);
		//     v4 = sub_7FFC805641F0(v13, HIWORD(v10) & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"top", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v13);
		//     v5 = sub_7FFC805641F0(v13, v11 & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"right", (__int64)"uint32_t", v5);
		//     sub_7FFC805644A0((__int64)v13);
		//     v6 = sub_7FFC805641F0(v13, v12 & 0x7FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"bottom", (__int64)"uint32_t", v6);
		//     sub_7FFC805644A0((__int64)v13);
		//     v7 = sub_7FFC80564220((__int64)v13, v10 >= 0);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"windowOffsetEnable", (__int64)"WindowOffsetMode", v7);
		//     sub_7FFC805644A0((__int64)v13);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setGenericScissor");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setGenericScissor");
			// m_cb->setGenericScissor();
			count = kPacketCountSetGenericScissor;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setViewportScissor(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v3; // edx
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   int a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned __int16 a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   int v12; // [rsp+30h] [rbp-78h]
		//   __int16 v13; // [rsp+34h] [rbp-74h]
		//   __int16 v14; // [rsp+36h] [rbp-72h]
		//   char v15[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 26880 )
		//   {
		//     v3 = a2_4 - 148;
		//     if ( v3 <= 0x1E && (a2 & 0x3FFF0000) == 0x20000 )
		//     {
		//       v4 = sub_7FFC80564220((__int64)v15, v3 >> 1);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"viewportId", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v15);
		//       v5 = sub_7FFC805641F0(v15, v12 & 0x7FFF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"left", (__int64)"uint32_t", v5);
		//       sub_7FFC805644A0((__int64)v15);
		//       v6 = sub_7FFC805641F0(v15, HIWORD(v12) & 0x7FFF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"top", (__int64)"uint32_t", v6);
		//       sub_7FFC805644A0((__int64)v15);
		//       v7 = sub_7FFC805641F0(v15, v13 & 0x7FFF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"right", (__int64)"uint32_t", v7);
		//       sub_7FFC805644A0((__int64)v15);
		//       v8 = sub_7FFC805641F0(v15, v14 & 0x7FFF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"bottom", (__int64)"uint32_t", v8);
		//       sub_7FFC805644A0((__int64)v15);
		//       v9 = sub_7FFC80564220((__int64)v15, v12 >= 0);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"windowOffsetEnable", (__int64)"WindowOffsetMode", v9);
		//       sub_7FFC805644A0((__int64)v15);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setViewportScissor");
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setViewportScissor");
			// m_cb->setViewportScissor();
			count = kPacketCountSetViewportScissor;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setViewport(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v3; // r8d
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   __int64 v10; // rax
		//   __int64 v11; // rax
		//   __int64 v12; // rax
		//   int a2; // [rsp+30h] [rbp-39h] BYREF
		//   unsigned __int16 a2_4; // [rsp+34h] [rbp-35h]
		//   int v15; // [rsp+40h] [rbp-29h]
		//   unsigned __int16 v16; // [rsp+44h] [rbp-25h]
		//   char v17[80]; // [rsp+60h] [rbp-9h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 48i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && (v15 & 0xFF00) == 26880 )
		//   {
		//     v3 = a2_4 - 180;
		//     if ( v3 <= 0x1E && (unsigned int)v16 - 271 <= 0x5A && (a2 & 0x3FFF0000) == 0x20000 && (v15 & 0x3FFF0000) == 393216 )
		//     {
		//       v4 = sub_7FFC80564220((__int64)v17, v3 >> 1);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"viewportId", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v17);
		//       v5 = sub_7FFC80564250(v17);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dmin", (__int64)"float", v5);
		//       sub_7FFC805644A0((__int64)v17);
		//       v6 = sub_7FFC80564250(v17);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dmax", (__int64)"float", v6);
		//       sub_7FFC805644A0((__int64)v17);
		//       v7 = sub_7FFC80564250(v17);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"scale[0]", (__int64)"float", v7);
		//       sub_7FFC805644A0((__int64)v17);
		//       v8 = sub_7FFC80564250(v17);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"scale[1]", (__int64)"float", v8);
		//       sub_7FFC805644A0((__int64)v17);
		//       v9 = sub_7FFC80564250(v17);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"scale[2]", (__int64)"float", v9);
		//       sub_7FFC805644A0((__int64)v17);
		//       v10 = sub_7FFC80564250(v17);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"offset[0]", (__int64)"float", v10);
		//       sub_7FFC805644A0((__int64)v17);
		//       v11 = sub_7FFC80564250(v17);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"offset[1]", (__int64)"float", v11);
		//       sub_7FFC805644A0((__int64)v17);
		//       v12 = sub_7FFC80564250(v17);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"offset[2]", (__int64)"float", v12);
		//       sub_7FFC805644A0((__int64)v17);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setViewport");
		//       set_packet_count((uint64_t)rdx0, 2u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setViewport");
			// m_cb->setViewport();
			count = kPacketCountSetViewport;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setScanModeControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v8; // [rsp+28h] [rbp-80h]
		//   __int64 v9; // [rsp+30h] [rbp-78h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 658 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setScanModeControl");
		//       v4 = sub_7FFC80564220((__int64)v10, v8 & 1);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"msaa", (__int64)"ScanModeControlAa", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       v5 = sub_7FFC80564220((__int64)v10, (v8 >> 1) & 1);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"viewportScissor", (__int64)"ScanModeControlViewportScissor", v5);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v10);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setScanModeControl");
			// m_cb->setScanModeControl();
			count = kPacketCountSetScanModeControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setAaSampleCount(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v8; // [rsp+28h] [rbp-80h]
		//   __int64 v9; // [rsp+30h] [rbp-78h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 760 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setAaSampleCount");
		//       v4 = sub_7FFC80564220((__int64)v10, v8 & 7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"logNumSamples", (__int64)"NumSamples", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       v5 = sub_7FFC80564220((__int64)v10, (v8 >> 13) & 0xF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"maxSampleDistance", (__int64)"uint32_t", v5);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v10);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setAaSampleCount");
			// m_cb->setAaSampleCount();
			count = kPacketCountSetAaSampleCount;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setAaSampleLocationControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   __int64 v10; // rax
		//   __int64 v11; // rax
		//   __int64 v12; // rax
		//   __int64 v13; // rax
		//   __int64 v14; // rax
		//   __int64 v15; // rax
		//   __int64 v16; // rax
		//   __int64 v17; // rax
		//   __int64 v18; // rax
		//   __int64 v19; // rax
		//   __int64 v20; // rax
		//   char v22[80]; // [rsp+30h] [rbp-69h] BYREF
		//   __int64 a2; // [rsp+80h] [rbp-19h] BYREF
		//   unsigned int v24; // [rsp+88h] [rbp-11h]
		//   unsigned int v25; // [rsp+8Ch] [rbp-Dh]
		//   unsigned int v26; // [rsp+90h] [rbp-9h]
		//   unsigned int v27; // [rsp+94h] [rbp-5h]
		//   unsigned int v28; // [rsp+98h] [rbp-1h]
		//   unsigned int v29; // [rsp+9Ch] [rbp+3h]
		//   unsigned int v30; // [rsp+A0h] [rbp+7h]
		//   unsigned int v31; // [rsp+A4h] [rbp+Bh]
		//   unsigned int v32; // [rsp+A8h] [rbp+Fh]
		//   unsigned int v33; // [rsp+ACh] [rbp+13h]
		//   unsigned int v34; // [rsp+B0h] [rbp+17h]
		//   unsigned int v35; // [rsp+B4h] [rbp+1Bh]
		//   unsigned int v36; // [rsp+B8h] [rbp+1Fh]
		//   unsigned int v37; // [rsp+BCh] [rbp+23h]
		//   unsigned int v38; // [rsp+C0h] [rbp+27h]
		//   unsigned int v39; // [rsp+C4h] [rbp+2Bh]
		//   int v40; // [rsp+C8h] [rbp+2Fh]
		//   __int16 v41; // [rsp+CCh] [rbp+33h]
		//   __int64 v42; // [rsp+D0h] [rbp+37h]
		//
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 88i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 766 )
		//     {
		//       v3 = v40 & 0xFF00;
		//       if ( v3 == 26880 && v41 == 757 )
		//       {
		//         set_packet_count((uint64_t)rdx0, 2u);
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"setAaSampleLocationControl");
		//         v4 = sub_7FFC80564220((__int64)v22, v24);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[0]", (__int64)"uint32_t", v4);
		//         sub_7FFC805644A0((__int64)v22);
		//         v5 = sub_7FFC80564220((__int64)v22, v25);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[1]", (__int64)"uint32_t", v5);
		//         sub_7FFC805644A0((__int64)v22);
		//         v6 = sub_7FFC80564220((__int64)v22, v26);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[2]", (__int64)"uint32_t", v6);
		//         sub_7FFC805644A0((__int64)v22);
		//         v7 = sub_7FFC80564220((__int64)v22, v27);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[3]", (__int64)"uint32_t", v7);
		//         sub_7FFC805644A0((__int64)v22);
		//         v8 = sub_7FFC80564220((__int64)v22, v28);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[4]", (__int64)"uint32_t", v8);
		//         sub_7FFC805644A0((__int64)v22);
		//         v9 = sub_7FFC80564220((__int64)v22, v29);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[5]", (__int64)"uint32_t", v9);
		//         sub_7FFC805644A0((__int64)v22);
		//         v10 = sub_7FFC80564220((__int64)v22, v30);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[6]", (__int64)"uint32_t", v10);
		//         sub_7FFC805644A0((__int64)v22);
		//         v11 = sub_7FFC80564220((__int64)v22, v31);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[7]", (__int64)"uint32_t", v11);
		//         sub_7FFC805644A0((__int64)v22);
		//         v12 = sub_7FFC80564220((__int64)v22, v32);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[8]", (__int64)"uint32_t", v12);
		//         sub_7FFC805644A0((__int64)v22);
		//         v13 = sub_7FFC80564220((__int64)v22, v33);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[9]", (__int64)"uint32_t", v13);
		//         sub_7FFC805644A0((__int64)v22);
		//         v14 = sub_7FFC80564220((__int64)v22, v34);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[10]", (__int64)"uint32_t", v14);
		//         sub_7FFC805644A0((__int64)v22);
		//         v15 = sub_7FFC80564220((__int64)v22, v35);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[11]", (__int64)"uint32_t", v15);
		//         sub_7FFC805644A0((__int64)v22);
		//         v16 = sub_7FFC80564220((__int64)v22, v36);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[12]", (__int64)"uint32_t", v16);
		//         sub_7FFC805644A0((__int64)v22);
		//         v17 = sub_7FFC80564220((__int64)v22, v37);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[13]", (__int64)"uint32_t", v17);
		//         sub_7FFC805644A0((__int64)v22);
		//         v18 = sub_7FFC80564220((__int64)v22, v38);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[14]", (__int64)"uint32_t", v18);
		//         sub_7FFC805644A0((__int64)v22);
		//         v19 = sub_7FFC80564220((__int64)v22, v39);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_locations[15]", (__int64)"uint32_t", v19);
		//         sub_7FFC805644A0((__int64)v22);
		//         v20 = sub_7FFC80564430(v22, v42);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"m_centroidPriority", (__int64)"uint64_t", v20);
		//         LOBYTE(v3) = sub_7FFC805644A0((__int64)v22);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setAaSampleLocationControl");
			// m_cb->setAaSampleLocationControl();
			count = kPacketCountSetAaSampleLocationControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPsShaderRate(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v7; // [rsp+28h] [rbp-80h]
		//   __int64 v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 659 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setPsShaderRate");
		//       v4 = sub_7FFC80564220((__int64)v9, HIWORD(v7) & 1);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"rate", (__int64)"PsShaderRate", v4);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v9);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPsShaderRate");
			// m_cb->setPsShaderRate();
			count = kPacketCountSetPsShaderRate;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setAaSampleMask(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v8; // [rsp+28h] [rbp-80h]
		//   int v9; // [rsp+2Ch] [rbp-7Ch]
		//   __int16 v10; // [rsp+30h] [rbp-78h]
		//   unsigned int v11; // [rsp+34h] [rbp-74h]
		//   __int64 v12; // [rsp+38h] [rbp-70h]
		//   char v13[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v12 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 24i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && a2_4 == 782 )
		//     {
		//       v3 = v9 & 0xFF00;
		//       if ( v3 == 26880 && v10 == 783 && (a2 & 0x3FFF0000) == 0x10000 && (v9 & 0x3FFF0000) == 0x10000 )
		//       {
		//         set_packet_count((uint64_t)rdx0, 2u);
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"setAaSampleMask");
		//         v4 = sub_7FFC80564430(v13, v8 | ((unsigned __int64)v11 << 32));
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"mask", (__int64)"uint64_t", v4);
		//         LOBYTE(v3) = sub_7FFC805644A0((__int64)v13);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setAaSampleMask");
			// m_cb->setAaSampleMask();
			count = kPacketCountSetAaSampleMask;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setAaSampleLocations(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   __int64 v10; // rax
		//   __int64 v11; // rax
		//   __int64 v12; // rax
		//   __int64 v13; // rax
		//   __int64 v14; // rax
		//   __int64 v15; // rax
		//   __int64 v16; // rax
		//   __int64 v17; // rax
		//   __int64 v18; // rax
		//   char v19[80]; // [rsp+30h] [rbp-59h] BYREF
		//   int a2; // [rsp+80h] [rbp-9h] BYREF
		//   __int16 a2_4; // [rsp+84h] [rbp-5h]
		//   unsigned int v22; // [rsp+88h] [rbp-1h]
		//   unsigned int v23; // [rsp+8Ch] [rbp+3h]
		//   unsigned int v24; // [rsp+90h] [rbp+7h]
		//   unsigned int v25; // [rsp+94h] [rbp+Bh]
		//   unsigned int v26; // [rsp+98h] [rbp+Fh]
		//   unsigned int v27; // [rsp+9Ch] [rbp+13h]
		//   unsigned int v28; // [rsp+A0h] [rbp+17h]
		//   unsigned int v29; // [rsp+A4h] [rbp+1Bh]
		//   unsigned int v30; // [rsp+A8h] [rbp+1Fh]
		//   unsigned int v31; // [rsp+ACh] [rbp+23h]
		//   unsigned int v32; // [rsp+B0h] [rbp+27h]
		//   unsigned int v33; // [rsp+B4h] [rbp+2Bh]
		//   unsigned int v34; // [rsp+B8h] [rbp+2Fh]
		//   unsigned int v35; // [rsp+BCh] [rbp+33h]
		//   unsigned int v36; // [rsp+C0h] [rbp+37h]
		//   unsigned int v37; // [rsp+C4h] [rbp+3Bh]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 72i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 766
		//     && (a2 & 0x3FFF0000) == 0x100000 )
		//   {
		//     v3 = sub_7FFC80564220((__int64)v19, v22);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[0]", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v19);
		//     v4 = sub_7FFC80564220((__int64)v19, v23);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[1]", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v19);
		//     v5 = sub_7FFC80564220((__int64)v19, v24);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[2]", (__int64)"uint32_t", v5);
		//     sub_7FFC805644A0((__int64)v19);
		//     v6 = sub_7FFC80564220((__int64)v19, v25);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[3]", (__int64)"uint32_t", v6);
		//     sub_7FFC805644A0((__int64)v19);
		//     v7 = sub_7FFC80564220((__int64)v19, v26);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[4]", (__int64)"uint32_t", v7);
		//     sub_7FFC805644A0((__int64)v19);
		//     v8 = sub_7FFC80564220((__int64)v19, v27);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[5]", (__int64)"uint32_t", v8);
		//     sub_7FFC805644A0((__int64)v19);
		//     v9 = sub_7FFC80564220((__int64)v19, v28);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[6]", (__int64)"uint32_t", v9);
		//     sub_7FFC805644A0((__int64)v19);
		//     v10 = sub_7FFC80564220((__int64)v19, v29);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[7]", (__int64)"uint32_t", v10);
		//     sub_7FFC805644A0((__int64)v19);
		//     v11 = sub_7FFC80564220((__int64)v19, v30);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[8]", (__int64)"uint32_t", v11);
		//     sub_7FFC805644A0((__int64)v19);
		//     v12 = sub_7FFC80564220((__int64)v19, v31);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[9]", (__int64)"uint32_t", v12);
		//     sub_7FFC805644A0((__int64)v19);
		//     v13 = sub_7FFC80564220((__int64)v19, v32);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[10]", (__int64)"uint32_t", v13);
		//     sub_7FFC805644A0((__int64)v19);
		//     v14 = sub_7FFC80564220((__int64)v19, v33);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[11]", (__int64)"uint32_t", v14);
		//     sub_7FFC805644A0((__int64)v19);
		//     v15 = sub_7FFC80564220((__int64)v19, v34);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[12]", (__int64)"uint32_t", v15);
		//     sub_7FFC805644A0((__int64)v19);
		//     v16 = sub_7FFC80564220((__int64)v19, v35);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[13]", (__int64)"uint32_t", v16);
		//     sub_7FFC805644A0((__int64)v19);
		//     v17 = sub_7FFC80564220((__int64)v19, v36);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[14]", (__int64)"uint32_t", v17);
		//     sub_7FFC805644A0((__int64)v19);
		//     v18 = sub_7FFC80564220((__int64)v19, v37);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"samples[15]", (__int64)"uint32_t", v18);
		//     sub_7FFC805644A0((__int64)v19);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setAaSampleLocations");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_WORD *)rdx0 + 12) = 257;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setAaSampleLocations");
			// m_cb->setAaSampleLocations();
			count = kPacketCountSetAaSampleLocations;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setCentroidPriority(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   int a2; // [rsp+28h] [rbp-80h] BYREF
		//   __int16 a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 757
		//     && (a2 & 0x3FFF0000) == 0x20000 )
		//   {
		//     v3 = sub_7FFC80564430(v7, v6);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"priority", (__int64)"uint64_t", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setCentroidPriority");
		//     *((_BYTE *)rdx0 + 25) = 1;
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setCentroidPriority");
			// m_cb->setCentroidPriority();
			count = kPacketCountSetCentroidPriority;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setLineWidth(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   _WORD a2[8]; // [rsp+20h] [rbp-88h] BYREF
		//   __int64 v7; // [rsp+30h] [rbp-78h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2[0] & 0xFF00;
		//     if ( v3 == 26880 && a2[2] == 642 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setLineWidth");
		//       v4 = sub_7FFC80564220((__int64)v8, a2[4]);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"widthIn8ths", (__int64)"uint16_t", v4);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v8);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setLineWidth");
			// m_cb->setLineWidth();
			count = kPacketCountSetLineWidth;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPointSize(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v8; // [rsp+28h] [rbp-80h]
		//   __int64 v9; // [rsp+30h] [rbp-78h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 640 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setPointSize");
		//       v4 = sub_7FFC80564220((__int64)v10, HIWORD(v8));
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"halfWidth", (__int64)"uint16_t", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       v5 = sub_7FFC80564220((__int64)v10, (unsigned __int16)v8);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"halfHeight", (__int64)"uint16_t", v5);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v10);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPointSize");
			// m_cb->setPointSize();
			count = kPacketCountSetPointSize;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPointMinMax(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v8; // [rsp+28h] [rbp-80h]
		//   __int64 v9; // [rsp+30h] [rbp-78h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 641 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setPointMinMax");
		//       v4 = sub_7FFC80564220((__int64)v10, (unsigned __int16)v8);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"minRadius", (__int64)"uint16_t", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       v5 = sub_7FFC80564220((__int64)v10, HIWORD(v8));
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"maxRadius", (__int64)"uint16_t", v5);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v10);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPointMinMax");
			// m_cb->setPointMinMax();
			count = kPacketCountSetPointMinMax;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPolygonOffsetClamp(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 a2; // [rsp+28h] [rbp-80h] BYREF
		//   __int64 v7; // [rsp+38h] [rbp-70h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 735 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setPolygonOffsetClamp");
		//       v4 = sub_7FFC80564250(v8);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"clamp", (__int64)"float", v4);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v8);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPolygonOffsetClamp");
			// m_cb->setPolygonOffsetClamp();
			count = kPacketCountSetPolygonOffsetClamp;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPolygonOffsetZFormat(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   unsigned int v4; // [rsp+20h] [rbp-88h] BYREF
		//   __int64 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int v6; // [rsp+30h] [rbp-78h]
		//   __int64 v7; // [rsp+38h] [rbp-70h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 734 )
		//   {
		//     if ( (unsigned __int8)sub_7FFC8056DC10(v6, &v4) )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setPolygonOffsetZFormat");
		//       v3 = sub_7FFC80564220((__int64)v8, v4);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"format", (__int64)"ZFormat", v3);
		//       sub_7FFC805644A0((__int64)v8);
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPolygonOffsetZFormat");
			// m_cb->setPolygonOffsetZFormat();
			count = kPacketCountSetPolygonOffsetZFormat;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPolygonOffsetFront(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   int a2; // [rsp+30h] [rbp-78h] BYREF
		//   __int16 a2_4; // [rsp+34h] [rbp-74h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 736
		//     && (a2 & 0x3FFF0000) == 0x20000 )
		//   {
		//     v3 = sub_7FFC80564250(v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"scale", (__int64)"float", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     v4 = sub_7FFC80564250(v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"offset", (__int64)"float", v4);
		//     sub_7FFC805644A0((__int64)v7);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setPolygonOffsetFront");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPolygonOffsetFront");
			// m_cb->setPolygonOffsetFront();
			count = kPacketCountSetPolygonOffsetFront;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPolygonOffsetBack(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   int a2; // [rsp+30h] [rbp-78h] BYREF
		//   __int16 a2_4; // [rsp+34h] [rbp-74h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 738
		//     && (a2 & 0x3FFF0000) == 0x20000 )
		//   {
		//     v3 = sub_7FFC80564250(v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"scale", (__int64)"float", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     v4 = sub_7FFC80564250(v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"offset", (__int64)"float", v4);
		//     sub_7FFC805644A0((__int64)v7);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setPolygonOffsetBack");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPolygonOffsetBack");
			// m_cb->setPolygonOffsetBack();
			count = kPacketCountSetPolygonOffsetBack;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setHardwareScreenOffset(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v8; // [rsp+28h] [rbp-80h]
		//   __int64 v9; // [rsp+30h] [rbp-78h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 141 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setHardwareScreenOffset");
		//       v4 = sub_7FFC80564220((__int64)v10, v8 & 0x1FF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"offsetX", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       v5 = sub_7FFC80564220((__int64)v10, HIWORD(v8) & 0x1FF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"offsetY", (__int64)"uint32_t", v5);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v10);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setHardwareScreenOffset");
			// m_cb->setHardwareScreenOffset();
			count = kPacketCountSetHardwareScreenOffset;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setGuardBandClip(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   _WORD a2[10]; // [rsp+28h] [rbp-90h] BYREF
		//   __int64 v6; // [rsp+40h] [rbp-78h]
		//   char v7[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)a2, 24i64, *(_DWORD *)(a1 + 8))
		//     && (a2[0] & 0xFF00) == 26880
		//     && a2[2] == 762
		//     && (a2[6] & 0xFF00) == 26880
		//     && a2[8] == 764 )
		//   {
		//     v3 = sub_7FFC80564250(v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"horzClip", (__int64)"float", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     v4 = sub_7FFC80564250(v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"vertClip", (__int64)"float", v4);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 2u);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setGuardBandClip");
		//     *((_WORD *)rdx0 + 12) = 257;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setGuardBandClip");
			// m_cb->setGuardBandClip();
			count = kPacketCountSetGuardBandClip;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setGuardBandDiscard(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   _WORD a2[10]; // [rsp+28h] [rbp-90h] BYREF
		//   __int64 v6; // [rsp+40h] [rbp-78h]
		//   char v7[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)a2, 24i64, *(_DWORD *)(a1 + 8))
		//     && (a2[0] & 0xFF00) == 26880
		//     && a2[2] == 763
		//     && (a2[6] & 0xFF00) == 26880
		//     && a2[8] == 765 )
		//   {
		//     v3 = sub_7FFC80564250(v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"horzClip", (__int64)"float", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     v4 = sub_7FFC80564250(v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"vertClip", (__int64)"float", v4);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 2u);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setGuardBandDiscard");
		//     *((_WORD *)rdx0 + 12) = 257;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setGuardBandDiscard");
			// m_cb->setGuardBandDiscard();
			count = kPacketCountSetGuardBandDiscard;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setInstanceStepRate(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   int a2; // [rsp+28h] [rbp-80h] BYREF
		//   __int16 a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v7; // [rsp+30h] [rbp-78h]
		//   unsigned int v8; // [rsp+34h] [rbp-74h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 680
		//     && (a2 & 0x3FFF0000) == 0x20000 )
		//   {
		//     v3 = sub_7FFC80564220((__int64)v9, v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"step0", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, v8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"step1", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setInstanceStepRate");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setInstanceStepRate");
			// m_cb->setInstanceStepRate();
			count = kPacketCountSetInstanceStepRate;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPsShaderUsage(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v4; // ebx
		//   unsigned int v5; // esi
		//   int v6; // edi
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // [rsp+20h] [rbp-98h] BYREF
		//   unsigned int a2; // [rsp+28h] [rbp-90h] BYREF
		//   __int16 a2_4; // [rsp+2Ch] [rbp-8Ch]
		//   __int64 v12; // [rsp+30h] [rbp-88h]
		//   char v13[80]; // [rsp+40h] [rbp-78h] BYREF
		//
		//   v12 = -2i64;
		//   if ( GetOpCodeType(a1, (__int64)&a2, 8i64, *((_DWORD *)a1 + 2)) && (a2 & 0xFF00) == 26880 && a2_4 == 401 )
		//   {
		//     v4 = HIWORD(a2) & 0x3FFF;
		//     v5 = 0;
		//     if ( v4 )
		//     {
		//       v6 = 0;
		//       while ( GetOpCodeType(a1, (__int64)&v9, 4i64, v6 + *((_DWORD *)a1 + 2) + 8) )
		//       {
		//         v7 = sub_7FFC80564220((__int64)v13, v9);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"inputTable", (__int64)"uint32_t", v7);
		//         sub_7FFC805644A0((__int64)v13);
		//         ++v5;
		//         v6 += 4;
		//         if ( v5 >= v4 )
		//           goto LABEL_8;
		//       }
		//     }
		//     else
		//     {
		// LABEL_8:
		//       v8 = sub_7FFC80564220((__int64)v13, v4);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"numItems", (__int64)"uint32_t", v8);
		//       sub_7FFC805644A0((__int64)v13);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setPsShaderUsage");
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPsShaderUsage");
			// m_cb->setPsShaderUsage();
			count = kPacketCountSetPsShaderUsage;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setRenderOverrideControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 26880 && WORD2(a2) == 3 )
		//   {
		//     v3 = sub_7FFC80564220((__int64)v7, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"renderOverrideControl.m_reg", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setRenderOverrideControl");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setRenderOverrideControl");
			// m_cb->setRenderOverrideControl();
			count = kPacketCountSetRenderOverrideControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setRenderOverride2Control(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 26880 && WORD2(a2) == 4 )
		//   {
		//     v3 = sub_7FFC80564220((__int64)v7, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"renderOverride2Control.m_reg", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setRenderOverride2Control");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setRenderOverride2Control");
			// m_cb->setRenderOverride2Control();
			count = kPacketCountSetRenderOverride2Control;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setActiveShaderStages(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v7; // [rsp+28h] [rbp-80h]
		//   __int64 v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 725 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setActiveShaderStages");
		//       v4 = sub_7FFC80564220((__int64)v9, v7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"activeStages", (__int64)"ActiveShaderStages", v4);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v9);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setActiveShaderStages");
			// m_cb->setActiveShaderStages();
			count = kPacketCountSetActiveShaderStages;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setGsMode(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v4; // ebx
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   int packet_size; // eax
		//   unsigned int a2; // [rsp+20h] [rbp-98h] BYREF
		//   __int16 a2_4; // [rsp+24h] [rbp-94h]
		//   unsigned int v10; // [rsp+28h] [rbp-90h]
		//   _WORD v11[8]; // [rsp+30h] [rbp-88h] BYREF
		//   __int64 v12; // [rsp+40h] [rbp-78h]
		//   char v13[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   v12 = -2i64;
		//   if ( GetOpCodeType(a1, (__int64)&a2, 12i64, *((_DWORD *)a1 + 2)) && (a2 & 0xFF00) == 26880 && a2_4 == 656 )
		//   {
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setGsMode");
		//     v4 = v10 & 0xFFFFFFCF;
		//     v5 = sub_7FFC80564220((__int64)v13, v10 & 0xFFFFFFCF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"mode", (__int64)"GsMode", v5);
		//     sub_7FFC805644A0((__int64)v13);
		//     v6 = sub_7FFC80564220((__int64)v13, (v10 >> 4) & 3);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"maxPrimDwordSize", (__int64)"GsMaxOutputPrimitiveDwordSize", v6);
		//     sub_7FFC805644A0((__int64)v13);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//     if ( !v4 )
		//     {
		//       packet_size = get_packet_size(a2);
		//       if ( GetOpCodeType(a1, (__int64)v11, 12i64, packet_size + *((_DWORD *)a1 + 2))
		//         && (v11[0] & 0xFF00) == 26880
		//         && v11[2] == 741 )
		//       {
		//         set_packet_count((uint64_t)rdx0, 2u);
		//       }
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setGsMode");
			// m_cb->setGsMode();
			count = kPacketCountSetGsMode;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setGsOnChipControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v8; // [rsp+28h] [rbp-80h]
		//   __int64 v9; // [rsp+30h] [rbp-78h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 657 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setGsOnChipControl");
		//       v4 = sub_7FFC80564220((__int64)v10, v8 & 0x7FF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"esVerticesPerSubGroup", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       v5 = sub_7FFC80564220((__int64)v10, (v8 >> 11) & 0x7FF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"gsInputPrimitivesPerSubGroup", (__int64)"uint32_t", v5);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v10);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setGsOnChipControl");
			// m_cb->setGsOnChipControl();
			count = kPacketCountSetGsOnChipControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setBorderColorTableAddr(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v7; // [rsp+28h] [rbp-80h]
		//   __int64 v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 26880 && WORD2(a2) == 32 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setBorderColorTableAddr");
		//       v4 = sub_7FFC80564430(v9, (unsigned __int64)v7 << 8);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"tableAddr", (__int64)"void*", v4);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v9);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setBorderColorTableAddr");
			// m_cb->setBorderColorTableAddr();
			count = kPacketCountSetBorderColorTableAddr;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPsShader(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+28h] [rbp-90h] BYREF
		//   int v5; // [rsp+30h] [rbp-88h]
		//   int v6; // [rsp+34h] [rbp-84h]
		//   int v7; // [rsp+38h] [rbp-80h]
		//   __int16 v8; // [rsp+3Ch] [rbp-7Ch]
		//   int v9; // [rsp+40h] [rbp-78h]
		//   int v10; // [rsp+44h] [rbp-74h]
		//   char v11[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 32i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && WORD2(a2) == 8
		//     && (v7 & 0xFF00) == 26880
		//     && v8 == 515
		//     && (v10 & 0xFF00) == 4096
		//     && !v5
		//     && !v6
		//     && !v9 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setPsShader");
		//     v3 = sub_7FFC80564430(v11, 0i64);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"psRegs", (__int64)"PsStageRegisters*", v3);
		//     sub_7FFC805644A0((__int64)v11);
		//     set_packet_count((uint64_t)rdx0, 3u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPsShader");
			// m_cb->setPsShader();
			count = kPacketCountSetPsShader;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPsShader2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+28h] [rbp-A0h] BYREF
		//   int v5; // [rsp+30h] [rbp-98h]
		//   int v6; // [rsp+34h] [rbp-94h]
		//   int v7; // [rsp+38h] [rbp-90h]
		//   __int16 v8; // [rsp+3Ch] [rbp-8Ch]
		//   int v9; // [rsp+40h] [rbp-88h]
		//   int v10; // [rsp+44h] [rbp-84h]
		//   __int16 v11; // [rsp+48h] [rbp-80h]
		//   int v12; // [rsp+50h] [rbp-78h]
		//   char v13[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 44i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && WORD2(a2) == 8
		//     && (v7 & 0xFF00) == 26880
		//     && v8 == 515
		//     && (v10 & 0xFF00) == 26880
		//     && v11 == 143
		//     && (v12 & 0xFF00) == 4096
		//     && !v5
		//     && !v6
		//     && !v9 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setPsShader");
		//     v3 = sub_7FFC80564430(v13, 0i64);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"psRegs", (__int64)"PsStageRegisters*", v3);
		//     sub_7FFC805644A0((__int64)v13);
		//     set_packet_count((uint64_t)rdx0, 4u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPsShader");
			// m_cb->setPsShader();
			count = kPacketCountSetPsShader2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPsShader3(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rbx
		//   char *v4; // rdx
		//   __int64 v5; // rax
		//   __int64 a2; // [rsp+30h] [rbp-D8h] BYREF
		//   __int64 v7; // [rsp+38h] [rbp-D0h]
		//   int v8; // [rsp+40h] [rbp-C8h]
		//   __int16 v9; // [rsp+44h] [rbp-C4h]
		//   int v10; // [rsp+50h] [rbp-B8h]
		//   __int16 v11; // [rsp+54h] [rbp-B4h]
		//   int v12; // [rsp+60h] [rbp-A8h]
		//   __int16 v13; // [rsp+64h] [rbp-A4h]
		//   int v14; // [rsp+70h] [rbp-98h]
		//   __int16 v15; // [rsp+74h] [rbp-94h]
		//   int v16; // [rsp+7Ch] [rbp-8Ch]
		//   __int16 v17; // [rsp+80h] [rbp-88h]
		//   int v18; // [rsp+88h] [rbp-80h]
		//   __int16 v19; // [rsp+8Ch] [rbp-7Ch]
		//   int v20; // [rsp+94h] [rbp-74h]
		//   __int16 v21; // [rsp+98h] [rbp-70h]
		//   char v22[80]; // [rsp+A0h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 112i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && WORD2(a2) == 8
		//     && (v8 & 0xFF00) == 30208
		//     && v9 == 10
		//     && (v10 & 0xFF00) == 26880
		//     && v11 == 452
		//     && (v12 & 0xFF00) == 26880
		//     && v13 == 435
		//     && (v14 & 0xFF00) == 26880
		//     && v15 == 438
		//     && (v16 & 0xFF00) == 26880
		//     && v17 == 440
		//     && (v18 & 0xFF00) == 26880
		//     && v19 == 515
		//     && (v20 & 0xFF00) == 26880
		//     && v21 == 143 )
		//   {
		//     v3 = v7 << 8;
		//     v4 = "setEmbeddedPsShader";
		//     if ( (unsigned __int64)((v7 << 8) - 0xFE0000000i64) > 0xFFFF )
		//       v4 = "setPsShader";
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(rdx0, (struct Concurrency::details::SchedulerNode *)v4);
		//     v5 = sub_7FFC80564430(v22, v3);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"shader", (__int64)"PsShader*", v5);
		//     sub_7FFC805644A0((__int64)v22);
		//     set_packet_count((uint64_t)rdx0, 9u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPsShader");
			// m_cb->setPsShader();
			count = kPacketCountSetPsShader3;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setVsShader(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rbx
		//   char *v4; // rdx
		//   __int64 v5; // rax
		//   __int64 a2; // [rsp+30h] [rbp-B8h] BYREF
		//   __int64 v7; // [rsp+38h] [rbp-B0h]
		//   int v8; // [rsp+40h] [rbp-A8h]
		//   __int16 v9; // [rsp+44h] [rbp-A4h]
		//   int v10; // [rsp+50h] [rbp-98h]
		//   __int16 v11; // [rsp+54h] [rbp-94h]
		//   int v12; // [rsp+5Ch] [rbp-8Ch]
		//   __int16 v13; // [rsp+60h] [rbp-88h]
		//   int v14; // [rsp+68h] [rbp-80h]
		//   __int16 v15; // [rsp+6Ch] [rbp-7Ch]
		//   char v16[80]; // [rsp+80h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 68i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && WORD2(a2) == 72
		//     && (v8 & 0xFF00) == 30208
		//     && v9 == 74
		//     && (v10 & 0xFF00) == 26880
		//     && v11 == 519
		//     && (v12 & 0xFF00) == 26880
		//     && v13 == 433
		//     && (v14 & 0xFF00) == 26880
		//     && v15 == 451 )
		//   {
		//     v3 = v7 << 8;
		//     v4 = "setEmbeddedVsShader";
		//     if ( (unsigned __int64)((v7 << 8) - 0xFE0000000i64) > 0xFFFF )
		//       v4 = "setVsShader";
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(rdx0, (struct Concurrency::details::SchedulerNode *)v4);
		//     v5 = sub_7FFC80564430(v16, v3);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"shader", (__int64)"VsShader*", v5);
		//     sub_7FFC805644A0((__int64)v16);
		//     set_packet_count((uint64_t)rdx0, 6u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setVsShader");
			// m_cb->setVsShader();
			count = kPacketCountSetVsShader;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setEsShader(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+28h] [rbp-90h] BYREF
		//   __int64 v5; // [rsp+30h] [rbp-88h]
		//   int v6; // [rsp+38h] [rbp-80h]
		//   __int16 v7; // [rsp+3Ch] [rbp-7Ch]
		//   char v8[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 32i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 0x7600
		//     && WORD2(a2) == 0xC8
		//     && (v6 & 0xFF00) == 0x7600
		//     && v7 == 0xCA )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setEsShader");
		//     v3 = sub_7FFC80564430(v8, v5 << 8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"shader", (__int64)"EsShader*", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     set_packet_count((uint64_t)rdx0, 3u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setEsShader");
			// m_cb->setEsShader();
			count = kPacketCountSetEsShader;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setGsShader(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+30h] [rbp-B8h] BYREF
		//   __int64 v5; // [rsp+38h] [rbp-B0h]
		//   int v6; // [rsp+40h] [rbp-A8h]
		//   __int16 v7; // [rsp+44h] [rbp-A4h]
		//   int v8; // [rsp+50h] [rbp-98h]
		//   __int16 v9; // [rsp+54h] [rbp-94h]
		//   int v10; // [rsp+5Ch] [rbp-8Ch]
		//   __int16 v11; // [rsp+60h] [rbp-88h]
		//   int v12; // [rsp+68h] [rbp-80h]
		//   __int16 v13; // [rsp+6Ch] [rbp-7Ch]
		//   char v14[80]; // [rsp+80h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 68i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && WORD2(a2) == 136
		//     && (v6 & 0xFF00) == 30208
		//     && v7 == 138
		//     && (v8 & 0xFF00) == 26880
		//     && v9 == 741
		//     && (v10 & 0xFF00) == 26880
		//     && v11 == 667
		//     && (v12 & 0xFF00) == 26880
		//     && v13 == 740 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setGsShader");
		//     v3 = sub_7FFC80564430(v14, v5 << 8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"shader", (__int64)"GsShader*", v3);
		//     sub_7FFC805644A0((__int64)v14);
		//     set_packet_count((uint64_t)rdx0, 6u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setGsShader");
			// m_cb->setGsShader();
			count = kPacketCountSetGsShader;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setCsShader(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+28h] [rbp-A0h] BYREF
		//   __int64 v5; // [rsp+30h] [rbp-98h]
		//   int v6; // [rsp+38h] [rbp-90h]
		//   __int16 v7; // [rsp+3Ch] [rbp-8Ch]
		//   int v8; // [rsp+48h] [rbp-80h]
		//   __int16 v9; // [rsp+4Ch] [rbp-7Ch]
		//   char v10[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 52i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && WORD2(a2) == 524
		//     && (v6 & 0xFF00) == 30208
		//     && v7 == 530
		//     && (v8 & 0xFF00) == 30208
		//     && v9 == 519 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setCsShader");
		//     v3 = sub_7FFC80564430(v10, v5 << 8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"shader", (__int64)"CsShader*", v3);
		//     sub_7FFC805644A0((__int64)v10);
		//     set_packet_count((uint64_t)rdx0, 4u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setCsShader");
			// m_cb->setCsShader();
			count = kPacketCountSetCsShader;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setLsShader(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+28h] [rbp-A0h] BYREF
		//   __int64 v5; // [rsp+30h] [rbp-98h]
		//   int v6; // [rsp+38h] [rbp-90h]
		//   __int16 v7; // [rsp+3Ch] [rbp-8Ch]
		//   int v8; // [rsp+44h] [rbp-84h]
		//   __int16 v9; // [rsp+48h] [rbp-80h]
		//   char v10[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 48i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && WORD2(a2) == 328
		//     && (v6 & 0xFF00) == 30208
		//     && v7 == 331
		//     && (v8 & 0xFF00) == 30208
		//     && v9 == 330 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setLsShader");
		//     v3 = sub_7FFC80564430(v10, v5 << 8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"shader", (__int64)"LsShader*", v3);
		//     sub_7FFC805644A0((__int64)v10);
		//     set_packet_count((uint64_t)rdx0, 4u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setLsShader");
			// m_cb->setLsShader();
			count = kPacketCountSetLsShader;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setHsShader(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+30h] [rbp-B8h] BYREF
		//   __int64 v5; // [rsp+38h] [rbp-B0h]
		//   int v6; // [rsp+40h] [rbp-A8h]
		//   __int16 v7; // [rsp+44h] [rbp-A4h]
		//   int v8; // [rsp+50h] [rbp-98h]
		//   __int16 v9; // [rsp+54h] [rbp-94h]
		//   int v10; // [rsp+60h] [rbp-88h]
		//   __int16 v11; // [rsp+64h] [rbp-84h]
		//   int v12; // [rsp+6Ch] [rbp-7Ch]
		//   __int16 v13; // [rsp+70h] [rbp-78h]
		//   char v14[80]; // [rsp+80h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 72i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && WORD2(a2) == 264
		//     && (v6 & 0xFF00) == 30208
		//     && v7 == 266
		//     && (v8 & 0xFF00) == 26880
		//     && v9 == 646
		//     && (v10 & 0xFF00) == 26880
		//     && v11 == 731
		//     && (v12 & 0xFF00) == 26880
		//     && v13 == 726 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setHsShader");
		//     v3 = sub_7FFC80564430(v14, v5 << 8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"shader", (__int64)"HsShader*", v3);
		//     sub_7FFC805644A0((__int64)v14);
		//     set_packet_count((uint64_t)rdx0, 6u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setHsShader");
			// m_cb->setHsShader();
			count = kPacketCountSetHsShader;
		}

		return count;
	}

	uint32_t GnmCommandProxy::updatePsShader(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+30h] [rbp-D8h] BYREF
		//   __int64 v5; // [rsp+38h] [rbp-D0h]
		//   int v6; // [rsp+40h] [rbp-C8h]
		//   __int16 v7; // [rsp+44h] [rbp-C4h]
		//   int v8; // [rsp+54h] [rbp-B4h]
		//   int v9; // [rsp+64h] [rbp-A4h]
		//   int v10; // [rsp+74h] [rbp-94h]
		//   int v11; // [rsp+80h] [rbp-88h]
		//   int v12; // [rsp+8Ch] [rbp-7Ch]
		//   int v13; // [rsp+98h] [rbp-70h]
		//   char v14[80]; // [rsp+A0h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 112i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && WORD2(a2) == 8
		//     && (v6 & 0xFF00) == 30208
		//     && v7 == 10
		//     && (v8 & 0xFFFF0000) == -1071775744
		//     && (_WORD)v8 == 452
		//     && (v9 & 0xFFFF0000) == -1071775744
		//     && (_WORD)v9 == 435
		//     && (v10 & 0xFFFF0000) == -1071775744
		//     && (_WORD)v10 == 438
		//     && (v11 & 0xFFFF0000) == -1071775744
		//     && (_WORD)v11 == 440
		//     && (v12 & 0xFFFF0000) == -1071775744
		//     && (_WORD)v12 == 515
		//     && (v13 & 0xFFFF0000) == -1071775744
		//     && (_WORD)v13 == 143 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"updatePsShader");
		//     v3 = sub_7FFC80564430(v14, v5 << 8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"shader", (__int64)"PsShader*", v3);
		//     sub_7FFC805644A0((__int64)v14);
		//     set_packet_count((uint64_t)rdx0, 9u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: updatePsShader");
			// m_cb->updatePsShader();
			count = kPacketCountUpdatePsShader;
		}

		return count;
	}

	uint32_t GnmCommandProxy::updateVsShader(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+30h] [rbp-B8h] BYREF
		//   __int64 v5; // [rsp+38h] [rbp-B0h]
		//   int v6; // [rsp+40h] [rbp-A8h]
		//   __int16 v7; // [rsp+44h] [rbp-A4h]
		//   int v8; // [rsp+54h] [rbp-94h]
		//   int v9; // [rsp+60h] [rbp-88h]
		//   int v10; // [rsp+6Ch] [rbp-7Ch]
		//   char v11[80]; // [rsp+80h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 68i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && WORD2(a2) == 72
		//     && (v6 & 0xFF00) == 30208
		//     && v7 == 74
		//     && (v8 & 0xFFFF0000) == -1071775744
		//     && (_WORD)v8 == 519
		//     && (v9 & 0xFFFF0000) == -1071775744
		//     && (_WORD)v9 == 433
		//     && (v10 & 0xFFFF0000) == -1071775744
		//     && (_WORD)v10 == 451 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"updateVsShader");
		//     v3 = sub_7FFC80564430(v11, v5 << 8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"shader", (__int64)"VsShader*", v3);
		//     sub_7FFC805644A0((__int64)v11);
		//     set_packet_count((uint64_t)rdx0, 6u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: updateVsShader");
			// m_cb->updateVsShader();
			count = kPacketCountUpdateVsShader;
		}

		return count;
	}

	uint32_t GnmCommandProxy::updateGsShader(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+30h] [rbp-B8h] BYREF
		//   __int64 v5; // [rsp+38h] [rbp-B0h]
		//   int v6; // [rsp+40h] [rbp-A8h]
		//   __int16 v7; // [rsp+44h] [rbp-A4h]
		//   int v8; // [rsp+54h] [rbp-94h]
		//   int v9; // [rsp+60h] [rbp-88h]
		//   int v10; // [rsp+6Ch] [rbp-7Ch]
		//   char v11[80]; // [rsp+80h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 68i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && WORD2(a2) == 136
		//     && (v6 & 0xFF00) == 30208
		//     && v7 == 138
		//     && (v8 & 0xFFFF0000) == -1071775744
		//     && (_WORD)v8 == 741
		//     && (v9 & 0xFFFF0000) == -1071775744
		//     && (_WORD)v9 == 667
		//     && (v10 & 0xFFFF0000) == -1071775744
		//     && (_WORD)v10 == 740 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"updateGsShader");
		//     v3 = sub_7FFC80564430(v11, v5 << 8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"shader", (__int64)"GsShader*", v3);
		//     sub_7FFC805644A0((__int64)v11);
		//     set_packet_count((uint64_t)rdx0, 6u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: updateGsShader");
			// m_cb->updateGsShader();
			count = kPacketCountUpdateGsShader;
		}

		return count;
	}

	uint32_t GnmCommandProxy::updateHsShader(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+30h] [rbp-B8h] BYREF
		//   __int64 v5; // [rsp+38h] [rbp-B0h]
		//   int v6; // [rsp+40h] [rbp-A8h]
		//   __int16 v7; // [rsp+44h] [rbp-A4h]
		//   int v8; // [rsp+54h] [rbp-94h]
		//   int v9; // [rsp+64h] [rbp-84h]
		//   int v10; // [rsp+70h] [rbp-78h]
		//   char v11[80]; // [rsp+80h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 72i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && WORD2(a2) == 264
		//     && (v6 & 0xFF00) == 30208
		//     && v7 == 266
		//     && (v8 & 0xFFFF0000) == -1071775744
		//     && (_WORD)v8 == 646
		//     && (v9 & 0xFFFF0000) == -1071775744
		//     && (_WORD)v9 == 731
		//     && (v10 & 0xFFFF0000) == -1071775744
		//     && (_WORD)v10 == 726 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"updateHsShader");
		//     v3 = sub_7FFC80564430(v11, v5 << 8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"shader", (__int64)"HsShader*", v3);
		//     sub_7FFC805644A0((__int64)v11);
		//     set_packet_count((uint64_t)rdx0, 6u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: updateHsShader");
			// m_cb->updateHsShader();
			count = kPacketCountUpdateHsShader;
		}

		return count;
	}

	uint32_t GnmCommandProxy::waitOnCe(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 a2; // [rsp+30h] [rbp+8h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 8i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 34304 )
		//   {
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"waitOnCe");
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: waitOnCe");
			// m_cb->waitOnCe();
			count = kPacketCountWaitOnCe;
		}

		return count;
	}

	uint32_t GnmCommandProxy::incrementDeCounter(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 a2; // [rsp+30h] [rbp+8h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 8i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 34048 )
		//   {
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"incrementDeCounter");
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: incrementDeCounter");
			// m_cb->incrementDeCounter();
			count = kPacketCountIncrementDeCounter;
		}

		return count;
	}

	uint32_t GnmCommandProxy::readDataFromGds(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int16 a2; // [rsp+28h] [rbp-80h] BYREF
		//   __int16 a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v11; // [rsp+30h] [rbp-78h]
		//   int v12; // [rsp+34h] [rbp-74h]
		//   unsigned int v13; // [rsp+38h] [rbp-70h]
		//   char v14[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 20i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 18432 )
		//     {
		//       v3 = a2_4 & 0xF00;
		//       if ( v3 == 0x600 )
		//       {
		//         LOBYTE(v3) = 0;
		//         if ( (v12 & 0xE0000000) == 0x20000000 )
		//         {
		//           set_packet_count((uint64_t)rdx0, 1u);
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"readDataFromGds");
		//           v4 = sub_7FFC80564220((__int64)v14, a2_4 & 0x3F);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"eventType", (__int64)"EndOfShaderEventType", v4);
		//           sub_7FFC805644A0((__int64)v14);
		//           v5 = sub_7FFC80564430(v14, v11 | ((unsigned __int64)(unsigned __int16)v12 << 32));
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dstGpuAddr", (__int64)"void*", v5);
		//           sub_7FFC805644A0((__int64)v14);
		//           v6 = sub_7FFC80564220((__int64)v14, (unsigned __int16)v13);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"gdsOffsetInDwords", (__int64)"uint32_t", v6);
		//           sub_7FFC805644A0((__int64)v14);
		//           v7 = sub_7FFC80564220((__int64)v14, HIWORD(v13));
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"gdsSizeInDwords", (__int64)"uint32_t", v7);
		//           LOBYTE(v3) = sub_7FFC805644A0((__int64)v14);
		//           *((_BYTE *)rdx0 + 24) = 1;
		//         }
		//       }
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: readDataFromGds");
			// m_cb->readDataFromGds();
			count = kPacketCountReadDataFromGds;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setRenderTarget(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v3; // ecx
		//   unsigned int v4; // ebx
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   int v7[10]; // [rsp+30h] [rbp-C8h] BYREF
		//   __int64 v8; // [rsp+58h] [rbp-A0h]
		//   __int64 v9; // [rsp+60h] [rbp-98h]
		//   int v10; // [rsp+68h] [rbp-90h]
		//   int v11; // [rsp+6Ch] [rbp-8Ch]
		//   __int16 a2; // [rsp+70h] [rbp-88h] BYREF
		//   unsigned __int16 a2_4; // [rsp+74h] [rbp-84h]
		//   int v14; // [rsp+78h] [rbp-80h]
		//   int v15; // [rsp+7Ch] [rbp-7Ch]
		//   int v16; // [rsp+80h] [rbp-78h]
		//   int v17; // [rsp+84h] [rbp-74h]
		//   int v18; // [rsp+88h] [rbp-70h]
		//   int v19; // [rsp+90h] [rbp-68h]
		//   int v20; // [rsp+94h] [rbp-64h]
		//   int v21; // [rsp+98h] [rbp-60h]
		//   int v22; // [rsp+A0h] [rbp-58h]
		//   int v23; // [rsp+A4h] [rbp-54h]
		//   char v24[80]; // [rsp+A8h] [rbp-50h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 64i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 0x6900
		//     && (unsigned int)a2_4 + 0xA000 >= 0xA318 )
		//   {
		//     v3 = a2_4 - 792;
		//     v4 = v3 / 0xF;
		//     if ( v3 == 15 * (v3 / 0xF) && v4 < 8 )
		//     {
		//       v7[0] = v14;
		//       v7[1] = v15;
		//       v7[2] = v14;
		//       v7[3] = v15;
		//       v7[4] = v16;
		//       v7[5] = v17;
		//       v7[6] = 0;
		//       v7[7] = v19;
		//       v7[8] = v20;
		//       v7[9] = v21;
		//       v8 = 0i64;
		//       v9 = 0i64;
		//       v10 = 0;
		//       v11 = v23;
		//       if ( (v18 & 0xFF00) == 0x6900 && (v22 & 0xFF00) == 0x1000 )
		//       {
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"setRenderTarget");
		//         v5 = sub_7FFC80564220((__int64)v24, v4);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"rtSlot", (__int64)"uint32_t", v5);
		//         sub_7FFC805644A0((__int64)v24);
		//         v6 = sub_7FFC80564280(v24, v7, 16i64);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"target", (__int64)"RenderTarget", v6);
		//         sub_7FFC805644A0((__int64)v24);
		//         set_packet_count((uint64_t)rdx0, 3u);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setRenderTarget");
			// m_cb->setRenderTarget();
			count = kPacketCountSetRenderTarget;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setRenderTarget2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v3; // ecx
		//   unsigned int v4; // ebx
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   int v7[11]; // [rsp+30h] [rbp-D0h] BYREF
		//   __int64 v8; // [rsp+5Ch] [rbp-A4h]
		//   __int64 v9; // [rsp+64h] [rbp-9Ch]
		//   int v10; // [rsp+6Ch] [rbp-94h]
		//   __int16 a2; // [rsp+70h] [rbp-90h] BYREF
		//   unsigned __int16 a2_4; // [rsp+74h] [rbp-8Ch]
		//   int v13; // [rsp+78h] [rbp-88h]
		//   int v14; // [rsp+7Ch] [rbp-84h]
		//   int v15; // [rsp+80h] [rbp-80h]
		//   int v16; // [rsp+84h] [rbp-7Ch]
		//   int v17; // [rsp+88h] [rbp-78h]
		//   int v18; // [rsp+8Ch] [rbp-74h]
		//   int v19; // [rsp+90h] [rbp-70h]
		//   int v20; // [rsp+94h] [rbp-6Ch]
		//   int v21; // [rsp+98h] [rbp-68h]
		//   int v22; // [rsp+9Ch] [rbp-64h]
		//   int v23; // [rsp+A0h] [rbp-60h]
		//   __int16 v24; // [rsp+A4h] [rbp-5Ch]
		//   _BYTE v25[72]; // [rsp+B0h] [rbp-50h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 60i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 0x6900
		//     && (unsigned int)a2_4 + 0xA000 >= 0xA318 )
		//   {
		//     v3 = a2_4 - 792;
		//     v4 = v3 / 0xF;
		//     if ( v3 == 0xF * (v3 / 0xF) && v4 < 8 )
		//     {
		//       v7[0] = v13;
		//       v7[1] = v14;
		//       v7[2] = v15;
		//       v7[3] = v16;
		//       v7[4] = v17;
		//       v7[5] = v18;
		//       v7[6] = 0;
		//       v7[7] = v20;
		//       v7[8] = v21;
		//       v7[9] = v22;
		//       v7[10] = v23;
		//       v8 = 0i64;
		//       v9 = 0i64;
		//       v10 = v19;
		//       if ( (v24 & 0xFF00) == 0x1000 )
		//       {
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"setRenderTarget");
		//         v5 = sub_7FFC80564220((__int64)v25, v4);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"rtSlot", (__int64)"uint32_t", v5);
		//         sub_7FFC805644A0((__int64)v25);
		//         v6 = sub_7FFC80564280(v25, v7, 16i64);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"target", (__int64)"RenderTarget", v6);
		//         sub_7FFC805644A0((__int64)v25);
		//         set_packet_count((uint64_t)rdx0, 2u);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setRenderTarget");
			// m_cb->setRenderTarget();
			count = kPacketCountSetRenderTarget2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setRenderTarget3(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v3; // ecx
		//   unsigned int v4; // ebx
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   int v7[16]; // [rsp+30h] [rbp-C8h] BYREF
		//   __int16 a2; // [rsp+70h] [rbp-88h] BYREF
		//   unsigned __int16 a2_4; // [rsp+74h] [rbp-84h]
		//   int v10; // [rsp+78h] [rbp-80h]
		//   int v11; // [rsp+7Ch] [rbp-7Ch]
		//   int v12; // [rsp+80h] [rbp-78h]
		//   int v13; // [rsp+84h] [rbp-74h]
		//   int v14; // [rsp+88h] [rbp-70h]
		//   int v15; // [rsp+8Ch] [rbp-6Ch]
		//   int v16; // [rsp+90h] [rbp-68h]
		//   int v17; // [rsp+94h] [rbp-64h]
		//   int v18; // [rsp+98h] [rbp-60h]
		//   int v19; // [rsp+9Ch] [rbp-5Ch]
		//   int v20; // [rsp+A0h] [rbp-58h]
		//   int v21; // [rsp+A4h] [rbp-54h]
		//   int v22; // [rsp+A8h] [rbp-50h]
		//   int v23; // [rsp+ACh] [rbp-4Ch]
		//   char v24[80]; // [rsp+B8h] [rbp-40h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 72i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 0x6900
		//     && (unsigned int)a2_4 + 0xA000 >= 0xA318 )
		//   {
		//     v3 = a2_4 - 792;
		//     v4 = v3 / 0xF;
		//     if ( v3 == 15 * (v3 / 0xF) && v4 < 8 )
		//     {
		//       v7[0] = v10;
		//       v7[1] = v11;
		//       v7[2] = v10;
		//       v7[3] = v11;
		//       v7[4] = v12;
		//       v7[5] = v13;
		//       v7[6] = v14;
		//       v7[7] = v15;
		//       v7[8] = v16;
		//       v7[9] = v17;
		//       v7[10] = v18;
		//       v7[11] = v19;
		//       v7[12] = v20;
		//       v7[13] = v21;
		//       v7[14] = 0;
		//       v7[15] = v23;
		//       if ( (v22 & 0xFF00) == 0x1000 )
		//       {
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"setRenderTarget");
		//         v5 = sub_7FFC80564220((__int64)v24, v4);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"rtSlot", (__int64)"uint32_t", v5);
		//         sub_7FFC805644A0((__int64)v24);
		//         v6 = sub_7FFC80564280(v24, v7, 16i64);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"target", (__int64)"RenderTarget", v6);
		//         sub_7FFC805644A0((__int64)v24);
		//         set_packet_count((uint64_t)rdx0, 2u);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setRenderTarget");
			// m_cb->setRenderTarget();
			count = kPacketCountSetRenderTarget3;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setRenderTarget4(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v3; // ecx
		//   unsigned int v4; // ebx
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int16 a2; // [rsp+30h] [rbp-78h] BYREF
		//   unsigned __int16 a2_4; // [rsp+34h] [rbp-74h]
		//   unsigned int v9; // [rsp+38h] [rbp-70h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 0x6900
		//     && (unsigned int)a2_4 + 0xA000 >= 0xA31C )
		//   {
		//     v3 = a2_4 - 0x31C;
		//     v4 = v3 / 0xF;
		//     if ( v3 == 0xF * (v3 / 0xF) && v4 < 8 )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setRenderTarget");
		//       if ( ((v9 >> 2) & 0x1F) == 0 )
		//       {
		//         v5 = sub_7FFC80564220((__int64)v10, v4);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"rtSlot", (__int64)"uint32_t", v5);
		//         sub_7FFC805644A0((__int64)v10);
		//         v6 = sub_7FFC80564430(v10, 0i64);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"target", (__int64)"RenderTarget", v6);
		//         sub_7FFC805644A0((__int64)v10);
		//         set_packet_count((uint64_t)rdx0, 1u);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setRenderTarget");
			// m_cb->setRenderTarget();
			count = kPacketCountSetRenderTarget4;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setDepthRenderTarget(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   int v4[14]; // [rsp+28h] [rbp-D8h] BYREF
		//   __int64 a2; // [rsp+60h] [rbp-A0h] BYREF
		//   int v6; // [rsp+68h] [rbp-98h]
		//   int v7; // [rsp+6Ch] [rbp-94h]
		//   int v8; // [rsp+70h] [rbp-90h]
		//   int v9; // [rsp+74h] [rbp-8Ch]
		//   int v10; // [rsp+78h] [rbp-88h]
		//   int v11; // [rsp+7Ch] [rbp-84h]
		//   int v12; // [rsp+80h] [rbp-80h]
		//   int v13; // [rsp+84h] [rbp-7Ch]
		//   int v14; // [rsp+88h] [rbp-78h]
		//   __int16 v15; // [rsp+8Ch] [rbp-74h]
		//   int v16; // [rsp+90h] [rbp-70h]
		//   int v17; // [rsp+94h] [rbp-6Ch]
		//   __int16 v18; // [rsp+98h] [rbp-68h]
		//   int v19; // [rsp+9Ch] [rbp-64h]
		//   int v20; // [rsp+A0h] [rbp-60h]
		//   __int16 v21; // [rsp+A4h] [rbp-5Ch]
		//   int v22; // [rsp+A8h] [rbp-58h]
		//   int v23; // [rsp+ACh] [rbp-54h]
		//   __int16 v24; // [rsp+B0h] [rbp-50h]
		//   int v25; // [rsp+B4h] [rbp-4Ch]
		//   int v26; // [rsp+B8h] [rbp-48h]
		//   int v27; // [rsp+BCh] [rbp-44h]
		//   char v28[80]; // [rsp+C0h] [rbp-40h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 96i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 16
		//     && (v14 & 0xFF00) == 26880
		//     && v15 == 15
		//     && (v17 & 0xFF00) == 26880
		//     && v18 == 2
		//     && (v20 & 0xFF00) == 26880
		//     && v21 == 5
		//     && (v23 & 0xFF00) == 26880
		//     && v24 == 687
		//     && (v26 & 0xFF00) == 4096 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setDepthRenderTarget");
		//     v4[0] = v6;
		//     v4[1] = v7;
		//     v4[2] = v8;
		//     v4[3] = v9;
		//     v4[4] = v10;
		//     v4[5] = v11;
		//     v4[6] = v12;
		//     v4[7] = v13;
		//     v4[8] = v19;
		//     v4[9] = v22;
		//     v4[10] = v25;
		//     v4[11] = v16;
		//     v4[12] = v27;
		//     v3 = sub_7FFC80564280(v28, v4, 13i64);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"depthTarget", (__int64)"DepthRenderTarget", v3);
		//     sub_7FFC805644A0((__int64)v28);
		//     set_packet_count((uint64_t)rdx0, 6u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setDepthRenderTarget");
			// m_cb->setDepthRenderTarget();
			count = kPacketCountSetDepthRenderTarget;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setDepthRenderTarget2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+28h] [rbp-80h] BYREF
		//   int v5; // [rsp+30h] [rbp-78h]
		//   int v6; // [rsp+34h] [rbp-74h]
		//   __int16 v7; // [rsp+38h] [rbp-70h]
		//   int v8; // [rsp+3Ch] [rbp-6Ch]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 24i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 16
		//     && (v6 & 0xFF00) == 26880
		//     && v7 == 17
		//     && !v5
		//     && !v8 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setDepthRenderTarget");
		//     v3 = sub_7FFC80564430(v9, 0i64);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"depthTarget", (__int64)"DepthRenderTarget", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 2u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setDepthRenderTarget");
			// m_cb->setDepthRenderTarget();
			count = kPacketCountSetDepthRenderTarget2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setDepthClearValue(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+28h] [rbp-80h] BYREF
		//   __int64 v5; // [rsp+38h] [rbp-70h]
		//   char v6[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v5 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 0x6900
		//     && WORD2(a2) == 11 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setDepthClearValue");
		//     v3 = sub_7FFC80564250(v6);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"clearValue", (__int64)"float", v3);
		//     sub_7FFC805644A0((__int64)v6);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setDepthClearValue");
			// m_cb->setDepthClearValue();
			count = kPacketCountSetDepthClearValue;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setStencilClearValue(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned __int8 v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 10 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setStencilClearValue");
		//     v3 = sub_7FFC80564220((__int64)v7, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"clearValue", (__int64)"uint8_t", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setStencilClearValue");
			// m_cb->setStencilClearValue();
			count = kPacketCountSetStencilClearValue;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setCmaskClearColor(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v3; // ecx
		//   unsigned int v4; // ebx
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   int a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned __int16 a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v10; // [rsp+30h] [rbp-78h]
		//   unsigned int v11; // [rsp+34h] [rbp-74h]
		//   char v12[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && (a2 & 0x3FFF0000) == 0x20000
		//     && (unsigned int)a2_4 + 40960 >= 0xA323 )
		//   {
		//     v3 = a2_4 - 803;
		//     v4 = v3 / 0xF;
		//     if ( v3 == 15 * (v3 / 0xF) && v4 < 8 )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setCmaskClearColor");
		//       v5 = sub_7FFC80564220((__int64)v12, v4);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"rtSlot", (__int64)"uint32_t", v5);
		//       sub_7FFC805644A0((__int64)v12);
		//       v6 = sub_7FFC80564220((__int64)v12, v10);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"clearColor[0]", (__int64)"uint32_t", v6);
		//       sub_7FFC805644A0((__int64)v12);
		//       v7 = sub_7FFC80564220((__int64)v12, v11);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"clearColor[1]", (__int64)"uint32_t", v7);
		//       sub_7FFC805644A0((__int64)v12);
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setCmaskClearColor");
			// m_cb->setCmaskClearColor();
			count = kPacketCountSetCmaskClearColor;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setRenderTargetMask(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v6; // [rsp+28h] [rbp-80h]
		//   __int64 v7; // [rsp+30h] [rbp-78h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 142
		//     && (a2 & 0x3FFF0000) == 0x10000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setRenderTargetMask");
		//     v3 = sub_7FFC80564220((__int64)v8, v6);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"mask", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setRenderTargetMask");
			// m_cb->setRenderTargetMask();
			count = kPacketCountSetRenderTargetMask;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setBlendControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v3; // ebx
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v8; // [rsp+28h] [rbp-80h]
		//   __int64 v9; // [rsp+30h] [rbp-78h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && (a2 & 0x3FFF0000) == 0x10000
		//     && (unsigned int)a2_4 + 40960 >= 0xA1E0 )
		//   {
		//     v3 = a2_4 - 480;
		//     if ( v3 < 8 )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setBlendControl");
		//       v4 = sub_7FFC80564220((__int64)v10, v3);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"rtSlot", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       v5 = sub_7FFC80564220((__int64)v10, v8);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"blendControl", (__int64)"BlendControl", v5);
		//       sub_7FFC805644A0((__int64)v10);
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setBlendControl");
			// m_cb->setBlendControl();
			count = kPacketCountSetBlendControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setBlendColor(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   int a2; // [rsp+30h] [rbp-88h] BYREF
		//   __int16 a2_4; // [rsp+34h] [rbp-84h]
		//   char v9[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 24i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 261
		//     && (a2 & 0x3FFF0000) == 0x40000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setBlendColor");
		//     v3 = sub_7FFC80564250(v9);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"red", (__int64)"float", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564250(v9);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"green", (__int64)"float", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     v5 = sub_7FFC80564250(v9);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"blue", (__int64)"float", v5);
		//     sub_7FFC805644A0((__int64)v9);
		//     v6 = sub_7FFC80564250(v9);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"alpha", (__int64)"float", v6);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setBlendColor");
			// m_cb->setBlendColor();
			count = kPacketCountSetBlendColor;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setStencilSeparate(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   int a2; // [rsp+28h] [rbp-80h] BYREF
		//   __int16 a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v8; // [rsp+30h] [rbp-78h]
		//   unsigned int v9; // [rsp+34h] [rbp-74h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 268
		//     && (a2 & 0x3FFF0000) == 0x20000 )
		//   {
		//     if ( v8 == v9 )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setStencil");
		//       v3 = sub_7FFC80564220((__int64)v10, v8);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stencilControl", (__int64)"StencilControl", v3);
		//     }
		//     else
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setStencilSeparate");
		//       v4 = sub_7FFC80564220((__int64)v10, v8);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"front", (__int64)"StencilControl", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       v5 = sub_7FFC80564220((__int64)v10, v9);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"back", (__int64)"StencilControl", v5);
		//     }
		//     sub_7FFC805644A0((__int64)v10);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setStencilSeparate");
			// m_cb->setStencilSeparate();
			count = kPacketCountSetStencilSeparate;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setAlphaToMaskControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 732 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setAlphaToMaskControl");
		//     v3 = sub_7FFC80564220((__int64)v7, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"alphaToMaskControl.m_reg", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setAlphaToMaskControl");
			// m_cb->setAlphaToMaskControl();
			count = kPacketCountSetAlphaToMaskControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setHtileStencil0(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 688 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setHtileStencil0");
		//     v3 = sub_7FFC80564220((__int64)v7, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"htileStencilControl", (__int64)"HtileStencilControl", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setHtileStencil");
			// m_cb->setHtileStencil();
			count = kPacketCountSetHtileStencil0;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setHtileStencil1(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 689 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setHtileStencil1");
		//     v3 = sub_7FFC80564220((__int64)v7, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"htileStencilControl", (__int64)"HtileStencilControl", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setHtileStencil");
			// m_cb->setHtileStencil();
			count = kPacketCountSetHtileStencil1;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setCbControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		// {
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v6; // [rsp+28h] [rbp-80h]
		//   __int64 v7; // [rsp+30h] [rbp-78h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;                                   // sub_1802C6D90
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 0x6900
		//     && WORD2(a2) == 0x202 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setCbControl");
		//     v3 = sub_7FFC80564220((__int64)v8, (v6 >> 4) & 7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"mode", (__int64)"CbMode", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     v4 = sub_7FFC80564220((__int64)v8, BYTE2(v6));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"op", (__int64)"RasterOp", v4);
		//     sub_7FFC805644A0((__int64)v8);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setCbControl");
			// m_cb->setCbControl();
			count = kPacketCountSetCbControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setDepthStencilDisable(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 512 )
		//   {
		//     if ( v5 )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setDepthStencilControl");
		//       v3 = sub_7FFC80564220((__int64)v7, v5);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"depthControl", (__int64)"DepthStencilControl", v3);
		//       sub_7FFC805644A0((__int64)v7);
		//     }
		//     else
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setDepthStencilDisable");
		//     }
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setDepthStencilDisable");
			// m_cb->setDepthStencilDisable();
			count = kPacketCountSetDepthStencilDisable;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setDepthBoundsRange(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   int a2; // [rsp+30h] [rbp-78h] BYREF
		//   __int16 a2_4; // [rsp+34h] [rbp-74h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 8
		//     && (a2 & 0x3FFF0000) == 0x20000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setDepthBoundsRange");
		//     v3 = sub_7FFC80564250(v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"depthBoundsMin", (__int64)"float", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     v4 = sub_7FFC80564250(v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"depthBoundsMax", (__int64)"float", v4);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setDepthBoundsRange");
			// m_cb->setDepthBoundsRange();
			count = kPacketCountSetDepthBoundsRange;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setStencilOpControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 267 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setStencilOpControl");
		//     v3 = sub_7FFC80564220((__int64)v7, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stencilControl", (__int64)"StencilOpControl", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setStencilOpControl");
			// m_cb->setStencilOpControl();
			count = kPacketCountSetStencilOpControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setDbRenderControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 26880 && !WORD2(a2) )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setDbRenderControl");
		//     v3 = sub_7FFC80564220((__int64)v7, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"reg", (__int64)"DbRenderControl", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setDbRenderControl");
			// m_cb->setDbRenderControl();
			count = kPacketCountSetDbRenderControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setDbCountControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v6; // [rsp+28h] [rbp-80h]
		//   __int64 v7; // [rsp+30h] [rbp-78h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 26880 && WORD2(a2) == 1 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setDbCountControl");
		//     if ( (v6 & 0xF00) == 256 && (v6 & 0xF000000) == 0x1000000 && (v6 & 0xF0000000) == 0x10000000 )
		//     {
		//       v3 = sub_7FFC80564220((__int64)v8, (v6 >> 1) & 1);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"perfectZPassCounts", (__int64)"DbCountControlPerfectZPassCounts", v3);
		//       sub_7FFC805644A0((__int64)v8);
		//       v4 = sub_7FFC80564220((__int64)v8, (v6 >> 4) & 7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"log2SampleRate", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v8);
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setDbCountControl");
			// m_cb->setDbCountControl();
			count = kPacketCountSetDbCountControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setDepthEqaaControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 513 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setDepthEqaaControl");
		//     v3 = sub_7FFC80564220((__int64)v7, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"depthEqaa", (__int64)"DepthEqaaControl", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setDepthEqaaControl");
			// m_cb->setDepthEqaaControl();
			count = kPacketCountSetDepthEqaaControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPrimitiveIdEnable(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 673 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setPrimitiveIdEnable");
		//     v3 = sub_7FFC80564220((__int64)v7, v5 & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"enable", (__int64)"bool", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPrimitiveIdEnable");
			// m_cb->setPrimitiveIdEnable();
			count = kPacketCountSetPrimitiveIdEnable;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setVgtControlForBase(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v6; // [rsp+28h] [rbp-80h]
		//   __int64 v7; // [rsp+30h] [rbp-78h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 682 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setVgtControlForBase");
		//     v3 = sub_7FFC80564220((__int64)v8, (unsigned __int16)v6);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"primGroupSizeMinusOne", (__int64)"uint8_t", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     v4 = sub_7FFC80564220((__int64)v8, HIWORD(v6) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"partialVsWaveMode", (__int64)"VgtPartialVsWaveMode", v4);
		//     sub_7FFC805644A0((__int64)v8);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setVgtControlForBase");
			// m_cb->setVgtControlForBase();
			count = kPacketCountSetVgtControlForBase;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setVgtControlForNeo(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v7; // [rsp+28h] [rbp-80h]
		//   __int64 v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 682 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setVgtControlForNeo");
		//     v3 = sub_7FFC80564220((__int64)v9, (unsigned __int16)v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"primGroupSizeMinusOne", (__int64)"uint8_t", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, (v7 >> 20) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"wdSwitchOnlyOnEopMode", (__int64)"WdSwitchOnlyOnEopMode", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     v5 = sub_7FFC80564220((__int64)v9, HIWORD(v7) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"partialVsWaveMode", (__int64)"VgtPartialVsWaveMode", v5);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setVgtControlForNeo");
			// m_cb->setVgtControlForNeo();
			count = kPacketCountSetVgtControlForNeo;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setVgtControlForNeo2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v7; // [rsp+28h] [rbp-80h]
		//   __int64 v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30976
		//     && WORD2(a2) == 600 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setVgtControlForNeo");
		//     v3 = sub_7FFC80564220((__int64)v9, (unsigned __int16)v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"primGroupSizeMinusOne", (__int64)"uint8_t", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, (v7 >> 20) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"wdSwitchOnlyOnEopMode", (__int64)"WdSwitchOnlyOnEopMode", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     v5 = sub_7FFC80564220((__int64)v9, HIWORD(v7) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"partialVsWaveMode", (__int64)"VgtPartialVsWaveMode", v5);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setVgtControlForNeo");
			// m_cb->setVgtControlForNeo();
			count = kPacketCountSetVgtControlForNeo2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setVertexReuseEnable(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 685 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setVertexReuseEnable");
		//     v3 = sub_7FFC80564220((__int64)v7, (v5 & 1) == 0);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"enable", (__int64)"bool", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setVertexReuseEnable");
			// m_cb->setVertexReuseEnable();
			count = kPacketCountSetVertexReuseEnable;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPrimitiveType(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 0x7900
		//     && WORD2(a2) == 0x242 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setPrimitiveType");
		//     v3 = sub_7FFC80564220((__int64)v7, v5 & 0x3F);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"primType", (__int64)"PrimitiveType", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPrimitiveType");
			// m_cb->setPrimitiveType();
			count = kPacketCountSetPrimitiveType;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setIndexSize(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // ebx
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int16 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int a2_4; // [rsp+24h] [rbp-84h]
		//   __int64 v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 8i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 10752 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setIndexSize");
		//     v3 = 2;
		//     if ( ((a2_4 >> 10) & 1) != 0 )
		//       v3 = (unsigned __int8)a2_4 >> 6;
		//     v4 = sub_7FFC80564220((__int64)v9, a2_4 & 3);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indexSize", (__int64)"IndexSize", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     v5 = sub_7FFC80564220((__int64)v9, v3);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cachePolicy", (__int64)"CachePolicy", v5);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setIndexSize");
			// m_cb->setIndexSize();
			count = kPacketCountSetIndexSize;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setNumInstances(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int16 a2; // [rsp+20h] [rbp-78h] BYREF
		//   unsigned int a2_4; // [rsp+24h] [rbp-74h]
		//   __int64 v6; // [rsp+28h] [rbp-70h]
		//   char v7[80]; // [rsp+30h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 8i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 12032 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setNumInstances");
		//     v3 = sub_7FFC80564220((__int64)v7, a2_4);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"numInstances", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setNumInstances");
			// m_cb->setNumInstances();
			count = kPacketCountSetNumInstances;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setIndexOffset(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 258 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setIndexOffset");
		//     v3 = sub_7FFC80564220((__int64)v7, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"offset", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setIndexOffset");
			// m_cb->setIndexOffset();
			count = kPacketCountSetIndexOffset;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setIndexOffset2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26624
		//     && WORD2(a2) == 0x8102 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setIndexOffset");
		//     v3 = sub_7FFC80564220((__int64)v7, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"offset", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setIndexOffset");
			// m_cb->setIndexOffset();
			count = kPacketCountSetIndexOffset2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::drawOpaqueAuto(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   char v8[20]; // [rsp+28h] [rbp-A0h] BYREF
		//   uint32_t v9; // [rsp+3Ch] [rbp-8Ch]
		//   __int16 a2; // [rsp+48h] [rbp-80h] BYREF
		//   int a2_4; // [rsp+4Ch] [rbp-7Ch]
		//   unsigned int v12; // [rsp+50h] [rbp-78h]
		//   char v13[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&a2, 12i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 11520 )
		//     {
		//       sub_7FFC805AD130((__int64)v8);
		//       v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//       if ( sub_7FFC805AD150(0, *a1, v5, (__int64)v8)
		//         && (v12 & 3) == 2
		//         && (v12 & 0xC) == 0
		//         && (v12 & 0x20) == 0
		//         && (v12 & 0x40) != 0
		//         && a2_4 == 16 )
		//       {
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"drawOpaqueAuto");
		//         v6 = sub_7FFC80564220((__int64)v13, v12 >> 29);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"modifier", (__int64)"DrawModifier", v6);
		//         sub_7FFC805644A0((__int64)v13);
		//         set_packet_count((uint64_t)rdx0, v9);
		//         sub_7FFC80564780((__int64)rdx0, 0);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//       LOBYTE(v4) = nullsub_1(v8);
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: drawOpaqueAuto");
			// m_cb->drawOpaqueAuto();
			count = kPacketCountDrawOpaqueAuto;
		}

		return count;
	}

	uint32_t GnmCommandProxy::drawIndexAuto(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   char v9[20]; // [rsp+28h] [rbp-A0h] BYREF
		//   uint32_t v10; // [rsp+3Ch] [rbp-8Ch]
		//   __int16 a2; // [rsp+48h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+4Ch] [rbp-7Ch]
		//   unsigned int v13; // [rsp+50h] [rbp-78h]
		//   char v14[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&a2, 12i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 0x2D00 )
		//     {
		//       sub_7FFC805AD130((__int64)v9);
		//       v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//       if ( sub_7FFC805AD150(0, *a1, v5, (__int64)v9)
		//         && (v13 & 3) == 2
		//         && (v13 & 0xC) == 0
		//         && (v13 & 0x20) == 0
		//         && (v13 & 0x40) == 0 )
		//       {
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"drawIndexAuto");
		//         v6 = sub_7FFC80564220((__int64)v14, a2_4);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indexCount", (__int64)"uint32_t", v6);
		//         sub_7FFC805644A0((__int64)v14);
		//         v7 = sub_7FFC80564220((__int64)v14, v13 >> 29);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"modifier", (__int64)"DrawModifier", v7);
		//         sub_7FFC805644A0((__int64)v14);
		//         set_packet_count((uint64_t)rdx0, v10);
		//         sub_7FFC80564780((__int64)rdx0, 0);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//       LOBYTE(v4) = nullsub_1(v9);
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: drawIndexAuto");
			// m_cb->drawIndexAuto();
			count = kPacketCountDrawIndexAuto;
		}

		return count;
	}

	uint32_t GnmCommandProxy::drawIndexInline(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rbx
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   unsigned int v9; // eax
		//   __int64 v10; // rax
		//   __int64 v11; // rax
		//   __int64 v12; // rax
		//   char v14[20]; // [rsp+28h] [rbp-A0h] BYREF
		//   uint32_t v15; // [rsp+3Ch] [rbp-8Ch]
		//   unsigned int a2; // [rsp+48h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+4Ch] [rbp-7Ch]
		//   unsigned int v18; // [rsp+50h] [rbp-78h]
		//   char v19[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&a2, 12i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 11776 )
		//     {
		//       sub_7FFC805AD130((__int64)v14);
		//       v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//       if ( !sub_7FFC805AD150(0, *a1, v5, (__int64)v14) )
		//       {
		// LABEL_10:
		//         LOBYTE(v4) = nullsub_1(v14);
		//         return v4;
		//       }
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"drawIndexInline");
		//       v6 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//       v7 = sub_7FFC80564220((__int64)v19, a2_4);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indexCount", (__int64)"uint32_t", v7);
		//       sub_7FFC805644A0((__int64)v19);
		//       v8 = sub_7FFC80564430(v19, v6 + 12);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indices", (__int64)"void*", v8);
		//       sub_7FFC805644A0((__int64)v19);
		//       v9 = get_packet_size(a2) - 12;
		//       if ( v9 < 4 * a2_4 )
		//       {
		//         if ( v9 < 2 * a2_4 )
		//         {
		// LABEL_9:
		//           v12 = sub_7FFC80564220((__int64)v19, v18 >> 29);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"modifier", (__int64)"DrawModifier", v12);
		//           sub_7FFC805644A0((__int64)v19);
		//           set_packet_count((uint64_t)rdx0, v15);
		//           sub_7FFC80564780((__int64)rdx0, 0);
		//           *((_BYTE *)rdx0 + 24) = 1;
		//           goto LABEL_10;
		//         }
		//         v11 = sub_7FFC80564220((__int64)v19, 2 * a2_4);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indicesSizeInBytes", (__int64)"uint32_t", v11);
		//       }
		//       else
		//       {
		//         v10 = sub_7FFC80564220((__int64)v19, 4 * a2_4);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indicesSizeInBytes", (__int64)"uint32_t", v10);
		//       }
		//       sub_7FFC805644A0((__int64)v19);
		//       goto LABEL_9;
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: drawIndexInline");
			// m_cb->drawIndexInline();
			count = kPacketCountDrawIndexInline;
		}

		return count;
	}

	uint32_t GnmCommandProxy::drawIndexInline2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rbx
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   int packet_size; // eax
		//   __int64 v11; // rax
		//   __int64 v12; // rax
		//   char v14[20]; // [rsp+28h] [rbp-C0h] BYREF
		//   uint32_t v15; // [rsp+3Ch] [rbp-ACh]
		//   __int64 a2; // [rsp+48h] [rbp-A0h] BYREF
		//   __int64 v17; // [rsp+50h] [rbp-98h]
		//   unsigned int v18; // [rsp+58h] [rbp-90h]
		//   unsigned int v19; // [rsp+5Ch] [rbp-8Ch]
		//   unsigned int v20; // [rsp+70h] [rbp-78h]
		//   int v21; // [rsp+74h] [rbp-74h]
		//   char v22[80]; // [rsp+80h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&a2, 52i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 9984 )
		//     {
		//       v4 = v20 & 0xFF00;
		//       if ( v4 == 4096 )
		//       {
		//         LOBYTE(v4) = 0;
		//         if ( (v21 & 0xFFFF0000) == 1752498176 )
		//         {
		//           sub_7FFC805AD130((__int64)v14);
		//           v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//           if ( sub_7FFC805AD150(0, *a1, v5, (__int64)v14) )
		//           {
		//             v6 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//             v7 = v17;
		//             if ( v6 + 48 == v17 )
		//             {
		//               v8 = sub_7FFC80564220((__int64)v22, v18);
		//               sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indexCount", (__int64)"uint32_t", v8);
		//               sub_7FFC805644A0((__int64)v22);
		//               v9 = sub_7FFC80564430(v22, v7);
		//               sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indices", (__int64)"void*", v9);
		//               sub_7FFC805644A0((__int64)v22);
		//               packet_size = get_packet_size(v20);
		//               v11 = sub_7FFC80564220((__int64)v22, packet_size - 8);
		//               sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indicesSizeInBytes", (__int64)"uint32_t", v11);
		//               sub_7FFC805644A0((__int64)v22);
		//               v12 = sub_7FFC80564220((__int64)v22, v19 >> 29);
		//               sub_7FFC805A8FC0((__int64)rdx0, (__int64)"modifier", (__int64)"DrawModifier", v12);
		//               sub_7FFC805644A0((__int64)v22);
		//               set_packet_count((uint64_t)rdx0, v15);
		//               Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//                 rdx0,
		//                 (struct Concurrency::details::SchedulerNode *)"drawIndexInline");
		//               sub_7FFC80564780((__int64)rdx0, 0);
		//               *((_BYTE *)rdx0 + 24) = 1;
		//             }
		//           }
		//           LOBYTE(v4) = nullsub_1(v14);
		//         }
		//       }
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: drawIndexInline");
			// m_cb->drawIndexInline();
			count = kPacketCountDrawIndexInline2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::drawIndex(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   char v10[20]; // [rsp+28h] [rbp-A0h] BYREF
		//   uint32_t v11; // [rsp+3Ch] [rbp-8Ch]
		//   __int64 a2[2]; // [rsp+48h] [rbp-80h] BYREF
		//   unsigned int v13; // [rsp+58h] [rbp-70h]
		//   unsigned int v14; // [rsp+5Ch] [rbp-6Ch]
		//   char v15[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)a2, 24i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2[0] & 0xFF00;
		//     if ( v4 == 9984 )
		//     {
		//       sub_7FFC805AD130((__int64)v10);
		//       v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//       if ( sub_7FFC805AD150(0, *a1, v5, (__int64)v10) )
		//       {
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"drawIndex");
		//         v6 = sub_7FFC80564220((__int64)v15, v13);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indexCount", (__int64)"uint32_t", v6);
		//         sub_7FFC805644A0((__int64)v15);
		//         v7 = sub_7FFC80564430(v15, a2[1]);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indices", (__int64)"void*", v7);
		//         sub_7FFC805644A0((__int64)v15);
		//         v8 = sub_7FFC80564220((__int64)v15, v14 >> 29);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"modifier", (__int64)"DrawModifier", v8);
		//         sub_7FFC805644A0((__int64)v15);
		//         set_packet_count((uint64_t)rdx0, v11);
		//         sub_7FFC80564780((__int64)rdx0, 0);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//       LOBYTE(v4) = nullsub_1(v10);
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: drawIndex");
			// m_cb->drawIndex();
			count = kPacketCountDrawIndex;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setIndexBuffer(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int16 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned __int16 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 9728 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setIndexBuffer");
		//     v3 = sub_7FFC80564430(v7, a2_4 | ((unsigned __int64)v6 << 32));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indexAddr", (__int64)"void*", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setIndexBuffer");
			// m_cb->setIndexBuffer();
			count = kPacketCountSetIndexBuffer;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setIndexCount(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int16 a2; // [rsp+20h] [rbp-78h] BYREF
		//   unsigned int a2_4; // [rsp+24h] [rbp-74h]
		//   __int64 v6; // [rsp+28h] [rbp-70h]
		//   char v7[80]; // [rsp+30h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 8i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 4864 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setIndexCount");
		//     v3 = sub_7FFC80564220((__int64)v7, a2_4);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indexCount", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setIndexCount");
			// m_cb->setIndexCount();
			count = kPacketCountSetIndexCount;
		}

		return count;
	}

	uint32_t GnmCommandProxy::drawIndexOffset(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   char v10[20]; // [rsp+28h] [rbp-A0h] BYREF
		//   uint32_t v11; // [rsp+3Ch] [rbp-8Ch]
		//   __int16 a2; // [rsp+48h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+4Ch] [rbp-7Ch]
		//   unsigned int v14; // [rsp+50h] [rbp-78h]
		//   unsigned int v15; // [rsp+58h] [rbp-70h]
		//   char v16[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&a2, 20i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 13568 )
		//     {
		//       LOBYTE(v4) = v15;
		//       if ( (v15 & 3) == 0 && (v15 & 0xC) == 0 && (v15 & 0x20) == 0 )
		//       {
		//         sub_7FFC805AD130((__int64)v10);
		//         v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//         if ( sub_7FFC805AD150(0, *a1, v5, (__int64)v10) )
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"drawIndexOffset");
		//           v6 = sub_7FFC80564220((__int64)v16, v14);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indexOffset", (__int64)"uint32_t", v6);
		//           sub_7FFC805644A0((__int64)v16);
		//           v7 = sub_7FFC80564220((__int64)v16, a2_4);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indexCount", (__int64)"uint32_t", v7);
		//           sub_7FFC805644A0((__int64)v16);
		//           v8 = sub_7FFC80564220((__int64)v16, v15 >> 29);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"modifier", (__int64)"DrawModifier", v8);
		//           sub_7FFC805644A0((__int64)v16);
		//           set_packet_count((uint64_t)rdx0, v11);
		//           sub_7FFC80564780((__int64)rdx0, 0);
		//           *((_BYTE *)rdx0 + 24) = 1;
		//         }
		//         LOBYTE(v4) = nullsub_1(v10);
		//       }
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: drawIndexOffset");
			// m_cb->drawIndexOffset();
			count = kPacketCountDrawIndexOffset;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setBaseIndirectArgs(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int v6; // [rsp+30h] [rbp-78h]
		//   unsigned __int16 v7; // [rsp+34h] [rbp-74h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 4352 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setBaseIndirectArgs");
		//     v3 = sub_7FFC80564220((__int64)v8, ((unsigned int)a2 >> 1) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"shaderType", (__int64)"ShaderType", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     v4 = sub_7FFC80564430(v8, v6 | ((unsigned __int64)v7 << 32));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indirectBaseAddr", (__int64)"void*", v4);
		//     sub_7FFC805644A0((__int64)v8);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setBaseIndirectArgs");
			// m_cb->setBaseIndirectArgs();
			count = kPacketCountSetBaseIndirectArgs;
		}

		return count;
	}

	uint32_t GnmCommandProxy::drawIndirect(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   char v9[20]; // [rsp+28h] [rbp-A0h] BYREF
		//   uint32_t v10; // [rsp+3Ch] [rbp-8Ch]
		//   __int16 a2; // [rsp+48h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+4Ch] [rbp-7Ch]
		//   unsigned __int16 v13; // [rsp+50h] [rbp-78h]
		//   unsigned __int16 v14; // [rsp+54h] [rbp-74h]
		//   unsigned int v15; // [rsp+58h] [rbp-70h]
		//   char v16[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&a2, 20i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 9216 )
		//     {
		//       LOBYTE(v4) = v15 & 3;
		//       if ( (v15 & 3) == 2 && (v15 & 0xC) == 0 && (v15 & 0x20) == 0 )
		//       {
		//         sub_7FFC805AD130((__int64)v9);
		//         v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//         if ( sub_7FFC805AD150(0, *a1, v5, (__int64)v9) )
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"drawIndirect");
		//           v6 = sub_7FFC80564220((__int64)v16, a2_4);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dataOffsetInBytes", (__int64)"uint32_t", v6);
		//           sub_7FFC805644A0((__int64)v16);
		//           if ( (unsigned __int8)sub_7FFC805A9210(v13, v14, rdx0) )
		//           {
		//             v7 = sub_7FFC80564220((__int64)v16, v15 >> 29);
		//             sub_7FFC805A8FC0((__int64)rdx0, (__int64)"modifier", (__int64)"DrawModifier", v7);
		//             sub_7FFC805644A0((__int64)v16);
		//             set_packet_count((uint64_t)rdx0, v10);
		//             sub_7FFC80564780((__int64)rdx0, 0);
		//             *((_BYTE *)rdx0 + 24) = 1;
		//           }
		//         }
		//         LOBYTE(v4) = nullsub_1(v9);
		//       }
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: drawIndirect");
			// m_cb->drawIndirect();
			count = kPacketCountDrawIndirect;
		}

		return count;
	}

	uint32_t GnmCommandProxy::drawIndirectMulti(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   char v10[20]; // [rsp+28h] [rbp-B0h] BYREF
		//   uint32_t v11; // [rsp+3Ch] [rbp-9Ch]
		//   __int16 a2; // [rsp+48h] [rbp-90h] BYREF
		//   unsigned int a2_4; // [rsp+4Ch] [rbp-8Ch]
		//   unsigned __int16 v14; // [rsp+50h] [rbp-88h]
		//   unsigned __int16 v15; // [rsp+54h] [rbp-84h]
		//   unsigned int v16; // [rsp+58h] [rbp-80h]
		//   unsigned int v17; // [rsp+60h] [rbp-78h]
		//   char v18[80]; // [rsp+70h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&a2, 28i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 11264 )
		//     {
		//       LOBYTE(v4) = v17 & 3;
		//       if ( (v17 & 3) == 2 && (v17 & 0xC) == 0 && (v17 & 0x20) == 0 )
		//       {
		//         sub_7FFC805AD130((__int64)v10);
		//         v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//         if ( sub_7FFC805AD150(0, *a1, v5, (__int64)v10) )
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"drawIndirectMulti");
		//           v6 = sub_7FFC80564220((__int64)v18, a2_4);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dataOffsetInBytes", (__int64)"uint32_t", v6);
		//           sub_7FFC805644A0((__int64)v18);
		//           v7 = sub_7FFC80564220((__int64)v18, v16);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"drawCount", (__int64)"uint32_t", v7);
		//           sub_7FFC805644A0((__int64)v18);
		//           if ( (unsigned __int8)sub_7FFC805A9210(v14, v15, rdx0) )
		//           {
		//             v8 = sub_7FFC80564220((__int64)v18, v17 >> 29);
		//             sub_7FFC805A8FC0((__int64)rdx0, (__int64)"modifier", (__int64)"DrawModifier", v8);
		//             sub_7FFC805644A0((__int64)v18);
		//             set_packet_count((uint64_t)rdx0, v11);
		//             sub_7FFC80564780((__int64)rdx0, 0);
		//             *((_BYTE *)rdx0 + 24) = 1;
		//           }
		//         }
		//         LOBYTE(v4) = nullsub_1(v10);
		//       }
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: drawIndirectMulti");
			// m_cb->drawIndirectMulti();
			count = kPacketCountDrawIndirectMulti;
		}

		return count;
	}

	uint32_t GnmCommandProxy::drawIndirectCountMulti(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   char v11[20]; // [rsp+28h] [rbp-B0h] BYREF
		//   uint32_t v12; // [rsp+3Ch] [rbp-9Ch]
		//   __int16 a2; // [rsp+48h] [rbp-90h] BYREF
		//   unsigned int a2_4; // [rsp+4Ch] [rbp-8Ch]
		//   unsigned __int16 v15; // [rsp+50h] [rbp-88h]
		//   unsigned __int16 v16; // [rsp+54h] [rbp-84h]
		//   unsigned int v17; // [rsp+5Ch] [rbp-7Ch]
		//   __int64 v18; // [rsp+60h] [rbp-78h]
		//   unsigned int v19; // [rsp+6Ch] [rbp-6Ch]
		//   char v20[80]; // [rsp+70h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&a2, 40i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 39936 )
		//     {
		//       LOBYTE(v4) = v19 & 3;
		//       if ( (v19 & 3) == 2 && (v19 & 0xC) == 0 && (v19 & 0x20) == 0 )
		//       {
		//         sub_7FFC805AD130((__int64)v11);
		//         v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//         if ( sub_7FFC805AD150(0, *a1, v5, (__int64)v11) )
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"drawIndirectCountMulti");
		//           v6 = sub_7FFC80564220((__int64)v20, a2_4);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dataOffsetInBytes", (__int64)"uint32_t", v6);
		//           sub_7FFC805644A0((__int64)v20);
		//           v7 = sub_7FFC80564220((__int64)v20, v17);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"count", (__int64)"uint32_t", v7);
		//           sub_7FFC805644A0((__int64)v20);
		//           v8 = sub_7FFC80564430(v20, v18);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"countAddress", (__int64)"void*", v8);
		//           sub_7FFC805644A0((__int64)v20);
		//           if ( (unsigned __int8)sub_7FFC805A9210(v15, v16, rdx0) )
		//           {
		//             v9 = sub_7FFC80564220((__int64)v20, v19 >> 29);
		//             sub_7FFC805A8FC0((__int64)rdx0, (__int64)"modifier", (__int64)"DrawModifier", v9);
		//             sub_7FFC805644A0((__int64)v20);
		//             set_packet_count((uint64_t)rdx0, v12);
		//             sub_7FFC80564780((__int64)rdx0, 0);
		//             *((_BYTE *)rdx0 + 24) = 1;
		//           }
		//         }
		//         LOBYTE(v4) = nullsub_1(v11);
		//       }
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: drawIndirectCountMulti");
			// m_cb->drawIndirectCountMulti();
			count = kPacketCountDrawIndirectCountMulti;
		}

		return count;
	}

	uint32_t GnmCommandProxy::drawIndexIndirect(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   char v9[20]; // [rsp+28h] [rbp-A0h] BYREF
		//   uint32_t v10; // [rsp+3Ch] [rbp-8Ch]
		//   __int16 a2; // [rsp+48h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+4Ch] [rbp-7Ch]
		//   unsigned __int16 v13; // [rsp+50h] [rbp-78h]
		//   unsigned __int16 v14; // [rsp+54h] [rbp-74h]
		//   unsigned int v15; // [rsp+58h] [rbp-70h]
		//   char v16[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&a2, 20i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 9472 )
		//     {
		//       LOBYTE(v4) = v15;
		//       if ( (v15 & 3) == 0 && (v15 & 0xC) == 0 && (v15 & 0x20) == 0 )
		//       {
		//         sub_7FFC805AD130((__int64)v9);
		//         v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//         if ( sub_7FFC805AD150(0, *a1, v5, (__int64)v9) )
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"drawIndexIndirect");
		//           v6 = sub_7FFC80564220((__int64)v16, a2_4);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dataOffsetInBytes", (__int64)"uint32_t", v6);
		//           sub_7FFC805644A0((__int64)v16);
		//           if ( (unsigned __int8)sub_7FFC805A9210(v13, v14, rdx0) )
		//           {
		//             v7 = sub_7FFC80564220((__int64)v16, v15 >> 29);
		//             sub_7FFC805A8FC0((__int64)rdx0, (__int64)"modifier", (__int64)"DrawModifier", v7);
		//             sub_7FFC805644A0((__int64)v16);
		//             set_packet_count((uint64_t)rdx0, v10);
		//             sub_7FFC80564780((__int64)rdx0, 0);
		//             *((_BYTE *)rdx0 + 24) = 1;
		//           }
		//         }
		//         LOBYTE(v4) = nullsub_1(v9);
		//       }
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: drawIndexIndirect");
			// m_cb->drawIndexIndirect();
			count = kPacketCountDrawIndexIndirect;
		}

		return count;
	}

	uint32_t GnmCommandProxy::drawIndexIndirectMulti(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   char v10[20]; // [rsp+28h] [rbp-B0h] BYREF
		//   uint32_t v11; // [rsp+3Ch] [rbp-9Ch]
		//   __int16 a2; // [rsp+48h] [rbp-90h] BYREF
		//   unsigned int a2_4; // [rsp+4Ch] [rbp-8Ch]
		//   unsigned __int16 v14; // [rsp+50h] [rbp-88h]
		//   unsigned __int16 v15; // [rsp+54h] [rbp-84h]
		//   unsigned int v16; // [rsp+58h] [rbp-80h]
		//   unsigned int v17; // [rsp+60h] [rbp-78h]
		//   char v18[80]; // [rsp+70h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&a2, 28i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 14336 )
		//     {
		//       LOBYTE(v4) = v17;
		//       if ( (v17 & 3) == 0 && (v17 & 0xC) == 0 && (v17 & 0x20) == 0 )
		//       {
		//         sub_7FFC805AD130((__int64)v10);
		//         v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//         if ( sub_7FFC805AD150(0, *a1, v5, (__int64)v10) )
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"drawIndexIndirectMulti");
		//           v6 = sub_7FFC80564220((__int64)v18, a2_4);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dataOffsetInBytes", (__int64)"uint32_t", v6);
		//           sub_7FFC805644A0((__int64)v18);
		//           v7 = sub_7FFC80564220((__int64)v18, v16);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"drawCount", (__int64)"uint32_t", v7);
		//           sub_7FFC805644A0((__int64)v18);
		//           if ( (unsigned __int8)sub_7FFC805A9210(v14, v15, rdx0) )
		//           {
		//             v8 = sub_7FFC80564220((__int64)v18, v17 >> 29);
		//             sub_7FFC805A8FC0((__int64)rdx0, (__int64)"modifier", (__int64)"DrawModifier", v8);
		//             sub_7FFC805644A0((__int64)v18);
		//             set_packet_count((uint64_t)rdx0, v11);
		//             sub_7FFC80564780((__int64)rdx0, 0);
		//             *((_BYTE *)rdx0 + 24) = 1;
		//           }
		//         }
		//         LOBYTE(v4) = nullsub_1(v10);
		//       }
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: drawIndexIndirectMulti");
			// m_cb->drawIndexIndirectMulti();
			count = kPacketCountDrawIndexIndirectMulti;
		}

		return count;
	}

	uint32_t GnmCommandProxy::drawIndexIndirectCountMulti(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   char v11[20]; // [rsp+28h] [rbp-B0h] BYREF
		//   uint32_t v12; // [rsp+3Ch] [rbp-9Ch]
		//   __int16 a2; // [rsp+48h] [rbp-90h] BYREF
		//   unsigned int a2_4; // [rsp+4Ch] [rbp-8Ch]
		//   unsigned __int16 v15; // [rsp+50h] [rbp-88h]
		//   unsigned __int16 v16; // [rsp+54h] [rbp-84h]
		//   unsigned int v17; // [rsp+5Ch] [rbp-7Ch]
		//   __int64 v18; // [rsp+60h] [rbp-78h]
		//   unsigned int v19; // [rsp+6Ch] [rbp-6Ch]
		//   char v20[80]; // [rsp+70h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&a2, 40i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 40192 )
		//     {
		//       LOBYTE(v4) = v19;
		//       if ( (v19 & 3) == 0 && (v19 & 0xC) == 0 && (v19 & 0x20) == 0 )
		//       {
		//         sub_7FFC805AD130((__int64)v11);
		//         v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//         if ( sub_7FFC805AD150(0, *a1, v5, (__int64)v11) )
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"drawIndexIndirectCountMulti");
		//           v6 = sub_7FFC80564220((__int64)v20, a2_4);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dataOffsetInBytes", (__int64)"uint32_t", v6);
		//           sub_7FFC805644A0((__int64)v20);
		//           v7 = sub_7FFC80564220((__int64)v20, v17);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"count", (__int64)"uint32_t", v7);
		//           sub_7FFC805644A0((__int64)v20);
		//           v8 = sub_7FFC80564430(v20, v18);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"countAddress", (__int64)"void*", v8);
		//           sub_7FFC805644A0((__int64)v20);
		//           if ( (unsigned __int8)sub_7FFC805A9210(v15, v16, rdx0) )
		//           {
		//             v9 = sub_7FFC80564220((__int64)v20, v19 >> 29);
		//             sub_7FFC805A8FC0((__int64)rdx0, (__int64)"modifier", (__int64)"DrawModifier", v9);
		//             sub_7FFC805644A0((__int64)v20);
		//             set_packet_count((uint64_t)rdx0, v12);
		//             sub_7FFC80564780((__int64)rdx0, 0);
		//             *((_BYTE *)rdx0 + 24) = 1;
		//           }
		//         }
		//         LOBYTE(v4) = nullsub_1(v11);
		//       }
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: drawIndexIndirectCountMulti");
			// m_cb->drawIndexIndirectCountMulti();
			count = kPacketCountDrawIndexIndirectCountMulti;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setDispatchDrawIndexDeallocationMask(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v5; // [rsp+28h] [rbp-80h]
		//   __int64 v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 733 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setDispatchDrawIndexDeallocationMask");
		//     v3 = sub_7FFC80564220((__int64)v7, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indexMask", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setDispatchDrawIndexDeallocationMask");
			// m_cb->setDispatchDrawIndexDeallocationMask();
			count = kPacketCountSetDispatchDrawIndexDeallocationMask;
		}

		return count;
	}

	uint32_t GnmCommandProxy::waitForSetupDispatchDrawKickRingBuffer(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int16 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   __int64 v10; // [rsp+34h] [rbp-74h]
		//   int v11; // [rsp+3Ch] [rbp-6Ch]
		//   char v12[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 24i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 35840 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"waitForSetupDispatchDrawKickRingBuffer");
		//     v3 = sub_7FFC80564220((__int64)v12, a2_4 & 0x3FF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"krbCount", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v12);
		//     v4 = sub_7FFC80564220((__int64)v12, (a2_4 >> 20) & 0x3FF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"gdsDwOffsetKrb", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v12);
		//     v5 = sub_7FFC80564220((__int64)v12, (a2_4 >> 10) & 0x3FF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"gdsDwOffsetKrbCounters", (__int64)"uint32_t", v5);
		//     sub_7FFC805644A0((__int64)v12);
		//     v6 = sub_7FFC80564430(v12, v10);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"addrIrb", (__int64)"void*", v6);
		//     sub_7FFC805644A0((__int64)v12);
		//     v7 = sub_7FFC80564220((__int64)v12, v11 << 8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"sizeofIrbInBytes", (__int64)"uint32_t", v7);
		//     sub_7FFC805644A0((__int64)v12);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: waitForSetupDispatchDrawKickRingBuffer");
			// m_cb->waitForSetupDispatchDrawKickRingBuffer();
			count = kPacketCountWaitForSetupDispatchDrawKickRingBuffer;
		}

		return count;
	}

	uint32_t GnmCommandProxy::dispatchDraw(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   __int64 v10; // rax
		//   __int64 v11; // rax
		//   __int64 v12; // rax
		//   __int64 v13; // rax
		//   char v15[20]; // [rsp+20h] [rbp-41h] BYREF
		//   uint32_t v16; // [rsp+34h] [rbp-2Dh]
		//   __int16 a2; // [rsp+40h] [rbp-21h]
		//   unsigned int a2_4; // [rsp+44h] [rbp-1Dh]
		//   unsigned int v19; // [rsp+48h] [rbp-19h] BYREF
		//   unsigned int v20; // [rsp+4Ch] [rbp-15h]
		//   char v21[80]; // [rsp+58h] [rbp-9h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&v19, 16i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 36096 && (v20 & 3) == 0 )
		//     {
		//       sub_7FFC805AD130((__int64)v15);
		//       v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//       if ( sub_7FFC805AD150(0, *a1, v5, (__int64)v15) )
		//       {
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"dispatchDraw");
		//         v6 = sub_7FFC80564220((__int64)v21, HIWORD(a2_4) & 0x1F);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"primType", (__int64)"PrimitiveType", v6);
		//         sub_7FFC805644A0((__int64)v21);
		//         v7 = sub_7FFC80564220((__int64)v21, (unsigned __int16)a2_4);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indexOffset", (__int64)"uint32_t", v7);
		//         sub_7FFC805644A0((__int64)v21);
		//         v8 = sub_7FFC80564220((__int64)v21, a2_4 >> 21);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"primGroupIndexCount", (__int64)"uint32_t", v8);
		//         sub_7FFC805644A0((__int64)v21);
		//         v9 = sub_7FFC80564220((__int64)v21, (unsigned int)((unsigned __int16)v19 >> 5) + 1);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"primGroupThreshold", (__int64)"uint32_t", v9);
		//         sub_7FFC805644A0((__int64)v21);
		//         v10 = sub_7FFC80564220((__int64)v21, HIWORD(v19));
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"pollIntervalThreshold", (__int64)"uint32_t", v10);
		//         sub_7FFC805644A0((__int64)v21);
		//         v11 = sub_7FFC80564220((__int64)v21, (v19 >> 4) & 1);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dispatchDrawMode", (__int64)"DispatchDrawMode", v11);
		//         sub_7FFC805644A0((__int64)v21);
		//         v12 = sub_7FFC80564220((__int64)v21, v19 & 0xF);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"sgprVrbLoc", (__int64)"uint32_t", v12);
		//         sub_7FFC805644A0((__int64)v21);
		//         v13 = sub_7FFC80564220((__int64)v21, v20 >> 29);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"modifier", (__int64)"DrawModifier", v13);
		//         sub_7FFC805644A0((__int64)v21);
		//         set_packet_count((uint64_t)rdx0, v16);
		//         sub_7FFC80564780((__int64)rdx0, 0);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//       LOBYTE(v4) = nullsub_1(v15);
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: dispatchDraw");
			// m_cb->dispatchDraw();
			count = kPacketCountDispatchDraw;
		}

		return count;
	}

	uint32_t GnmCommandProxy::dispatchWithOrderedAppend(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   unsigned int v6; // edi
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   int v10; // edi
		//   __int64 v11; // rax
		//   char v13[20]; // [rsp+28h] [rbp-A0h] BYREF
		//   uint32_t v14; // [rsp+3Ch] [rbp-8Ch]
		//   __int16 a2; // [rsp+48h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+4Ch] [rbp-7Ch]
		//   unsigned int v17; // [rsp+50h] [rbp-78h]
		//   unsigned int v18; // [rsp+54h] [rbp-74h]
		//   unsigned int v19; // [rsp+58h] [rbp-70h]
		//   char v20[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&a2, 20i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 5376 )
		//     {
		//       sub_7FFC805AD130((__int64)v13);
		//       v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//       if ( sub_7FFC805AD150(0, *a1, v5, (__int64)v13) )
		//       {
		//         v6 = v19 >> 3;
		//         v7 = sub_7FFC80564220((__int64)v20, a2_4);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"threadGroupX", (__int64)"uint32_t", v7);
		//         sub_7FFC805644A0((__int64)v20);
		//         v8 = sub_7FFC80564220((__int64)v20, v17);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"threadGroupY", (__int64)"uint32_t", v8);
		//         sub_7FFC805644A0((__int64)v20);
		//         v9 = sub_7FFC80564220((__int64)v20, v18);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"threadGroupZ", (__int64)"uint32_t", v9);
		//         sub_7FFC805644A0((__int64)v20);
		//         v10 = v6 & 3;
		//         if ( v10 )
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"dispatchWithOrderedAppend");
		//           v11 = sub_7FFC80564220((__int64)v20, v10);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"orderedAppendMode", (__int64)"DispatchOrderedAppendMode", v11);
		//           sub_7FFC805644A0((__int64)v20);
		//         }
		//         else
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"dispatch");
		//         }
		//         set_packet_count((uint64_t)rdx0, v14);
		//         sub_7FFC80564780((__int64)rdx0, 0);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//       LOBYTE(v4) = nullsub_1(v13);
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: dispatchWithOrderedAppend");
			// m_cb->dispatchWithOrderedAppend();
			count = kPacketCountDispatchWithOrderedAppend;
		}

		return count;
	}

	uint32_t GnmCommandProxy::dispatchIndirectWithOrderedAppend(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   unsigned int v6; // edi
		//   __int64 v7; // rax
		//   int v8; // edi
		//   __int64 v9; // rax
		//   char v11[20]; // [rsp+28h] [rbp-A0h] BYREF
		//   uint32_t v12; // [rsp+3Ch] [rbp-8Ch]
		//   __int16 a2; // [rsp+48h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+4Ch] [rbp-7Ch]
		//   unsigned int v15; // [rsp+50h] [rbp-78h]
		//   char v16[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&a2, 12i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 5632 )
		//     {
		//       sub_7FFC805AD130((__int64)v11);
		//       v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//       if ( sub_7FFC805AD150(0, *a1, v5, (__int64)v11) )
		//       {
		//         v6 = v15 >> 3;
		//         v7 = sub_7FFC80564220((__int64)v16, a2_4);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dataOffsetInBytes", (__int64)"dataOffsetInBytes", v7);
		//         sub_7FFC805644A0((__int64)v16);
		//         v8 = v6 & 3;
		//         if ( v8 )
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"dispatchIndirectWithOrderedAppend");
		//           v9 = sub_7FFC80564220((__int64)v16, v8);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"orderedAppendMode", (__int64)"DispatchOrderedAppendMode", v9);
		//           sub_7FFC805644A0((__int64)v16);
		//         }
		//         else
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"dispatchIndirect");
		//         }
		//         set_packet_count((uint64_t)rdx0, v12);
		//         sub_7FFC80564780((__int64)rdx0, 0);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//       LOBYTE(v4) = nullsub_1(v11);
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: dispatchIndirectWithOrderedAppend");
			// m_cb->dispatchIndirectWithOrderedAppend();
			count = kPacketCountDispatchIndirectWithOrderedAppend;
		}

		return count;
	}

	uint32_t GnmCommandProxy::drawIndexMultiInstanced(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   __int64 v10; // rax
		//   char v12[20]; // [rsp+28h] [rbp-B0h] BYREF
		//   uint32_t v13; // [rsp+3Ch] [rbp-9Ch]
		//   __int16 a2; // [rsp+48h] [rbp-90h] BYREF
		//   unsigned int a2_4; // [rsp+4Ch] [rbp-8Ch]
		//   __int64 v16; // [rsp+50h] [rbp-88h]
		//   __int64 v17; // [rsp+5Ch] [rbp-7Ch]
		//   unsigned int v18; // [rsp+64h] [rbp-74h]
		//   unsigned int v19; // [rsp+68h] [rbp-70h]
		//   char v20[80]; // [rsp+70h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&a2, 36i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 14848 )
		//     {
		//       sub_7FFC805AD130((__int64)v12);
		//       v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//       if ( sub_7FFC805AD150(0, *a1, v5, (__int64)v12) )
		//       {
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"drawIndexMultiInstanced");
		//         v6 = sub_7FFC80564220((__int64)v20, v18);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indexCount", (__int64)"uint32_t", v6);
		//         sub_7FFC805644A0((__int64)v20);
		//         v7 = sub_7FFC80564220((__int64)v20, a2_4);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"instanceCount", (__int64)"uint32_t", v7);
		//         sub_7FFC805644A0((__int64)v20);
		//         v8 = sub_7FFC80564430(v20, v17);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"indexAddr", (__int64)"void*", v8);
		//         sub_7FFC805644A0((__int64)v20);
		//         v9 = sub_7FFC80564430(v20, v16);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"objectIdAddr", (__int64)"void*", v9);
		//         sub_7FFC805644A0((__int64)v20);
		//         v10 = sub_7FFC80564220((__int64)v20, v19 >> 29);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"modifier", (__int64)"DrawModifier", v10);
		//         sub_7FFC805644A0((__int64)v20);
		//         set_packet_count((uint64_t)rdx0, v13);
		//         sub_7FFC80564780((__int64)rdx0, 0);
		//         *((_BYTE *)rdx0 + 26) = 1;
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//       LOBYTE(v4) = nullsub_1(v12);
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: drawIndexMultiInstanced");
			// m_cb->drawIndexMultiInstanced();
			count = kPacketCountDrawIndexMultiInstanced;
		}

		return count;
	}

	uint32_t GnmCommandProxy::writeOcclusionQuery(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 a2; // [rsp+30h] [rbp-C8h] BYREF
		//   __int64 v6; // [rsp+40h] [rbp-B8h]
		//   int v7; // [rsp+4Ch] [rbp-ACh]
		//   int v8; // [rsp+68h] [rbp-90h]
		//   int v9; // [rsp+6Ch] [rbp-8Ch]
		//   int v10; // [rsp+78h] [rbp-80h]
		//   int v11; // [rsp+7Ch] [rbp-7Ch]
		//   unsigned int v12; // [rsp+80h] [rbp-78h]
		//   unsigned __int16 v13; // [rsp+84h] [rbp-74h]
		//   char v14[80]; // [rsp+90h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 88i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 20480
		//     && (v7 & 0xFF00) == 15360
		//     && (v8 & 0xFF00) == 17920
		//     && (v10 & 0xFF00) == 17920
		//     && (v9 & 0x3F) == 56
		//     && (v11 & 0x3F) == 57
		//     && v6 == __PAIR64__(v13, v12) )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"writeOcclusionQuery");
		//     v3 = sub_7FFC80564220((__int64)v14, 0);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"queryOp", (__int64)"OcclusionQueryOp", v3);
		//     sub_7FFC805644A0((__int64)v14);
		//     v4 = sub_7FFC80564430(v14, v6);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"queryResults", (__int64)"OcclusionQueryResults*", v4);
		//     sub_7FFC805644A0((__int64)v14);
		//     set_packet_count((uint64_t)rdx0, 4u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: writeOcclusionQuery");
			// m_cb->writeOcclusionQuery();
			count = kPacketCountWriteOcclusionQuery;
		}

		return count;
	}

	uint32_t GnmCommandProxy::writeOcclusionQuery2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int16 a2; // [rsp+28h] [rbp-90h] BYREF
		//   char a2_4; // [rsp+2Ch] [rbp-8Ch]
		//   int v7; // [rsp+38h] [rbp-80h]
		//   int v8; // [rsp+3Ch] [rbp-7Ch]
		//   unsigned int v9; // [rsp+40h] [rbp-78h]
		//   unsigned __int16 v10; // [rsp+44h] [rbp-74h]
		//   char v11[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 32i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 17920
		//     && (v7 & 0xFF00) == 17920
		//     && (a2_4 & 0x3F) == 56
		//     && (v8 & 0x3F) == 57 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"writeOcclusionQuery");
		//     v3 = sub_7FFC80564400(v11);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"queryOp", (__int64)"OcclusionQueryOp", v3);
		//     sub_7FFC805644A0((__int64)v11);
		//     v4 = sub_7FFC80564430(v11, v9 | ((unsigned __int64)v10 << 32));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"queryResults", (__int64)"OcclusionQueryResults*", v4);
		//     sub_7FFC805644A0((__int64)v11);
		//     set_packet_count((uint64_t)rdx0, 2u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: writeOcclusionQuery");
			// m_cb->writeOcclusionQuery();
			count = kPacketCountWriteOcclusionQuery2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setZPassPredicationEnable(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int16 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 0x2000
		//     && (v8 & 0x70000) == 0x10000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setZPassPredicationEnable");
		//     v3 = sub_7FFC80564430(v9, a2_4 | ((unsigned __int64)(unsigned __int8)v8 << 32));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"queryResults", (__int64)"OcclusionQueryResults*", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, (v8 >> 12) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"hint", (__int64)"PredicationZPassHint", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     v5 = sub_7FFC80564220((__int64)v9, (v8 >> 8) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"action", (__int64)"PredicationZPassAction", v5);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setZPassPredicationEnable");
			// m_cb->setZPassPredicationEnable();
			count = kPacketCountSetZPassPredicationEnable;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setZPassPredicationDisable(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 a2; // [rsp+20h] [rbp-28h] BYREF
		//   int v4; // [rsp+28h] [rbp-20h]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 0x2000
		//     && (v4 & 0x70000) == 0 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setZPassPredicationDisable");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setZPassPredicationDisable");
			// m_cb->setZPassPredicationDisable();
			count = kPacketCountSetZPassPredicationDisable;
		}

		return count;
	}

	uint32_t GnmCommandProxy::writeDataInline(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   unsigned int packet_size; // eax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   unsigned int a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   __int64 v12; // [rsp+30h] [rbp-78h]
		//   char v13[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType(a1, (__int64)&a2, 16i64, *((_DWORD *)a1 + 2)) && (a2 & 0xFF00) == 14080 && (a2_4 & 0xF00) == 1280 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"writeDataInline");
		//     v4 = sub_7FFC80564430(v13, v12);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dstGpuAddr", (__int64)"void*", v4);
		//     sub_7FFC805644A0((__int64)v13);
		//     v5 = sub_7FFC80594650(*a1, a1[1]);
		//     v6 = sub_7FFC80564430(v13, v5 + 16);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"data", (__int64)"void*", v6);
		//     sub_7FFC805644A0((__int64)v13);
		//     packet_size = get_packet_size(a2);
		//     v8 = sub_7FFC80564430(v13, ((unsigned __int64)packet_size - 16) >> 2);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"sizeInDwords", (__int64)"uint32_t", v8);
		//     sub_7FFC805644A0((__int64)v13);
		//     v9 = sub_7FFC80564220((__int64)v13, (a2_4 >> 20) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"writeConfirm", (__int64)"WriteDataConfirmMode", v9);
		//     sub_7FFC805644A0((__int64)v13);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: writeDataInline");
			// m_cb->writeDataInline();
			count = kPacketCountWriteDataInline;
		}

		return count;
	}

	uint32_t GnmCommandProxy::writeDataInlineThroughL2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   unsigned int packet_size; // eax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   __int64 v10; // rax
		//   unsigned int a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   __int64 v13; // [rsp+30h] [rbp-78h]
		//   char v14[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType(a1, (__int64)&a2, 16i64, *((_DWORD *)a1 + 2)) && (a2 & 0xFF00) == 14080 && (a2_4 & 0xF00) == 512 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"writeDataInlineThroughL2");
		//     v4 = sub_7FFC80564430(v14, v13);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dstGpuAddr", (__int64)"void*", v4);
		//     sub_7FFC805644A0((__int64)v14);
		//     v5 = sub_7FFC80594650(*a1, a1[1]);
		//     v6 = sub_7FFC80564430(v14, v5 + 16);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"data", (__int64)"void*", v6);
		//     sub_7FFC805644A0((__int64)v14);
		//     packet_size = get_packet_size(a2);
		//     v8 = sub_7FFC80564430(v14, ((unsigned __int64)packet_size - 16) >> 2);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"sizeInDwords", (__int64)"uint32_t", v8);
		//     sub_7FFC805644A0((__int64)v14);
		//     v9 = sub_7FFC80564220((__int64)v14, (a2_4 >> 25) & 3);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cachePolicy", (__int64)"CachePolicy", v9);
		//     sub_7FFC805644A0((__int64)v14);
		//     v10 = sub_7FFC80564220((__int64)v14, (a2_4 >> 20) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"writeConfirm", (__int64)"WriteDataConfirmMode", v10);
		//     sub_7FFC805644A0((__int64)v14);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: writeDataInlineThroughL");
			// m_cb->writeDataInlineThroughL();
			count = kPacketCountWriteDataInlineThroughL2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::writeAtEndOfPipe(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // r8
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   __int64 v10; // rax
		//   __int64 v11; // rax
		//   char v13[16]; // [rsp+18h] [rbp-39h] BYREF
		//   __int64 v14; // [rsp+28h] [rbp-29h]
		//   __int16 a2; // [rsp+30h] [rbp-21h]
		//   unsigned int a2_4; // [rsp+34h] [rbp-1Dh]
		//   unsigned int v17; // [rsp+38h] [rbp-19h] BYREF
		//   unsigned int v18; // [rsp+3Ch] [rbp-15h]
		//   __int64 v19; // [rsp+40h] [rbp-11h]
		//   char v20[80]; // [rsp+48h] [rbp-9h] BYREF
		//
		//   v14 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&v17, 24i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 18176 )
		//     {
		//       v3 = a2_4 & 0xF00;
		//       if ( v3 == 1280 && (v18 & 0x7000000) == 0 )
		//       {
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"writeAtEndOfPipe");
		//         if ( v18 >> 29 )
		//         {
		//           v4 = 8i64;
		//           if ( v18 >> 29 == 1 )
		//             v4 = 4i64;
		//         }
		//         else
		//         {
		//           v4 = 0i64;
		//         }
		//         _lambda_082c17da81b0962e08c0587ee0fac50c_::_lambda_082c17da81b0962e08c0587ee0fac50c_(
		//           (_lambda_082c17da81b0962e08c0587ee0fac50c_ *)v13,
		//           (int *)(v17 | ((unsigned __int64)(unsigned __int16)v18 << 32)),
		//           (const char **)v4);
		//         v5 = sub_7FFC80564220((__int64)v20, a2_4 & 0x3F);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"eventType", (__int64)"EndOfPipeEventType", v5);
		//         sub_7FFC805644A0((__int64)v20);
		//         v6 = sub_7FFC80564220((__int64)v20, (v18 & 0x30000 | (a2_4 >> 7) & 0x100000) >> 16);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dstSelector", (__int64)"EventWriteDest", v6);
		//         sub_7FFC805644A0((__int64)v20);
		//         v7 = sub_7FFC80564280(v20, v13, 4i64);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"label", (__int64)"ResourceLabel", v7);
		//         sub_7FFC805644A0((__int64)v20);
		//         v8 = sub_7FFC80564220((__int64)v20, v18 >> 29);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"srcSelector", (__int64)"EventWriteSource", v8);
		//         sub_7FFC805644A0((__int64)v20);
		//         v9 = sub_7FFC80564430(v20, v19);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"immValue", (__int64)"uint64_t", v9);
		//         sub_7FFC805644A0((__int64)v20);
		//         v10 = sub_7FFC80564220((__int64)v20, (a2_4 >> 12) & 0x3F);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cacheAction", (__int64)"CacheAction", v10);
		//         sub_7FFC805644A0((__int64)v20);
		//         v11 = sub_7FFC80564220((__int64)v20, (a2_4 >> 25) & 3);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cachePolicy", (__int64)"CachePolicy", v11);
		//         sub_7FFC805644A0((__int64)v20);
		//         set_packet_count((uint64_t)rdx0, 1u);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//         LOBYTE(v3) = nullsub_1(v13);
		//       }
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: writeAtEndOfPipe");
			// m_cb->writeAtEndOfPipe();
			count = kPacketCountWriteAtEndOfPipe;
		}

		return count;
	}

	uint32_t GnmCommandProxy::writeAtEndOfPipeWithInterrupt(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // r8
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   __int64 v10; // rax
		//   __int64 v11; // rax
		//   char v13[16]; // [rsp+18h] [rbp-39h] BYREF
		//   __int64 v14; // [rsp+28h] [rbp-29h]
		//   __int16 a2; // [rsp+30h] [rbp-21h]
		//   unsigned int a2_4; // [rsp+34h] [rbp-1Dh]
		//   unsigned int v17; // [rsp+38h] [rbp-19h] BYREF
		//   unsigned int v18; // [rsp+3Ch] [rbp-15h]
		//   __int64 v19; // [rsp+40h] [rbp-11h]
		//   char v20[80]; // [rsp+48h] [rbp-9h] BYREF
		//
		//   v14 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&v17, 24i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 18176 )
		//     {
		//       v3 = a2_4 & 0xF00;
		//       if ( v3 == 1280 )
		//       {
		//         LOBYTE(v3) = 0;
		//         if ( (v18 & 0x7000000) == 0x2000000 )
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"writeAtEndOfPipeWithInterrupt");
		//           if ( v18 >> 29 )
		//           {
		//             v4 = 8i64;
		//             if ( v18 >> 29 == 1 )
		//               v4 = 4i64;
		//           }
		//           else
		//           {
		//             v4 = 0i64;
		//           }
		//           _lambda_082c17da81b0962e08c0587ee0fac50c_::_lambda_082c17da81b0962e08c0587ee0fac50c_(
		//             (_lambda_082c17da81b0962e08c0587ee0fac50c_ *)v13,
		//             (int *)(v17 | ((unsigned __int64)(unsigned __int16)v18 << 32)),
		//             (const char **)v4);
		//           v5 = sub_7FFC80564220((__int64)v20, a2_4 & 0x3F);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"eventType", (__int64)"EndOfPipeEventType", v5);
		//           sub_7FFC805644A0((__int64)v20);
		//           v6 = sub_7FFC80564220((__int64)v20, (v18 & 0x30000 | (a2_4 >> 7) & 0x100000) >> 16);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dstSelector", (__int64)"EventWriteDest", v6);
		//           sub_7FFC805644A0((__int64)v20);
		//           v7 = sub_7FFC80564280(v20, v13, 4i64);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"label", (__int64)"ResourceLabel", v7);
		//           sub_7FFC805644A0((__int64)v20);
		//           v8 = sub_7FFC80564220((__int64)v20, v18 >> 29);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"srcSelector", (__int64)"EventWriteSource", v8);
		//           sub_7FFC805644A0((__int64)v20);
		//           v9 = sub_7FFC80564430(v20, v19);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"immValue", (__int64)"uint64_t", v9);
		//           sub_7FFC805644A0((__int64)v20);
		//           v10 = sub_7FFC80564220((__int64)v20, (a2_4 >> 12) & 0x3F);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cacheAction", (__int64)"CacheAction", v10);
		//           sub_7FFC805644A0((__int64)v20);
		//           v11 = sub_7FFC80564220((__int64)v20, (a2_4 >> 25) & 3);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cachePolicy", (__int64)"CachePolicy", v11);
		//           sub_7FFC805644A0((__int64)v20);
		//           set_packet_count((uint64_t)rdx0, 1u);
		//           *((_BYTE *)rdx0 + 24) = 1;
		//           LOBYTE(v3) = nullsub_1(v13);
		//         }
		//       }
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: writeAtEndOfPipeWithInterrupt");
			// m_cb->writeAtEndOfPipeWithInterrupt();
			count = kPacketCountWriteAtEndOfPipeWithInterrupt;
		}

		return count;
	}

	uint32_t GnmCommandProxy::triggerEndOfPipeInterrupt(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int16 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   int v7; // [rsp+34h] [rbp-74h]
		//   int v8; // [rsp+38h] [rbp-70h]
		//   int v9; // [rsp+3Ch] [rbp-6Ch]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 24i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 18176
		//     && (a2_4 & 0xF00) == 1280
		//     && (v7 & 0x7000000) == 0x1000000
		//     && (v7 & 0xE0000000) == 0
		//     && !v8
		//     && !v9 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"triggerEndOfPipeInterrupt");
		//     v3 = sub_7FFC80564220((__int64)v10, a2_4 & 0x3F);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"eventType", (__int64)"EndOfPipeEventType", v3);
		//     sub_7FFC805644A0((__int64)v10);
		//     v4 = sub_7FFC80564220((__int64)v10, (a2_4 >> 12) & 0x3F);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cacheAction", (__int64)"CacheAction", v4);
		//     sub_7FFC805644A0((__int64)v10);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: triggerEndOfPipeInterrupt");
			// m_cb->triggerEndOfPipeInterrupt();
			count = kPacketCountTriggerEndOfPipeInterrupt;
		}

		return count;
	}

	uint32_t GnmCommandProxy::writeAtEndOfShader(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   char v8[16]; // [rsp+20h] [rbp-98h] BYREF
		//   __int64 v9; // [rsp+30h] [rbp-88h]
		//   __int16 a2; // [rsp+38h] [rbp-80h] BYREF
		//   __int16 a2_4; // [rsp+3Ch] [rbp-7Ch]
		//   unsigned int v12; // [rsp+40h] [rbp-78h]
		//   int v13; // [rsp+44h] [rbp-74h]
		//   unsigned int v14; // [rsp+48h] [rbp-70h]
		//   char v15[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 20i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 18432 )
		//     {
		//       v3 = a2_4 & 0xF00;
		//       if ( v3 == 1536 )
		//       {
		//         LOBYTE(v3) = 0;
		//         if ( (v13 & 0xE0000000) == 0x40000000 )
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"writeAtEndOfShader");
		//           v4 = sub_7FFC80564220((__int64)v15, a2_4 & 0x3F);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"eventType", (__int64)"EndOfShaderEventType", v4);
		//           sub_7FFC805644A0((__int64)v15);
		//           _lambda_082c17da81b0962e08c0587ee0fac50c_::_lambda_082c17da81b0962e08c0587ee0fac50c_(
		//             (_lambda_082c17da81b0962e08c0587ee0fac50c_ *)v8,
		//             (int *)(v12 | ((unsigned __int64)(unsigned __int16)v13 << 32)),
		//             (const char **)4);
		//           v5 = sub_7FFC80564280(v15, v8, 4i64);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"label", (__int64)"ResourceLabel", v5);
		//           sub_7FFC805644A0((__int64)v15);
		//           v6 = sub_7FFC80564220((__int64)v15, v14);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"immValue", (__int64)"uint32_t", v6);
		//           sub_7FFC805644A0((__int64)v15);
		//           set_packet_count((uint64_t)rdx0, 1u);
		//           *((_BYTE *)rdx0 + 24) = 1;
		//           LOBYTE(v3) = nullsub_1(v8);
		//         }
		//       }
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: writeAtEndOfShader");
			// m_cb->writeAtEndOfShader();
			count = kPacketCountWriteAtEndOfShader;
		}

		return count;
	}

	uint32_t GnmCommandProxy::stallCommandBufferParser(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int16 a2; // [rsp+30h] [rbp+8h] BYREF
		//   int a2_4; // [rsp+34h] [rbp+Ch]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 8i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 16896 && !a2_4 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"stallCommandBufferParser");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: stallCommandBufferParser");
			// m_cb->stallCommandBufferParser();
			count = kPacketCountStallCommandBufferParser;
		}

		return count;
	}

	uint32_t GnmCommandProxy::waitOnAddressAndStallCommandBufferParser(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   char v8[16]; // [rsp+20h] [rbp-A8h] BYREF
		//   __int64 v9; // [rsp+30h] [rbp-98h]
		//   __int16 a2; // [rsp+38h] [rbp-90h] BYREF
		//   __int16 a2_4; // [rsp+3Ch] [rbp-8Ch]
		//   unsigned int v12; // [rsp+40h] [rbp-88h]
		//   unsigned __int16 v13; // [rsp+44h] [rbp-84h]
		//   unsigned int v14; // [rsp+48h] [rbp-80h]
		//   unsigned int v15; // [rsp+4Ch] [rbp-7Ch]
		//   char v16[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 15360 )
		//     {
		//       LOBYTE(v3) = a2_4;
		//       if ( (a2_4 & 0x100) != 0 )
		//       {
		//         LOBYTE(v3) = a2_4 & 7;
		//         if ( (a2_4 & 7) == 5 )
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"waitOnAddressAndStallCommandBufferParser");
		//           _lambda_082c17da81b0962e08c0587ee0fac50c_::_lambda_082c17da81b0962e08c0587ee0fac50c_(
		//             (_lambda_082c17da81b0962e08c0587ee0fac50c_ *)v8,
		//             (int *)(v12 | ((unsigned __int64)v13 << 32)),
		//             (const char **)4);
		//           v4 = sub_7FFC80564280(v16, v8, 4i64);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"label", (__int64)"ResourceLabel", v4);
		//           sub_7FFC805644A0((__int64)v16);
		//           v5 = sub_7FFC80564220((__int64)v16, v15);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"mask", (__int64)"uint32_t", v5);
		//           sub_7FFC805644A0((__int64)v16);
		//           v6 = sub_7FFC80564220((__int64)v16, v14);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"refValue", (__int64)"uint32_t", v6);
		//           sub_7FFC805644A0((__int64)v16);
		//           set_packet_count((uint64_t)rdx0, 1u);
		//           *((_BYTE *)rdx0 + 24) = 1;
		//           LOBYTE(v3) = nullsub_1(v8);
		//         }
		//       }
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: waitOnAddressAndStallCommandBufferParser");
			// m_cb->waitOnAddressAndStallCommandBufferParser();
			count = kPacketCountWaitOnAddressAndStallCommandBufferParser;
		}

		return count;
	}

	uint32_t GnmCommandProxy::waitOnRegister(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int16 a2; // [rsp+28h] [rbp-90h] BYREF
		//   __int16 a2_4; // [rsp+2Ch] [rbp-8Ch]
		//   unsigned int v9; // [rsp+30h] [rbp-88h]
		//   unsigned int v10; // [rsp+38h] [rbp-80h]
		//   unsigned int v11; // [rsp+3Ch] [rbp-7Ch]
		//   char v12[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 15360
		//     && (a2_4 & 0x100) == 0
		//     && (a2_4 & 0x30) == 0 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"waitOnRegister");
		//     v3 = sub_7FFC80564220((__int64)v12, v9);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"gpuReg", (__int64)"uint16_t", v3);
		//     sub_7FFC805644A0((__int64)v12);
		//     v4 = sub_7FFC80564220((__int64)v12, v11);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"mask", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v12);
		//     v5 = sub_7FFC80564220((__int64)v12, a2_4 & 7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"compareFunc", (__int64)"WaitCompareFunc", v5);
		//     sub_7FFC805644A0((__int64)v12);
		//     v6 = sub_7FFC80564220((__int64)v12, v10);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"refValue", (__int64)"uint32_t", v6);
		//     sub_7FFC805644A0((__int64)v12);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: waitOnRegister");
			// m_cb->waitOnRegister();
			count = kPacketCountWaitOnRegister;
		}

		return count;
	}

	uint32_t GnmCommandProxy::flushShaderCachesAndWait(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int16 a2; // [rsp+28h] [rbp-90h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-8Ch]
		//   int v8; // [rsp+38h] [rbp-80h]
		//   char v9[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 22528
		//     && !v8
		//     && (a2_4 & 0x7FC0) == 0 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"flushShaderCachesAndWait");
		//     v3 = sub_7FFC80564220((__int64)v9, (a2_4 & 0x58000 | (a2_4 >> 3) & 0x180000) >> 15);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cacheAction", (__int64)"CacheAction", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, a2_4 & 0x2E000000);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"extendedCacheMask", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     v5 = sub_7FFC80564220((__int64)v9, a2_4 >> 31);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"commandBufferStallMode", (__int64)"StallCommandBufferParserMode", v5);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: flushShaderCachesAndWait");
			// m_cb->flushShaderCachesAndWait();
			count = kPacketCountFlushShaderCachesAndWait;
		}

		return count;
	}

	uint32_t GnmCommandProxy::signalSemaphore(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int16 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 14592
		//     && (v8 & 0xE0000000) == -1073741824 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"signalSemaphore");
		//     v3 = sub_7FFC80564430(v9, a2_4 | ((unsigned __int64)(unsigned __int16)v8 << 32));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"semAddr", (__int64)"uint64_t*", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, (v8 >> 20) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"behavior", (__int64)"SemaphoreSignalBehavior", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     v5 = sub_7FFC80564220((__int64)v9, HIWORD(v8) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"updateConfirm", (__int64)"SemaphoreUpdateConfirmMode", v5);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: signalSemaphore");
			// m_cb->signalSemaphore();
			count = kPacketCountSignalSemaphore;
		}

		return count;
	}

	uint32_t GnmCommandProxy::waitSemaphore(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int16 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v7; // [rsp+30h] [rbp-78h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 14592
		//     && (v7 & 0xE0000000) == -536870912 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"waitSemaphore");
		//     v3 = sub_7FFC80564430(v8, a2_4 | ((unsigned __int64)(unsigned __int16)v7 << 32));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"semAddr", (__int64)"uint64_t*", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     v4 = sub_7FFC80564220((__int64)v8, (v7 >> 20) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"behavior", (__int64)"SemaphoreWaitBehavior", v4);
		//     sub_7FFC805644A0((__int64)v8);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: waitSemaphore");
			// m_cb->waitSemaphore();
			count = kPacketCountWaitSemaphore;
		}

		return count;
	}

	uint32_t GnmCommandProxy::writeEventStats(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // ecx
		//   int v4; // eax
		//   unsigned int v5; // ebx
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int16 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v10; // [rsp+30h] [rbp-78h]
		//   unsigned __int16 v11; // [rsp+34h] [rbp-74h]
		//   char v12[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 17920 )
		//   {
		//     v3 = a2_4 & 0x3F;
		//     v4 = (a2_4 >> 8) & 0xF;
		//     switch ( v3 )
		//     {
		//       case 21:
		//         if ( v4 != 1 )
		//           return;
		//         v5 = 0;
		//         goto LABEL_21;
		//       case 30:
		//         if ( v4 != 2 )
		//           return;
		//         v5 = 1;
		//         goto LABEL_21;
		//       case 32:
		//         if ( v4 != 3 )
		//           return;
		//         v5 = 2;
		//         goto LABEL_21;
		//       case 1:
		//         if ( v4 != 3 )
		//           return;
		//         v5 = 3;
		//         goto LABEL_21;
		//       case 2:
		//         if ( v4 != 3 )
		//           return;
		//         v5 = 4;
		//         goto LABEL_21;
		//     }
		//     if ( v3 == 3 && v4 == 3 )
		//     {
		//       v5 = 5;
		// LABEL_21:
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"writeEventStats");
		//       v6 = sub_7FFC80564220((__int64)v12, v5);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"eventStats", (__int64)"EventStats", v6);
		//       sub_7FFC805644A0((__int64)v12);
		//       v7 = sub_7FFC80564430(v12, v10 | ((unsigned __int64)v11 << 32));
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dstGpuAddr", (__int64)"void*", v7);
		//       sub_7FFC805644A0((__int64)v12);
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: writeEventStats");
			// m_cb->writeEventStats();
			count = kPacketCountWriteEventStats;
		}

		return count;
	}

	uint32_t GnmCommandProxy::dmaData(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   bool v3; // si
		//   int v4; // eax
		//   bool v5; // di
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   const char *v8; // rbx
		//   const char *v9; // r8
		//   __int64 v10; // rax
		//   __int64 v11; // rax
		//   __int64 v12; // rax
		//   __int64 v13; // rax
		//   __int16 a2; // [rsp+28h] [rbp-51h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-4Dh]
		//   __int64 v16; // [rsp+30h] [rbp-49h]
		//   __int64 v17; // [rsp+38h] [rbp-41h]
		//   unsigned int v18; // [rsp+40h] [rbp-39h]
		//   char v19[80]; // [rsp+50h] [rbp-29h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 20480 )
		//   {
		//     v3 = ((a2_4 >> 29) & 3) == 0 || ((a2_4 >> 29) & 3) == 3;
		//     v4 = (a2_4 >> 20) & 3;
		//     v5 = !v4 || v4 == 3;
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"dmaData");
		//     v6 = sub_7FFC80564220((__int64)v19, (a2_4 & 0x300000 | ((v18 & 0x8000000 | (v18 >> 1) & 0x10000000) >> 5)) >> 20);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dstSel", (__int64)"DmaDataDst", v6);
		//     sub_7FFC805644A0((__int64)v19);
		//     v7 = sub_7FFC80564430(v19, v17);
		//     v8 = "uint64_t";
		//     v9 = "uint64_t";
		//     if ( v5 )
		//       v9 = "void*";
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dst", (__int64)v9, v7);
		//     sub_7FFC805644A0((__int64)v19);
		//     v10 = sub_7FFC80564220((__int64)v19, (v18 & 0x4000000 | ((v18 & 0x10000000 | (a2_4 >> 4) & 0x6000000) >> 1)) >> 24);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"srcSel", (__int64)"DmaDataSrc", v10);
		//     sub_7FFC805644A0((__int64)v19);
		//     v11 = sub_7FFC80564430(v19, v16);
		//     if ( v3 )
		//       v8 = "void*";
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"srcOrData", (__int64)v8, v11);
		//     sub_7FFC805644A0((__int64)v19);
		//     v12 = sub_7FFC80564220((__int64)v19, v18 & 0x1FFFFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"numBytes", (__int64)"uint32_t", v12);
		//     sub_7FFC805644A0((__int64)v19);
		//     v13 = sub_7FFC80564220((__int64)v19, a2_4 >> 31);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"isBlocking", (__int64)"DmaDataBlockingMode", v13);
		//     sub_7FFC805644A0((__int64)v19);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: dmaData");
			// m_cb->dmaData();
			count = kPacketCountDmaData;
		}

		return count;
	}

	uint32_t GnmCommandProxy::disableOrderedAppendAllocationCounter(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int v5; // [rsp+30h] [rbp-78h]
		//   unsigned int v6; // [rsp+38h] [rbp-70h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 20i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30976
		//     && WORD2(a2) == 1053
		//     && (v6 & 0x80000000) == 0 )
		//   {
		//     set_packet_count((uint64_t)rdx0, (v6 >> 31) + 1);
		//     v3 = sub_7FFC80564220((__int64)v7, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"oaCounterIndex", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"disableOrderedAppendAllocationCounter");
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: disableOrderedAppendAllocationCounter");
			// m_cb->disableOrderedAppendAllocationCounter();
			count = kPacketCountDisableOrderedAppendAllocationCounter;
		}

		return count;
	}

	uint32_t GnmCommandProxy::enableOrderedAppendAllocationCounter(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v3; // esi
		//   unsigned int v4; // ebp
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   int v7; // esi
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   __int64 v10; // rax
		//   __int64 v11; // rax
		//   __int64 a2; // [rsp+28h] [rbp-90h] BYREF
		//   unsigned int v13; // [rsp+30h] [rbp-88h]
		//   unsigned int v14; // [rsp+34h] [rbp-84h]
		//   unsigned int v15; // [rsp+38h] [rbp-80h]
		//   char v16[80]; // [rsp+40h] [rbp-78h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 20i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30976
		//     && WORD2(a2) == 1053 )
		//   {
		//     v3 = v15;
		//     if ( v15 >> 31 == 1 )
		//     {
		//       v4 = (v15 >> 20) & 0xF;
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       v5 = sub_7FFC80564220((__int64)v16, v13);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"oaCounterIndex", (__int64)"uint32_t", v5);
		//       sub_7FFC805644A0((__int64)v16);
		//       v6 = sub_7FFC80564220((__int64)v16, (unsigned __int16)v3 >> 2);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"gdsDwOffsetOfCounter", (__int64)"uint32_t", v6);
		//       sub_7FFC805644A0((__int64)v16);
		//       v7 = HIWORD(v3) & 0xF;
		//       if ( v7 )
		//       {
		//         if ( v7 == 1 )
		//           v9 = sub_7FFC80564220((__int64)v16, 2u);
		//         else
		//           v9 = sub_7FFC80564400(v16);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stage", (__int64)"ShaderStage", v9);
		//       }
		//       else
		//       {
		//         v8 = sub_7FFC80564220((__int64)v16, 1u);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stage", (__int64)"ShaderStage", v8);
		//       }
		//       sub_7FFC805644A0((__int64)v16);
		//       v10 = sub_7FFC80564220((__int64)v16, v4);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"oaOpIndex", (__int64)"uint32_t", v10);
		//       sub_7FFC805644A0((__int64)v16);
		//       v11 = sub_7FFC80564220((__int64)v16, v14);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"spaceInAllocationUnits", (__int64)"uint32_t", v11);
		//       sub_7FFC805644A0((__int64)v16);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"enableOrderedAppendAllocationCounter");
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: enableOrderedAppendAllocationCounter");
			// m_cb->enableOrderedAppendAllocationCounter();
			count = kPacketCountEnableOrderedAppendAllocationCounter;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setVsShaderStreamoutEnable(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   int v6; // [rsp+28h] [rbp-80h]
		//   __int64 v7; // [rsp+30h] [rbp-78h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 741
		//     && (a2 & 0x3FFF0000) == 0x10000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setVsShaderStreamoutEnable");
		//     v3 = sub_7FFC80564220((__int64)v8, v6 & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"enable", (__int64)"bool", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setVsShaderStreamoutEnable");
			// m_cb->setVsShaderStreamoutEnable();
			count = kPacketCountSetVsShaderStreamoutEnable;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setupDrawOpaqueParameters(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 a2; // [rsp+28h] [rbp-A0h] BYREF
		//   unsigned int v7; // [rsp+30h] [rbp-98h]
		//   int v8; // [rsp+34h] [rbp-94h]
		//   __int16 v9; // [rsp+38h] [rbp-90h]
		//   unsigned int v10; // [rsp+3Ch] [rbp-8Ch]
		//   int v11; // [rsp+40h] [rbp-88h]
		//   int v12; // [rsp+44h] [rbp-84h]
		//   __int64 v13; // [rsp+48h] [rbp-80h]
		//   int v14; // [rsp+50h] [rbp-78h]
		//   int v15; // [rsp+54h] [rbp-74h]
		//   char v16[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 48i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && WORD2(a2) == 716
		//     && (v8 & 0xFF00) == 26880
		//     && v9 == 714
		//     && (v11 & 0xFF00) == 0x4000
		//     && (v12 & 0xF) == 1
		//     && v14 == 41675
		//     && !v15 )
		//   {
		//     v3 = sub_7FFC80564430(v16, v13);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"sizeLocation", (__int64)"void*", v3);
		//     sub_7FFC805644A0((__int64)v16);
		//     v4 = sub_7FFC80564220((__int64)v16, v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"stride", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v16);
		//     v5 = sub_7FFC80564220((__int64)v16, v10);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"offset", (__int64)"uint32_t", v5);
		//     sub_7FFC805644A0((__int64)v16);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setupDrawOpaqueParameters");
		//     set_packet_count((uint64_t)rdx0, 3u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setupDrawOpaqueParameters");
			// m_cb->setupDrawOpaqueParameters();
			count = kPacketCountSetupDrawOpaqueParameters;
		}

		return count;
	}

	uint32_t GnmCommandProxy::requestMipStatsReportAndReset(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned __int64 v3; // rbx
		//   int v4; // edi
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   int v7[3]; // [rsp+28h] [rbp-80h] BYREF
		//   int v8; // [rsp+34h] [rbp-74h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)v7, 16i64, *(_DWORD *)(a1 + 8))
		//     && (v7[0] & 0xFF00) == 36352
		//     && (v8 & 0x3FC) == 0
		//     && (v8 & 0x3FC00) == 261120
		//     && (v8 & 0x40000) == 0
		//     && (v8 & 0x80000) != 0
		//     && (v8 & 0x300000) == 0 )
		//   {
		//     v3 = ((unsigned int)v7[2] | ((unsigned __int64)(v8 & 3) << 32)) << 6;
		//     v4 = v7[1];
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       a2,
		//       (struct Concurrency::details::SchedulerNode *)"requestMipStatsReportAndReset");
		//     v5 = sub_7FFC80564430(v9, v3);
		//     sub_7FFC805A8FC0((__int64)a2, (__int64)"outputBuffer", (__int64)"void*", v5);
		//     sub_7FFC805644A0((__int64)v9);
		//     v6 = sub_7FFC80564220((__int64)v9, v4 + 32);
		//     sub_7FFC805A8FC0((__int64)a2, (__int64)"sizeInByte", (__int64)"uint32_t", v6);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)a2, 1u);
		//     *((_BYTE *)a2 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: requestMipStatsReportAndReset");
			// m_cb->requestMipStatsReportAndReset();
			count = kPacketCountRequestMipStatsReportAndReset;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setGuardBands(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   int a2; // [rsp+30h] [rbp-88h] BYREF
		//   __int16 a2_4; // [rsp+34h] [rbp-84h]
		//   char v9[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 24i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 762
		//     && (a2 & 0x3FFF0000) == 0x40000 )
		//   {
		//     v3 = sub_7FFC80564250(v9);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"horzClip", (__int64)"float", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564250(v9);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"vertClip", (__int64)"float", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     v5 = sub_7FFC80564250(v9);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"horzDiscard", (__int64)"float", v5);
		//     sub_7FFC805644A0((__int64)v9);
		//     v6 = sub_7FFC80564250(v9);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"vertDiscard", (__int64)"float", v6);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setGuardBands");
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setGuardBands");
			// m_cb->setGuardBands();
			count = kPacketCountSetGuardBands;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPredication(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned __int64 v3; // rbx
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int16 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned __int16 v8; // [rsp+30h] [rbp-78h]
		//   int v9; // [rsp+34h] [rbp-74h]
		//   int v10; // [rsp+38h] [rbp-70h]
		//   char v11[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 20i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 8704
		//     && (v9 & 0xF0000000) == 0 )
		//   {
		//     v3 = a2_4 | ((unsigned __int64)v8 << 32);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setPredication");
		//     v4 = sub_7FFC80564430(v11, v3);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"condAddr", (__int64)"void*", v4);
		//     sub_7FFC805644A0((__int64)v11);
		//     v5 = sub_7FFC80564220((__int64)v11, v10 & 0x3FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"predCountInDwords", (__int64)"uint32_t", v5);
		//     sub_7FFC805644A0((__int64)v11);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPredication");
			// m_cb->setPredication();
			count = kPacketCountSetPredication;
		}

		return count;
	}

	uint32_t GnmCommandProxy::prepareFlip(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 a2; // [rsp+20h] [rbp-A8h] BYREF
		//   int v4; // [rsp+30h] [rbp-98h]
		//   int v5; // [rsp+44h] [rbp-84h]
		//   int v6; // [rsp+58h] [rbp-70h]
		//   int v7; // [rsp+74h] [rbp-54h]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 132i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 16128
		//     && (v5 & 0xFF00) == 18432
		//     && (v4 & 0xFF00) == 18432
		//     && (v6 & 0xFF00) == 15360
		//     && (v7 & 0xFF00) == 15360 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"prepareFlip");
		//     set_packet_count((uint64_t)rdx0, 7u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: prepareFlip");
			// m_cb->prepareFlip();
			count = kPacketCountPrepareFlip;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setObjectId(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v6; // [rsp+28h] [rbp-80h]
		//   __int64 v7; // [rsp+30h] [rbp-78h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30976
		//     && a2_4 == 584
		//     && (a2 & 0x3FFF0000) == 0x10000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setObjectId");
		//     v3 = sub_7FFC80564220((__int64)v8, v6);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"id", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 26) = 1;
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setObjectId");
			// m_cb->setObjectId();
			count = kPacketCountSetObjectId;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setObjectIdMode(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v7; // [rsp+28h] [rbp-80h]
		//   __int64 v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 525
		//     && (a2 & 0x3FFF0000) == 0x10000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setObjectIdMode");
		//     v3 = sub_7FFC80564220((__int64)v9, v7 & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"objIdSource", (__int64)"ObjectIdSource", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, (v7 >> 1) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"addPrimitiveId", (__int64)"AddPrimitiveId", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 26) = 1;
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setObjectIdMode");
			// m_cb->setObjectIdMode();
			count = kPacketCountSetObjectIdMode;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setDrawPayloadControl(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v6; // [rsp+28h] [rbp-80h]
		//   __int64 v7; // [rsp+30h] [rbp-78h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 678
		//     && (a2 & 0x3FFF0000) == 0x10000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setDrawPayloadControl");
		//     v3 = sub_7FFC80564220((__int64)v8, v6);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cntrl", (__int64)"DrawPayloadControl", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 26) = 1;
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setDrawPayloadControl");
			// m_cb->setDrawPayloadControl();
			count = kPacketCountSetDrawPayloadControl;
		}

		return count;
	}

	uint32_t GnmCommandProxy::resetFoveatedWindow(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int a2; // [rsp+20h] [rbp-28h] BYREF
		//   __int16 a2_4; // [rsp+24h] [rbp-24h]
		//   int v5; // [rsp+28h] [rbp-20h]
		//   int v6; // [rsp+2Ch] [rbp-1Ch]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 235
		//     && (a2 & 0x3FFF0000) == 0x20000
		//     && v5 == -16711936
		//     && v6 == 65280 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"resetFoveatedWindow");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 26) = 1;
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: resetFoveatedWindow");
			// m_cb->resetFoveatedWindow();
			count = kPacketCountResetFoveatedWindow;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setFoveatedWindow(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   int a2; // [rsp+28h] [rbp-80h] BYREF
		//   __int16 a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v11; // [rsp+30h] [rbp-78h]
		//   unsigned __int8 v12; // [rsp+34h] [rbp-74h]
		//   unsigned __int8 v13; // [rsp+35h] [rbp-73h]
		//   char v14[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 235
		//     && (a2 & 0x3FFF0000) == 0x20000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setFoveatedWindow");
		//     v3 = sub_7FFC80564220((__int64)v14, (unsigned __int8)v11);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"xMinLeftEye", (__int64)"uint8_t", v3);
		//     sub_7FFC805644A0((__int64)v14);
		//     v4 = sub_7FFC80564220((__int64)v14, BYTE1(v11));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"xMaxLeftEye", (__int64)"uint8_t", v4);
		//     sub_7FFC805644A0((__int64)v14);
		//     v5 = sub_7FFC80564220((__int64)v14, BYTE2(v11));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"xMinRightEye", (__int64)"uint8_t", v5);
		//     sub_7FFC805644A0((__int64)v14);
		//     v6 = sub_7FFC80564220((__int64)v14, HIBYTE(v11));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"xMaxRightEye", (__int64)"uint8_t", v6);
		//     sub_7FFC805644A0((__int64)v14);
		//     v7 = sub_7FFC80564220((__int64)v14, v12);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"yMinBothEyes", (__int64)"uint8_t", v7);
		//     sub_7FFC805644A0((__int64)v14);
		//     v8 = sub_7FFC80564220((__int64)v14, v13);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"xMaxRightEye", (__int64)"uint8_t", v8);
		//     sub_7FFC805644A0((__int64)v14);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 26) = 1;
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setFoveatedWindow");
			// m_cb->setFoveatedWindow();
			count = kPacketCountSetFoveatedWindow;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPsShaderSampleExclusionMask(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned __int16 v6; // [rsp+28h] [rbp-80h]
		//   __int64 v7; // [rsp+30h] [rbp-78h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 6
		//     && (a2 & 0x3FFF0000) == 0x10000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setPsShaderSampleExclusionMask");
		//     v3 = sub_7FFC80564220((__int64)v8, v6);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"mask", (__int64)"uint16_t", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 26) = 1;
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPsShaderSampleExclusionMask");
			// m_cb->setPsShaderSampleExclusionMask();
			count = kPacketCountSetPsShaderSampleExclusionMask;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setScaledResolutionGrid(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   int a2; // [rsp+28h] [rbp-80h] BYREF
		//   __int16 a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v8; // [rsp+30h] [rbp-78h]
		//   unsigned int v9; // [rsp+34h] [rbp-74h]
		//   unsigned int v10; // [rsp+38h] [rbp-70h]
		//   char v11[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 20i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 232
		//     && (a2 & 0x3FFF0000) == 196608 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setScaledResolutionGrid");
		//     v3 = sub_7FFC80564220((__int64)v11, v9);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"xAxisLeftEye", (__int64)"ScaledResolutionGridAxis", v3);
		//     sub_7FFC805644A0((__int64)v11);
		//     v4 = sub_7FFC80564220((__int64)v11, v8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"xAxisRightEye", (__int64)"ScaledResolutionGridAxis", v4);
		//     sub_7FFC805644A0((__int64)v11);
		//     v5 = sub_7FFC80564220((__int64)v11, v10);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"yAxisBothEyes", (__int64)"ScaledResolutionGridAxis", v5);
		//     sub_7FFC805644A0((__int64)v11);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 26) = 1;
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setScaledResolutionGrid");
			// m_cb->setScaledResolutionGrid();
			count = kPacketCountSetScaledResolutionGrid;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setTessellationDistributionThresholds(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v6; // [rsp+28h] [rbp-80h]
		//   __int64 v7; // [rsp+30h] [rbp-78h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && a2_4 == 724
		//     && (a2 & 0x3FFF0000) == 0x10000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setTessellationDistributionThresholds");
		//     v3 = sub_7FFC80564220((__int64)v8, v6);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"thresholds", (__int64)"TessellationDistributionThresholds", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 26) = 1;
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setTessellationDistributionThresholds");
			// m_cb->setTessellationDistributionThresholds();
			count = kPacketCountSetTessellationDistributionThresholds;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setTextureGradientFactors(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   char v3; // si
		//   unsigned int v4; // ebx
		//   unsigned int v5; // edi
		//   unsigned int v6; // ebp
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   __int64 v10; // rax
		//   __int64 v11; // rax
		//   __int64 v12; // rax
		//   int a2; // [rsp+20h] [rbp-98h] BYREF
		//   __int16 a2_4; // [rsp+24h] [rbp-94h]
		//   unsigned int v15; // [rsp+28h] [rbp-90h]
		//   __int64 v16; // [rsp+30h] [rbp-88h]
		//   char v17[80]; // [rsp+40h] [rbp-78h] BYREF
		//
		//   v16 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30976
		//     && a2_4 == 898
		//     && (a2 & 0x3FFF0000) == 0x10000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setTextureGradientFactors");
		//     v3 = v15;
		//     v4 = BYTE1(v15);
		//     v5 = BYTE2(v15);
		//     v6 = HIBYTE(v15);
		//     v7 = sub_7FFC80564220((__int64)v17, (unsigned __int8)v15 & 0xFE);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"factor0", (__int64)"uint8_t", v7);
		//     sub_7FFC805644A0((__int64)v17);
		//     v8 = sub_7FFC80564220((__int64)v17, v4);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"factor1", (__int64)"uint8_t", v8);
		//     sub_7FFC805644A0((__int64)v17);
		//     v9 = sub_7FFC80564220((__int64)v17, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"factor2", (__int64)"uint8_t", v9);
		//     sub_7FFC805644A0((__int64)v17);
		//     v10 = sub_7FFC80564220((__int64)v17, (unsigned __int8)v6 & 0xFE);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"factor3", (__int64)"uint8_t", v10);
		//     sub_7FFC805644A0((__int64)v17);
		//     v11 = sub_7FFC80564220((__int64)v17, v3 & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"factor01sb", (__int64)"TextureGradientFactor01SignNegationBehavior", v11);
		//     sub_7FFC805644A0((__int64)v17);
		//     v12 = sub_7FFC80564220((__int64)v17, v6 & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"factor10sb", (__int64)"TextureGradientFactor10SignNegationBehavior", v12);
		//     sub_7FFC805644A0((__int64)v17);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 26) = 1;
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setTextureGradientFactors");
			// m_cb->setTextureGradientFactors();
			count = kPacketCountSetTextureGradientFactors;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setVgtNumInstances(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   int a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v6; // [rsp+28h] [rbp-80h]
		//   __int64 v7; // [rsp+30h] [rbp-78h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30976
		//     && (_WORD)a2_4 == 589
		//     && (a2 & 0x3FFF0000) == 0x10000
		//     && (a2_4 & 0xF0000000) == 805306368 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       (Concurrency::details::SchedulerProxy *)rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setVgtNumInstances");
		//     v3 = sub_7FFC80564220((__int64)v8, v6);
		//     sub_7FFC805A8FC0(rdx0, (__int64)"instanceCount", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     set_packet_count(rdx0, 1u);
		//     *(_WORD *)(rdx0 + 25) = 257;
		//     *(_BYTE *)(rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setVgtNumInstances");
			// m_cb->setVgtNumInstances();
			count = kPacketCountSetVgtNumInstances;
		}

		return count;
	}

	uint32_t GnmCommandProxy::RttvThreadTraceMarker(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int16 a2; // [rsp+20h] [rbp-28h] BYREF
		//   __int16 a2_4; // [rsp+24h] [rbp-24h]
		//   int v5; // [rsp+28h] [rbp-20h]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 17920
		//     && (v5 & 0xFF00) == 4096
		//     && (a2_4 & 0xF00) == 0
		//     && (a2_4 & 0x3F) == 53 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"RttvThreadTraceMarker");
		//     set_packet_count((uint64_t)rdx0, 2u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: RttvThreadTraceMarker");
			// m_cb->RttvThreadTraceMarker();
			count = kPacketCountRttvThreadTraceMarker;
		}

		return count;
	}

	uint32_t GnmCommandProxy::RttvThreadTraceMarker2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int16 a2; // [rsp+30h] [rbp+8h] BYREF
		//   __int16 a2_4; // [rsp+34h] [rbp+Ch]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 8i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 17920
		//     && (a2_4 & 0xF00) == 0
		//     && (a2_4 & 0x3F) == 53 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"RttvThreadTraceMarker");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: RttvThreadTraceMarker");
			// m_cb->RttvThreadTraceMarker();
			count = kPacketCountRttvThreadTraceMarker2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setContextRegisterWithIndex(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v8; // [rsp+28h] [rbp-80h]
		//   __int64 v9; // [rsp+30h] [rbp-78h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && (a2 & 0x3FFF0000) == 0x10000
		//     && (a2_4 & 0xF0000000) != 0 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setContextRegisterWithIndex");
		//     v3 = sub_7FFC80564220((__int64)v10, (unsigned int)(unsigned __int16)a2_4 + 40960);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"regAddr", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v10);
		//     v4 = sub_7FFC80564220((__int64)v10, v8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"value", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v10);
		//     v5 = sub_7FFC80564220((__int64)v10, a2_4 >> 28);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"index", (__int64)"uint32_t", v5);
		//     sub_7FFC805644A0((__int64)v10);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setContextRegisterWithIndex");
			// m_cb->setContextRegisterWithIndex();
			count = kPacketCountSetContextRegisterWithIndex;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setContextRegister(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v7; // [rsp+28h] [rbp-80h]
		//   __int64 v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26880
		//     && (a2 & 0x3FFF0000) == 0x10000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setContextRegister");
		//     v3 = sub_7FFC80564220((__int64)v9, (unsigned int)a2_4 + 40960);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"regAddr", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"value", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setContextRegister");
			// m_cb->setContextRegister();
			count = kPacketCountSetContextRegister;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setConfigRegister(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v7; // [rsp+28h] [rbp-80h]
		//   __int64 v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 26624
		//     && (a2 & 0x3FFF0000) == 0x10000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setConfigRegister");
		//     v3 = sub_7FFC80564220((__int64)v9, (unsigned int)a2_4 + 0x2000);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"regAddr", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"value", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setConfigRegister");
			// m_cb->setConfigRegister();
			count = kPacketCountSetConfigRegister;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setUserConfigRegisterWithIndex(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v8; // [rsp+28h] [rbp-80h]
		//   __int64 v9; // [rsp+30h] [rbp-78h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30976
		//     && (a2 & 0x3FFF0000) == 0x10000
		//     && (a2_4 & 0xF0000000) != 0 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setUserConfigRegisterWithIndex");
		//     v3 = sub_7FFC80564220((__int64)v10, (unsigned int)(unsigned __int16)a2_4 + 49152);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"regAddr", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v10);
		//     v4 = sub_7FFC80564220((__int64)v10, v8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"value", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v10);
		//     v5 = sub_7FFC80564220((__int64)v10, a2_4 >> 28);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"index", (__int64)"uint32_t", v5);
		//     sub_7FFC805644A0((__int64)v10);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setUserConfigRegisterWithIndex");
			// m_cb->setUserConfigRegisterWithIndex();
			count = kPacketCountSetUserConfigRegisterWithIndex;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setUserConfigRegister(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   int a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v7; // [rsp+28h] [rbp-80h]
		//   __int64 v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30976
		//     && (a2 & 0x3FFF0000) == 0x10000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setUserConfigRegister");
		//     v3 = sub_7FFC80564220((__int64)v9, (unsigned int)a2_4 + 49152);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"regAddr", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"value", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setUserConfigRegister");
			// m_cb->setUserConfigRegister();
			count = kPacketCountSetUserConfigRegister;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPersistentRegister(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   unsigned int a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned __int16 a2_4; // [rsp+24h] [rbp-84h]
		//   unsigned int v8; // [rsp+28h] [rbp-80h]
		//   __int64 v9; // [rsp+30h] [rbp-78h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && (a2 & 0x3FFF0000) == 0x10000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setPersistentRegister");
		//     v3 = sub_7FFC80564220((__int64)v10, (a2 >> 1) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"shaderType", (__int64)"ShaderType", v3);
		//     sub_7FFC805644A0((__int64)v10);
		//     v4 = sub_7FFC80564220((__int64)v10, (unsigned int)a2_4 + 11264);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"regAddr", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v10);
		//     v5 = sub_7FFC80564220((__int64)v10, v8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"value", (__int64)"uint32_t", v5);
		//     sub_7FFC805644A0((__int64)v10);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPersistentRegister");
			// m_cb->setPersistentRegister();
			count = kPacketCountSetPersistentRegister;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setContextRegisterRange(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   unsigned int a2; // [rsp+20h] [rbp-78h] BYREF
		//   unsigned __int16 a2_4; // [rsp+24h] [rbp-74h]
		//   __int64 v10; // [rsp+28h] [rbp-70h]
		//   char v11[80]; // [rsp+30h] [rbp-68h] BYREF
		//
		//   v10 = -2i64;
		//   if ( GetOpCodeType(a1, (__int64)&a2, 8i64, *((_DWORD *)a1 + 2)) && (a2 & 0xFF00) == 26880 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setContextRegisterRange");
		//     v4 = sub_7FFC80564220((__int64)v11, (unsigned int)a2_4 + 40960);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"regAddr", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v11);
		//     v5 = sub_7FFC80594650(*a1, a1[1]);
		//     v6 = sub_7FFC80564430(v11, v5 + 8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"pRegValues", (__int64)"uint32_t*", v6);
		//     sub_7FFC805644A0((__int64)v11);
		//     v7 = sub_7FFC80564220((__int64)v11, HIWORD(a2) & 0x3FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"numValues", (__int64)"uint32_t", v7);
		//     sub_7FFC805644A0((__int64)v11);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setContextRegisterRange");
			// m_cb->setContextRegisterRange();
			count = kPacketCountSetContextRegisterRange;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setConfigRegisterRange(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   unsigned int a2; // [rsp+20h] [rbp-78h] BYREF
		//   unsigned __int16 a2_4; // [rsp+24h] [rbp-74h]
		//   __int64 v10; // [rsp+28h] [rbp-70h]
		//   char v11[80]; // [rsp+30h] [rbp-68h] BYREF
		//
		//   v10 = -2i64;
		//   if ( GetOpCodeType(a1, (__int64)&a2, 8i64, *((_DWORD *)a1 + 2)) && (a2 & 0xFF00) == 26624 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setConfigRegisterRange");
		//     v4 = sub_7FFC80564220((__int64)v11, (unsigned int)a2_4 + 0x2000);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"regAddr", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v11);
		//     v5 = sub_7FFC80594650(*a1, a1[1]);
		//     v6 = sub_7FFC80564430(v11, v5 + 8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"pRegValues", (__int64)"uint32_t*", v6);
		//     sub_7FFC805644A0((__int64)v11);
		//     v7 = sub_7FFC80564220((__int64)v11, HIWORD(a2) & 0x3FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"numValues", (__int64)"uint32_t", v7);
		//     sub_7FFC805644A0((__int64)v11);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setConfigRegisterRange");
			// m_cb->setConfigRegisterRange();
			count = kPacketCountSetConfigRegisterRange;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setUserConfigRegisterRange(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   unsigned int a2; // [rsp+20h] [rbp-78h] BYREF
		//   unsigned __int16 a2_4; // [rsp+24h] [rbp-74h]
		//   __int64 v10; // [rsp+28h] [rbp-70h]
		//   char v11[80]; // [rsp+30h] [rbp-68h] BYREF
		//
		//   v10 = -2i64;
		//   if ( GetOpCodeType(a1, (__int64)&a2, 8i64, *((_DWORD *)a1 + 2)) && (a2 & 0xFF00) == 30976 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setUserConfigRegisterRange");
		//     v4 = sub_7FFC80564220((__int64)v11, (unsigned int)a2_4 + 49152);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"regAddr", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v11);
		//     v5 = sub_7FFC80594650(*a1, a1[1]);
		//     v6 = sub_7FFC80564430(v11, v5 + 8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"pRegValues", (__int64)"uint32_t*", v6);
		//     sub_7FFC805644A0((__int64)v11);
		//     v7 = sub_7FFC80564220((__int64)v11, HIWORD(a2) & 0x3FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"numValues", (__int64)"uint32_t", v7);
		//     sub_7FFC805644A0((__int64)v11);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setUserConfigRegisterRange");
			// m_cb->setUserConfigRegisterRange();
			count = kPacketCountSetUserConfigRegisterRange;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPersistentRegisterRange(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   unsigned int a2; // [rsp+20h] [rbp-78h] BYREF
		//   unsigned __int16 a2_4; // [rsp+24h] [rbp-74h]
		//   __int64 v11; // [rsp+28h] [rbp-70h]
		//   char v12[80]; // [rsp+30h] [rbp-68h] BYREF
		//
		//   v11 = -2i64;
		//   if ( GetOpCodeType(a1, (__int64)&a2, 8i64, *((_DWORD *)a1 + 2)) && (a2 & 0xFF00) == 30208 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setPersistentRegisterRange");
		//     v4 = sub_7FFC80564220((__int64)v12, (a2 >> 1) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"shaderType", (__int64)"ShaderType", v4);
		//     sub_7FFC805644A0((__int64)v12);
		//     v5 = sub_7FFC80564220((__int64)v12, (unsigned int)a2_4 + 11264);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"regAddr", (__int64)"uint32_t", v5);
		//     sub_7FFC805644A0((__int64)v12);
		//     v6 = sub_7FFC80594650(*a1, a1[1]);
		//     v7 = sub_7FFC80564430(v12, v6 + 8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"pRegValues", (__int64)"uint32_t*", v7);
		//     sub_7FFC805644A0((__int64)v12);
		//     v8 = sub_7FFC80564220((__int64)v12, HIWORD(a2) & 0x3FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"numValues", (__int64)"uint32_t", v8);
		//     sub_7FFC805644A0((__int64)v12);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPersistentRegisterRange");
			// m_cb->setPersistentRegisterRange();
			count = kPacketCountSetPersistentRegisterRange;
		}

		return count;
	}

	uint32_t GnmCommandProxy::chainCommandBuffer(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int16 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned __int16 v7; // [rsp+30h] [rbp-78h]
		//   int v8; // [rsp+34h] [rbp-74h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 16128
		//     && (v8 & 0x100000) != 0 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"chainCommandBuffer");
		//     v3 = sub_7FFC80564430(v9, a2_4 | ((unsigned __int64)v7 << 32));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"ibBaseAddr", (__int64)"void*", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, v8 & 0xFFFFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"ibSizeInDW", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: chainCommandBuffer");
			// m_cb->chainCommandBuffer();
			count = kPacketCountChainCommandBuffer;
		}

		return count;
	}

	uint32_t GnmCommandProxy::pause(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2; // [rsp+20h] [rbp-A8h] BYREF
		//   char v5[16]; // [rsp+28h] [rbp-A0h] BYREF
		//   __int64 v6; // [rsp+38h] [rbp-90h]
		//   __int64 v7; // [rsp+40h] [rbp-88h]
		//   __int64 v8; // [rsp+48h] [rbp-80h]
		//   char v9[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 8i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 22784 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(rdx0, (struct Concurrency::details::SchedulerNode *)"pause");
		//     v6 = 0i64;
		//     v7 = 15i64;
		//     v5[0] = 0;
		//     sub_7FFC802A8CB0(v5);
		//     v3 = sub_7FFC80564310(v9, v5);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"reservedDWs", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: pause");
			// m_cb->pause();
			count = kPacketCountPause;
		}

		return count;
	}

	uint32_t GnmCommandProxy::triggerEvent(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int16 a2; // [rsp+20h] [rbp-78h] BYREF
		//   char a2_4; // [rsp+24h] [rbp-74h]
		//   __int64 v6; // [rsp+28h] [rbp-70h]
		//   char v7[80]; // [rsp+30h] [rbp-68h] BYREF
		//
		//   v6 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 8i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 17920 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"triggerEvent");
		//     v3 = sub_7FFC80564220((__int64)v7, a2_4 & 0x3F);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"eventType", (__int64)"EventType", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: triggerEvent");
			// m_cb->triggerEvent();
			count = kPacketCountTriggerEvent;
		}

		return count;
	}

	uint32_t GnmCommandProxy::waitOnAddress(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   char v9[16]; // [rsp+20h] [rbp-A8h] BYREF
		//   __int64 v10; // [rsp+30h] [rbp-98h]
		//   __int16 a2; // [rsp+38h] [rbp-90h] BYREF
		//   __int16 a2_4; // [rsp+3Ch] [rbp-8Ch]
		//   unsigned int v13; // [rsp+40h] [rbp-88h]
		//   unsigned __int16 v14; // [rsp+44h] [rbp-84h]
		//   unsigned int v15; // [rsp+48h] [rbp-80h]
		//   unsigned int v16; // [rsp+4Ch] [rbp-7Ch]
		//   __int16 v17; // [rsp+50h] [rbp-78h]
		//   char v18[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   v10 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 15360 )
		//     {
		//       LOBYTE(v3) = a2_4;
		//       if ( (a2_4 & 0x100) == 0 )
		//       {
		//         LOBYTE(v3) = a2_4 & 0x30;
		//         if ( (a2_4 & 0x30) == 16 && v17 == 10 )
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"waitOnAddress");
		//           _lambda_082c17da81b0962e08c0587ee0fac50c_::_lambda_082c17da81b0962e08c0587ee0fac50c_(
		//             (_lambda_082c17da81b0962e08c0587ee0fac50c_ *)v9,
		//             (int *)(v13 | ((unsigned __int64)v14 << 32)),
		//             (const char **)4);
		//           v4 = sub_7FFC80564280(v18, v9, 4i64);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"label", (__int64)"ResourceLabel", v4);
		//           sub_7FFC805644A0((__int64)v18);
		//           v5 = sub_7FFC80564220((__int64)v18, v16);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"mask", (__int64)"uint32_t", v5);
		//           sub_7FFC805644A0((__int64)v18);
		//           v6 = sub_7FFC80564220((__int64)v18, a2_4 & 7);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"compareFunc", (__int64)"WaitCompareFunc", v6);
		//           sub_7FFC805644A0((__int64)v18);
		//           v7 = sub_7FFC80564220((__int64)v18, v15);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"refValue", (__int64)"uint32_t", v7);
		//           sub_7FFC805644A0((__int64)v18);
		//           set_packet_count((uint64_t)rdx0, 1u);
		//           *((_BYTE *)rdx0 + 24) = 1;
		//           LOBYTE(v3) = nullsub_1(v9);
		//         }
		//       }
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: waitOnAddress");
			// m_cb->waitOnAddress();
			count = kPacketCountWaitOnAddress;
		}

		return count;
	}

	uint32_t GnmCommandProxy::waitForGraphicsWrites(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int16 a2; // [rsp+28h] [rbp-90h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-8Ch]
		//   unsigned int v11; // [rsp+30h] [rbp-88h]
		//   unsigned int v12; // [rsp+38h] [rbp-80h]
		//   char v13[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 22528 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"waitForGraphicsWrites");
		//     v3 = sub_7FFC80564430(v13, (unsigned __int64)v12 << 8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"baseAddr", (__int64)"uint32_t*", v3);
		//     sub_7FFC805644A0((__int64)v13);
		//     v4 = sub_7FFC80564220((__int64)v13, v11);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"sizeIn256ByteBlocks", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v13);
		//     v5 = sub_7FFC80564220((__int64)v13, a2_4 & 0x7FC0);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"targetMask", (__int64)"WaitTargetSlot", v5);
		//     sub_7FFC805644A0((__int64)v13);
		//     v6 = sub_7FFC80564220((__int64)v13, (a2_4 & 0x58000 | (a2_4 >> 3) & 0x180000) >> 15);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cacheAction", (__int64)"CacheAction", v6);
		//     sub_7FFC805644A0((__int64)v13);
		//     v7 = sub_7FFC80564220((__int64)v13, a2_4 & 0x2E000000);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"extendedCacheMask", (__int64)"uint32_t", v7);
		//     sub_7FFC805644A0((__int64)v13);
		//     v8 = sub_7FFC80564220((__int64)v13, a2_4 >> 31);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"commandBufferStallMode", (__int64)"StallCommandBufferParserMode", v8);
		//     sub_7FFC805644A0((__int64)v13);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: waitForGraphicsWrites");
			// m_cb->waitForGraphicsWrites();
			count = kPacketCountWaitForGraphicsWrites;
		}

		return count;
	}

	uint32_t GnmCommandProxy::insertNop(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int packet_size; // eax
		//   __int64 v4; // rax
		//   __int64 a2[2]; // [rsp+20h] [rbp-78h] BYREF
		//   char v6[80]; // [rsp+30h] [rbp-68h] BYREF
		//
		//   a2[1] = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)a2, 4i64, *(_DWORD *)(a1 + 8)) && (a2[0] & 0xFF00) == 4096 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"insertNop");
		//     packet_size = get_packet_size(a2[0]);
		//     v4 = sub_7FFC80564430(v6, (unsigned __int64)packet_size >> 2);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"numDwords", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v6);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: insertNop");
			// m_cb->insertNop();
			count = kPacketCountInsertNop;
		}

		return count;
	}

	uint32_t GnmCommandProxy::insertNop2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 a2[2]; // [rsp+20h] [rbp-78h] BYREF
		//   char v5[80]; // [rsp+30h] [rbp-68h] BYREF
		//
		//   a2[1] = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)a2, 4i64, *(_DWORD *)(a1 + 8)) && (a2[0] & 0xC0000000) == 0x80000000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"insertNop");
		//     v3 = sub_7FFC80564220((__int64)v5, 1u);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"numDwords", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v5);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: insertNop");
			// m_cb->insertNop();
			count = kPacketCountInsertNop2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::incrementCeCounter(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int16 a2; // [rsp+30h] [rbp+8h] BYREF
		//   int a2_4; // [rsp+34h] [rbp+Ch]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 8i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 33792 && a2_4 == 1 )
		//   {
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"incrementCeCounter");
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: incrementCeCounter");
			// m_cb->incrementCeCounter();
			count = kPacketCountIncrementCeCounter;
		}

		return count;
	}

	uint32_t GnmCommandProxy::incrementCeCounterForDispatchDraw(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int16 a2; // [rsp+30h] [rbp+8h] BYREF
		//   int a2_4; // [rsp+34h] [rbp+Ch]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 8i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 33792 && a2_4 == 3 )
		//   {
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"incrementCeCounterForDispatchDraw");
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: incrementCeCounterForDispatchDraw");
			// m_cb->incrementCeCounterForDispatchDraw();
			count = kPacketCountIncrementCeCounterForDispatchDraw;
		}

		return count;
	}

	uint32_t GnmCommandProxy::waitOnDeCounterDiff(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int16 a2; // [rsp+20h] [rbp-78h] BYREF
		//   unsigned int a2_4; // [rsp+24h] [rbp-74h]
		//   __int64 v8; // [rsp+28h] [rbp-70h]
		//   char v9[80]; // [rsp+30h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 8i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 34816 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"waitOnDeCounterDiff");
		//       v4 = sub_7FFC80564220((__int64)v9, a2_4);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"diff", (__int64)"uint32_t", v4);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v9);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: waitOnDeCounterDiff");
			// m_cb->waitOnDeCounterDiff();
			count = kPacketCountWaitOnDeCounterDiff;
		}

		return count;
	}

	uint32_t GnmCommandProxy::writeToCpRam(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   unsigned int a2; // [rsp+20h] [rbp-78h] BYREF
		//   unsigned __int16 a2_4; // [rsp+24h] [rbp-74h]
		//   __int64 v12; // [rsp+28h] [rbp-70h]
		//   char v13[80]; // [rsp+30h] [rbp-68h] BYREF
		//
		//   v12 = -2i64;
		//   LOBYTE(v4) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 8i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 33024 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"writeToCpRam");
		//       v5 = sub_7FFC80564220((__int64)v13, a2_4);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"destByteOffset", (__int64)"uint16_t", v5);
		//       sub_7FFC805644A0((__int64)v13);
		//       v6 = sub_7FFC80594650(*(_QWORD *)a1, *(_QWORD *)(a1 + 8));
		//       v7 = sub_7FFC80564430(v13, v6 + 8);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"srcData", (__int64)"void*", v7);
		//       sub_7FFC805644A0((__int64)v13);
		//       v8 = sub_7FFC80564220((__int64)v13, HIWORD(a2) & 0x3FFF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"numDwords", (__int64)"uint32_t", v8);
		//       LOBYTE(v4) = sub_7FFC805644A0((__int64)v13);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: writeToCpRam");
			// m_cb->writeToCpRam();
			count = kPacketCountWriteToCpRam;
		}

		return count;
	}

	uint32_t GnmCommandProxy::cpRamDump(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   _WORD a2[6]; // [rsp+28h] [rbp-80h] BYREF
		//   __int64 v9; // [rsp+34h] [rbp-74h]
		//   char v10[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)a2, 20i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2[0] & 0xFF00;
		//     if ( v3 == 33536 )
		//     {
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"cpRamDump");
		//       v4 = sub_7FFC80564430(v10, v9);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"destAddr", (__int64)"void*", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       v5 = sub_7FFC80564220((__int64)v10, a2[2]);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"srcByteOffset", (__int64)"uint16_t", v5);
		//       sub_7FFC805644A0((__int64)v10);
		//       v6 = sub_7FFC80564220((__int64)v10, a2[4] & 0x7FFF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"numDwords", (__int64)"uint32_t", v6);
		//       LOBYTE(v3) = sub_7FFC805644A0((__int64)v10);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: cpRamDump");
			// m_cb->cpRamDump();
			count = kPacketCountCpRamDump;
		}

		return count;
	}

	uint32_t GnmCommandProxy::chainCommandBuffer2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int16 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned __int16 v7; // [rsp+30h] [rbp-78h]
		//   int v8; // [rsp+34h] [rbp-74h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 13056
		//     && (v8 & 0x100000) != 0 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"chainCommandBuffer");
		//     v3 = sub_7FFC80564430(v9, a2_4 | ((unsigned __int64)v7 << 32));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"ibBaseAddr", (__int64)"void*", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, v8 & 0xFFFFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"ibSizeInDW", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_WORD *)rdx0 + 12) = 257;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: chainCommandBuffer");
			// m_cb->chainCommandBuffer();
			count = kPacketCountChainCommandBuffer2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::callCommandBuffer(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int16 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned __int16 v7; // [rsp+30h] [rbp-78h]
		//   int v8; // [rsp+34h] [rbp-74h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 13056
		//     && (v8 & 0x100000) == 0 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"callCommandBuffer");
		//     v3 = sub_7FFC80564430(v9, a2_4 | ((unsigned __int64)v7 << 32));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"ibBaseAddr", (__int64)"void*", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, v8 & 0xFFFFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"ibSizeInDW", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: callCommandBuffer");
			// m_cb->callCommandBuffer();
			count = kPacketCountCallCommandBuffer;
		}

		return count;
	}

	uint32_t GnmCommandProxy::initializeDefaultHardwareState2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   _WORD a2[32]; // [rsp+20h] [rbp-58h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)a2, 64i64, *(_DWORD *)(a1 + 8))
		//     && (a2[0] & 0xFF00) == 30208
		//     && a2[2] == 534
		//     && (a2[6] & 0xFF00) == 30208
		//     && a2[8] == 535
		//     && (a2[12] & 0xFF00) == 30208
		//     && a2[14] == 533
		//     && (a2[18] & 0xFF00) == 22528 )
		//   {
		//     v4 = sub_7FFC805A9130((_QWORD *)a1, 16);
		//     set_packet_count((uint64_t)rdx0, v4 + 1);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"initializeDefaultHardwareState");
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: initializeDefaultHardwareState");
			// m_cb->initializeDefaultHardwareState();
			count = kPacketCountInitializeDefaultHardwareState2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setQueuePriority(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   unsigned int a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v7; // [rsp+30h] [rbp-78h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30720
		//     && (unsigned int)get_packet_size(a2) == 12
		//     && (a2 & 2) != 0
		//     && (_WORD)a2_4 == 11 )
		//   {
		//     v3 = sub_7FFC80564220((__int64)v8, HIWORD(a2_4));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"queueId", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     v4 = sub_7FFC80564220((__int64)v8, v7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"priority", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v8);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setQueuePriority");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setQueuePriority");
			// m_cb->setQueuePriority();
			count = kPacketCountSetQueuePriority;
		}

		return count;
	}

	uint32_t GnmCommandProxy::dispatchIndirectWithOrderedAppend2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   int v7; // edi
		//   __int64 v8; // rax
		//   char v10[20]; // [rsp+28h] [rbp-A0h] BYREF
		//   uint32_t v11; // [rsp+3Ch] [rbp-8Ch]
		//   _DWORD a2[6]; // [rsp+48h] [rbp-80h] BYREF
		//   char v13[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)a2, 16i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2[0] & 0xFF00;
		//     if ( v4 == 5632 )
		//     {
		//       sub_7FFC805AD130((__int64)v10);
		//       v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//       if ( sub_7FFC805AD150(3, *a1, v5, (__int64)v10) )
		//       {
		//         v6 = sub_7FFC80564430(v13, *(_QWORD *)&a2[1]);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"args", (__int64)"DispatchIndirectArgs*", v6);
		//         sub_7FFC805644A0((__int64)v13);
		//         v7 = (a2[3] >> 3) & 3;
		//         if ( v7 )
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"dispatchIndirectWithOrderedAppend");
		//           v8 = sub_7FFC80564220((__int64)v13, v7);
		//           sub_7FFC805A8FC0((__int64)rdx0, (__int64)"orderedAppendMode", (__int64)"DispatchOrderedAppendMode", v8);
		//           sub_7FFC805644A0((__int64)v13);
		//         }
		//         else
		//         {
		//           Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//             rdx0,
		//             (struct Concurrency::details::SchedulerNode *)"dispatchIndirect");
		//         }
		//         set_packet_count((uint64_t)rdx0, v11);
		//         sub_7FFC80564780((__int64)rdx0, 0);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//       LOBYTE(v4) = nullsub_1(v10);
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: dispatchIndirectWithOrderedAppend");
			// m_cb->dispatchIndirectWithOrderedAppend();
			count = kPacketCountDispatchIndirectWithOrderedAppend2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::sub_7FFC805A03E0(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   sub_7FFC805A9580(a1, a2, 1, 1);

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: sub_7FFC805A03E");
			// m_cb->sub_7FFC805A03E();
			count = kPacketCountSub_7FFC805A03E0;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setVsharpInUserData2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   unsigned int v5; // [rsp+20h] [rbp-98h] BYREF
		//   char v6[4]; // [rsp+24h] [rbp-94h] BYREF
		//   __int64 v7; // [rsp+28h] [rbp-90h]
		//   __int16 a2; // [rsp+30h] [rbp-88h] BYREF
		//   int a2_4; // [rsp+34h] [rbp-84h]
		//   int v10; // [rsp+38h] [rbp-80h]
		//   unsigned __int16 v11; // [rsp+3Ch] [rbp-7Ch]
		//   char v12[16]; // [rsp+40h] [rbp-78h] BYREF
		//   char v13[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 32i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 4096
		//     && (v10 & 0xFF00) == 30208
		//     && (a2_4 & 0xFFFF0FFF) == 1752498180
		//     && (v10 & 0x3FFF0000) == 0x40000 )
		//   {
		//     if ( (unsigned __int8)sub_7FFC80575550((unsigned int)v11 + 11264, v6, &v5) )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"setVsharpInUserData");
		//       v3 = sub_7FFC80564220((__int64)v13, v5);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"startUserDataSlot", (__int64)"uint32_t", v3);
		//       sub_7FFC805644A0((__int64)v13);
		//       v4 = sub_7FFC80564280(v13, v12, 4i64);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"buffer", (__int64)"ResourceVsharp", v4);
		//       sub_7FFC805644A0((__int64)v13);
		//       set_packet_count((uint64_t)rdx0, 2u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setVsharpInUserData");
			// m_cb->setVsharpInUserData();
			count = kPacketCountSetVsharpInUserData2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setTsharpInUserData2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   int v5; // [rsp+20h] [rbp-A8h] BYREF
		//   unsigned int v6; // [rsp+24h] [rbp-A4h] BYREF
		//   __int64 v7; // [rsp+28h] [rbp-A0h]
		//   __int16 a2; // [rsp+30h] [rbp-98h] BYREF
		//   int a2_4; // [rsp+34h] [rbp-94h]
		//   int v10; // [rsp+38h] [rbp-90h]
		//   unsigned __int16 v11; // [rsp+3Ch] [rbp-8Ch]
		//   char v12[32]; // [rsp+40h] [rbp-88h] BYREF
		//   char v13[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 48i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 4096
		//     && (v10 & 0xFF00) == 30208
		//     && (a2_4 & 0xFFFF0FFF) == 1752498181
		//     && (v10 & 0x3FFF0000) == 0x80000
		//     && (unsigned __int8)sub_7FFC80575550((unsigned int)v11 + 11264, &v5, &v6)
		//     && !v5 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setTsharpInUserData");
		//     v3 = sub_7FFC80564220((__int64)v13, v6);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"startUserDataSlot", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v13);
		//     v4 = sub_7FFC80564280(v13, v12, 8i64);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"tex", (__int64)"ResourceTsharp", v4);
		//     sub_7FFC805644A0((__int64)v13);
		//     set_packet_count((uint64_t)rdx0, 2u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setTsharpInUserData");
			// m_cb->setTsharpInUserData();
			count = kPacketCountSetTsharpInUserData2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setSsharpInUserData2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   int v5; // [rsp+20h] [rbp-98h] BYREF
		//   unsigned int v6; // [rsp+24h] [rbp-94h] BYREF
		//   __int64 v7; // [rsp+28h] [rbp-90h]
		//   __int16 a2; // [rsp+30h] [rbp-88h] BYREF
		//   int a2_4; // [rsp+34h] [rbp-84h]
		//   int v10; // [rsp+38h] [rbp-80h]
		//   unsigned __int16 v11; // [rsp+3Ch] [rbp-7Ch]
		//   char v12[16]; // [rsp+40h] [rbp-78h] BYREF
		//   char v13[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 32i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 4096
		//     && (v10 & 0xFF00) == 30208
		//     && (a2_4 & 0xFFFF0FFF) == 1752498182
		//     && (v10 & 0x3FFF0000) == 0x40000
		//     && (unsigned __int8)sub_7FFC80575550((unsigned int)v11 + 11264, &v5, &v6)
		//     && !v5 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setSsharpInUserData");
		//     v3 = sub_7FFC80564220((__int64)v13, v6);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"startUserDataSlot", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v13);
		//     v4 = sub_7FFC80564280(v13, v12, 4i64);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"sampler", (__int64)"ResourceSsharp", v4);
		//     sub_7FFC805644A0((__int64)v13);
		//     set_packet_count((uint64_t)rdx0, 2u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setSsharpInUserData");
			// m_cb->setSsharpInUserData();
			count = kPacketCountSetSsharpInUserData2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setUserDataRegion2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned __int64 v4; // rdi
		//   __int64 v5; // rbx
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   int v9; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v10; // [rsp+24h] [rbp-84h] BYREF
		//   __int64 v11; // [rsp+28h] [rbp-80h]
		//   int v12[2]; // [rsp+30h] [rbp-78h] BYREF
		//   unsigned int v13; // [rsp+38h] [rbp-70h]
		//   unsigned __int16 v14; // [rsp+3Ch] [rbp-6Ch]
		//   char v15[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v11 = -2i64;
		//   if ( GetOpCodeType(a1, (__int64)v12, 16i64, *((_DWORD *)a1 + 2))
		//     && (v12[0] & 0xFF00) == 4096
		//     && (v12[1] & 0xFFFF0FFF) == 1752498189
		//     && (v13 & 0xFF00) == 30208
		//     && (unsigned __int8)sub_7FFC80575550((unsigned int)v14 + 11264, &v9, &v10)
		//     && !v9 )
		//   {
		//     v4 = ((unsigned __int64)(unsigned int)get_packet_size(v13) - 8) >> 2;
		//     v5 = sub_7FFC80594650(*a1, a1[1]);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       a2,
		//       (struct Concurrency::details::SchedulerNode *)"setUserDataRegion");
		//     v6 = sub_7FFC80564220((__int64)v15, v10);
		//     sub_7FFC805A8FC0((__int64)a2, (__int64)"startUserDataSlot", (__int64)"uint32_t", v6);
		//     sub_7FFC805644A0((__int64)v15);
		//     v7 = sub_7FFC80564430(v15, v5 + 16);
		//     sub_7FFC805A8FC0((__int64)a2, (__int64)"userData", (__int64)"uint32_t*", v7);
		//     sub_7FFC805644A0((__int64)v15);
		//     v8 = sub_7FFC80564220((__int64)v15, v4);
		//     sub_7FFC805A8FC0((__int64)a2, (__int64)"numDwords", (__int64)"uint32_t", v8);
		//     sub_7FFC805644A0((__int64)v15);
		//     set_packet_count((uint64_t)a2, 2u);
		//     *((_BYTE *)a2 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setUserDataRegion");
			// m_cb->setUserDataRegion();
			count = kPacketCountSetUserDataRegion2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setPointerInUserData2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   int v5; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v6; // [rsp+24h] [rbp-84h] BYREF
		//   __int64 v7; // [rsp+28h] [rbp-80h]
		//   int a2; // [rsp+30h] [rbp-78h] BYREF
		//   unsigned __int16 a2_4; // [rsp+34h] [rbp-74h]
		//   __int64 v10; // [rsp+38h] [rbp-70h]
		//   char v11[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && (a2 & 0x3FFF0000) == 0x20000
		//     && (unsigned __int8)sub_7FFC80575550((unsigned int)a2_4 + 11264, &v5, &v6)
		//     && !v5 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setPointerInUserData");
		//     v3 = sub_7FFC80564220((__int64)v11, v6);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"startUserDataSlot", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v11);
		//     v4 = sub_7FFC80564430(v11, v10);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"gpuAddr", (__int64)"void*", v4);
		//     sub_7FFC805644A0((__int64)v11);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setPointerInUserData");
			// m_cb->setPointerInUserData();
			count = kPacketCountSetPointerInUserData2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setUserData2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   int v5; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v6; // [rsp+24h] [rbp-84h] BYREF
		//   __int64 v7; // [rsp+28h] [rbp-80h]
		//   int a2; // [rsp+30h] [rbp-78h] BYREF
		//   unsigned __int16 a2_4; // [rsp+34h] [rbp-74h]
		//   unsigned int v10; // [rsp+38h] [rbp-70h]
		//   char v11[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && (a2 & 0x3FFF0000) == 0x10000
		//     && (unsigned __int8)sub_7FFC80575550((unsigned int)a2_4 + 11264, &v5, &v6)
		//     && !v5 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setUserData");
		//     v3 = sub_7FFC80564220((__int64)v11, v6);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"userDataSlot", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v11);
		//     v4 = sub_7FFC80564220((__int64)v11, v10);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"data", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v11);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setUserData");
			// m_cb->setUserData();
			count = kPacketCountSetUserData2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::prefetchIntoL22(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int16 a2; // [rsp+28h] [rbp-90h] BYREF
		//   int a2_4; // [rsp+2Ch] [rbp-8Ch]
		//   __int64 v7; // [rsp+30h] [rbp-88h]
		//   __int64 v8; // [rsp+38h] [rbp-80h]
		//   unsigned int v9; // [rsp+40h] [rbp-78h]
		//   char v10[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 20480 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"dmaData");
		//     if ( (a2_4 & 0x300000 | ((v9 & 0x8000000 | (v9 >> 1) & 0x10000000) >> 5)) >> 20 == 12 && v8 == 197164 )
		//     {
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         rdx0,
		//         (struct Concurrency::details::SchedulerNode *)"prefetchIntoL2");
		//       v3 = sub_7FFC80564430(v10, v7);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dataAddr", (__int64)"void*", v3);
		//       sub_7FFC805644A0((__int64)v10);
		//       v4 = sub_7FFC80564220((__int64)v10, v9 & 0x1FFFFF);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"sizeInBytes", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v10);
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       *((_WORD *)rdx0 + 12) = 257;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: prefetchIntoL2");
			// m_cb->prefetchIntoL2();
			count = kPacketCountPrefetchIntoL22;
		}

		return count;
	}

	uint32_t GnmCommandProxy::readDataFromGds2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   unsigned int v4; // ebx
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int16 a2; // [rsp+28h] [rbp-90h] BYREF
		//   int a2_4; // [rsp+2Ch] [rbp-8Ch]
		//   int v12; // [rsp+30h] [rbp-88h]
		//   __int64 v13; // [rsp+34h] [rbp-84h]
		//   unsigned int v14; // [rsp+3Ch] [rbp-7Ch]
		//   char v15[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     LOBYTE(v3) = a2;
		//     if ( (a2 & 0xFF00) == 18688 && (a2 & 2) != 0 )
		//     {
		//       LOBYTE(v3) = a2_4 & 0x3F;
		//       if ( (a2_4 & 0x3F) == 47 )
		//       {
		//         v3 = a2_4 & 0xF00;
		//         if ( v3 == 1536
		//           && (a2_4 & 0x3F000) == 0
		//           && (a2_4 & 0x6000000) == 0
		//           && (a2_4 & 0x8000000) == 0
		//           && (v12 & 0x30000) == 0 )
		//         {
		//           LOBYTE(v3) = 0;
		//           if ( (v12 & 0x7000000) == 50331648 && (v12 & 0xE0000000) == -1610612736 )
		//           {
		//             v4 = v14;
		//             set_packet_count((uint64_t)rdx0, 1u);
		//             Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//               rdx0,
		//               (struct Concurrency::details::SchedulerNode *)"readDataFromGds");
		//             v5 = sub_7FFC80564220((__int64)v15, a2_4 & 0x3F);
		//             sub_7FFC805A8FC0((__int64)rdx0, (__int64)"eventType", (__int64)"EndOfShaderEventType", v5);
		//             sub_7FFC805644A0((__int64)v15);
		//             v6 = sub_7FFC80564430(v15, v13);
		//             sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dstGpuAddr", (__int64)"void*", v6);
		//             sub_7FFC805644A0((__int64)v15);
		//             v7 = sub_7FFC80564220((__int64)v15, (unsigned __int16)v4);
		//             sub_7FFC805A8FC0((__int64)rdx0, (__int64)"gdsOffsetInDwords", (__int64)"uint32_t", v7);
		//             sub_7FFC805644A0((__int64)v15);
		//             v8 = sub_7FFC80564220((__int64)v15, HIWORD(v4));
		//             sub_7FFC805A8FC0((__int64)rdx0, (__int64)"gdsSizeInDwords", (__int64)"uint32_t", v8);
		//             LOBYTE(v3) = sub_7FFC805644A0((__int64)v15);
		//             *((_BYTE *)rdx0 + 24) = 1;
		//           }
		//         }
		//       }
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: readDataFromGds");
			// m_cb->readDataFromGds();
			count = kPacketCountReadDataFromGds2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::waitOnAddress2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v3; // eax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   char v9[16]; // [rsp+20h] [rbp-59h] BYREF
		//   __int64 v10; // [rsp+30h] [rbp-49h]
		//   __int64 a2; // [rsp+38h] [rbp-41h] BYREF
		//   int v12; // [rsp+40h] [rbp-39h]
		//   int v13; // [rsp+48h] [rbp-31h]
		//   int v14; // [rsp+4Ch] [rbp-2Dh]
		//   unsigned int v15; // [rsp+50h] [rbp-29h]
		//   unsigned __int16 v16; // [rsp+54h] [rbp-25h]
		//   unsigned int v17; // [rsp+58h] [rbp-21h]
		//   unsigned int v18; // [rsp+5Ch] [rbp-1Dh]
		//   __int16 v19; // [rsp+60h] [rbp-19h]
		//   int v20; // [rsp+64h] [rbp-15h]
		//   __int16 v21; // [rsp+68h] [rbp-11h]
		//   int v22; // [rsp+6Ch] [rbp-Dh]
		//   char v23[80]; // [rsp+70h] [rbp-9h] BYREF
		//
		//   v10 = -2i64;
		//   LOBYTE(v3) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 56i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v3 )
		//   {
		//     v3 = a2 & 0xFF00;
		//     if ( v3 == 30976 && WORD2(a2) == 834 )
		//     {
		//       v3 = v20 & 0xFF00;
		//       if ( v3 == 30976 && v21 == 834 )
		//       {
		//         v3 = v13 & 0xFF00;
		//         if ( v3 == 15360 )
		//         {
		//           LOBYTE(v3) = 0;
		//           if ( (v12 & 0xFF000000) == -1862270976 )
		//           {
		//             LOBYTE(v3) = 0;
		//             if ( (v22 & 0xFF000000) == -1862270976 )
		//             {
		//               LOBYTE(v3) = v14;
		//               if ( (v14 & 0x100) == 0 )
		//               {
		//                 LOBYTE(v3) = v14 & 0x30;
		//                 if ( (v14 & 0x30) == 16 && v19 == 10 )
		//                 {
		//                   Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//                     rdx0,
		//                     (struct Concurrency::details::SchedulerNode *)"waitOnAddress");
		//                   _lambda_082c17da81b0962e08c0587ee0fac50c_::_lambda_082c17da81b0962e08c0587ee0fac50c_(
		//                     (_lambda_082c17da81b0962e08c0587ee0fac50c_ *)v9,
		//                     (int *)(v15 | ((unsigned __int64)v16 << 32)),
		//                     (const char **)4);
		//                   v4 = sub_7FFC80564280(v23, v9, 4i64);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"label", (__int64)"ResourceLabel", v4);
		//                   sub_7FFC805644A0((__int64)v23);
		//                   v5 = sub_7FFC80564220((__int64)v23, v18);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"mask", (__int64)"uint32_t", v5);
		//                   sub_7FFC805644A0((__int64)v23);
		//                   v6 = sub_7FFC80564220((__int64)v23, v14 & 7);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"compareFunc", (__int64)"WaitCompareFunc", v6);
		//                   sub_7FFC805644A0((__int64)v23);
		//                   v7 = sub_7FFC80564220((__int64)v23, v17);
		//                   sub_7FFC805A8FC0((__int64)rdx0, (__int64)"refValue", (__int64)"uint32_t", v7);
		//                   sub_7FFC805644A0((__int64)v23);
		//                   set_packet_count((uint64_t)rdx0, 3u);
		//                   *((_BYTE *)rdx0 + 24) = 1;
		//                   LOBYTE(v3) = nullsub_1(v9);
		//                 }
		//               }
		//             }
		//           }
		//         }
		//       }
		//     }
		//   }
		//   return v3;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: waitOnAddress");
			// m_cb->waitOnAddress();
			count = kPacketCountWaitOnAddress2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::dispatchDraw2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   __int64 v5; // rax
		//   unsigned int v6; // ebx
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   __int64 v10; // rax
		//   __int64 v11; // rax
		//   char v13[20]; // [rsp+28h] [rbp-A0h] BYREF
		//   uint32_t v14; // [rsp+3Ch] [rbp-8Ch]
		//   __int16 a2; // [rsp+48h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+4Ch] [rbp-7Ch]
		//   unsigned int v17; // [rsp+50h] [rbp-78h]
		//   unsigned int v18; // [rsp+54h] [rbp-74h]
		//   unsigned int v19; // [rsp+58h] [rbp-70h]
		//   unsigned int v20; // [rsp+5Ch] [rbp-6Ch]
		//   char v21[80]; // [rsp+60h] [rbp-68h] BYREF
		//
		//   LOBYTE(v4) = GetOpCodeType(a1, (__int64)&a2, 24i64, *((_DWORD *)a1 + 2));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 36096 )
		//     {
		//       sub_7FFC805AD130((__int64)v13);
		//       v5 = sub_7FFC80594650((__int64)*a1, (__int64)a1[1]);
		//       if ( sub_7FFC805AD150(3, *a1, v5, (__int64)v13) )
		//       {
		//         v6 = (v20 >> 3) & 3;
		//         v7 = sub_7FFC80564220((__int64)v21, v17);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"threadGroupX", (__int64)"uint32_t", v7);
		//         sub_7FFC805644A0((__int64)v21);
		//         v8 = sub_7FFC80564220((__int64)v21, v18);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"threadGroupY", (__int64)"uint32_t", v8);
		//         sub_7FFC805644A0((__int64)v21);
		//         v9 = sub_7FFC80564220((__int64)v21, v19);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"threadGroupZ", (__int64)"uint32_t", v9);
		//         sub_7FFC805644A0((__int64)v21);
		//         v10 = sub_7FFC80564220((__int64)v21, v6);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"orderedAppendMode", (__int64)"DispatchOrderedAppendMode", v10);
		//         sub_7FFC805644A0((__int64)v21);
		//         v11 = sub_7FFC80564220((__int64)v21, a2_4);
		//         sub_7FFC805A8FC0((__int64)rdx0, (__int64)"sgprKrbLoc", (__int64)"uint32_t", v11);
		//         sub_7FFC805644A0((__int64)v21);
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"dispatchDraw");
		//         set_packet_count((uint64_t)rdx0, v14);
		//         sub_7FFC80564780((__int64)rdx0, 0);
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//       LOBYTE(v4) = nullsub_1(v13);
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: dispatchDraw");
			// m_cb->dispatchDraw();
			count = kPacketCountDispatchDraw2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::flushShaderCachesAndWait2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int16 a2; // [rsp+28h] [rbp-90h] BYREF
		//   int a2_4; // [rsp+2Ch] [rbp-8Ch]
		//   int v7; // [rsp+38h] [rbp-80h]
		//   char v8[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 22528
		//     && !v7
		//     && (a2_4 & 0x7FC0) == 0
		//     && a2_4 >= 0 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"flushShaderCachesAndWait");
		//     v3 = sub_7FFC80564220((__int64)v8, (a2_4 & 0x58000 | ((unsigned int)a2_4 >> 3) & 0x180000) >> 15);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cacheAction", (__int64)"CacheAction", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     v4 = sub_7FFC80564220((__int64)v8, a2_4 & 0x2E000000);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"extendedCacheMask", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v8);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: flushShaderCachesAndWait");
			// m_cb->flushShaderCachesAndWait();
			count = kPacketCountFlushShaderCachesAndWait2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::enableOrderedAppendAllocationCounter2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v3; // edi
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   int v8; // [rsp+28h] [rbp-80h] BYREF
		//   __int16 v9; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v10; // [rsp+30h] [rbp-78h]
		//   unsigned int v11; // [rsp+34h] [rbp-74h]
		//   unsigned int v12; // [rsp+38h] [rbp-70h]
		//   char v13[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&v8, 20i64, *(_DWORD *)(a1 + 8)) && (v8 & 0xFF00) == 30976 && v9 == 1053 )
		//   {
		//     v3 = v12;
		//     if ( v12 >> 31 == 1 )
		//     {
		//       set_packet_count((uint64_t)a2, 1u);
		//       v4 = sub_7FFC80564220((__int64)v13, v10);
		//       sub_7FFC805A8FC0((__int64)a2, (__int64)"oaCounterIndex", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v13);
		//       v5 = sub_7FFC80564220((__int64)v13, (unsigned __int16)v3 >> 2);
		//       sub_7FFC805A8FC0((__int64)a2, (__int64)"gdsDwOffsetOfCounter", (__int64)"uint32_t", v5);
		//       sub_7FFC805644A0((__int64)v13);
		//       v6 = sub_7FFC80564220((__int64)v13, (v3 >> 20) & 0xF);
		//       sub_7FFC805A8FC0((__int64)a2, (__int64)"oaOpIndex", (__int64)"uint32_t", v6);
		//       sub_7FFC805644A0((__int64)v13);
		//       v7 = sub_7FFC80564220((__int64)v13, v11);
		//       sub_7FFC805A8FC0((__int64)a2, (__int64)"spaceInAllocationUnits", (__int64)"uint32_t", v7);
		//       sub_7FFC805644A0((__int64)v13);
		//       Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//         a2,
		//         (struct Concurrency::details::SchedulerNode *)"enableOrderedAppendAllocationCounter");
		//       *((_BYTE *)a2 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: enableOrderedAppendAllocationCounter");
			// m_cb->enableOrderedAppendAllocationCounter();
			count = kPacketCountEnableOrderedAppendAllocationCounter2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setupDispatchDrawKickRingBuffer(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int16 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int a2_4; // [rsp+24h] [rbp-84h]
		//   __int64 v8; // [rsp+38h] [rbp-70h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v8 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 20i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 35840 )
		//   {
		//     v3 = sub_7FFC80564220((__int64)v9, a2_4 & 0x3FF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"krbCount", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, (a2_4 >> 20) & 0x3FF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"gdsDwOffsetKrb", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     v5 = sub_7FFC80564220((__int64)v9, (a2_4 >> 10) & 0x3FF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"gdsDwOffsetKrbCounters", (__int64)"uint32_t", v5);
		//     sub_7FFC805644A0((__int64)v9);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setupDispatchDrawKickRingBuffer");
		//     set_packet_count((uint64_t)rdx0, 2u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setupDispatchDrawKickRingBuffer");
			// m_cb->setupDispatchDrawKickRingBuffer();
			count = kPacketCountSetupDispatchDrawKickRingBuffer;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setScratchSize(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   unsigned int v6; // [rsp+28h] [rbp-80h]
		//   __int64 v7; // [rsp+30h] [rbp-78h]
		//   char v8[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   v7 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30208
		//     && WORD2(a2) == 536 )
		//   {
		//     v3 = sub_7FFC80564220((__int64)v8, v6 & 0xFFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"maxNumWaves", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     v4 = sub_7FFC80564220((__int64)v8, (v6 >> 12) & 0x1FFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"num1KByteChunksPerWave", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v8);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setScratchSize");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setScratchSize");
			// m_cb->setScratchSize();
			count = kPacketCountSetScratchSize;
		}

		return count;
	}

	uint32_t GnmCommandProxy::triggerReleaseMemEventInterrupt(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int16 a2; // [rsp+28h] [rbp-90h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-8Ch]
		//   int v7; // [rsp+30h] [rbp-88h]
		//   char v8[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 18688
		//     && (v7 & 0x7000000) == 0x1000000
		//     && (a2 & 2) != 0 )
		//   {
		//     v3 = sub_7FFC80564220((__int64)v8, a2_4 & 0x3F);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"eventType", (__int64)"ReleaseMemEventType", v3);
		//     sub_7FFC805644A0((__int64)v8);
		//     v4 = sub_7FFC80564220((__int64)v8, (a2_4 >> 12) & 0x3F);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cacheAction", (__int64)"CacheAction", v4);
		//     sub_7FFC805644A0((__int64)v8);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"triggerReleaseMemEventInterrupt");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: triggerReleaseMemEventInterrupt");
			// m_cb->triggerReleaseMemEventInterrupt();
			count = kPacketCountTriggerReleaseMemEventInterrupt;
		}

		return count;
	}

	uint32_t GnmCommandProxy::waitForResume(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int v4; // eax
		//   unsigned __int64 v5; // rbx
		//   __int64 v6; // rax
		//   unsigned int v8; // [rsp+20h] [rbp-98h] BYREF
		//   __int64 v9; // [rsp+28h] [rbp-90h]
		//   __int16 a2; // [rsp+30h] [rbp-88h] BYREF
		//   __int16 a2_4; // [rsp+34h] [rbp-84h]
		//   unsigned int v12; // [rsp+38h] [rbp-80h]
		//   unsigned __int16 v13; // [rsp+3Ch] [rbp-7Ch]
		//   int v14; // [rsp+40h] [rbp-78h]
		//   int v15; // [rsp+44h] [rbp-74h]
		//   char v16[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   LOBYTE(v4) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)v4 )
		//   {
		//     v4 = a2 & 0xFF00;
		//     if ( v4 == 15360 && (a2_4 & 0x100) == 0 )
		//     {
		//       LOBYTE(v4) = a2_4 & 0x30;
		//       if ( (a2_4 & 0x30) == 16 && v14 == 0x80000000 && v15 == 0x80000000 && (a2_4 & 7) == 3 )
		//       {
		//         v5 = v12 | ((unsigned __int64)v13 << 32);
		//         LOBYTE(v4) = sub_7FFC805B46E0(a1, &v8, 4i64, v5 - 4);
		//         if ( (_BYTE)v4 )
		//         {
		//           v4 = sub_7FFC805A9100(v8);
		//           if ( v4 == 89 )
		//           {
		//             set_packet_count((uint64_t)rdx0, 1u);
		//             Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//               rdx0,
		//               (struct Concurrency::details::SchedulerNode *)"waitForResume");
		//             v6 = sub_7FFC80564430(v16, v5 + 4);
		//             sub_7FFC805A8FC0((__int64)rdx0, (__int64)"holeAddr", (__int64)"void*", v6);
		//             LOBYTE(v4) = sub_7FFC805644A0((__int64)v16);
		//             *((_BYTE *)rdx0 + 24) = 1;
		//           }
		//         }
		//       }
		//     }
		//   }
		//   return v4;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: waitForResume");
			// m_cb->waitForResume();
			count = kPacketCountWaitForResume;
		}

		return count;
	}

	uint32_t GnmCommandProxy::writeResume(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   int packet_size; // eax
		//   __int64 v5; // rbx
		//   __int64 v6; // rax
		//   char v8[8]; // [rsp+20h] [rbp-98h] BYREF
		//   __int64 v9; // [rsp+28h] [rbp-90h]
		//   unsigned int a2; // [rsp+30h] [rbp-88h] BYREF
		//   int a2_4; // [rsp+34h] [rbp-84h]
		//   __int64 v12; // [rsp+38h] [rbp-80h]
		//   int v13; // [rsp+40h] [rbp-78h]
		//   char v14[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   v9 = -2i64;
		//   LOBYTE(packet_size) = GetOpCodeType((_QWORD *)a1, (__int64)&a2, 20i64, *(_DWORD *)(a1 + 8));
		//   if ( (_BYTE)packet_size )
		//   {
		//     packet_size = a2 & 0xFF00;
		//     if ( packet_size == 14080 )
		//     {
		//       packet_size = a2_4 & 0xF00;
		//       if ( packet_size == 1280 && (a2_4 & 0x6000000) == 0x4000000 )
		//       {
		//         packet_size = get_packet_size(a2);
		//         if ( packet_size == 20 && v13 == 0x80000000 )
		//         {
		//           v5 = v12;
		//           LOBYTE(packet_size) = sub_7FFC805B46E0(a1, v8, 4i64, v12 - 4);
		//           if ( (_BYTE)packet_size )
		//           {
		//             set_packet_count((uint64_t)rdx0, 1u);
		//             Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//               rdx0,
		//               (struct Concurrency::details::SchedulerNode *)"writeResume");
		//             v6 = sub_7FFC80564430(v14, v5 + 4);
		//             sub_7FFC805A8FC0((__int64)rdx0, (__int64)"holeAddr", (__int64)"void*", v6);
		//             LOBYTE(packet_size) = sub_7FFC805644A0((__int64)v14);
		//             *((_BYTE *)rdx0 + 24) = 1;
		//           }
		//         }
		//       }
		//     }
		//   }
		//   return packet_size;

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: writeResume");
			// m_cb->writeResume();
			count = kPacketCountWriteResume;
		}

		return count;
	}

	uint32_t GnmCommandProxy::enableQueueQuantumTimer(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   unsigned int a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30720
		//     && (unsigned int)get_packet_size(a2) == 12
		//     && (a2 & 2) != 0
		//     && (_WORD)a2_4 == 12
		//     && (v8 & 1) != 0 )
		//   {
		//     v3 = sub_7FFC80564220((__int64)v9, HIWORD(a2_4));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"queueId", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, (v8 >> 4) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"quantumScale", (__int64)"QuantumScale", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     v5 = sub_7FFC80564220((__int64)v9, (v8 >> 8) & 0x3F);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"quantumDuration", (__int64)"uint32_t", v5);
		//     sub_7FFC805644A0((__int64)v9);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"enableQueueQuantumTimer");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: enableQueueQuantumTimer");
			// m_cb->enableQueueQuantumTimer();
			count = kPacketCountEnableQueueQuantumTimer;
		}

		return count;
	}

	uint32_t GnmCommandProxy::disableQueueQuantumTimer(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   unsigned int a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   char v6; // [rsp+30h] [rbp-78h]
		//   char v7[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30720
		//     && (unsigned int)get_packet_size(a2) == 12
		//     && (a2 & 2) != 0
		//     && (_WORD)a2_4 == 12
		//     && (v6 & 1) == 0 )
		//   {
		//     v3 = sub_7FFC80564220((__int64)v7, HIWORD(a2_4));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"queueId", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v7);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"disableQueueQuantumTimer");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: disableQueueQuantumTimer");
			// m_cb->disableQueueQuantumTimer();
			count = kPacketCountDisableQueueQuantumTimer;
		}

		return count;
	}

	uint32_t GnmCommandProxy::insertRttvDingDongMarker(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 a2; // [rsp+20h] [rbp-88h] BYREF
		//   int v4; // [rsp+2Ch] [rbp-7Ch]
		//   int v5; // [rsp+38h] [rbp-70h]
		//   int v6; // [rsp+44h] [rbp-64h]
		//   int v7; // [rsp+50h] [rbp-58h]
		//   int v8; // [rsp+6Ch] [rbp-3Ch]
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 100i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30976
		//     && (v4 & 0xFF00) == 14592
		//     && (v5 & 0xFF00) == 14592
		//     && (v6 & 0xFF00) == 14592
		//     && (v7 & 0xFF00) == 15360
		//     && (v8 & 0xFF00) == 0x4000 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"insertRttvDingDongMarker");
		//     set_packet_count((uint64_t)rdx0, 6u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: insertRttvDingDongMarker");
			// m_cb->insertRttvDingDongMarker();
			count = kPacketCountInsertRttvDingDongMarker;
		}

		return count;
	}

	uint32_t GnmCommandProxy::RttvThreadTraceMarker3(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   _WORD a2[12]; // [rsp+20h] [rbp-18h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2[0] & 0xFF00) == 30976
		//     && (unsigned int)a2[2] - 832 <= 3 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"RttvThreadTraceMarker");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: RttvThreadTraceMarker");
			// m_cb->RttvThreadTraceMarker();
			count = kPacketCountRttvThreadTraceMarker3;
		}

		return count;
	}

	uint32_t GnmCommandProxy::setQueueRegister(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   unsigned int a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned int v8; // [rsp+30h] [rbp-78h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 12i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 30720
		//     && (unsigned int)get_packet_size(a2) == 12
		//     && (a2 & 2) != 0 )
		//   {
		//     v3 = sub_7FFC80564220((__int64)v9, (unsigned int)(unsigned __int16)a2_4 + 12864);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"regAddr", (__int64)"uint32_t", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, HIWORD(a2_4));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"vqid", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     v5 = sub_7FFC80564220((__int64)v9, v8);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"regValue", (__int64)"uint32_t", v5);
		//     sub_7FFC805644A0((__int64)v9);
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"setQueueRegister");
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: setQueueRegister");
			// m_cb->setQueueRegister();
			count = kPacketCountSetQueueRegister;
		}

		return count;
	}

	uint32_t GnmCommandProxy::callCommandBuffer2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int16 a2; // [rsp+28h] [rbp-80h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-7Ch]
		//   unsigned __int16 v7; // [rsp+30h] [rbp-78h]
		//   int v8; // [rsp+34h] [rbp-74h]
		//   char v9[80]; // [rsp+40h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 16i64, *(_DWORD *)(a1 + 8))
		//     && (a2 & 0xFF00) == 16128
		//     && (v8 & 0x100000) == 0 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"callCommandBuffer");
		//     v3 = sub_7FFC80564430(v9, a2_4 | ((unsigned __int64)v7 << 32));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"ibBaseAddr", (__int64)"void*", v3);
		//     sub_7FFC805644A0((__int64)v9);
		//     v4 = sub_7FFC80564220((__int64)v9, v8 & 0xFFFFF);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"ibSizeInDW", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v9);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: callCommandBuffer");
			// m_cb->callCommandBuffer();
			count = kPacketCountCallCommandBuffer2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::waitForGraphicsWrites2(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int16 a2; // [rsp+28h] [rbp-90h] BYREF
		//   int a2_4; // [rsp+2Ch] [rbp-8Ch]
		//   unsigned int v10; // [rsp+30h] [rbp-88h]
		//   unsigned int v11; // [rsp+38h] [rbp-80h]
		//   char v12[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 22528 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"waitForGraphicsWrites");
		//     if ( a2_4 >= 0 )
		//     {
		//       v3 = sub_7FFC80564430(v12, (unsigned __int64)v11 << 8);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"baseAddr", (__int64)"uint32_t*", v3);
		//       sub_7FFC805644A0((__int64)v12);
		//       v4 = sub_7FFC80564220((__int64)v12, v10);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"sizeIn256ByteBlocks", (__int64)"uint32_t", v4);
		//       sub_7FFC805644A0((__int64)v12);
		//       v5 = sub_7FFC80564220((__int64)v12, a2_4 & 0x7FC0);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"targetMask", (__int64)"WaitTargetSlot", v5);
		//       sub_7FFC805644A0((__int64)v12);
		//       v6 = sub_7FFC80564220((__int64)v12, (a2_4 & 0x58000 | ((unsigned int)a2_4 >> 3) & 0x180000) >> 15);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cacheAction", (__int64)"CacheAction", v6);
		//       sub_7FFC805644A0((__int64)v12);
		//       v7 = sub_7FFC80564220((__int64)v12, a2_4 & 0x2E000000);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"extendedCacheMask", (__int64)"uint32_t", v7);
		//       sub_7FFC805644A0((__int64)v12);
		//       set_packet_count((uint64_t)rdx0, 1u);
		//       *((_BYTE *)rdx0 + 24) = 1;
		//     }
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: waitForGraphicsWrites");
			// m_cb->waitForGraphicsWrites();
			count = kPacketCountWaitForGraphicsWrites2;
		}

		return count;
	}

	uint32_t GnmCommandProxy::writeResumeEventWithInterrupt(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   unsigned int v4; // edi
		//   int *v5; // rsi
		//   unsigned int v6; // ecx
		//   __int64 v7; // r14
		//   __int64 v8; // rax
		//   __int64 v9; // rax
		//   __int64 v10; // rax
		//   __int64 v11; // rax
		//   unsigned int v12; // eax
		//   __int64 v13; // rax
		//   __int64 v14; // rax
		//   __int64 v15; // rax
		//   __int64 v16; // rax
		//   __int64 v17; // rax
		//   __int64 v18; // rax
		//   __int64 v19; // rax
		//   unsigned int v20; // eax
		//   char *v21; // rdx
		//   unsigned int v22; // [rsp+20h] [rbp-69h] BYREF
		//   char v23[16]; // [rsp+28h] [rbp-61h] BYREF
		//   __int64 v24; // [rsp+38h] [rbp-51h]
		//   __int16 a2; // [rsp+40h] [rbp-49h] BYREF
		//   unsigned int a2_4; // [rsp+44h] [rbp-45h]
		//   unsigned int v27; // [rsp+48h] [rbp-41h]
		//   int *v28; // [rsp+4Ch] [rbp-3Dh]
		//   __int64 v29; // [rsp+54h] [rbp-35h]
		//   char v30[80]; // [rsp+60h] [rbp-29h] BYREF
		//
		//   v24 = -2i64;
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 18688 && (a2 & 2) != 0 )
		//   {
		//     v4 = (v27 & 0x30000 | (a2_4 >> 7) & 0x100000) >> 16;
		//     v5 = v28;
		//     v6 = v27 >> 29;
		//     if ( v27 >> 29 )
		//     {
		//       if ( v6 == 1 )
		//       {
		//         v7 = 4i64;
		//       }
		//       else
		//       {
		//         if ( v6 <= 1 )
		//           return;
		//         if ( v6 <= 4 )
		//         {
		//           v7 = 8i64;
		//         }
		//         else
		//         {
		//           if ( v6 != 5 )
		//             return;
		//           v7 = 4 * WORD1(v29);
		//         }
		//       }
		//     }
		//     else
		//     {
		//       v7 = 0i64;
		//     }
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     if ( !v4
		//       && !HIDWORD(v29)
		//       && (_DWORD)v29 == 0x80000000
		//       && (v27 & 0xE0000000) == 0x20000000
		//       && (unsigned __int8)sub_7FFC805B46E0(a1, &v22, 4i64, v28 - 1)
		//       && (unsigned int)sub_7FFC805A9100(v22) == 89 )
		//     {
		//       v8 = sub_7FFC80564220((__int64)v30, a2_4 & 0x3F);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"eventType", (__int64)"ReleaseMemEventType", v8);
		//       sub_7FFC805644A0((__int64)v30);
		//       v9 = sub_7FFC80564430(v30, v5 + 1);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"holeAddr", (__int64)"void*", v9);
		//       sub_7FFC805644A0((__int64)v30);
		//       v10 = sub_7FFC80564220((__int64)v30, (a2_4 >> 12) & 0x3F);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cacheAction", (__int64)"CacheAction", v10);
		//       sub_7FFC805644A0((__int64)v30);
		//       v11 = sub_7FFC80564220((__int64)v30, (a2_4 >> 25) & 3);
		//       sub_7FFC805A8FC0((__int64)rdx0, (__int64)"writePolicy", (__int64)"CachePolicy", v11);
		//       sub_7FFC805644A0((__int64)v30);
		//       v12 = HIBYTE(v27) & 7;
		//       if ( v12 == 2 )
		//       {
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"writeResumeEventWithInterrupt");
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//       else if ( v12 == 3 )
		//       {
		//         Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//           rdx0,
		//           (struct Concurrency::details::SchedulerNode *)"writeResumeEvent");
		//         *((_BYTE *)rdx0 + 24) = 1;
		//       }
		//       return;
		//     }
		//     _lambda_082c17da81b0962e08c0587ee0fac50c_::_lambda_082c17da81b0962e08c0587ee0fac50c_(
		//       (_lambda_082c17da81b0962e08c0587ee0fac50c_ *)v23,
		//       v5,
		//       (const char **)v7);
		//     v13 = sub_7FFC80564220((__int64)v30, a2_4 & 0x3F);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"eventType", (__int64)"ReleaseMemEventType", v13);
		//     sub_7FFC805644A0((__int64)v30);
		//     v14 = sub_7FFC80564220((__int64)v30, v4);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dstSelector", (__int64)"EventWriteDest", v14);
		//     sub_7FFC805644A0((__int64)v30);
		//     v15 = sub_7FFC80564280(v30, v23, 4i64);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"dstGpuAddr", (__int64)"ResourceLabel", v15);
		//     sub_7FFC805644A0((__int64)v30);
		//     v16 = sub_7FFC80564220((__int64)v30, v27 >> 29);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"srcSelector", (__int64)"EventWriteSource", v16);
		//     sub_7FFC805644A0((__int64)v30);
		//     v17 = sub_7FFC80564430(v30, v29);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"immValue", (__int64)"uint64_t", v17);
		//     sub_7FFC805644A0((__int64)v30);
		//     v18 = sub_7FFC80564220((__int64)v30, (a2_4 >> 12) & 0x3F);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"cacheAction", (__int64)"CacheAction", v18);
		//     sub_7FFC805644A0((__int64)v30);
		//     v19 = sub_7FFC80564220((__int64)v30, (a2_4 >> 25) & 3);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"writePolicy", (__int64)"CachePolicy", v19);
		//     sub_7FFC805644A0((__int64)v30);
		//     v20 = HIBYTE(v27) & 7;
		//     if ( v20 == 2 )
		//     {
		//       v21 = "writeReleaseMemEventWithInterrupt";
		//     }
		//     else
		//     {
		//       if ( v20 != 3 )
		//       {
		// LABEL_28:
		//         nullsub_1(v23);
		//         return;
		//       }
		//       v21 = "writeReleaseMemEvent";
		//     }
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(rdx0, (struct Concurrency::details::SchedulerNode *)v21);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//     goto LABEL_28;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: writeResumeEventWithInterrupt");
			// m_cb->writeResumeEventWithInterrupt();
			count = kPacketCountWriteResumeEventWithInterrupt;
		}

		return count;
	}

	uint32_t GnmCommandProxy::writeWaitMemCmd(PPM4_TYPE_3_HEADER pm4Hdr)
	{
		//   __int64 v3; // rax
		//   __int64 v4; // rax
		//   __int64 v5; // rax
		//   __int64 v6; // rax
		//   __int64 v7; // rax
		//   __int64 v8; // rax
		//   __int16 a2; // [rsp+28h] [rbp-90h] BYREF
		//   unsigned int a2_4; // [rsp+2Ch] [rbp-8Ch]
		//   unsigned int v11; // [rsp+30h] [rbp-88h]
		//   unsigned __int16 v12; // [rsp+34h] [rbp-84h]
		//   unsigned int v13; // [rsp+38h] [rbp-80h]
		//   unsigned int v14; // [rsp+3Ch] [rbp-7Ch]
		//   unsigned __int16 v15; // [rsp+40h] [rbp-78h]
		//   char v16[80]; // [rsp+50h] [rbp-68h] BYREF
		//
		//   if ( GetOpCodeType((_QWORD *)a1, (__int64)&a2, 28i64, *(_DWORD *)(a1 + 8)) && (a2 & 0xFF00) == 15360 )
		//   {
		//     Concurrency::details::SchedulerProxy::SetAllocatedNodes(
		//       rdx0,
		//       (struct Concurrency::details::SchedulerNode *)"writeWaitMemCmd");
		//     v3 = sub_7FFC80564220((__int64)v16, a2_4 & 7);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"function", (__int64)"WaitCompareFunc", v3);
		//     sub_7FFC805644A0((__int64)v16);
		//     v4 = sub_7FFC80564220((__int64)v16, (a2_4 >> 8) & 1);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"engine", (__int64)"uint32_t", v4);
		//     sub_7FFC805644A0((__int64)v16);
		//     v5 = sub_7FFC80564430(v16, v11 | ((unsigned __int64)v12 << 32));
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"pollRegAddrOrMemOffset", (__int64)"void*", v5);
		//     sub_7FFC805644A0((__int64)v16);
		//     v6 = sub_7FFC80564220((__int64)v16, v13);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"reference", (__int64)"uint32_t", v6);
		//     sub_7FFC805644A0((__int64)v16);
		//     v7 = sub_7FFC80564220((__int64)v16, v14);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"mask", (__int64)"uint32_t", v7);
		//     sub_7FFC805644A0((__int64)v16);
		//     v8 = sub_7FFC80564220((__int64)v16, v15);
		//     sub_7FFC805A8FC0((__int64)rdx0, (__int64)"interval", (__int64)"uint32_t", v8);
		//     sub_7FFC805644A0((__int64)v16);
		//     set_packet_count((uint64_t)rdx0, 1u);
		//     *((_BYTE *)rdx0 + 24) = 1;
		//   }

		uint32_t count = 0;
		if (false)
		{
			LOG_TRACE("Gnm: writeWaitMemCmd");
			// m_cb->writeWaitMemCmd();
			count = kPacketCountWriteWaitMemCmd;
		}

		return count;
	}

}  // namespace sce::Gnm
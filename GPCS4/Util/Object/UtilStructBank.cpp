#include "UtilStructBank.h"

#include "GPCS4Log.h"

LOG_CHANNEL(Util.StructBank);

namespace util
{
	struct StructLink
	{
		struct StructLink* m_slNext;
	};

	struct StructBankPage
	{
		struct StructBankPage* m_next;
		unsigned long          m_objects;  // How many objects are in this page?
		uint32_t               m_data[1];
	};

	struct StructBank
	{
		// Struct size.
		unsigned long m_structSize;

		// The actual size we allocate with (aligned to 4 bytes and with 4 bytes
		// extra for the StructLink).
		unsigned long m_alignedStructSize;

		// How many structs per page.
		unsigned long m_cacheSize;

		// How many pages have we allocated?
		unsigned long m_numPages;

		// Total number of objects this StructBank has allocated.
		unsigned long m_numTotalObjects;

		// The first page.
		StructBankPage* m_pageHead;

		// The free list.
		StructLink* m_freeListHead;
	};

	bool sbAllocateNewStructPage(StructBank* pBank, uint32_t nAllocations);

	StructBank* sbCreate()
	{
		return (StructBank*)calloc(1, sizeof(StructBank));
	}

	void sbDestroy(StructBank* pBank)
	{
		if (pBank)
		{
			sbTerm(pBank);
		}
		free(pBank);
	}

	void sbInit(StructBank* pBank, uint32_t structSize, uint32_t cacheSize)
	{
		pBank->m_structSize = structSize;

		pBank->m_alignedStructSize = structSize;
		if (pBank->m_alignedStructSize < sizeof(StructLink))
			pBank->m_alignedStructSize = sizeof(StructLink);

		if (pBank->m_alignedStructSize % 4 != 0)
		{
			pBank->m_alignedStructSize += (4 - (pBank->m_alignedStructSize % 4));
		}

		pBank->m_numPages        = 0;
		pBank->m_numTotalObjects = 0;
		pBank->m_cacheSize       = cacheSize;
		pBank->m_pageHead        = NULL;
		pBank->m_freeListHead    = NULL;
	}

	bool sbInitEx(StructBank* pBank, uint32_t structSize, uint32_t cacheSize, uint32_t nPreallocations)
	{
		sbInit(pBank, structSize, cacheSize);

		return sbAllocateNewStructPage(pBank, nPreallocations);
	}

	void sbTerm(StructBank* pBank)
	{
		StructBankPage* pPage;
		StructBankPage* pNext;

		pPage = pBank->m_pageHead;
		while (pPage)
		{
			pNext = pPage->m_next;
			free(pPage);
			pPage = pNext;
		}

		pBank->m_pageHead        = NULL;
		pBank->m_freeListHead    = NULL;
		pBank->m_numPages        = 0;
		pBank->m_numTotalObjects = 0;
	}

	void* sbAllocate(StructBank* pBank)
	{
		StructLink* pRet = nullptr;
		do
		{
			pRet = pBank->m_freeListHead;
			if (!pRet)
			{
				sbAllocateNewStructPage(pBank, pBank->m_cacheSize);
				pRet = pBank->m_freeListHead;
				if (!pRet)
				{
					break;
				}
			}

			pBank->m_freeListHead = pRet->m_slNext;

		} while (false);
		return pRet;
	}

	void* sbAllocateZero(StructBank* pBank)
	{
		StructLink* pRet = (StructLink*)sbAllocate(pBank);
		memset(pRet, 0, pBank->m_structSize);
		return pRet;
	}

	void sbFree(StructBank* pBank, void* pObj)
	{
		StructLink* pLink     = (StructLink*)pObj;
		pLink->m_slNext       = pBank->m_freeListHead;
		pBank->m_freeListHead = pLink;
	}

	void sbFreeAll(StructBank* pBank)
	{
		StructBankPage* pPage;
		StructLink*     pStruct;
		uint8_t*        pDataPos;
		uint32_t        count;

		pBank->m_freeListHead = NULL;

		pPage = pBank->m_pageHead;
		while (pPage)
		{
#ifdef _DEBUG
			memset(pPage->m_data, 0xEA, pBank->m_alignedStructSize * pPage->m_objects);
#endif

			pDataPos = (uint8_t*)pPage->m_data;
			count    = pPage->m_objects;
			while (count--)
			{
				pStruct               = (StructLink*)pDataPos;
				pStruct->m_slNext     = pBank->m_freeListHead;
				pBank->m_freeListHead = pStruct;
				pDataPos += pBank->m_alignedStructSize;
			}

			pPage = pPage->m_next;
		}
	}

	bool sbAllocateNewStructPage(StructBank* pBank, uint32_t nAllocations)
	{
		bool ret = false;
		do
		{
			StructLink*     pStruct;
			StructBankPage* pPage;
			uint8_t*        pDataPos;
			uint32_t        count;

			if (nAllocations == 0)
			{
				ret = true;
				break;
			}

			// Allocate a new page.
			pPage = (StructBankPage*)malloc(
				(pBank->m_alignedStructSize * nAllocations) + 
				(sizeof(StructBankPage) - sizeof(uint32_t)));
			if (!pPage)
			{
				break;
			}

			pPage->m_next    = pBank->m_pageHead;
			pPage->m_objects = nAllocations;

			pBank->m_pageHead = pPage;

			++pBank->m_numPages;
			pBank->m_numTotalObjects += nAllocations;

			// Put its contents into the free list.
			pDataPos = (uint8_t*)pPage->m_data;
			count    = nAllocations;
			while (count--)
			{
				pStruct               = (StructLink*)pDataPos;
				pStruct->m_slNext     = pBank->m_freeListHead;
				pBank->m_freeListHead = pStruct;
				pDataPos += pBank->m_alignedStructSize;
			}

			ret = true;
		} while (false);
		return ret;
	}

}  // namespace util

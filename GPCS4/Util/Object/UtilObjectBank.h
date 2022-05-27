#pragma once

#include "UtilStructBank.h"

namespace util
{
	constexpr uint32_t DEFAULT_OBJECT_CACHE_SIZE = 32;

	class NullCS
	{
	public:
		void enter()
		{
		}
		void leave()
		{
		}
	};

	template <class T, class CS = NullCS>
	class ObjectBank
	{
	public:
		ObjectBank()
		{
			m_bank = sbCreate();
			sbInit(m_bank, sizeof(T), DEFAULT_OBJECT_CACHE_SIZE);
		}

		ObjectBank(uint32_t cacheSize, uint32_t preAllocate = 0)
		{
			m_bank = sbCreate();
			sbInitEx(m_bank, sizeof(T), cacheSize, preAllocate);
		}

		~ObjectBank()
		{
			sbDestroy(m_bank);
		}

		void init(uint32_t cacheSize, uint32_t preAllocate = 0)
		{
			m_cs.enter();
			sbTerm(m_bank);
			sbInitEx(m_bank, sizeof(T), cacheSize, preAllocate);
			m_cs.leave();
		}

		void term()
		{
			m_cs.enter();
			sbTerm(m_bank);
			sbInit(m_bank, sizeof(T), DEFAULT_OBJECT_CACHE_SIZE);
			m_cs.leave();
		}

		/**
		 * \brief Set page granularity
		 * 
		 * Set number of objects for each real memory allocation (malloc)
		 * Default is DEFAULT_OBJECT_CACHE_SIZE.
		 */
		void setPageGranularity(uint32_t size)
		{
			m_cs.enter();
			m_bank->m_cacheSize = size;
			m_cs.leave();
		}

		template <typename... Args>
		//typename std::enable_if<std::is_constructible_v<T, Args...>, T*>::type
		requires std::is_constructible_v<T, Args...>
		T* allocate(Args&&... args)
		{
			T* object = nullptr;

			m_cs.enter();
			object = (T*)sbAllocate(m_bank);
			if (object)
			{
				std::construct_at(object, std::forward<Args>(args)...);
			}
			m_cs.leave();

			return object;
		}

		void free(T* pObj)
		{
			m_cs.enter();
			std::destroy_at(pObj);
			sbFree(m_bank, object);
			m_cs.leave();
		}

	public:
		StructBank* m_bank = nullptr;
		CS          m_cs;
	};

}  // namespace util

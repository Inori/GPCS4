
#include "UtilStructBank.h"

namespace util
{
	// These work just like StructBanks but are for C++ objects that need their
	// constructors and destructors called.

	constexpr uint32_t DEFAULT_OBJECT_CACHE_SIZE = 32;

	class DummyThingForConstructor
	{
	public:
	};

	class NullCS
	{
	public:
		void Enter()
		{
		}
		void Leave()
		{
		}
	};

	template <class T, class CS = NullCS>
	class ObjectBank : public BaseObjectBank
	{
	public:
		ObjectBank();
		ObjectBank(uint32_t cacheSize, uint32_t preAllocate = 0);
		virtual ~ObjectBank();

		void init(uint32_t cacheSize, uint32_t preAllocate = 0);

		void term();

		// Set the cache size (in numbers of objects).
		// Default is DEFAULT_OBJECT_CACHE_SIZE.
		void setCacheSize(uint32_t size);

		T*   allocate();
		void free(T* pObj);

	public:
		StructBank m_bank;
		CS         m_cs;
	};

	template <class T, class CS>
	inline ObjectBank<T, CS>::ObjectBank()
	{
		sbInit(&m_bank, sizeof(T), DEFAULT_OBJECT_CACHE_SIZE);
	}

	template <class T, class CS>
	inline ObjectBank<T, CS>::ObjectBank(uint32_t cacheSize, uint32_t preAllocate)
	{
		sbInitEx(&m_bank, sizeof(T), cacheSize, preAllocate);
	}

	template <class T, class CS>
	inline ObjectBank<T, CS>::~ObjectBank()
	{
		sbTerm(&m_bank);
	}

	template <class T, class CS>
	inline void ObjectBank<T, CS>::init(uint32_t cacheSize, uint32_t preAllocate)
	{
		m_cs.Enter();
		sbTerm(&m_bank);
		sbInitEx(&m_bank, sizeof(T), cacheSize, preAllocate);
		m_cs.Leave();
	}

	template <class T, class CS>
	inline void ObjectBank<T, CS>::term()
	{
		m_cs.Enter();
		sbTerm(&m_bank);
		sbInit(&m_bank, sizeof(T), DEFAULT_OBJECT_CACHE_SIZE);
		m_cs.Leave();
	}

	template <class T, class CS>
	inline void ObjectBank<T, CS>::setCacheSize(uint32_t size)
	{
		m_cs.Enter();
		m_bank.m_cacheSize = size;
		m_cs.Leave();
	}

	template <class T, class CS>
	inline T* ObjectBank<T, CS>::allocate()
	{
		T* object;

		m_cs.Enter();
		object = (T*)sbAllocate(&m_Bank);
		if (object)
		{
			::new ((DummyThingForConstructor*)0, object) T;
		}
		m_cs.Leave();

		return object;
	}

	template <class T, class CS>
	inline void ObjectBank<T, CS>::free(T* object)
	{
		m_cs.Enter();
		object->~T();
		sbFree(&m_bank, object);
		m_cs.Leave();
	}

}  // namespace util

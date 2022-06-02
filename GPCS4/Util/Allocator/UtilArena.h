#pragma once

#include "PlatMemory.h"

#include <atomic>
#include <cassert>
#include <thread>
#include <vector>

namespace util
{
	/**
	 * \brief Arena allocation for mixed-type data.
	 * 
	 * Arena-style bump allocation is important for two reasons: First, so that
	 * allocation is quick, and second, so that allocated items are close together,
	 * which is cache-friendy. Arena allocation is also useful for a minor third
	 * reason which is to make freeing all the items in an arena very quick.
	 * 
	 * Note that the construct is properly called for every object,
	 * but the destructor is not called since we can't detect every
	 * object's type in one clear call.
	 * Care must be taken when allocating objects which has resource release
	 * operations in their destructor.
	 * 
	 */

	class ArenaAllocator
	{
		static constexpr size_t CHUNK_SIZE = 0x8000;
		static constexpr size_t MAX_ALIGN  = 16;  // allow 128bit SIMD

	public:
		ArenaAllocator()
		{
			m_threadId = std::this_thread::get_id();
			m_next.store(nullptr);
		}

		~ArenaAllocator()
		{
			clear();
			if (m_next.load())
			{
				delete m_next.load();
			}
		}

		template <class T>
		T* alloc()
		{
			static_assert(alignof(T) <= MAX_ALIGN,
						  "maximum alignment not large enough");
			auto* ret = static_cast<T*>(allocSpace(sizeof(T), alignof(T)));
			new (ret) T(*this);  // allocated objects receive the allocator, so they can
								 // allocate more later if necessary
			return ret;
		}

		void clear()
		{
			for (auto* chunk : m_chunks)
			{
				plat::aligned_free(chunk);
			}
			m_chunks.clear();
		}

	private:
		// Allocate an amount of space with a guaranteed alignment
		void* allocSpace(size_t size, size_t align)
		{
			// the bump allocator data should not be modified by multiple threads at
			// once.
			auto myId = std::this_thread::get_id();
			if (myId != m_threadId)
			{
				ArenaAllocator* curr      = this;
				ArenaAllocator* allocated = nullptr;
				while (myId != curr->m_threadId)
				{
					auto seen = curr->m_next.load();
					if (seen)
					{
						curr = seen;
						continue;
					}
					// there is a nullptr for next, so we may be able to place a new
					// allocator for us there. but carefully, as others may do so as
					// well. we may waste a few allocations here, but it doesn't matter
					// as this can only happen as the chain is built up, i.e.,
					// O(# of cores) per allocator, and our allocatrs are long-lived.
					if (!allocated)
					{
						allocated = new ArenaAllocator();  // has our thread id
					}
					if (curr->m_next.compare_exchange_strong(seen, allocated))
					{
						// we replaced it, so we are the next in the chain
						// we can forget about allocated, it is owned by the chain now
						allocated = nullptr;
						break;
					}
					// otherwise, the cmpxchg updated seen, and we continue to loop
					curr = seen;
				}
				if (allocated)
				{
					delete allocated;
				}
				return curr->allocSpace(size, align);
			}
			// First, move the current index in the last chunk to an aligned position.
			m_index = (m_index + align - 1) & (-align);
			if (m_index + size > CHUNK_SIZE || m_chunks.size() == 0)
			{
				// Allocate a new chunk.
				auto numChunks = (size + CHUNK_SIZE - 1) / CHUNK_SIZE;
				assert(size <= numChunks * CHUNK_SIZE);
				auto* allocation =
					plat::aligned_malloc(MAX_ALIGN, numChunks * CHUNK_SIZE);
				if (!allocation)
				{
					abort();
				}
				m_chunks.push_back(allocation);
				m_index = 0;
			}
			uint8_t* ret = static_cast<uint8_t*>(m_chunks.back());
			ret += m_index;
			m_index += size;  // TODO: if we allocated more than 1 chunk, reuse the
							  // remainder, right now we allocate another next time
			return static_cast<void*>(ret);
		}

	private:
		// Each pointer in chunks is to a multiple of CHUNK_SIZE - typically 1,
		// but possibly more.
		std::vector<void*> m_chunks;
		size_t             m_index = 0;  // in last chunk
		std::thread::id    m_threadId;

		// multithreaded allocation - each arena is valid on a specific thread.
		// if we are on the wrong thread, we atomically look in the linked
		// list of next, adding an allocator if necessary
		std::atomic<ArenaAllocator*> m_next;
	};
}  // namespace util

#pragma once

#include "GPCS4Common.h"
#include "UtilLikely.h"
#include "Violet/VltRc.h"

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <emmintrin.h>


namespace util::sync
{

	/**
     * \brief Signal
     * 
     * Interface for a CPU-side fence. Can be signaled
     * to a given value, and any thread waiting for a
     * lower value will be woken up.
     */
	class Signal : public sce::vlt::RcObject
	{

	public:
		virtual ~Signal()
		{
		}

		/**
         * \brief Last signaled value
         * \returns Last signaled value
         */
		virtual uint64_t value() const = 0;

		/**
         * \brief Notifies signal
         *
         * Wakes up all threads currently waiting for
         * a value lower than \c value. Note that
         * \c value must monotonically increase.
         * \param [in] value Value to signal to
         */
		virtual void signal(uint64_t value) = 0;

		/**
         * \brief Waits for signal
         * 
         * Blocks the calling thread until another
         * thread signals it with a value equal to
         * or greater than \c value.
         * \param [in] value The value to wait for
         */
		virtual void wait(uint64_t value) = 0;
	};

	/**
     * \brief Fence
     *
     * Simple CPU-side fence.
     */
	class Fence final : public Signal
	{

	public:
		Fence() :
			m_value(0ull)
		{
		}

		explicit Fence(uint64_t value) :
			m_value(value)
		{
		}

		uint64_t value() const
		{
			return m_value.load(std::memory_order_acquire);
		}

		void signal(uint64_t value)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_value.store(value, std::memory_order_release);
			m_cond.notify_all();
		}

		void wait(uint64_t value)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cond.wait(lock, [this, value]
						{ return value <= m_value.load(std::memory_order_acquire); });
		}

	private:
		std::atomic<uint64_t>   m_value;
		std::mutex              m_mutex;
		std::condition_variable m_cond;
	};


    /**
     * \brief Generic spin function
     *
     * Blocks calling thread until a condition becomes
     * \c true, calling \c yield every few iterations.
     * \param [in] spinCount Number of probes between each yield
     * \param [in] fn Condition to test
     */
	template <typename Fn>
	void spin(uint32_t spinCount, const Fn& fn)
	{
		while (unlikely(!fn()))
		{
			for (uint32_t i = 1; i < spinCount; i++)
			{
				_mm_pause();
				if (fn())
				{
					return;
				}
			}

			std::this_thread::yield();
		}
	}

	/**
     * \brief Spin lock
     * 
     * A low-overhead spin lock which can be used to
     * protect data structures for a short duration
     * in case the structure is not likely contested.
     */
	class Spinlock
	{

	public:
		Spinlock()
		{
		}
		~Spinlock()
		{
		}

		Spinlock(const Spinlock&) = delete;
		Spinlock& operator=(const Spinlock&) = delete;

		void lock()
		{
			spin(200, [this] { return try_lock(); });
		}

		void unlock()
		{
			m_lock.store(0, std::memory_order_release);
		}

		bool try_lock()
		{
			return likely(!m_lock.load()) && 
				likely(!m_lock.exchange(1, std::memory_order_acquire));
		}

	private:
		std::atomic<uint32_t> m_lock = { 0 };
	};

}  // namespace util::sync

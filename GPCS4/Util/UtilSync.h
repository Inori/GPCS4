#pragma once

#include "GPCS4Common.h"
#include <atomic>
#include <thread>

/**
 * \brief Spin lock
 *
 * A low-overhead spin lock which can be used to
 * protect data structures for a short duration
 * in case the structure is not likely contested.
 */

// detect x86
#ifdef _M_X64
#define __UTIL_SYNC_X86__ 1
#elif _M_IX86
#define __UTIL_SYNC_X86__ 1
#elif __x86_64
#define __UTIL_SYNC_X86__ 1
#elif __i386
#define __UTIL_SYNC_X86__ 1
#else
// none of the above
#define __UTIL_SYNC_UNKNOWN_ARCH__ 1
#endif

#ifdef __UTIL_SYNC_UNKNOWN_ARCH__
#error "unsupported platform"
#endif

#ifdef __UTIL_SYNC_X86__
//https://software.intel.com/content/www/us/en/develop/articles/benefitting-power-and-performance-sleep-loops.html
//https://www.felixcloutier.com/x86/pause
#ifdef _MSC_VER
#include <intrin.h>
#endif
void pause() {
#ifdef _MSC_VER
    _mm_pause();
#else
    __builtin_ia32_pause();
#endif
}
#endif

class Spinlock {

public:
    Spinlock() = default;
    Spinlock(const Spinlock &) = delete;
    Spinlock &operator=(const Spinlock &) = delete;

    void lock() {
        uint32_t expected;
        while (!m_lock.compare_exchange_weak(expected = 0, 1, std::memory_order_release, std::memory_order_consume)) {
            pause();
        }
    }

    void unlock() {
        m_lock.store(0, std::memory_order_release);
    }

    bool try_lock() {
        uint32_t expected = 0;
        return m_lock.compare_exchange_strong(expected, 1, std::memory_order_release, std::memory_order_consume);
    }

private:
    std::atomic<uint32_t> m_lock{};
};

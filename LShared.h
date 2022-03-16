//
// Created by andrzej on 10/11/21.
//

#ifndef MYSMARTPTR_LSHARED_H
#define MYSMARTPTR_LSHARED_H

#include <mutex>
#include <atomic>
#include <bits/atomic_word.h>
#include <ext/atomicity.h>

struct spinlock {
    std::atomic<bool> lock_ = {0};

    void lock() noexcept {
        for (;;) {
            // Optimistically assume the lock is free on the first try
            if (!lock_.exchange(true, std::memory_order_acquire)) {
                return;
            }
            // Wait for lock to be released without generating cache misses
            while (lock_.load(std::memory_order_relaxed)) {
                // Issue X86 PAUSE or ARM YIELD instruction to reduce contention between
                // hyper-threads
                __builtin_ia32_pause();
            }
        }
    }

    bool try_lock() noexcept {
        // First do a relaxed load to check if lock is free in order to prevent
        // unnecessary cache misses if someone does while(!try_lock())
        return !lock_.load(std::memory_order_relaxed) &&
               !lock_.exchange(true, std::memory_order_acquire);
    }

    void unlock() noexcept {
        lock_.store(false, std::memory_order_release);
    }
};

struct Object {
    _Atomic_word use_count;
    std::mutex m_mutex;
    spinlock m_spin;
    virtual ~Object(){}
};

void lshared_release_atomic(struct Object** pp);
void lshared_init_elem(Object* p);
void lshared_assign(struct Object** pp, Object* p);
void lweak_assign(Object** pp, Object* p);
void lshared_release_break(Object** pp, int maxn);

void lshared_release_single(Object** pp);
void lshared_assign_single(Object** pp, Object* p);

void lshared_release_mutex(Object** pp);
void lshared_assign_mutex(Object** pp, Object* p);

void lshared_release_spin(Object** pp);
void lshared_assign_spin(Object** pp, Object* p);

#endif //MYSMARTPTR_LSHARED_H

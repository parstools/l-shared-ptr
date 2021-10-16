#include <cassert>
#include "LShared.h"
void lshared_init_elem(Object** pp, Object* p) {
    p->use_count = 1;
    *pp = p;
}

void lshared_init_elem(Object* p) {
    p->use_count = 1;
}


void lshared_add_ref_single(Object* p) {
    ++p->use_count;
}

void lshared_add_ref_atomic(Object* p) {
    __gnu_cxx::__atomic_add_dispatch(&p->use_count, 1);
}

void lshared_release_single(Object** pp) {
    Object *p = *pp;
    if (--p->use_count == 0)
    {
        *pp = nullptr;
        delete p;
    }
}

void lshared_release_atomic(Object** pp)  {
    Object *p = *pp;
    if (!p) return;
    // Be race-detector-friendly.  For more info see bits/c++config.
    _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(&use_count);
    if (__gnu_cxx::__exchange_and_add_dispatch(&p->use_count, -1) == 1) {
        _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(&use_count);
        *pp = nullptr;
        delete p;
    }
}

long lshared_count_single(Object* p) {
    return p->use_count;
}

long lshared_count_atomic(Object* p) {
    if (!p) return 0;
    // No memory barrier is used here so there is no synchronization
    // with other threads.
    return __atomic_load_n(&p->use_count, __ATOMIC_RELAXED);
}

bool lshared_add_ref_lock_atomic(Object* p) {
    // Perform lock-free add-if-not-zero operation.
    _Atomic_word __count = lshared_count_atomic(p);
    do {
        if (__count == 0)
            return false;
        // Replace the current counter value with the old value + 1, as
        // long as it's not changed meanwhile.
    } while (!__atomic_compare_exchange_n(&p->use_count, &__count, __count + 1,
                                          true, __ATOMIC_ACQ_REL,
                                          __ATOMIC_RELAXED));
    return true;
}

void lshared_init_other(Object** pp, Object*__r) {
    Object *sc =__r;
    if (sc != 0)
        lshared_add_ref_atomic(sc);
    *pp = sc;
}

void    lshared_assign(Object** pp, Object* p) {
    assert(pp);
    Object*__tmp = p;
    if (__tmp != *pp) {
        if (__tmp != nullptr)
            lshared_add_ref_atomic(__tmp);
        lshared_release_atomic(pp);
        *pp = __tmp;
    }
}

void lweak_assign(Object** pp, Object* p) {
    assert(pp);
    *pp = p;
}

void lshared_swap(Object** pp, Object** pr) {
    Object* __tmp = *pr;
    *pr = *pp;
    *pp = __tmp;
}

bool share_count_unique(Object *sc) {
    return lshared_count_atomic(sc) == 1;
}

bool lshared_equal(Object *__a, Object *__b) {
    return __a == __b;
}


void lshared_ptr_init(Object** pp) {
    *pp = nullptr;
}


void lshared_ptr_reset(Object** pp, Object** pr) {
    lshared_swap(pp, pr);
}

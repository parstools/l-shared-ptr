#include <stdlib.h>
#include "CShared.h"

void object_destructor(struct Object *obj) {
    obj->vmt->destructor(obj);
    for (int i=0; i<obj->vmt->pointerFieldCount; i++)
        cshared_release_atomic((struct Object **)((void*)obj+obj->vmt->pointerFieldDeltas[i]));
}

void cshared_init_elem(struct Object** pp, struct Object* p) {
    p->use_count = 1;
    *pp = p;
}

void cshared_add_ref_single(struct Object* p) {
    ++p->use_count;
}

void cshared_add_ref_atomic(struct Object* p) {
    __atomic_fetch_add(&p->use_count, 1, __ATOMIC_ACQ_REL);
}

void cshared_release_single(struct Object** pp) {
    struct Object *p = *pp;
    if (--p->use_count == 0)
    {
        *pp = NULL;
        object_destructor(p);
        free(p);
    }
}

void cshared_release_atomic(struct Object** pp)  {
    struct Object *p = *pp;
    if (!p) return;
    // Be race-detector-friendly.  For more info see bits/c++config.
    if (__atomic_fetch_add(&p->use_count, -1, __ATOMIC_ACQ_REL) == 1) {
        *pp = NULL;
        object_destructor(p);
        free(p);
    }
}

long cshared_count_single(struct Object* p) {
    return p->use_count;
}

long cshared_count_atomic(struct Object* p) {
    if (!p) return 0;
    // No memory barrier is used here so there is no synchronization
    // with other threads.
    return __atomic_load_n(&p->use_count, __ATOMIC_RELAXED);
}

char cshared_add_ref_lock_atomic(struct Object* p) {
    // Perform lock-free add-if-not-zero operation.
    int __count = cshared_count_atomic(p);
    do {
        if (__count == 0)
            return 0;
        // Replace the current counter value with the old value + 1, as
        // long as it's not changed meanwhile.
    } while (!__atomic_compare_exchange_n(&p->use_count, &__count, __count + 1,
                                          1, __ATOMIC_ACQ_REL,
                                          __ATOMIC_RELAXED));
    return 1;
}

void cshared_init_other(struct Object** pp, struct Object*__r) {
    struct Object *sc =__r;
    if (sc != 0)
        cshared_add_ref_atomic(sc);
    *pp = sc;
}

void cshared_assign(struct Object** pp, struct Object** pr) {
    struct Object*__tmp = *pr;
    if (__tmp != *pp) {
        if (__tmp != 0)
            cshared_add_ref_atomic(__tmp);
        cshared_release_atomic(pp);
        *pp = __tmp;
    }
}

void cshared_swap(struct Object** pp, struct Object** pr) {
    struct Object* __tmp = *pr;
    *pr = *pp;
    *pp = __tmp;
}

char share_count_unique(struct Object *sc) {
    return cshared_count_atomic(sc) == 1;
}

char cshared_equal(struct Object *__a, struct Object *__b) {
    return __a == __b;
}


void cshared_ptr_init(struct Object** pp) {
    *pp = NULL;
}


void cshared_ptr_reset(struct Object** pp, struct Object** pr) {
    cshared_swap(pp, pr);
}

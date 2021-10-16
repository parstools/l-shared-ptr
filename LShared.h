//
// Created by andrzej on 10/11/21.
//

#ifndef MYSMARTPTR_LSHARED_H
#define MYSMARTPTR_LSHARED_H

#include <bits/atomic_word.h>
#include <ext/atomicity.h>

struct Object {
    _Atomic_word use_count;
    virtual ~Object(){}
};

void lshared_release_atomic(struct Object** pp);
void lshared_init_elem(Object* p);
void lshared_assign(struct Object** pp, Object* p);
void lweak_assign(Object** pp, Object* p);

#endif //MYSMARTPTR_LSHARED_H

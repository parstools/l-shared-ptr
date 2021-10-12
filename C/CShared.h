//
// Created by andrzej on 10/11/21.
//

#ifndef MYSMARTPTR_LSHARED_H
#define MYSMARTPTR_LSHARED_H

#include <stdatomic.h>

struct VMT {
    void (*destructor)(struct Object*);
    int pointerFieldCount;
    int pointerFieldDeltas[];
};

struct Object {
    struct VMT* vmt;
    int use_count;
};

void object_destructor(struct Object *obj);

void cshared_release_atomic(struct Object** pp);
void cshared_init_elem(struct Object** pp, struct Object* p);
void cshared_assign(struct Object** pp, struct Object** pr);

#endif //MYSMARTPTR_LSHARED_H

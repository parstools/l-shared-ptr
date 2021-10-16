//
// Created by andrzej on 10/13/21.
//

#ifndef L_SHARED_PTR_WEAK_H
#define L_SHARED_PTR_WEAK_H
#include "LShared.h"

struct Apartment;

struct Person: Object {
    ~Person();
    Apartment* apartment;
};

struct Apartment: Object {
    ~Apartment();
    Person *person;
};

void testweak();

#endif //L_SHARED_PTR_WEAK_H

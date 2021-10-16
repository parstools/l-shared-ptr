#include <malloc.h>
#include <string.h>
#include "CShared.h"

struct Elem {
    struct Object o;
    struct Object* next;
    char cargo[10];
    //~Elem() {
//        cout << "delete Object " << cargo << endl;
//        cshared_release_atomic(&next);

};

void elem_destructor(struct Elem *elem) {
    printf("delete object %s\n",elem->cargo);
}

struct VMT elemVMT = {(void (*)(struct Object *)) elem_destructor, 1, {sizeof(struct Object)}};

void constructor(struct Elem *elem) {
    elem->o.use_count = 0;
    elem->o.vmt = &elemVMT;
}

void constructor(struct Elem *pElem1);

struct Object* make3nonCyclic() {
    struct Elem *elem = malloc(sizeof(struct Elem));
    constructor(elem);
    strcpy(elem->cargo, "A");
    struct Object* A;
    cshared_init_elem(&A, (struct Object*)elem);

    elem = malloc(sizeof(struct Elem));
    constructor(elem);
    strcpy(elem->cargo, "B");
    struct Object* B;
    cshared_init_elem(&B, (struct Object*)elem);

    elem = malloc(sizeof(struct Elem));
    constructor(elem);
    strcpy(elem->cargo, "C");
    struct Object* C;
    cshared_init_elem(&C, (struct Object*)elem);

    struct Object* p = A;
    cshared_assign(&((struct Elem *) p)->next, &B);
    p = B;
    cshared_assign(&((struct Elem *) p)->next, &C);
    cshared_release_atomic(&B);
    cshared_release_atomic(&C);
    return A;
}


struct Object* makeLongList() {
    struct Elem *elem = malloc(sizeof(struct Elem));
    constructor(elem);
    strcpy(elem->cargo, "A");
    struct Object* A;
    cshared_init_elem(&A, (struct Object*)elem);

    elem = malloc(sizeof(struct Elem));
    constructor(elem);
    strcpy(elem->cargo, "B");
    struct Object* B;
    cshared_init_elem(&B, (struct Object*)elem);

    elem = malloc(sizeof(struct Elem));
    constructor(elem);
    strcpy(elem->cargo, "C");
    struct Object* C;
    cshared_init_elem(&C, (struct Object*)elem);

    struct Object* p = A;
    cshared_assign(&((struct Elem *) p)->next, &B);
    p = B;
    cshared_assign(&((struct Elem *) p)->next, &C);
    cshared_release_atomic(&B);
    cshared_release_atomic(&C);
    return A;
}

static void test2() {
    //non cyclic list of three
    struct Object* head = make3nonCyclic();
    struct Object*p = head;
    cshared_assign(&head, &((struct Elem *) p)->next);
    printf("elem A should be deleted\n");
    cshared_release_atomic(&head);
}

int main() {
    test2();
    return 0;
}

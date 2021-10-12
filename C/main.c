#include <malloc.h>
#include <string.h>
#include "CShared.h"

struct Elem1 {
    struct Object o;
    struct Object* next;
    char cargo[10];
    //~Elem1() {
//        cout << "delete Object " << cargo << endl;
//        cshared_release_atomic(&next);

};

struct Object* make3nonCyclic() {
    struct Elem1 *elem = malloc(sizeof(struct Elem1));
    strcpy(elem->cargo, "A");
    struct Object* A;
    cshared_init_elem(&A, (struct Object*)elem);

    elem = malloc(sizeof(struct Elem1));
    strcpy(elem->cargo, "B");
    struct Object* B;
    cshared_init_elem(&B, (struct Object*)elem);

    elem = malloc(sizeof(struct Elem1));
    strcpy(elem->cargo, "C");
    struct Object* C;
    cshared_init_elem(&C, (struct Object*)elem);

    struct Object* p = A;
    cshared_assign(&((struct Elem1 *) p)->next, &B);
    p = B;
    cshared_assign(&((struct Elem1 *) p)->next, &C);
    cshared_release_atomic(&B);
    cshared_release_atomic(&C);
    return A;
}

static void test2() {
    //non cyclic list of three
    struct Object* head = make3nonCyclic();
    struct Object*p = head;
    cshared_assign(&head, &((struct Elem1 *) p)->next);
    printf("elem A should be deleted\n");
    cshared_release_atomic(&head);
}

int main() {
    test2();
    return 0;
}

#include <iostream>
#include "LShared.h"

using namespace std;

struct Elem1: public Object {
    Object* next;
    string cargo;
    Elem1(string cargo) {
        this->cargo = cargo;
    }
    ~Elem1() {
        cout << "delete Object " << cargo << endl;
        lshared_release_atomic(&next);
    }
};

static Object* make3nonCyclic() {
    Elem1 *elem = new Elem1("A");
    Object* A;
    lshared_init_elem(&A, elem);
    elem = new Elem1("B");
    Object* B;
    lshared_init_elem(&B, elem);
    elem = new Elem1("C");
    Object* C;
    lshared_init_elem(&C, elem);
    Object* p = A;
    lshared_assign(&((Elem1 *) p)->next, &B);
    p = B;
    lshared_assign(&((Elem1 *) p)->next, &C);
    lshared_release_atomic(&B);
    lshared_release_atomic(&C);
    return A;
}

static void test2() {
    //non cyclic list of three
    Object* head = make3nonCyclic();
    Object*p = head;
    lshared_assign(&head, &((Elem1 *) p)->next);
    cout << "elem A should be deleted" << endl;
    lshared_release_atomic(&head);
}

int main() {
    test2();
    return 0;
}

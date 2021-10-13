#include <iostream>
#include "LShared.h"

using namespace std;

struct Elem1: public Object {
    Elem1* next;
    string cargo;
    Elem1(string cargo) {
        this->cargo = cargo;
    }
    ~Elem1() {
        cout << "delete Elem1 " << cargo << endl;
        lshared_release_atomic((Object**)&next);
    }
};

static Elem1* make3nonCyclic() {
    Elem1 *elem = new Elem1("A");
    Elem1* A;
    lshared_init_elem((Object**)&A, elem);
    elem = new Elem1("B");
    Elem1* B;
    lshared_init_elem((Object**)&B, elem);
    elem = new Elem1("C");
    Elem1* C;
    lshared_init_elem((Object**)&C, elem);
    lshared_assign((Object**)&A->next, (Object**)&B);
    lshared_assign((Object**)&B->next, (Object**)&C);
    lshared_release_atomic((Object**)&B);
    lshared_release_atomic((Object**)&C);
    return A;
}

static void test2() {
    //non cyclic list of three
    Elem1* head = make3nonCyclic();
    lshared_assign((Object**)&head, (Object**)&head->next);
    cout << "elem A should be deleted" << endl;
    lshared_release_atomic((Object**)&head);
}

int main() {
    test2();
    return 0;
}

#include <iostream>
#include "LShared.h"
#include "tree.h"
#include "weak.h"

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

static Elem1* make3cyclic() {
    Elem1 *A = new Elem1("A");
    lshared_init_elem(A);
    Elem1* B = new Elem1("B");
    lshared_init_elem(B);
    Elem1* C = new Elem1("C");
    lshared_init_elem(C);
    lshared_assign((Object**)&A->next, B);
    lshared_assign((Object**)&B->next, C);
    lshared_assign((Object**)&C->next, A);
    lshared_release_atomic((Object**)&B);
    lshared_release_atomic((Object**)&C);
    return A;
}

static Elem1* make3nonCyclic() {
    Elem1 *A = new Elem1("A");
    lshared_init_elem(A);
    Elem1* B = new Elem1("B");
    lshared_init_elem(B);
    Elem1* C = new Elem1("C");
    lshared_init_elem(C);
    lshared_assign((Object**)&A->next, B);
    lshared_assign((Object**)&B->next, C);
    lshared_release_atomic((Object**)&B);
    lshared_release_atomic((Object**)&C);
    return A;
}

static void test1() {
    //cyclic list of three
    Elem1* head = make3cyclic();
    lshared_assign((Object**)&head, head->next);
    lshared_release_atomic((Object**)&head);
}

static void test2() {
    //non cyclic list of three
    Elem1* head = make3nonCyclic();
    lshared_assign((Object**)&head, head->next);
    cout << "elem A should be deleted" << endl;
    lshared_release_atomic((Object**)&head);
}

int main() {
    test1();
    //test2();
    //testTree();
    //testweak();
    return 0;
}

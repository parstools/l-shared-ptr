//
// Created by andrzej on 10/13/21.
//
#include "weak.h"
#include <iostream>
using namespace std;

Apartment::~Apartment() {
    cout << "delete Apartment"<< endl;
    //lshared_release_atomic((Object**)&parent);weak!
    lshared_release_atomic((Object**)&person);
}

Person::~Person() {
    cout << "delete Person"<< endl;
    //lshared_release_atomic((Object**)&parent);weak!
    lshared_release_atomic((Object**)&apartment);
}



void testweak() {
    Person* person = new Person();
    lshared_init_elem(person);
    Apartment *apartment = new Apartment();
    lshared_init_elem(apartment);
    person->apartment = apartment;
    lshared_assign((Object**)&person->apartment, apartment);
    apartment->person = person;
    lshared_assign((Object**)&apartment->person, person);
    lshared_release_atomic((Object**)&person);
    lshared_release_atomic((Object**)&apartment);
}
#include <iostream>
#include <cstring>
#include "../inc/survivor.h"
using namespace std;

void Survivor::init(const char* newName, int newAge)
{
    strncpy(name,newName,19);
    name[19] = '\0';

    age = newAge;
}

void Survivor::print()
{
    cout << "Survivor Name: " << name << endl;
    cout << "Survivor Age: " << age << endl;
}
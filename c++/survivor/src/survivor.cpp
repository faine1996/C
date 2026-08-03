#include <iostream>
#include <cstring>
#include "../inc/survivor.h"
using namespace std;

void Survivor::init(const char* newName, int newAge, MaritalStatus newStatus)
{
    strncpy(name,newName,19);
    name[19] = '\0';
    age = newAge;
    status = newStatus;
}

void Survivor::print() const
{
    cout << "Survivor Name: " << name << endl;
    cout << "Survivor Age: " << age << endl;

    cout << "Marital Status: ";
    switch (status)
    {
        case MaritalStatus::SINGLE:
            cout << "Single" << endl;
            break;
        case MaritalStatus::MARRIED:
            cout << "Married" << endl;
            break;
        case MaritalStatus::RELATIONSHIP:
            cout << "In a Relationship" << endl;
            break;
    }
}
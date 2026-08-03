#include <iostream>
#include <cstring>
#include "../inc/survivor.h"
using namespace std;

Survivor::Survivor(const char* newName, int newAge, MaritalStatus newStatus)
{
    name = new char[strlen(newName) + 1];
    strcpy(name, newName);
    age = newAge;
    status = newStatus;
}

Survivor::Survivor(const Survivor& other)
{
    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);
    age = other.age;
    status = other.status;
}

Survivor::~Survivor()
{
    delete[] name;
}

void Survivor::setName(const char* newName)
{
    delete[] name;
    name = new char[strlen(newName) + 1];
    strcpy(name, newName);
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
#include <iostream>
#include <cstring>
#include "../inc/survivor.h"
using namespace std;

int Survivor::nextSerialNumber = 1;

Survivor::Survivor(const char* newName, int newAge, MaritalStatus newStatus, const Bandana& newBandana)
    : bandana(newBandana), serialNumber(nextSerialNumber++)
{
    name = new char[strlen(newName) + 1];
    strcpy(name, newName);
    age = newAge;
    status = newStatus;
    tribe = nullptr;
}

Survivor::Survivor(const Survivor& other)
    : bandana(other.bandana), serialNumber(other.serialNumber)
{
    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);
    age = other.age;
    status = other.status;
    tribe = other.tribe;
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
    cout << "Serial Number: " << serialNumber << endl;
    cout << "Bandana Color: " << bandana.getColor() << " (" << bandana.getLength() << " cm)" << endl;

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
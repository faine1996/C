#include "../inc/tribe.h"
#include <iostream>

using namespace std;

void Tribe::init(int capacity)
{
    maxCapacity = capacity;
    currentCount = 0;

    members = new Survivor*[maxCapacity];

    for (int i = 0; i < maxCapacity; ++i)
    {
        members[i] = nullptr;
    }
}

bool Tribe::addSurvivor(Survivor* s)
{
    if (currentCount < maxCapacity)
    {
        members[currentCount] = s;
        currentCount++;
        return true;
    }
    return false;
}

void Tribe::print()
{
    for (int i = 0; i < currentCount; ++i)
    {
        members[i]->print();
        cout << "------------------" << endl;
    }
}

void Tribe::freeMemory()
{
    for (int i = 0; i < currentCount; ++i)
    {
        delete members[i];
    }
    delete[] members;
}

int Tribe::getCurrentCount() 
{
    return currentCount;
}

int Tribe::getMaxCapacity() 
{
    return maxCapacity;
}


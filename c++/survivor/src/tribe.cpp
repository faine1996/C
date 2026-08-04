#include "../inc/tribe.h"
#include <iostream>
#include <cstring>

using namespace std;

Tribe::Tribe(const char* tribeName, int capacity)
{
    name = new char[strlen(tribeName) + 1];
    strcpy(name, tribeName);

    maxCapacity = capacity;
    currentCount = 0;

    members = new Survivor*[maxCapacity];

    for (int i = 0; i < maxCapacity; ++i)
    {
        members[i] = nullptr;
    }
}

Tribe::Tribe(const Tribe& other)
{
    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);

    maxCapacity = other.maxCapacity;
    currentCount = other.currentCount;

    members = new Survivor*[maxCapacity];

    for (int i = 0; i < currentCount; ++i)
    {
        members[i] = new Survivor(*other.members[i]);
        members[i]->setTribe(this);
    }
    for (int i = currentCount; i < maxCapacity; ++i)
    {
        members[i] = nullptr;
    }
}

Tribe::~Tribe()
{
    delete[] name;

    for (int i = 0; i < currentCount; ++i)
    {
        delete members[i];
    }
    delete[] members;
}

bool Tribe::addSurvivor(Survivor* s)
{
    if (currentCount < maxCapacity)
    {
        members[currentCount] = s;
        currentCount++;
        s->setTribe(this);
        return true;
    }
    return false;
}


bool Tribe::eliminateSurvivor(const char* survivorName)
{
    for (int i = 0; i < currentCount; ++i)
    {
        if (0 == strcmp(members[i]->getName(),survivorName))
        {
            delete members[i];

            for (int j = i; j < currentCount - 1; ++j)
            {
                members[j] = members[j+1];
            }

            members[currentCount - 1] = nullptr;
            currentCount--;

            return true;
        }
    }
    return false;
}

void Tribe::print() const
{

    cout << "--- Tribe: " << name << " ---" << endl;

    for (int i = 0; i < currentCount; ++i)
    {
        members[i]->print();
        cout << "------------------" << endl;
    }
}
#include "../inc/tribe.h"
#include <iostream>
#include <cstring>

using namespace std;

void Tribe::init(const char* tribeName,int capacity)
{
    strncpy(name, tribeName, 19);
    name[19] = '\0';
    
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

void Tribe::print()
{

    cout << "--- Tribe: " << name << " ---" << endl;

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


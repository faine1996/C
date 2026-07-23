#include <iostream>
#include "../inc/survivor.h"
using namespace std;

void InitSurvivor(Survivor &s)
{
    cout << "Enter Survivor Name: ";
    cin.getline(s.name,20);

    cout<< "Enter Survivor Age: ";
    cin >> s.age;

    cin.ignore();
}

void printSurvivor(const Survivor &s)
{
    cout << "Survivor Name: " << s.name << endl;
    cout << "Survivor Age: " << s.age << endl;
}

int getTribeSize()
{
    int size;
    cout << "Enter max survivors per tribe: ";
    cin >> size;

    cin.ignore();;

    return size;
}

void InitTribe(Tribe &t, int capacity)
{
    t.maxCapacity = capacity;
    t.currentCount = 0;
    t.members = new Survivor*[capacity];

    for (int i = 0; i < capacity; ++i)
    {
        t.members[i] = nullptr;
    }
}

void populateTribes(Tribe tribes[2])
{
    int choice = -1;

    while(true)
    {
        if (tribes[0].currentCount == tribes[1].maxCapacity && tribes[1].currentCount == tribes[1].maxCapacity)
        {
            cout << "\n Both tribes are full. Ending allocation. " << endl;
            break;
        }

        cout << "0. Stop " << endl;
        cout << "\n1. Add to Tribe 1 (Current 1: " << tribes[0].currentCount << "/" << tribes[1].maxCapacity << ")" <<endl;
        cout << "2. Add to Tribe 2 (Current 2: " << tribes[1].currentCount << "/" << tribes[2].maxCapacity << ")" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            choice = -1; // Force invalid choice
        }
        cin.ignore(10000, '\n');

        if (0 == choice)
        {
            break;
        }
        else if(1 == choice || 2 == choice)
        {
            int tIndex = choice - 1;
            if (tribes[tIndex].currentCount < tribes[tIndex].maxCapacity)
            {
                tribes[tIndex].members[tribes[tIndex].currentCount] = new Survivor;

                InitSurvivor(*(tribes[tIndex].members[tribes[tIndex].currentCount]));

                tribes[tIndex].currentCount++;
            }
            else
            {
                cout << "Error: Tribe " << choice << " is full. " << endl; 
            }
        }

        else
        {
            cout << "Invalid choicce try again." << endl;   
        }
    }
}

void printTribes(const Tribe tribes[2])
{
    for (int t = 0; t < 2; ++t)
    {
        cout << "\n-- Tribe " << (t + 1) << " Survivors ---" << endl;

        for (int i = 0; i < tribes[t].currentCount; ++i)
        {
            printSurvivor(*(tribes[t].members[i]));
        }
    }
}

void freeTribes(Tribe tribes[2])
{
    for (int t = 0; t < 2; ++t)
    {
        for (int i = 0; i < tribes[t].currentCount; ++i)
        {
            delete tribes[t].members[i];
        }
        delete[] tribes[t].members;
    }
}
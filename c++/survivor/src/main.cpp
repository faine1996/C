#include <iostream>
#include "../inc/tribe.h"
using namespace std;

int getTribeSize();
void handleMenu(Tribe tribes[2]);

int main()
{

    int tribeSize = getTribeSize();
    Tribe tribes[2];

    tribes[0].init(tribeSize);
    tribes[1].init(tribeSize);

    handleMenu(tribes);

    cout << "\n--- Tribe 1 ---" << endl;
    tribes[0].print();

    cout << "\n--- Tribe 2 ---" << endl;
    tribes[1].print();
    
    tribes[0].freeMemory();
    tribes[1].freeMemory();

    return 0;
}

int getTribeSize()
{
    int size;

    cout << "Enter max survivors per tribe: ";

    cin >> size;

    cin.ignore();

    return size;
}

void handleMenu(Tribe tribes[2])
{
    int choice = -1;

    while(true)
    {

        cout << "0. Stop " << endl;
        cout << "\n1. Add to Tribe 1 (Current 1: " << tribes[0].getCurrentCount() << "/" << tribes[0].getMaxCapacity() << ")" <<endl;
        cout << "2. Add to Tribe 2 (Current 2: " << tribes[1].getCurrentCount() << "/" << tribes[1].getMaxCapacity() << ")" << endl;
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
        if(1 == choice || 2 == choice)
        {
            int tIndex = choice - 1;
            char tempName[20];
            int tempAge;

            cout << "Enter Survivor Name: ";
            cin.getline(tempName,20);
            cout << "Enter Survivor Age: ";
            cin >> tempAge;
            cin.ignore();

            Survivor* newSurvivor = new Survivor;
            newSurvivor->init(tempName,tempAge);

            if (!tribes[tIndex].addSurvivor(newSurvivor))
            {
                cout << "Error: Tribe " << choice << " is full. " << endl;
                delete newSurvivor;
            }
        }

        else
        {
            cout << "Invalid choice try again." << endl;   
        }
    }

}
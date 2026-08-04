#include <iostream>
#include "../inc/tribe.h"
#include "../inc/bandana.h"
using namespace std;

int getTribeSize();
void handleMenu(Tribe& tribe1, Tribe& tribe2);
void runTribalCouncil(Tribe& tribe1, Tribe& tribe2);

int main()
{

    int tribeSize = getTribeSize();
    Tribe tribe1("Heroes", tribeSize);
    Tribe tribe2("Villains", tribeSize);

    handleMenu(tribe1, tribe2);

    cout << "\n--- Tribes Before Elimination ---" << endl;
    tribe1.print();
    tribe2.print();

    runTribalCouncil(tribe1, tribe2);

    cout << "\n--- Tribes After Elimination ---" << endl;
    tribe1.print();
    tribe2.print();

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

void handleMenu(Tribe& tribe1, Tribe& tribe2)
{
    int choice = -1;

    while(true)
    {

        cout << "0. Stop " << endl;
        cout << "\n1. Add to Tribe 1 (Current 1: " << tribe1.getCurrentCount() << "/" << tribe1.getMaxCapacity() << ")" <<endl;
        cout << "2. Add to Tribe 2 (Current 2: " << tribe2.getCurrentCount() << "/" << tribe2.getMaxCapacity() << ")" << endl;
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
            char tempName[20];
            int tempAge;
            MaritalStatus tempStatus;
            int statusInput;
            char tempColor[BANDANA_COLOR_MAX_LEN];
            int tempBandanaLength;

            cout << "Enter Survivor Name: ";
            cin.getline(tempName,20);
            cout << "Enter Survivor Age: ";
            cin >> tempAge;
            cout << "Enter Marital Status (0=Single, 1=Married, 2=Relationship): ";
            cin >> statusInput;
            tempStatus = static_cast<MaritalStatus>(statusInput);
            cin.ignore();
            cout << "Enter Bandana Color: ";
            cin.getline(tempColor, BANDANA_COLOR_MAX_LEN);
            cout << "Enter Bandana Length (cm): ";
            cin >> tempBandanaLength;
            cin.ignore();

            Bandana tempBandana(tempColor, tempBandanaLength);
            Survivor* newSurvivor = new Survivor(tempName, tempAge, tempStatus, tempBandana);
            Tribe& targetTribe = (1 == choice) ? tribe1 : tribe2;

            if (!targetTribe.addSurvivor(newSurvivor))
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

void runTribalCouncil(Tribe& tribe1, Tribe& tribe2)
{
    cout << "\n--- Tribal Council (Eliminations) ---" << endl;
    for (int i = 0; i < 2; ++i)
    {
        char elimName[20];
        cout << "Enter the name of survivor " << (i + 1) << " to eliminate: ";
        cin.getline(elimName, 20);

        bool eliminated = tribe1.eliminateSurvivor(elimName);
        if (!eliminated)
        {
            eliminated = tribe2.eliminateSurvivor(elimName);
        }

        if (eliminated) 
        {
            cout << ">> " << elimName << " was eliminated!" << endl;
        } 
        else 
        {
            cout << ">> " << elimName << " was not found." << endl;
        }
    }
}
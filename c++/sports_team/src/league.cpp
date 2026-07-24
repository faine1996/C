#include <iostream>
#include <cstring>
#include "../inc/league.h"
using namespace std;

void setupTeamHelper(Team& t, int teamNumber) 
{
    cout << "\n--- Setting up Team " << teamNumber << " ---" << endl;
    
    char tempName[30];
    cout << "Enter Team Name (or press Enter for default): ";
    cin.getline(tempName, 30);

    if (strlen(tempName) > 0) InitTeam(t, tempName);
    else
    {
        InitTeam(t);
    }

}

void runHardCodedTests(Team& t)
{
    addPlayer(t, "Michael Jordan", 1963);
    addPlayer(t, "Luka Angel");
}

void InitLeague(League& l, int capacity)
{
    l.maxTeams = capacity;
    l.numOfTeams = 0; 
    l.allTeams = new Team[l.maxTeams]; 
    
    for (int i = 0; i < l.maxTeams; ++i)
    {
        InitTeam(l.allTeams[i]);
    }
}

void setupLeague(League& l)
{
    for (int i = 0; i < l.maxTeams; ++i)
    {
        setupTeamHelper(l.allTeams[i], i + 1);

        if (i == 0)
        {
            cout << "[System: Adding Hard-coded test players to Team 1...]\n";
            runHardCodedTests(l.allTeams[i]); 
        }

        populateTeam(l.allTeams[i]);
        l.numOfTeams++; // Increment logical count after successful setup
    }
}

void printLeague(const League& l)
{
    cout << "\n================ LEAGUE ROSTER ================\n";
    for (int i = 0; i < l.numOfTeams; ++i)
    {
        printTeam(l.allTeams[i]);
    }
    cout << "===============================================\n";
}

void freeLeague(League& l)
{
    for (int i = 0; i < l.numOfTeams; ++i)
    {
        freeTeam(l.allTeams[i]); 
    }
    delete[] l.allTeams; 
    l.allTeams = nullptr;
    l.maxTeams = 0;
    l.numOfTeams = 0;
}
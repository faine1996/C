#include <iostream>
#include <cstring>
#include "../inc/team.h"
#include "../inc/player.h"
using namespace std;


void InitTeam(Team& t, const char* teamName)
{
    strncpy(t.name, teamName, 29);
    t.name[29] = '\0';
    t.numOfPlayers = 0;

    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        t.allPlayers[i] = nullptr;
    }
}

bool addPlayer(Team& t, const char* playerName, int birthYear)
{
    if (t.numOfPlayers >= MAX_PLAYERS)
    {
        return false;
    }

    int index = t.numOfPlayers;
    t.allPlayers[index] = new Player;

    strncpy(t.allPlayers[index]->name, playerName, 19);
    t.allPlayers[index]->name[19] = '\0';
    t.allPlayers[index]->birthYear = birthYear;

    t.numOfPlayers++;

    return true;
}

void printTeam(const Team& t)
{
    cout << "\n--- Team: " << t.name << " ---" << endl;
    cout << "Current number of players: " << t.numOfPlayers << "/" << MAX_PLAYERS << endl;

    for (int i = 0; i < t.numOfPlayers; ++i)
    {
        printPlayer(*(t.allPlayers[i]));
    }
}

void populateTeam(Team& t)
{
    char answer;

    do
    {
        cout << "Add a player interactively to " << t.name << "? (y/n): ";
        cin >> answer;
        cin.ignore(10000, '\n');

        if ('y' == answer)
        {
            Player tempPlayer;
            initPlayer(tempPlayer);

            if (!addPlayer(t,tempPlayer.name, tempPlayer.birthYear))
            {
                cout << "Team is full! Cannot add more players. " << endl;
                break;
            }

        }
    }
    while('y' == answer && t.numOfPlayers < MAX_PLAYERS);
}

void freeTeam(Team& t)
{
    for (int i = 0; i < t.numOfPlayers; ++i)
    {
        delete t.allPlayers[i];
        t.allPlayers[i] = nullptr;
    }
    t.numOfPlayers = 0;
}
#ifndef TEAM_H
#define TEAM_H

#include "player.h"

const int MAX_PLAYERS = 11;

struct Team
{
    char name[30];
    Player* allPlayers[MAX_PLAYERS];
    int numOfPlayers;
};

void InitTeam(Team& t, const char* teamName = "Default Team");

bool addPlayer(Team& t, const char* playerName, int birthYear = 1999);

void printTeam(const Team& t);

void populateTeam(Team& t);

void freeTeam(Team& t);


#endif // TEAM_H
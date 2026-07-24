#ifndef LEAGUE_H
#define LEAGUE_H

#include "team.h"

struct League 
{
    int maxTeams;
    Team* allTeams;
    int numOfTeams;
};

void InitLeague(League& l, int capacity);

void setupLeague(League& l);

void printLeague(const League& l);

void freeLeague(League& l);

#endif
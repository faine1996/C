#ifndef PLAYER_H
#define PLAYER_H

struct Player
{
    char name[20];
    int birthYear;
};

void initPlayer(Player& player);

void printPlayer(const Player& player);

#endif // PLAYER_H
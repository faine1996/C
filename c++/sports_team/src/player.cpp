#include <iostream>
#include <cstring>
#include "../inc/player.h"
using namespace std;

void initPlayer(Player& player)
{
    cout << "Enter player name: ";
    cin.getline(player.name,20);

    cout << "Enter player birth year: ";
    cin >> player.birthYear;

    if (cin.fail())
    {
        cin.clear();
        player.birthYear = 0;

    }
    cin.ignore(10000, '\n');
}

void printPlayer(const Player& player)
{
    cout << "  Name: " << player.name << ", Born: " << player.birthYear << endl;
}
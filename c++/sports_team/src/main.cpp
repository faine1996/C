#include <iostream>
#include "../inc/league.h"
using namespace std;

int main()
{
    int numTeams;
    cout << "Enter number of teams: ";
    cin >> numTeams;
    cin.ignore(10000, '\n');

    League myLeague;

    InitLeague(myLeague, numTeams);
    setupLeague(myLeague);
    printLeague(myLeague);
    freeLeague(myLeague);

    return 0;
}
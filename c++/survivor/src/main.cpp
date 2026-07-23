#include "../inc/survivor.h"

int main()
{
    int tribeSize = getTribeSize();

    Tribe tribes[2];

    InitTribe(tribes[0], tribeSize);
    InitTribe(tribes[1], tribeSize);

    populateTribes(tribes);
    printTribes(tribes);
    freeTribes(tribes);

    return 0;
}
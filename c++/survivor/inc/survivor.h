#ifndef SURVIVOR_H
#define SURVIVOR_H

struct Survivor
{
    char name[20];
    int age;
};

struct Tribe
{
    int maxCapacity;
    int currentCount;
    Survivor** members;
};

void InitSurvivor(Survivor &s);
void printSurvivor(const Survivor &s);
void InitTribe(Tribe &t, int capacity);
int getTribeSize();
void populateTribes(Tribe tribes[2]);
void printTribes(const Tribe tribes[2]);
void freeTribes(Tribe tribes[2]);



#endif
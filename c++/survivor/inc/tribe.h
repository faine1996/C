#ifndef TRIBE_H
#define TRIBE_H

#include "./survivor.h"

class Tribe
{
    private:
        char name[20];
        int maxCapacity;
        int currentCount;
        Survivor** members;

    public:
        void init(const char* tribeName,int capacity);
        bool addSurvivor(Survivor*);
        bool eliminateSurvivor(const char* survivorName);
        void print();
        void freeMemory();
        int getCurrentCount();
        int getMaxCapacity();
};

#endif //TRIBE_H

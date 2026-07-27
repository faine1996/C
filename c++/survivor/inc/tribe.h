#ifndef TRIBE_H
#define TRIBE_H

#include "./survivor.h"

class Tribe
{
    private:

        int maxCapacity;
        int currentCount;
        Survivor** members;

    public:
        void init(int capacity);
        bool addSurvivor(Survivor*);
        void print();
        void freeMemory();
        int getCurrentCount();
        int getMaxCapacity();
};

#endif //TRIBE_H

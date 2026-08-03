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
        void print() const;
        void freeMemory();
        inline int getCurrentCount() const { return currentCount; };
        inline int getMaxCapacity() const { return maxCapacity; };
};

#endif //TRIBE_H

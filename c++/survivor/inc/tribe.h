#ifndef TRIBE_H
#define TRIBE_H

#include "./survivor.h"

class Tribe
{
    private:
        char* name;
        int maxCapacity;
        int currentCount;
        Survivor** members;

    public:
        Tribe(const char* tribeName, int capacity);
        Tribe(const Tribe& other);
        ~Tribe();

        bool addSurvivor(Survivor*);
        bool eliminateSurvivor(const char* survivorName);
        void print() const;
        inline int getCurrentCount() const { return currentCount; };
        inline int getMaxCapacity() const { return maxCapacity; };
};

#endif //TRIBE_H


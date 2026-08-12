#ifndef STREETCAT_H
#define STREETCAT_H

#include "Cat.h"

class StreetCat : virtual public Cat
{
public:
    StreetCat(const char* name, const char* color, int age, double weight, int fightCount);

    virtual ~StreetCat() = default;

    friend std::ostream& operator<<(std::ostream& os, const StreetCat& cat);

protected:
    int fightCount;
};

#endif
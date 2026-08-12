#ifndef SIAMICAT_H
#define SIAMICAT_H

#include "Cat.h"

enum class FavoriteFood
{
    Fish,
    Chicken,
    Milk
};

class SiamiCat : virtual public Cat
{
public:
    SiamiCat(const char* name, const char* color, int age, double weight, FavoriteFood food);

    virtual ~SiamiCat() = default;

    friend std::ostream& operator<<(std::ostream& os, const SiamiCat& cat);

protected:
    FavoriteFood food;

};

const char* toStringFood(FavoriteFood food);

#endif
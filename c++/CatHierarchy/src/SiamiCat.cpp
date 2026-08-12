#include "SiamiCat.h"

const char* toStringFood(FavoriteFood food)
{
    switch (food)
    {
        case FavoriteFood::Fish:
            return "Fish";
        case FavoriteFood::Chicken:
            return "Chicken";
        case FavoriteFood::Milk:
            return "Milk";
    }
    return "Unknown";
}

SiamiCat::SiamiCat(const char* name, const char* color, int age, double weight, FavoriteFood food)
    : Cat(name, color, age, weight), food(food)
{
}

std::ostream& operator<<(std::ostream& os, const SiamiCat& cat)
{
    os << static_cast<const Cat&>(cat) << ", Favorite Food: " << toStringFood(cat.food);
    return os;
}
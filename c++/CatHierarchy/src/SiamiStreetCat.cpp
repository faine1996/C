#include "SiamiStreetCat.h"

SiamiStreetCat::SiamiStreetCat(const char* name, const char* color, int age, double weight,
                                int fightCount, FavoriteFood food)
    : Cat(name, color, age, weight),
      StreetCat(name, color, age, weight, fightCount),
      SiamiCat(name, color, age, weight, food)
{
}

std::ostream& operator<<(std::ostream& os, const SiamiStreetCat& cat)
{
    os << static_cast<const Cat&>(cat)
       << ", Fights: " << cat.fightCount
       << ", Favorite Food: " << toStringFood(cat.food);
    return os;
}
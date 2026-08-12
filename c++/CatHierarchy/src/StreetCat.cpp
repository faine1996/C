#include "StreetCat.h"

StreetCat::StreetCat(const char* name, const char* color, int age, double weight, int fightCount)
    : Cat(name, color, age, weight), fightCount(fightCount)
{
}

std::ostream& operator<<(std::ostream& os, const StreetCat& cat)
{
    os << static_cast<const Cat&>(cat) << ", Fights: " << cat.fightCount;
    return os;
}
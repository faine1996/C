#ifndef SIAMISTREETCAT_H
#define SIAMISTREETCAT_H

#include "StreetCat.h"
#include "SiamiCat.h"

class SiamiStreetCat : public StreetCat, public SiamiCat
{
public:
    SiamiStreetCat(const char* name, const char* color, int age, double weight,
                    int fightCount, FavoriteFood food);

    ~SiamiStreetCat() override = default;

    friend std::ostream& operator<<(std::ostream& os, const SiamiStreetCat& cat);
};

#endif
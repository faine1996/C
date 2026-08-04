#ifndef BANDANA_H
#define BANDANA_H

#define BANDANA_COLOR_MAX_LEN 20

class Bandana
{
private:
    char color[BANDANA_COLOR_MAX_LEN];
    int length;

public:
    Bandana(const char* newColor, int newLength);
    Bandana(const Bandana& other);

    inline const char* getColor() const { return color; };
    inline int getLength() const { return length; };
};

#endif
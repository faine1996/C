#ifndef SURVIVOR_H
#define SURVIVOR_H

#include "./bandana.h"

class Tribe;

enum class MaritalStatus
{
    SINGLE,
    MARRIED,
    RELATIONSHIP
};

class Survivor
{
private:
    static int nextSerialNumber;

    char* name;
    int age;
    MaritalStatus status;
    Bandana bandana;
    const int serialNumber;
    Tribe* tribe;

public:
    Survivor(const char* newName, int newAge, MaritalStatus newStatus, const Bandana& newBandana);
    Survivor(const Survivor& other);
    ~Survivor();

    void setName(const char* newName);
    void print() const;
    inline const char* getName() const { return name; };
    inline int getSerialNumber() const { return serialNumber; };
    inline Tribe* getTribe() const { return tribe; };
    inline void setTribe(Tribe* newTribe) { tribe = newTribe; };
};

#endif
#ifndef SURVIVOR_H
#define SURVIVOR_H

enum class MaritalStatus
{
    SINGLE,
    MARRIED,
    RELATIONSHIP
};

class Survivor
{
private:
    char name[20];
    int age;
    MaritalStatus status;

public:
    void init(const char* newName, int newAge, MaritalStatus newStatus);    
    void print() const;
    inline const char* getName() const { return name; };

    
};

#endif
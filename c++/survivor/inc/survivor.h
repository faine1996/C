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
    char* name;
    int age;
    MaritalStatus status;

public:
    Survivor(const char* newName, int newAge, MaritalStatus newStatus);
    Survivor(const Survivor& other);
    ~Survivor();

    void setName(const char* newName);
    void print() const;
    inline const char* getName() const { return name; };
};

#endif
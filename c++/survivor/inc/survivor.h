#ifndef SURVIVOR_H
#define SURVIVOR_H

class Survivor
{
private:
    char name[20];
    int age;

public:
    void init(const char* newName, int newAge);    
    void print();
    const char* getName();
};

#endif
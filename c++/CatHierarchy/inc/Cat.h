#ifndef CAT_H
#define CAT_H

#include <iostream>

class Cat
{
public:
    Cat(const char* name, const char* color, int age, double weight);

    Cat(const Cat& other);
    Cat& operator=(const Cat& other);

    Cat(Cat&& other) noexcept;
    Cat& operator=(Cat&& other) noexcept;

    virtual ~Cat();

    friend std::ostream& operator<<(std::ostream& os, const Cat& cat);

private:
    char* name;
    char* color;
    int age;
    double weight;
    
};

#endif
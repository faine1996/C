#include "Cat.h"
#include <cstring>

namespace
{
    char* allocateAndCopy(const char* source)
    {
        char* buffer = new char[std::strlen(source) + 1];
        std::strcpy(buffer, source);
        return buffer;
    }
}

Cat::Cat(const char* name, const char* color, int age, double weight)
    : name(allocateAndCopy(name)),
      color(allocateAndCopy(color)),
      age(age),
      weight(weight)
{
}

Cat::Cat(const Cat& other)
    : name(nullptr), color(nullptr), age(0), weight(0.0)
{
    *this = other;
}

Cat& Cat::operator=(const Cat& other)
{
    if (this != &other)
    {
        delete[] name;
        delete[] color;

        name = allocateAndCopy(other.name);
        color = allocateAndCopy(other.color);
        age = other.age;
        weight = other.weight;
    }
    return *this;
}

Cat::Cat(Cat&& other) noexcept
    : name(other.name), color(other.color), age(other.age), weight(other.weight)
{
    other.name = nullptr;
    other.color = nullptr;
    other.age = 0;
    other.weight = 0.0;
}

Cat& Cat::operator=(Cat&& other) noexcept
{
    if (this != &other)
    {
        delete[] name;
        delete[] color;

        name = other.name;
        color = other.color;
        age = other.age;
        weight = other.weight;

        other.name = nullptr;
        other.color = nullptr;
        other.age = 0;
        other.weight = 0.0;
    }
    return *this;
}

Cat::~Cat()
{
    delete[] name;
    delete[] color;
}

std::ostream& operator<<(std::ostream& os, const Cat& cat)
{
    os << "Name: " << cat.name
       << ", Color: " << cat.color
       << ", Age: " << cat.age
       << ", Weight: " << cat.weight;
    return os;
}
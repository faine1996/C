#ifndef SET_H
#define SET_H

#include <iostream>

class Set
{
public:
    enum class AddResult
    {
        Added,
        AlreadyExists,
        SetFull
    };

    class SetRef
    {
    public:
        SetRef(Set &owner, int index);
        SetRef &operator=(int value);
        operator int() const;

    private:
        Set &owner;
        int index;
    };

    explicit Set(int maxMembers = 1000);
    Set(const Set &other);
    Set &operator=(const Set &other);
    Set(Set &&other) noexcept;
    Set &operator=(Set &&other) noexcept;
    ~Set();

    Set &operator+=(int value);
    Set operator+(int value) const;
    Set operator+(const Set &other) const;

    bool operator>(const Set &other) const;
    bool operator==(const Set &other) const;
    bool operator!=(const Set &other) const;

    int operator[](int index) const;
    SetRef operator[](int index);

    AddResult tryAdd(int value);
    bool contains(int value) const;
    AddResult getLastAddResult() const;

    friend std::ostream &operator<<(std::ostream &os, const Set &s);

private:
    int *arr;
    int size;
    int capacity;
    AddResult lastAddResult;
};

#endif
#include "set.h"
#include <cassert>

Set::Set(int maxMembers)
{
    capacity = maxMembers;
    size = 0;
    arr = new int[capacity];
    lastAddResult = AddResult::Added;
}

Set::Set(const Set &other)
{
    capacity = other.capacity;
    size = other.size;
    arr = new int[capacity];
    lastAddResult = other.lastAddResult;

    for (int i = 0; i < size; i++)
    {
        arr[i] = other.arr[i];
    }
}

Set &Set::operator=(const Set &other)
{
    if (this != &other)
    {
        delete[] arr;

        capacity = other.capacity;
        size = other.size;
        arr = new int[capacity];
        lastAddResult = other.lastAddResult;

        for (int i = 0; i < size; i++)
        {
            arr[i] = other.arr[i];
        }
    }

    return *this;
}

Set::Set(Set &&other) noexcept
{
    capacity = other.capacity;
    size = other.size;
    arr = other.arr;
    lastAddResult = other.lastAddResult;

    other.arr = nullptr;
    other.capacity = 0;
    other.size = 0;
}

Set &Set::operator=(Set &&other) noexcept
{
    if (this != &other)
    {
        delete[] arr;

        capacity = other.capacity;
        size = other.size;
        arr = other.arr;
        lastAddResult = other.lastAddResult;

        other.arr = nullptr;
        other.capacity = 0;
        other.size = 0;
    }

    return *this;
}

Set::~Set()
{
    delete[] arr;
}

bool Set::contains(int value) const
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == value)
        {
            return true;
        }
    }

    return false;
}

Set::AddResult Set::tryAdd(int value)
{
    if (contains(value))
    {
        lastAddResult = AddResult::AlreadyExists;
        return lastAddResult;
    }

    if (size == capacity)
    {
        lastAddResult = AddResult::SetFull;
        return lastAddResult;
    }

    arr[size] = value;
    size++;
    lastAddResult = AddResult::Added;
    return lastAddResult;
}

Set::AddResult Set::getLastAddResult() const
{
    return lastAddResult;
}

Set &Set::operator+=(int value)
{
    tryAdd(value);
    return *this;
}

Set Set::operator+(int value) const
{
    Set result(*this);
    result += value;
    return result;
}

Set Set::operator+(const Set &other) const
{
    Set result(*this);

    for (int i = 0; i < other.size; i++)
    {
        result += other.arr[i];
    }

    return result;
}

bool Set::operator>(const Set &other) const
{
    return size > other.size;
}

bool Set::operator==(const Set &other) const
{
    if (size != other.size)
    {
        return false;
    }

    for (int i = 0; i < size; i++)
    {
        if (!other.contains(arr[i]))
        {
            return false;
        }
    }

    return true;
}

bool Set::operator!=(const Set &other) const
{
    return !(*this == other);
}

int Set::operator[](int index) const
{
    assert(index >= 0 && index < size);
    return arr[index];
}

Set::SetRef Set::operator[](int index)
{
    assert(index >= 0 && index < size);
    return SetRef(*this, index);
}

Set::SetRef::SetRef(Set &owner, int index)
    : owner(owner), index(index)
{
}

Set::SetRef &Set::SetRef::operator=(int value)
{
    if (!owner.contains(value))
    {
        owner.arr[index] = value;
    }

    return *this;
}

Set::SetRef::operator int() const
{
    return owner.arr[index];
}

std::ostream &operator<<(std::ostream &os, const Set &s)
{
    os << "{";

    for (int i = 0; i < s.size; i++)
    {
        os << s.arr[i];

        if (i < s.size - 1)
        {
            os << ", ";
        }
    }

    os << "}";

    return os;
}
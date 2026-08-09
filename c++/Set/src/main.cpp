#include "set.h"
#include <iostream>
#include <utility>

static void printAddResult(int value, Set::AddResult result)
{
    if (result == Set::AddResult::Added)
    {
        std::cout << "Added " << value << std::endl;
    }
    else if (result == Set::AddResult::AlreadyExists)
    {
        std::cout << "Value " << value << " already in set, not added" << std::endl;
    }
    else
    {
        std::cout << "Set is full, cannot add " << value << std::endl;
    }
}

static void testConstructionAndAdd()
{
    std::cout << "--- testConstructionAndAdd ---" << std::endl;

    Set a(3);

    printAddResult(1, a.tryAdd(1));
    printAddResult(2, a.tryAdd(2));
    printAddResult(1, a.tryAdd(1));
    printAddResult(3, a.tryAdd(3));
    printAddResult(4, a.tryAdd(4));

    std::cout << "a = " << a << std::endl;
}

static void testOperatorPlusEquals()
{
    std::cout << "--- testOperatorPlusEquals ---" << std::endl;

    Set a(2);

    a += 1;
    printAddResult(1, a.getLastAddResult());

    a += 1;
    printAddResult(1, a.getLastAddResult());

    a += 2;
    printAddResult(2, a.getLastAddResult());

    a += 3;
    printAddResult(3, a.getLastAddResult());

    std::cout << "a = " << a << std::endl;
}

static void testCopyConstructor()
{
    std::cout << "--- testCopyConstructor ---" << std::endl;

    Set a(5);
    a.tryAdd(10);
    a.tryAdd(20);

    Set b(a);
    b.tryAdd(30);

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
}

static void testCopyAssignment()
{
    std::cout << "--- testCopyAssignment ---" << std::endl;

    Set a(5);
    a.tryAdd(1);
    a.tryAdd(2);

    Set b(5);
    b.tryAdd(99);

    b = a;
    b.tryAdd(3);

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

    a = a;
    std::cout << "after self-assignment, a = " << a << std::endl;
}

static Set makeSet()
{
    Set temp(5);
    temp.tryAdd(7);
    temp.tryAdd(8);
    return temp;
}

static void testMoveConstructor()
{
    std::cout << "--- testMoveConstructor ---" << std::endl;

    Set a(makeSet());

    std::cout << "a = " << a << std::endl;
}

static void testMoveAssignment()
{
    std::cout << "--- testMoveAssignment ---" << std::endl;

    Set a(5);
    a.tryAdd(1);

    Set b(5);
    b.tryAdd(2);
    b.tryAdd(3);

    a = std::move(b);

    std::cout << "a = " << a << std::endl;
}

static void testOperatorPlusInt()
{
    std::cout << "--- testOperatorPlusInt ---" << std::endl;

    Set a(5);
    a.tryAdd(1);
    a.tryAdd(2);

    Set b = a + 3;

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
}

static void testOperatorPlusSet()
{
    std::cout << "--- testOperatorPlusSet ---" << std::endl;

    Set a(5);
    a.tryAdd(1);
    a.tryAdd(2);

    Set b(5);
    b.tryAdd(2);
    b.tryAdd(3);

    Set c = a + b;

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = " << c << std::endl;
}

static void testComparisons()
{
    std::cout << "--- testComparisons ---" << std::endl;

    Set a(5);
    a.tryAdd(1);
    a.tryAdd(2);

    Set b(5);
    b.tryAdd(2);
    b.tryAdd(1);

    Set c(5);
    c.tryAdd(1);

    std::cout << "a == b: " << (a == b) << std::endl;
    std::cout << "a != c: " << (a != c) << std::endl;
    std::cout << "a > c: " << (a > c) << std::endl;
    std::cout << "c > a: " << (c > a) << std::endl;
}

static void testIndexRead()
{
    std::cout << "--- testIndexRead ---" << std::endl;

    Set a(5);
    a.tryAdd(10);
    a.tryAdd(20);

    const Set &constRef = a;

    std::cout << "a[0] = " << a[0] << std::endl;
    std::cout << "constRef[1] = " << constRef[1] << std::endl;
}

static void testIndexWrite()
{
    std::cout << "--- testIndexWrite ---" << std::endl;

    Set a(5);
    a.tryAdd(10);
    a.tryAdd(20);

    std::cout << "before: " << a << std::endl;

    if (a.contains(20))
    {
        std::cout << "cannot write 20 into a[0], already present" << std::endl;
    }
    else
    {
        a[0] = 20;
    }

    a[0] = 99;

    std::cout << "after: " << a << std::endl;
}

int main()
{
    testConstructionAndAdd();
    testOperatorPlusEquals();
    testCopyConstructor();
    testCopyAssignment();
    testMoveConstructor();
    testMoveAssignment();
    testOperatorPlusInt();
    testOperatorPlusSet();
    testComparisons();
    testIndexRead();
    testIndexWrite();

    return 0;
}
#include "Cat.h"
#include "StreetCat.h"
#include "SiamiCat.h"
#include "SiamiStreetCat.h"
#include <iostream>

void printCatByValue(Cat cat)
{
    std::cout << "Inside printCatByValue (received by value): " << cat << std::endl;
}

void demonstrateCat()
{
    std::cout << "--- Cat scope start ---" << std::endl;
    Cat original("Whiskers", "Black", 3, 4.2);
    std::cout << "Original: " << original << std::endl;

    std::cout << "Calling printCatByValue(original) -- expect copy ctor to fire:" << std::endl;
    printCatByValue(original);

    std::cout << "Original still intact after the call: " << original << std::endl;
    std::cout << "--- Cat scope end ---" << std::endl;
}

void demonstrateStreetCat()
{
    std::cout << "--- StreetCat scope start ---" << std::endl;
    StreetCat street("Scrapper", "Gray", 5, 5.1, 12);
    std::cout << street << std::endl;
    std::cout << "--- StreetCat scope end ---" << std::endl;
}

void demonstrateSiamiCat()
{
    std::cout << "--- SiamiCat scope start ---" << std::endl;
    SiamiCat siami("Empress", "Cream", 2, 3.6, FavoriteFood::Fish);
    std::cout << siami << std::endl;
    std::cout << "--- SiamiCat scope end ---" << std::endl;
}

void demonstrateSiamiStreetCat()
{
    std::cout << "--- SiamiStreetCat scope start ---" << std::endl;
    SiamiStreetCat hybrid("Duke", "Tan", 4, 4.8, 7, FavoriteFood::Chicken);
    std::cout << hybrid << std::endl;

    std::cout << "Copy-constructing a second SiamiStreetCat from hybrid:" << std::endl;
    SiamiStreetCat hybridCopy(hybrid);
    std::cout << hybridCopy << std::endl;

    std::cout << "Move-constructing a third SiamiStreetCat from a temporary:" << std::endl;
    SiamiStreetCat hybridMoved(SiamiStreetCat("Ash", "White", 1, 2.9, 0, FavoriteFood::Milk));
    std::cout << hybridMoved << std::endl;

    std::cout << "--- SiamiStreetCat scope end ---" << std::endl;
}

int main()
{
    demonstrateCat();
    std::cout << std::endl;

    demonstrateStreetCat();
    std::cout << std::endl;

    demonstrateSiamiCat();
    std::cout << std::endl;

    demonstrateSiamiStreetCat();

    return 0;
}
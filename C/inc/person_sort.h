#ifndef PERSON_SORT_H
#define PERSON_SORT_H

#define SUCCESS 0
#define ERROR -1

struct Person
{
    int id;
    char name[20];
    int age;
};

int SortPersonsById(struct Person *array, int size);
int SortPersonsByName(struct Person *array, int size);

#endif
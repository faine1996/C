#ifndef __PERSON_LIST_H__
#define __PERSON_LIST_H__

#include <stddef.h> /* for size_t */
#include <string.h>

typedef struct Person Person;

struct Person
{
    int id;
    char name[32];
    Person* next;
};

/**
 * @brief Add a person to the beginning of the list
 * @param[in] _head - Current head
 * @param[in] _p - Person to insert
 * @return Person* - The new head
 */
Person* ListInsertHead(Person* _head, Person* _p);

/**
 * @brief Remove the head of the list
 * @param[in] _head - Current head
 * @param[out] _item - Double pointer to store address of removed person
 * @return Person* - The new head
 */
Person* ListRemoveHead(Person* _head, Person** _item);

/**
 * @brief Iteratively insert a person sorted by ID
 * @param[in] _head - Current head
 * @param[in] _key - ID to assign
 * @param[in] _p - Person node
 * @return Person* - The head of the list
 */
Person* ListInsertByKey(Person* _head, int _key, Person* _p);

/**
 * @brief Recursively insert a person sorted by ID
 * @param[in] _head - Current node
 * @param[in] _key - ID to assign
 * @param[in] _p - Person node
 * @return Person* - The head of the list
 */
Person* ListInsertByKeyRec(Person* _head, int _key, Person* _p);

/**
 * @brief Iteratively remove a person by ID
 * @param[in] _head - Current head
 * @param[in] _key - ID to find
 * @param[out] _p - Double pointer to store removed person
 * @return Person* - The head of the list
 */
Person* ListRemoveByKey(Person* _head, int _key, Person** _p);

/**
 * @brief Recursively remove a person by ID
 * @param[in] _head - Current node
 * @param[in] _key - ID to find
 * @param[out] _p - Double pointer to store removed person
 * @return Person* - The head of the list
 */
Person* ListRemoveByKeyRec(Person* _head, int _key, Person** _p);

#endif /* __PERSON_LIST_H__ */
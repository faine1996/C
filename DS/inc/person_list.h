#ifndef __PERSON_LIST_H__
#define __PERSON_LIST_H__

#include <stddef.h> /* for size_t */

typedef struct Person Person;

struct Person
{
    int id;
    char name[32];
    Person* next;
};

/** * @brief Add a new Person to the beginning of the list
 * @param[in] _head - The current head of the list
 * @param[in] _p - The person node to insert
 * @return Person pointer - The new head of the list. Returns original head if _p is NULL or duplicate.
 */
Person* ListInsertHead(Person* _head, Person* _p);

/** * @brief Remove the Person currently at the head of the list
 * @param[in] _head - The current head of the list
 * @param[out] _item - A double pointer to store the address of the removed person
 * @return Person pointer - The new head of the list
 */
Person* ListRemoveHead(Person* _head, Person** _item);

/** * @brief Insert a Person into the list sorted by their ID key
 * @param[in] _head - The current head of the list
 * @param[in] _key - The ID value to assign to the person and use for sorting
 * @param[in] _p - The person node to insert
 * @return Person pointer - The head of the list. Returns original head if duplicate key is found.
 */
Person* ListInsertByKey(Person* _head, int _key, Person* _p);

/** * @brief Find and remove a specific Person from the list by their ID key
 * @param[in] _head - The current head of the list
 * @param[in] _key - The ID key of the person to remove
 * @param[out] _p - A double pointer to store the address of the removed person
 * @return Person pointer - The head of the list (may change if head was removed)
 */
Person* ListRemoveByKey(Person* _head, int _key, Person** _p);

/**
 * @brief Recursively find the last node in the list
 * @param[in] _head - The current node being inspected
 * @return Person pointer - Pointer to the last node, or NULL if list is empty
 */
Person* GetLastNodeRecursive(Person* _head);

#endif /* __PERSON_LIST_H__ */
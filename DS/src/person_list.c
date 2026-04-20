#include <stdio.h>
#include <stdlib.h>
#include "../inc/person_list.h"

/* Internal Helper Declarations */
static int IsDuplicate(Person* _head, int _key);

/* 1. Insert at Head: O(1) if no duplicate check, but we must check duplicates */
Person* ListInsertHead(Person* _head, Person* _p)
{
    if (NULL == _p)
    {
        return _head;
    }

    if (0 != IsDuplicate(_head, _p->id))
    {
        printf("Error: Duplicate ID %d\n", _p->id);
        return _head;
    }

    _p->next = _head;
    return _p;
}

/* 2. Remove from Head: Sets _item to the removed node and returns new head */
Person* ListRemoveHead(Person* _head, Person** _item)
{
    Person* nextNode;

    if (NULL == _head || NULL == _item)
    {
        return _head;
    }

    nextNode = _head->next;
    *_item = _head;
    (*_item)->next = NULL;

    return nextNode;
}

/* 3. Insert By Key: Keeps the list sorted by ID and prevents duplicates */
Person* ListInsertByKey(Person* _head, int _key, Person* _p)
{
    Person* curr;
    Person* prev;

    if (NULL == _p)
    {
        return _head;
    }

    _p->id = _key;
    curr = _head;
    prev = NULL;

    /* Traverse to find insertion point or duplicate */
    while (NULL != curr && curr->id < _key)
    {
        prev = curr;
        curr = curr->next;
    }

    /* Check for duplicates at the current position */
    if (NULL != curr && _key == curr->id)
    {
        printf("Error: Duplicate key %d\n", _key);
        return _head;
    }

    _p->next = curr;

    if (NULL == prev)
    {
        return _p; /* Insert at the very beginning */
    }

    prev->next = _p;
    return _head;
}

/* 4. Remove By Key: Finds node, unlinks it, and returns it via pointer */
Person* ListRemoveByKey(Person* _head, int _key, Person** _p)
{
    Person* curr;
    Person* prev;

    if (NULL == _head || NULL == _p)
    {
        return _head;
    }

    curr = _head;
    prev = NULL;

    while (NULL != curr && _key != curr->id)
    {
        prev = curr;
        curr = curr->next;
    }

    if (NULL == curr)
    {
        *_p = NULL; /* Key not found */
        return _head;
    }

    if (NULL == prev)
    {
        _head = curr->next; /* Removing the head */
    }
    else
    {
        prev->next = curr->next;
    }

    curr->next = NULL;
    *_p = curr;

    return _head;
}

Person* GetLastNodeRecursive(Person* _head)
{
    /* Base Case 1: Empty list */
    if (NULL == _head)
    {
        return NULL;
    }

    /* Base Case 2: Last node found (no next node) */
    if (NULL == _head->next)
    {
        return _head;
    }

    /* Recursive Step: Move to the next node */
    return GetLastNodeRecursive(_head->next);
}

/* Helper Definitions */

static int IsDuplicate(Person* _head, int _key)
{
    Person* curr;
    curr = _head;

    while (NULL != curr)
    {
        if (_key == curr->id)
        {
            return 1; /* Found duplicate */
        }
        curr = curr->next;
    }

    return 0;
}
#include <stdlib.h>
#include <string.h>
#include "../inc/person_list_rec.h"

/* --- Helper Function Declarations --- */
static int IsDuplicate(Person* _head, int _key);

/* --- Main Functions --- */

Person* ListInsertHead(Person* _head, Person* _p)
{
    if (NULL == _p || 1 == IsDuplicate(_head, _p->id))
    {
        return _head;
    }

    _p->next = _head;
    return _p;
}

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

Person* ListInsertByKey(Person* _head, int _key, Person* _p)
{
    Person* curr;
    Person* prev;

    if (NULL == _p || 1 == IsDuplicate(_head, _key))
    {
        return _head;
    }

    _p->id = _key;
    curr = _head;
    prev = NULL;

    while (NULL != curr && curr->id < _key)
    {
        prev = curr;
        curr = curr->next;
    }

    if (NULL == prev)
    {
        _p->next = _head;
        return _p;
    }

    _p->next = curr;
    prev->next = _p;
    return _head;
}

Person* ListInsertByKeyRec(Person* _head, int _key, Person* _p)
{
    if (NULL == _p)
    {
        return _head;
    }

    if (NULL == _head || _key < _head->id)
    {
        _p->id = _key;
        _p->next = _head;
        return _p;
    }

    if (_key == _head->id)
    {
        return _head;
    }

    _head->next = ListInsertByKeyRec(_head->next, _key, _p);
    return _head;
}

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
        *_p = NULL;
        return _head;
    }

    if (NULL == prev)
    {
        _head = curr->next;
    }
    else
    {
        prev->next = curr->next;
    }

    *_p = curr;
    (*_p)->next = NULL;
    return _head;
}

Person* ListRemoveByKeyRec(Person* _head, int _key, Person** _p)
{
    Person* nextNode;

    if (NULL == _head || NULL == _p)
    {
        return _head;
    }

    if (_key == _head->id)
    {
        nextNode = _head->next;
        *_p = _head;
        (*_p)->next = NULL;
        return nextNode;
    }

    _head->next = ListRemoveByKeyRec(_head->next, _key, _p);
    return _head;
}

/* --- Helper Function Definitions --- */

static int IsDuplicate(Person* _head, int _key)
{
    Person* curr;
    curr = _head;

    while (NULL != curr)
    {
        if (_key == curr->id)
        {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}
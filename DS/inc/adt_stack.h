#ifndef __ADT_STACK_H__
#define __ADT_STACK_H__

#include <stddef.h> /* for size_t */

typedef enum 
{
    STACK_SUCCESS,
    STACK_UNINITIALIZED_ERROR,
    STACK_OVERFLOW_ERROR,
    STACK_DATA_NOT_FOUND_ERROR
} StackResult;

typedef struct Stack Stack;

/**
 * @brief Create a new stack with a given initial capacity
 * @param[in] _initialCapacity - The starting capacity of the stack
 * @return Stack pointer - on success, NULL on failure
 */
Stack* StackCreate(size_t _initialCapacity);

/**
 * @brief Unallocate a previously created stack
 * @param[in] _stack - A double pointer to the stack to unallocate
 * @return void - no return value
 */
void StackDestroy(Stack** _stack);

/**
 * @brief Add a new item to the stack
 * @param[in] _stack - The stack to add the item to
 * @param[in] _item - the char item to add
 * @return StackResult - 
 * @retval STACK_SUCCESS - on success
 * @retval STACK_UNINITIALIZED_ERROR - if _stack is NULL
 * @retval STACK_OVERFLOW_ERROR - if reallocation fails
 */
StackResult StackPush(Stack* _stack, int _item);

/**
 * @brief Remove the top item from the stack
 * @param[in] _stack - the stack to remove the information from
 * @param[in] _item - A pointer to store the removed char
 * @return StackResult -
 * @retval STACK_SUCCESS - on success
 * @retval STACK_UNINITIALIZED_ERROR - if _stack or _item is NULL
 * @retval STACK_DATA_NOT_FOUND_ERROR - if stack is empty
 */
StackResult StackPop(Stack* _stack, int* _item);

/**
 * @brief Check if a given stack is empty
 * @param[in] _stack - The stack to check
 * @return size_t - returns non-zero (1) if empty, 0 otherwise
 */
size_t StackIsEmpty(const Stack* _stack);

#endif /* __ADT_STACK_H__ */
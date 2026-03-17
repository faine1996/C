#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

/*Error Code Macros*/
#define ERROR -1
#define DARRAY_OK          0
#define DARRAY_NULL_PTR    1  
#define DARRAY_OVERFLOW    2  
#define DARRAY_RESIZE      3  
#define DARRAY_UNDERFLOW   4  

/*Data Structure Definition*/
struct DynamicArray {
    int *dArray;           
    int dArraySize;        
    int dArrayBlockSize;   
    int NumOfElements;     
};

/*Function Prototypes*/

/*
 * Description: Allocates memory for the DynamicArray struct and its internal array.
 * Expected Input: 'size' (initial array capacity) and 'blockSize' (growth amount).
 * Expected Output: A fully initialized DynamicArray struct in memory.
 * Return: Pointer to the newly created struct, or NULL if allocation fails.
 */
struct DynamicArray* DynamicArrayCreate(int size, int blockSize);

/*
 * Description: Frees the memory allocated for the internal array and the struct to prevent leaks.
 * Expected Input: A pointer to the DynamicArray struct.
 * Expected Output: Memory is cleanly released back to the operating system.
 * Return: Void.
 */
void DynamicArrayDestroy(struct DynamicArray *DynamicArrayPtr);

/*
 * Description: Inserts an integer at the end of the array, resizing it by blockSize if full.
 * Expected Input: A pointer to the DynamicArray struct, and the integer data to insert.
 * Expected Output: Data is added to the array; internal capacity is increased if it was full.
 * Return: DARRAY_OK, DARRAY_RESIZE if it expanded, DARRAY_OVERFLOW if memory allocation failed, or DARRAY_NULL_PTR.
 */
int DynamicArrayInsert(struct DynamicArray *DynamicArrayPtr, int data);

/*
 * Description: Removes the last element from the array and retrieves its value.
 * Expected Input: A pointer to the DynamicArray struct, and an integer pointer to hold the retrieved data.
 * Expected Output: The last element is removed from the array count and copied into the provided data pointer.
 * Return: DARRAY_OK, DARRAY_UNDERFLOW if the array is already empty, or DARRAY_NULL_PTR.
 */
int DynamicArrayDelete(struct DynamicArray *DynamicArrayPtr, int *data);

#endif /* DYNAMIC_ARRAY_H */
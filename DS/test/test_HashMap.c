/* HashMap_Tests.c */
#include "HashMap.h"
#include <stdio.h>

/* --- Test Helper Declarations --- */
static size_t HashInt(void* _key);
static int IntEquals(void* _firstKey, void* _secondKey);
static void RunTest(const char* _testName, int _condition);

/* --- Test Main Functions Declarations --- */
void Test_HashMap_Create_Valid(void);
void Test_HashMap_Insert_Valid(void);
void Test_HashMap_Insert_Duplicate(void);

/* --- Main Entry Point --- */
int main(void)
{
    Test_HashMap_Create_Valid();
    Test_HashMap_Insert_Valid();
    Test_HashMap_Insert_Duplicate();

    return 0;
}

/* --- Test Main Functions Definitions --- */

void Test_HashMap_Create_Valid(void)
{
    HashMap* map = NULL;

    map = HashMap_Create(10, HashInt, IntEquals);
    
    RunTest("HashMap_Create - Valid allocation", NULL != map);
    
    HashMap_Destroy(&map, NULL, NULL);
}

void Test_HashMap_Insert_Valid(void)
{
    HashMap* map = NULL;
    int key1 = 5;
    int val1 = 100;
    Map_Result res;

    map = HashMap_Create(10, HashInt, IntEquals);
    res = HashMap_Insert(map, &key1, &val1);

    RunTest("HashMap_Insert - Valid insertion", MAP_SUCCESS == res && 1 == HashMap_Size(map));

    HashMap_Destroy(&map, NULL, NULL);
}

void Test_HashMap_Insert_Duplicate(void)
{
    HashMap* map = NULL;
    int key1 = 5;
    int val1 = 100;
    int val2 = 200;
    Map_Result res;

    map = HashMap_Create(10, HashInt, IntEquals);
    
    HashMap_Insert(map, &key1, &val1);
    res = HashMap_Insert(map, &key1, &val2); /* Duplicate Key */

    RunTest("HashMap_Insert - Prevent duplicate key", MAP_KEY_DUPLICATE_ERROR == res && 1 == HashMap_Size(map));

    HashMap_Destroy(&map, NULL, NULL);
}

/* --- Test Helper Definitions --- */

static size_t HashInt(void* _key)
{
    if (NULL == _key)
    {
        return 0;
    }
    return (size_t)(*(int*)_key);
}

static int IntEquals(void* _firstKey, void* _secondKey)
{
    if (NULL == _firstKey || NULL == _secondKey)
    {
        return 0;
    }
    return (*(int*)_firstKey == *(int*)_secondKey);
}

static void RunTest(const char* _testName, int _condition)
{
    if (0 != _condition)
    {
        printf("PASS : %s\n", _testName);
    }
    else
    {
        printf("FAIL : %s\n", _testName);
    }
}
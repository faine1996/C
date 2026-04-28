#include <stdio.h>
#include "../inc/BSTree.h"

/* --- Test Function Declarations --- */

static void TestCreateAndDestroy(void);
static void TestInsertAndGet(void);
static void TestIteration(void);
static int CompareInts(void* _left, void* _right);
static int PrintAction(void* _element, void* _context);

/* --- Main --- */

int main(void)
{
    TestCreateAndDestroy();
    TestInsertAndGet();
    TestIteration();

    return 0;
}

/* --- Test Function Definitions --- */

static int CompareInts(void* _left, void* _right)
{
    int left = *(int*)_left;
    int right = *(int*)_right;

    if (left < right)
    {
        return 1;
    }
    if (left > right)
    {
        return -1;
    }
    return 0;
}

static int PrintAction(void* _element, void* _context)
{
    /* Silence unused parameter warning for C89 */
    (void)_context;

    printf("%d ", *(int*)_element);

    /* Yoda style return */
    return 1;
}

static void TestCreateAndDestroy(void)
{
    BSTree* tree = BSTreeCreate(CompareInts);
    if (NULL != tree)
    {
        printf("Test Create: PASS\n");
        BSTreeDestroy(&tree, NULL);
        if (NULL == tree)
        {
            printf("Test Destroy: PASS\n");
        }
    }
}

static void TestInsertAndGet(void)
{
    int a = 50, b = 30, c = 70;
    BSTree* tree = BSTreeCreate(CompareInts);
    BSTreeItr it = NULL;

    BSTreeInsert(tree, &a);
    BSTreeInsert(tree, &b);
    it = BSTreeInsert(tree, &c);

    if (70 == *(int*)BSTreeItrGet(it))
    {
        printf("Test Insert/Get: PASS\n");
    }

    BSTreeDestroy(&tree, NULL);
}

static void TestIteration(void)
{
    int arr[] = {50, 20, 80, 10, 40};
    int i = 0;
    BSTree* tree = BSTreeCreate(CompareInts);

    for (i = 0; i < 5; ++i)
    {
        BSTreeInsert(tree, &arr[i]);
    }

    printf("In-order traversal: ");
    BSTreeForEach(tree, BSTREE_TRAVERSAL_INORDER, PrintAction, NULL);
    printf("\n");

    BSTreeDestroy(&tree, NULL);
}
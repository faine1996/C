#include <stdio.h>
#include "../inc/triangles.h"

int main(void)
{
    int n = 0;
    int bad_n = -5;
    char c = '\0';
    int status = 0;

    printf("Enter the number of lines (N): ");
    scanf("%d", &n);
    
    printf("Enter the character to print: ");
    scanf(" %c", &c);

    printf("\n=== func1 (Ascending Left) ===\n");
    status = func1(n, c);
    printf("Return Code: %d\n", status);

    printf("\n=== func2 (Descending Left) ===\n");
    status = func2(n, c);
    printf("Return Code: %d\n", status);

    printf("\n=== func3 (Arrow Left) ===\n");
    status = func3(n, c);
    printf("Return Code: %d\n", status);

    printf("\n=== func4 (Ascending Right) ===\n");
    status = func4(n, c);
    printf("Return Code: %d\n", status);

    printf("\n=== func5 (Descending Right) ===\n");
    status = func5(n, c);
    printf("Return Code: %d\n", status);

    printf("\n=== func6 (Arrow Right) ===\n");
    status = func6(n, c);
    printf("Return Code: %d\n", status);

    /* Test the defensive error handling limits */
    printf("\n=== Testing Error Handling (N = %d) ===\n", bad_n);
    status = func1(bad_n, c);
    if (ERROR == status)
    {
        printf("[PASS] Function correctly blocked invalid input. Return Code: %d\n", status);
    }
    else
    {
        printf("[FAIL] Function allowed invalid input!\n");
    }

    return 0;
}
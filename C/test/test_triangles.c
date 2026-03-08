#include <stdio.h>
#include "../inc/triangles.h"

int main(void)
{
    int n = 0;
    char c = '\0';

    printf("Enter the number of lines (N): ");
    scanf("%d", &n);
    
    printf("Enter the character to print: ");
    scanf(" %c", &c);

    printf("\n=== func1 (Ascending Left) ===\n");
    func1(n, c);

    printf("\n=== func2 (Descending Left) ===\n");
    func2(n, c);

    printf("\n=== func3 (Arrow Left) ===\n");
    func3(n, c);

    printf("\n=== func4 (Ascending Right) ===\n");
    func4(n, c);

    printf("\n=== func5 (Descending Right) ===\n");
    func5(n, c);

    printf("\n=== func6 (Arrow Right) ===\n");
    func6(n, c);

    return 0;
}
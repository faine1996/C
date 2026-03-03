#include <stdio.h>

void printSum(int a, int b);
void printDifference(int a, int b);
void printProduct(int a, int b);

int main(void) {
    int choice;
    int num1;
    int num2;

    printf("--- Math Operations Menu ---\n");
    printf("1. Sum (a+b)\n2. Difference (a-b)\n3. Product (a*b)\n");
    printf("Choose operation (1-3): ");
    scanf("%d", &choice);

    printf("Enter 2 integers separated by a space: ");
    scanf("%d %d", &num1, &num2);

    switch (choice) {
        case 1: 
            printSum(num1, num2); 
            break;
        case 2: 
            printDifference(num1, num2); 
            break;
        case 3: 
            printProduct(num1, num2); 
            break;
        default: 
            printf("Invalid selection.\n"); 
            break;
    }

    return 0;
}

void printSum(int a, int b) {
    printf("The sum of the 2 numbers (a+b) is: %d\n", a + b);
}

void printDifference(int a, int b) {
    printf("The difference between the 2 numbers (a-b) is: %d\n", a - b);
}

void printProduct(int a, int b) {
    printf("The multiply of the 2 numbers (a*b) is: %d\n", a * b);
}
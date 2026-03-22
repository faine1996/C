#include <stdio.h>
#include <stdlib.h>
#include "../inc/text_ex.h"

int test_small_file(void);
int test_large_file(void);
int test_file_smaller_than_n(void);
int test_zero_lines(void);

/* Helper function to generate our text files for testing */
void create_dummy_file(const char* filename, int num_lines);

int main(void)
{
    int passed = 0;
    int total = 4;

    printf("--- Running Tail Function Tests ---\n");

    passed += test_small_file();
    passed += test_large_file();
    passed += test_file_smaller_than_n();
    passed += test_zero_lines();

    printf("-------------------------------\n");
    printf("Results: %d/%d tests executed successfully.\n", passed, total);
    printf("(Please verify the printed output above matches expectations)\n");

    return 0;
}

void create_dummy_file(const char* filename, int num_lines)
{
    FILE* f;
    int i;

    f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Error creating helper file: %s\n", filename);
        return;
    }

    for (i = 1; i <= num_lines; i++)
    {
        fprintf(f, "This is line number %d\n", i);
    }

    fclose(f);
}

int test_small_file(void)
{
    FILE* in;

    printf("\n=== test_small_file ===\n");
    printf("Expected: Lines 4 and 5\n");
    
    create_dummy_file("small_test.txt", 5);
    in = fopen("small_test.txt", "r");

    if (in != NULL)
    {
        tail(in, 2);
        fclose(in);
        printf("[PASS] test_small_file\n");
        return 1;
    }
    else
    {
        printf("[FAIL] test_small_file\n");
        return 0;
    }
}

int test_large_file(void)
{
    FILE* in;

    printf("\n=== test_large_file ===\n");
    printf("Expected: Lines 996 to 1000\n");
    
    /* Generates a 1000 line text file */
    create_dummy_file("large_test.txt", 1000);
    in = fopen("large_test.txt", "r");

    if (in != NULL)
    {
        tail(in, 5);
        fclose(in);
        printf("[PASS] test_large_file\n");
        return 1;
    }
    else
    {
        printf("[FAIL] test_large_file\n");
        return 0;
    }
}

int test_file_smaller_than_n(void)
{
    FILE* in;

    printf("\n=== test_file_smaller_than_n ===\n");
    printf("Expected: Lines 1, 2, and 3 (All lines)\n");
    
    /* File has 3 lines, but we ask for 10 */
    create_dummy_file("short_test.txt", 3);
    in = fopen("short_test.txt", "r");

    if (in != NULL)
    {
        tail(in, 10);
        fclose(in);
        printf("[PASS] test_file_smaller_than_n\n");
        return 1;
    }
    else
    {
        printf("[FAIL] test_file_smaller_than_n\n");
        return 0;
    }
}

int test_zero_lines(void)
{
    FILE* in;

    printf("\n=== test_zero_lines ===\n");
    printf("Expected: (Nothing should print)\n");
    
    create_dummy_file("zero_test.txt", 10);
    in = fopen("zero_test.txt", "r");

    if (in != NULL)
    {
        tail(in, 0);
        fclose(in);
        printf("[PASS] test_zero_lines\n");
        return 1;
    }
    else
    {
        printf("[FAIL] test_zero_lines\n");
        return 0;
    }
}
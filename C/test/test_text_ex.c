#include <stdio.h>
#include <stdlib.h>
#include "../inc/text_ex.h"

int test_small_file(void);
int test_large_file(void);
int test_file_smaller_than_n(void);
int test_zero_lines(void);
int test_empty_file(void);
int test_standard_text(void);
int test_excessive_whitespace(void);

/* Helper for tail: generates N lines of text */
void create_dummy_file_lines(const char* filename, int num_lines);

/* Helper for wc: writes a specific exact string */
void create_dummy_file_text(const char* filename, const char* content);

int main(void)
{
    int passed = 0;
    int total = 7;

    printf("--- Running Tail Function Tests ---\n");

    passed += test_small_file();
    passed += test_large_file();
    passed += test_file_smaller_than_n();
    passed += test_zero_lines();
    passed += test_empty_file();
    passed += test_standard_text();
    passed += test_excessive_whitespace();

    printf("-------------------------------\n");
    printf("Results: %d/%d tests executed successfully.\n", passed, total);
    printf("(Please verify the printed output above matches expectations)\n");

    return 0;
}

int test_small_file(void)
{
    FILE* in;

    printf("\n=== test_small_file ===\n");
    printf("Expected: Lines 4 and 5\n");
    
    create_dummy_file_lines("small_test.txt", 5);
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
    create_dummy_file_lines("large_test.txt", 1000);
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
    create_dummy_file_lines("short_test.txt", 3);
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
    
    create_dummy_file_lines("zero_test.txt", 10);
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

int test_empty_file(void)
{
    FILE* in;
    unsigned long l, w, c;

    printf("\n=== test_empty_file ===\n");
    
    create_dummy_file_text("empty_test.txt", "");
    in = fopen("empty_test.txt", "r");

    if (in != NULL)
    {
        wc(in, &l, &w, &c);
        fclose(in);
        
        if (l == 0 && w == 0 && c == 0)
        {
            printf("[PASS] test_empty_file\n");
            return 1;
        }
    }
    printf("[FAIL] test_empty_file\n");
    return 0;
}

int test_standard_text(void)
{
    FILE* in;
    unsigned long l, w, c;
    const char* text = "Hello world\nThis is C89\n";

    printf("\n=== test_standard_text ===\n");
    
    create_dummy_file_text("std_test.txt", text);
    in = fopen("std_test.txt", "r");

    if (in != NULL)
    {
        wc(in, &l, &w, &c);
        fclose(in);
        
        /* Expected: 2 lines, 5 words, 24 chars */
        if (l == 2 && w == 5 && c == 24)
        {
            printf("[PASS] test_standard_text\n");
            return 1;
        }
        else
        {
            printf("Got: %lu lines, %lu words, %lu chars\n", l, w, c);
        }
    }
    printf("[FAIL] test_standard_text\n");
    return 0;
}

int test_excessive_whitespace(void)
{
    FILE* in;
    unsigned long l, w, c;
    const char* text = "   \n\n  word1 \t\t word2  \n \n";

    printf("\n=== test_excessive_whitespace ===\n");
    
    create_dummy_file_text("ws_test.txt", text);
    in = fopen("ws_test.txt", "r");

    if (in != NULL)
    {
        wc(in, &l, &w, &c);
        fclose(in);
        
        /* Expected: 4 lines, 2 words, 26 chars */
        if (l == 4 && w == 2 && c == 26)
        {
            printf("[PASS] test_excessive_whitespace\n");
            return 1;
        }
        else
        {
             printf("Got: %lu lines, %lu words, %lu chars\n", l, w, c);
        }
    }
    printf("[FAIL] test_excessive_whitespace\n");
    return 0;
}

void create_dummy_file_lines(const char* filename, int num_lines)
{
    FILE* f;
    int i;

    f = fopen(filename, "w");
    if (NULL != f)
    {
        for (i = 1; i <= num_lines; ++i)
        {
            fprintf(f, "This is line number %d\n", i);
        }
        fclose(f);
    }
    else
    {
        printf("Error creating helper file: %s\n", filename);
    }
}

void create_dummy_file_text(const char* filename, const char* content)
{
    FILE* f = fopen(filename, "w");
    
    if (NULL != f)
    {
        if (NULL != content)
        {
            fputs(content, f);
        }
        fclose(f);
    }
    else
    {
        printf("Error creating helper file: %s\n", filename);
    }
}
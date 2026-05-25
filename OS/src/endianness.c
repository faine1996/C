#include <stdio.h>

/* --- Forward Declarations --- */
int check_endianness(void);

/* --- Main --- */
int main(void)
{
    int is_little;

    is_little = check_endianness();

    if (1 == is_little)
    {
        printf("Processor Architecture: LITTLE ENDIAN (Host Byte Order)\n");
    }
    else
    {
        printf("Processor Architecture: BIG ENDIAN (Network Byte Order)\n");
    }

    return 0;
}

/* --- Function Definitions --- */

/* * Returns 1 if Little Endian 
 * Returns 0 if Big Endian 
 */
int check_endianness(void)
{
    unsigned int test_val;
    char *byte_ptr;
    
    test_val = 1;

    /* * Cast the integer's memory address to a single-byte character pointer.
     * This restricts the compiler's view to ONLY the first physical byte in RAM.
     */
    byte_ptr = (char *)&test_val;

    /* Dereference the pointer to check the value of the very first byte */
    if (1 == *byte_ptr)
    {
        return 1;
    }
    
    return 0; 
}

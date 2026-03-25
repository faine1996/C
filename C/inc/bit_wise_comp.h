#ifndef COMPRESS_H
#define COMPRESS_H

/* * Structure representing two 4-bit members.
 * Note on C89 Standard: Strict ANSI C89 requires bit-fields to be of type 
 * int, signed int, or unsigned int. Because of this, the compiler may pad 
 * this struct to 4 bytes. To force it into 1 byte, modern compilers use 
 * 'unsigned char' or __attribute__((packed)), but 'unsigned int' is used 
 * here for strict C89 compliance.
 */
struct BitFieldPair
{
    unsigned int first  : 4;
    unsigned int second : 4;
};

/* Function Prototypes */

/*
 * Description: Compresses a string by packing two characters into one byte using bitwise operations.
 * Expected Output: Compressed bytes are written to dest.
 * Return: Number of bytes written, or -1 on error.
 */
int CompressBitwise(const char *src, unsigned char *dest);

/*
 * Description: Decompresses a bitwise-compressed byte array back to a string.
 */
void DecompressBitwise(const unsigned char *src, int compressed_bytes, char *dest);

/*
 * Description: Compresses a string into an array of bit-field structs.
 * Expected Output: Compressed data written to dest.
 * Return: Number of structs written, or -1 on error.
 */
int CompressBitField(const char *src, struct BitFieldPair *dest);

/*
 * Description: Decompresses a bit-field-compressed array back to a string.
 */
void DecompressBitField(const struct BitFieldPair *src, int compressed_pairs, char *dest);

#endif /* COMPRESS_H */
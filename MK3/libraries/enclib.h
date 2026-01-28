#include <stdint.h>
#include <stdio.h>

#define ENDCODING_EXTENSION ".bmp"
#define DECODING_EXTENSION ".txt"
#define HEADER 0x00003342



void dtob(int num, char *string, int strlength);
void invert_file_text(FILE *file);
int query();
int encode_seed(char *seed);

typedef struct
{
    uint32_t header;
    uint16_t seed;
} __attribute__((__packed__)) 
B3FileHeader;
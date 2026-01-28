#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libraries/enclib.h"
#include "libraries/bitmap.h"

void encode_text(FILE *infile, FILE *outfile, int seed);

void decode_text(FILE *infile, FILE *outfile, int seed);

int main(int argc, char *argv[])
{
    FILE *f_read;
    FILE *f_write;

    char *seed = argv[3];
    char *out_path = argv[1];
    char *in_path = argv[2];

    int opt;
    int mode = 0;

    while((opt = getopt(argc, argv, "ed")) != -1)
    {
        switch(opt)
        {
            case 'e':
                mode = 0;
                break;
            case 'd':
                mode = 1;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }
    

    if(argc != 5)
    {
        printf("INCORRECT USE OF ARGUMENTS\n");
        printf("<write path> <read path> <seed> <mode>\n");
        printf("-e: encoding mode\n-d: decoding mode\n");
        return EXIT_FAILURE;
    }

    if(encode_seed(seed) > 0xFFFF)
    {
        printf("seed is too large\n");
        printf("%d\n", encode_seed(seed));
        exit(EXIT_FAILURE);
    }

    if(mode == 0) printf("encoding\nread path: %s\nwrite path: %s%s\nseed: %s\n", in_path, out_path, ENDCODING_EXTENSION, seed);
    if(mode == 1) printf("decoding\nread path: %s\nwrite path: %s%s\nseed: %s\n", in_path, out_path, DECODING_EXTENSION, seed);

    int result = query();
    if(result != 0)
    {
        printf("file path incorrect, exiting program\n");
        return EXIT_FAILURE;
    }
    
    if(mode == 0) 
    {
        f_read = fopen(in_path, "r");

        f_write = fopen(strcat(out_path, ENDCODING_EXTENSION), "wb+");
    }

    if(mode == 1) 
    {
        f_read = fopen(in_path, "rb");
    
        f_write = fopen(strcat(out_path, DECODING_EXTENSION), "w+");
    }

    if(f_read == NULL) 
    {
        printf("ERROR READING FILE (file does not exist?)\n");
        return EXIT_FAILURE;
    }

    if(f_write == NULL)
    {
        printf("ERROR WRITING FILE\n");
        return EXIT_FAILURE;
    }

    if(mode == 0) encode_text(f_read, f_write, encode_seed(seed));
    if(mode == 1){decode_text(f_read, f_write, encode_seed(seed)); invert_file_text(f_write);}

    fclose(f_read);
    fclose(f_write);
    printf("Successfully wrote to file: %s\n", out_path);

    return EXIT_SUCCESS;
}

//encodes the text by multiplying it with a given seed, and writing it to a binary file
void encode_text(FILE *infile, FILE *outfile, int seed)
{
    int buffer;

    fseek(infile, 0, SEEK_END);
    int length = (ftell(infile) - 1);
    char string[32];

    create_bitmap(length, outfile);

    for(int i = length; i >= 0; i--)
    {
        fseek(infile, i, SEEK_SET);
        buffer = fgetc(infile);
        buffer *= seed;
        dtob(buffer, string, 32);
        for(int j = 0; j < strlen(string); j++)
        {
            if(string[j] == '1')
            write_pixel(0,0,0,outfile,j,i);
            if(string[j] == '0')
            write_pixel(255,0,0,outfile,j,i);
        }
    }
}

void decode_text(FILE *infile, FILE *outfile, int seed)
{
    int buffer;
    fseek(infile, sizeof(B3FileHeader), SEEK_SET);

    while(!feof(infile))
    {
        fread(&buffer, sizeof(buffer), 1, infile);
        if(feof(infile)) break;

        buffer /= seed;

        fprintf(outfile, "%c", buffer);
    }
}
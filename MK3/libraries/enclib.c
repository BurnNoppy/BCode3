#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "enclib.h"

//converts a decimal number to binary
void dtob(int num, char *string, int strlength)
{
    int length = 0;
    //initialize string to "0"
    for(int i = 0; i < strlength; i++)
    {
        string[i] = '0';
    }

    do
    {
        if(num % 2 == 0) string[length] = '0';
        else string[length] = '1';
        num /= 2;
        length++;
    } while (num != 0);

    string[strlength] = '\0';
    length = strlength -1;
    int middle = length / 2;

    char temp;

    for(int i = 0; i < middle; i++)
    {
        temp = string[i];
        string[i] = string[length - i];
        string[length - i] = temp;
    }
    return;
}

//inverts the content of a file
void invert_file_text(FILE *file)
{
    fseek(file, 0, SEEK_END);
    int length = (ftell(file) - 1);
    int middle = length / 2;

    for(int i = 0; i < middle; i++)
    {
        fseek(file, length - i, SEEK_SET);
        int temp = fgetc(file);
        fseek(file, i, SEEK_SET);
        int temp2 = fgetc(file);
        fseek(file, length - i, SEEK_SET);
        fputc(temp2, file);
        fseek(file, i, SEEK_SET);
        fputc(temp, file);
    }
    return;
}

//a simple y/n prompt
int query()
{
    char response;

    printf("is this correct? (y/n) ");
    scanf("%c", &response);

    switch (response)
    {
    case 'y': case 'Y':
        return 0;

    case 'n': case 'N':
        return 1;
    default:
        printf("!!!invalid answer\n");
        return 2;
    }
    
}

//takes a string pointer and makes a number based on it
int encode_seed(char *seed)
{
    int seedNum = 0;
    for(int i = 0; i < strlen(seed); i++)
    {
        if(seed[i] >= 48 && seed[i] <= 57)
        {
            seedNum *= seed[i];
        }
        else
        {
            seedNum += seed[i];
        }
    }
    return seedNum;
}

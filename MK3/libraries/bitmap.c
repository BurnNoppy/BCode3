#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bitmap.h"

void create_bitmap(int height, FILE *file)
{

    const int width = 32;

    BMFileHeader bh_header ={
        bh_header.bh_header = 0x4d42,
        bh_header.bh_size_of_file = 54 + width * height,
        bh_header.bh_reserved_0 = 0x0000,
        bh_header.bh_reserved_1 = 0x0000,
        bh_header.bh_offset = 54,
    };
    BMInfoHeader bi_header = 
    {
        bi_header.bi_header_size = sizeof(BMInfoHeader),
        bi_header.bi_width = width,
        bi_header.bi_height = height,
        bi_header.bi_color_planes = 1,
        bi_header.bi_color_depth = 24,
        bi_header.bi_compression = 0,
        bi_header.bi_map_size = width * height,
        bi_header.bi_h_ppm = 0,
        bi_header.bi_v_ppm = 0,
        bi_header.bi_colors_pallette = 0,
        bi_header.bi_important_colors = 0
    };

    fwrite(&bh_header, sizeof(BMFileHeader), 1, file);
    fwrite(&bi_header, sizeof(BMInfoHeader), 1, file);

    for(int i = 0;i < height; i ++)
    {
        for(int j = 0; j < width; j++)
        {
            write_pixel(255,0,0,file,j,i);
        }
    }
}

void write_pixel_line( uint8_t red, uint8_t green, uint8_t blue, FILE *file, int where)
{
    RGBTriple_24 pixel = {
        pixel.blue = blue,
        pixel.green = green,
        pixel.red = red
    };

    int offset = 0;

    fseek(file, 10, SEEK_SET);
    fread(&offset, 4, 1, file);
    fseek(file, offset + where, SEEK_SET);

    for(int i = 0; i < 32; i++)
    {
        fwrite(&pixel, sizeof(RGBTriple_24), 1, file);
    }
}

void write_pixel(uint8_t red, uint8_t blue, uint8_t green, FILE *file, int x, int y)
{
    RGBTriple_24 pixel = {
        pixel.blue = blue,
        pixel.green = green,
        pixel.red = red
    };
    int offset = 0;
    fseek(file, 10, SEEK_SET);
    fread(&offset, 4, 1, file);
    offset += (x * sizeof(RGBTriple_24));
    offset += (y * (sizeof(RGBTriple_24) * 32));
    fseek(file, offset, SEEK_SET);
    fwrite(&pixel, sizeof(RGBTriple_24), 1, file);
}
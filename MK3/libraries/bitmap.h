#include <stdint.h>
#include <stdio.h>

//bitmap compression methods:
//0, BI_RGB [none]
//1 BI_RLE8 [RLE 8-bit/pixel]
//2 BI_RLE4 [RLE 4-bit/pixel]
//3 BI_BITFIELDS [I do not know]
//4 BI_JPEG [RLE-24]
//5 BI_PNG
//6 BI_ALPHABITFIELDS [RGBA bit field masks]
//11 - 13 CMYK related

//put function declarations here
void create_bitmap(int height, FILE *file);

void write_pixel_line( uint8_t red, uint8_t green, uint8_t blue, FILE *file, int where);

void write_pixel(uint8_t red, uint8_t blue, uint8_t green, FILE *file, int x, int y);

//a struct for defining a bitmap file header
typedef struct 
{
    //this will almost always be "BM" or 0x4d42
    uint16_t bh_header;
    //size of file in bytes
    uint32_t bh_size_of_file;
    //reserved space
    uint16_t bh_reserved_0;
    //reserved space
    uint16_t bh_reserved_1;
    //where the actual map can be found
    uint32_t bh_offset;
}__attribute__((__packed__))
BMFileHeader;

typedef struct 
{
    //size of this header in bytes
    uint32_t bi_header_size;
    //bitmap width in pixels (signed int)
    int32_t bi_width;
    //bitmap height in pixels (signed int)
    int32_t bi_height;
    //number of color planes (must be 1)
    uint16_t bi_color_planes;
    //number of bits per pixel
    uint16_t bi_color_depth;
    //the compression method being used (table in enclib header)
    uint32_t bi_compression;
    //the image size (raw bitmap data)
    uint32_t bi_map_size;
    //horizontal resolution (pixels per meter, signed)
    int32_t bi_h_ppm;
    //vertical resolution (pixels per meter, signed)
    int32_t bi_v_ppm;
    //number of colors in the pallette
    uint32_t bi_colors_pallette;
    //number of important colors
    uint32_t bi_important_colors;
}__attribute__((__packed__))
BMInfoHeader;

typedef struct 
{
    unsigned blue:2;
    unsigned green:3;
    unsigned red:3;
}__attribute__((__packed__)) 
RGBTriple_8;

typedef struct 
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
}__attribute__((__packed__)) 
RGBTriple_24;

typedef struct 
{
    unsigned blue:10;
    unsigned green:11;
    unsigned red:11;
}__attribute__((__packed__)) 
RGBTriple_32;
//
//  BmpReader.h
//  bmp
//
//  Created by Alexey Zabrodin on 30.06.15.
//  Copyright (c) 2015 Alexey Zabodin. All rights reserved.
//

#ifndef __bmp__BmpReader__
#define __bmp__BmpReader__

#include <stdio.h>
#include <stdint.h>
#include "FileReader.h"

#pragma pack(push)
#pragma pack(2)
typedef struct
{
    uint16_t    bfType;
    uint32_t    bfSize;
    uint16_t    bfReserved1;
    uint16_t    bfReserved2;
    uint32_t    bfOffBits;
} BitMapFileHeader;

#pragma pack(2)
typedef struct
{
    uint32_t   biSize;
    uint32_t   biWidth;
    uint32_t   biHeight;
    uint16_t   biPlanes;
    uint16_t   biBitCount;
    uint32_t   biCompression;
    uint32_t   biSizeImage;
    uint32_t   biXPelsPerMeter;
    uint32_t   biYPelsPerMeter;
    uint32_t   biClrUsed;
    uint32_t   biClrImportant;
} BitMapInfoHeader;

class BmpReader {
    FileReader* _fileReader;
    BitMapFileHeader _bitMapFileHeader;
    BitMapInfoHeader _bitMapInfoHeader;
public:
    BmpReader(FileReader* fileReader);
    uint32_t height (void);
    uint32_t width  (void);
    uint8_t readByte (int x, int y);
};

#pragma pack(pop)
#endif /* defined(__bmp__BmpReader__) */

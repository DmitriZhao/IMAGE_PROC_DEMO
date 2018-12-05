//
//  BmpReader.h
//  bmp
//
//  Created by Alexey Zabrodin on 30.06.15.
//  Copyright (c) 2015 Alexey Zabodin. All rights reserved.
//

#ifndef __bmp__BmpReader__
#define __bmp__BmpReader__

#include <memory>
#include "FileReader.hpp"
#include "Types.h"

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

class BmpReader : public std::enable_shared_from_this<BmpReader>
{
    FileReader::Ptr     _fileReader;
    BitMapFileHeader    _bitMapFileHeader;
    BitMapInfoHeader    _bitMapInfoHeader;
public:
    explicit BmpReader(FileReader::Ptr fileReader);
    typedef  std::shared_ptr<BmpReader> Ptr;
    Ptr      getPtr();
    uint32_t height (void);
    uint32_t width  (void);
    Pixel    readPixel (int x, int y);
};
#pragma pack(pop)

inline BmpReader::BmpReader(FileReader::Ptr fileReader){
    _fileReader = fileReader;
    
    uint8_t* ptrBitMapFileHeader = (uint8_t*)&_bitMapFileHeader;
    int bitMapHeaderSize = sizeof(BitMapFileHeader);
    _fileReader->seek(0);
    for (int currentByte = 0; currentByte < bitMapHeaderSize; currentByte++)
    {
        ptrBitMapFileHeader[currentByte] = _fileReader->read();
    }
    
    uint8_t* ptrBitMapInfoFileHeader = (uint8_t*)&_bitMapInfoHeader;
    int bitMapInfoFileHeaderSize = sizeof(BitMapInfoHeader);
//    _fileReader->seek(bitMapHeaderSize);
    for (int currentByte = 0; currentByte < bitMapInfoFileHeaderSize; currentByte++)
    {
        ptrBitMapInfoFileHeader[currentByte] = _fileReader->read();
    }
}

inline uint32_t BmpReader::height (void)
{
    return _bitMapInfoHeader.biHeight;
}

inline uint32_t BmpReader::width  (void)
{
    return _bitMapInfoHeader.biWidth;
}

inline BmpReader::Ptr BmpReader::getPtr()
{   
    return shared_from_this();
}

////////////////////////////////////////

inline Pixel BmpReader::readPixel (int x, int y)
{
    Pixel px;
    uint32_t offset = _bitMapFileHeader.bfOffBits 
                    + width() * (height() - 1 - y) * 3
                    + x * 3;
    if (_fileReader->seek(offset))
    {
        px.b = _fileReader->read();
        px.g = _fileReader->read();
        px.r = _fileReader->read();
    }
    //std::cout<<std::dec<<x<<' '<<y<<' '<<std::hex<<offset<<':'<<(int)px.r<<(int)px.g<<(int)px.b<<' ';
    return px;
}

#endif /* defined(__bmp__BmpReader__) */
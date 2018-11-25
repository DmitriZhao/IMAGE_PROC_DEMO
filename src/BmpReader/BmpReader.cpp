//
//  BmpReader.cpp
//  bmp
//
//  Created by Alexey Zabrodin on 30.06.15.
//  Copyright (c) 2015 Alexey Zabodin. All rights reserved.
//

#include "BmpReader.h"
#include "Types.h"
#include <iostream>
BmpReader::BmpReader(FileReader* fileReader){
    _fileReader = fileReader;
    
    uint8_t* ptrBitMapFileHeader = (uint8_t*)&_bitMapFileHeader;
    int bitMapHeaderSize = sizeof(BitMapFileHeader);
    _fileReader->seek(0);
    for (int currentByte = 0; currentByte < bitMapHeaderSize; currentByte++) {
        ptrBitMapFileHeader[currentByte] = _fileReader->read();
    }
    
    uint8_t* ptrBitMapInfoFileHeader = (uint8_t*)&_bitMapInfoHeader;
    int bitMapInfoFileHeaderSize = sizeof(BitMapInfoHeader);
//    _fileReader->seek(bitMapHeaderSize);
    for (int currentByte = 0; currentByte < bitMapInfoFileHeaderSize; currentByte++) {
        ptrBitMapInfoFileHeader[currentByte] = _fileReader->read();
    }
}


uint32_t BmpReader::height (void){
    return _bitMapInfoHeader.biHeight;
}

uint32_t BmpReader::width  (void){
    return _bitMapInfoHeader.biWidth;
}

////////////////////////////////////////

Pixel BmpReader::readPixel (int x, int y)
{
    Pixel px;
    uint8_t byte;
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
    if(!px.b)
    {
        std::cin.get();
    }
    return px;
}
//
//  BmpReader.cpp
//  bmp
//
//  Created by Alexey Zabrodin on 30.06.15.
//  Copyright (c) 2015 Alexey Zabodin. All rights reserved.
//

#include "BmpReader.h"

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

uint8_t BmpReader::readByte (int x, int y)
{
    uint16_t pixel;
    uint8_t byte;
    uint32_t offset = _bitMapFileHeader.bfOffBits 
                    + _bitMapInfoHeader.biWidth * (height() - 1 - y) * _bitMapInfoHeader.biBitCount / 8
                    + x * _bitMapInfoHeader.biBitCount/8;
    if (_fileReader->seek(offset))
    {
        byte = _fileReader->read();
    }
    
    return byte;
}
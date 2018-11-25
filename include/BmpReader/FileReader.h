//
//  FileReader.h
//  bmp
//
//  Created by Alexey Zabrodin on 30.06.15.
//  Copyright (c) 2015 Alexey Zabodin. All rights reserved.
//

#ifndef __bmp__FileReader__
#define __bmp__FileReader__

#include <stdio.h>
#include <stdint.h>

class FileReader {
protected:
    const char* _fileName;
public:
    FileReader(const char *filename);
    const char* filename();
    virtual uint8_t read() = 0;
    virtual uint8_t peek() = 0;
    virtual int available() = 0;
    virtual bool seek(uint32_t pos) = 0;
    virtual uint32_t position() = 0;
//    virtual uint32_t size();
//    virtual void close();
};

#endif /* defined(__bmp__FileReader__) */

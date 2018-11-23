//
//  FileReader.cpp
//  bmp
//
//  Created by Alexey Zabrodin on 30.06.15.
//  Copyright (c) 2015 Alexey Zabodin. All rights reserved.
//

#include "FileReader.h"

FileReader::FileReader(const char *filename)
{
    _fileName = filename;
};

const char* FileReader::filename(){
    return _fileName;
}
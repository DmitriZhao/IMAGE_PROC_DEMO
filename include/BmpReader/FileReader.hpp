//
//  FileReader.h
//  bmp
//
//  Created by Alexey Zabrodin on 30.06.15.
//  Copyright (c) 2015 Alexey Zabodin. All rights reserved.
//

#ifndef __bmp__FileReader__
#define __bmp__FileReader__

#include <memory>

class FileReader : public std::enable_shared_from_this<FileReader>
{
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
//  virtual uint32_t size();
//  virtual void close();
    typedef std::shared_ptr<FileReader> Ptr;
    Ptr     getPtr();
};

inline FileReader::FileReader(const char *filename)
{
    _fileName = filename;
}

inline const char* FileReader::filename()
{
    return _fileName;
}

inline FileReader::Ptr FileReader::getPtr()
{
    return shared_from_this();
}

#endif /* defined(__bmp__FileReader__) */
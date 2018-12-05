//
//  FileReaderInstance.h
//  bmp
//

#ifndef __bmp__FileReaderInstance__
#define __bmp__FileReaderInstance__

#include "FileReader.hpp"
#include <fstream>
#include <memory>

class FileReaderInstance : public FileReader
{
private:
    std::unique_ptr<std::ifstream>   _file;
public:
    explicit FileReaderInstance(const char *filename);
    uint8_t read();
    uint8_t peek();
    int available();
    bool seek(uint32_t pos);
    uint32_t position();
};

inline FileReaderInstance::FileReaderInstance(const char *filename) : FileReader(filename)
{
    
    _fileName = filename;
    _file = std::make_unique<std::ifstream>(_fileName, std::ios::in | std::ios::binary);
}

inline uint8_t FileReaderInstance::read()
{
    uint8_t byteData;
    *_file >> byteData;
    return byteData;
};

inline uint8_t FileReaderInstance::peek()
{
    uint8_t byteData = _file->peek();
    return byteData;
};

inline int FileReaderInstance::available()
{
    return _file->is_open();
};

inline bool FileReaderInstance::seek(uint32_t pos)
{
    _file->clear();
    _file->seekg(0,std::ios::beg);
    return (bool)_file->seekg(pos,std::ios::beg);
};

inline uint32_t FileReaderInstance::position()
{
    return std::ios::cur;
};

#endif /* defined(__bmp__FileReaderInstance__) */
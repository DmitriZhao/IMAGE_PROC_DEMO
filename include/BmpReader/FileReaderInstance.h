//
//  FileReaderInstance.h
//  bmp
//

#ifndef __bmp__FileReaderInstance__
#define __bmp__FileReaderInstance__

#include "FileReader.h"
#include <fstream>

class FileReaderInstance : public  FileReader{
private:
    std::fstream   _file;
public:
    FileReaderInstance(const char *filename);
    uint8_t read();
    uint8_t peek();
    int available();
    bool seek(uint32_t pos);
    uint32_t position();
};

#endif /* defined(__bmp__FileReaderInstance__) */

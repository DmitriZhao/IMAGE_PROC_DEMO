//
//  FileReaderInstance.cpp
//  bmp
//

#include "FileReaderInstance.h"
FileReaderInstance::FileReaderInstance(const char *filename) : FileReader(filename){
    
    _fileName = filename;
    _file.open(_fileName);
}

uint8_t FileReaderInstance::read(){
    uint8_t byteData;
    _file >> byteData;
    return byteData;
};

uint8_t FileReaderInstance::peek(){
    uint8_t byteData = _file.peek();
    return byteData;
};

int FileReaderInstance::available(){
    return _file.is_open();
};

bool FileReaderInstance::seek(uint32_t pos){
    return (bool)_file.seekg(pos,std::ios::beg);
};

uint32_t FileReaderInstance::position(){
    return std::ios::cur;
};

#ifndef _IMAGE_READER_H
#define _IMAGE_READER_H
#include <iostream>
#include "FileReaderInstance.hpp"
#include "BmpReader.hpp"
#include "GreyScaleImage.hpp"

class ImageReader
{
public:
    BOOL init(const char* fileName);
    Size size() const;
    GreyScaleImage::Ptr getImage();
private:
    FileReaderInstance::Ptr _fileReader;
    BmpReader::Ptr _bmpReader;
    GreyScaleImage::Ptr _image;
};

inline BOOL ImageReader::init(const char* fileName)
{
    _fileReader = std::make_shared<FileReaderInstance>(fileName);
    if(!_fileReader->available())
    {
        std::cerr<<"BMP read failed, aborting"<<std::endl;
        return false;
    }
    _bmpReader = std::make_shared<BmpReader>(_fileReader);
    _image = std::make_shared<GreyScaleImage>(_bmpReader);
    return true;
}

inline Size ImageReader::size() const
{
    return _image->size();
}

inline GreyScaleImage::Ptr ImageReader::getImage()
{
    return _image->getPtr();
}

#endif // !_IMAGE_READER_H
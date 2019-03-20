#ifndef _IMAGE_RECEIVER_H
#define _IMAGE_RECEIVER_H

#include "GreyScaleImage.hpp"

class ImageReceiver
{
public:
    BOOL init(const uint8_t MT9V032_ImageBuffer[HEIGHT][WIDTH]);
    Size size() const;
    GreyScaleImage::Ptr getImage();
private:
    GreyScaleImage::Ptr _image;
};

inline BOOL ImageReceiver::init(const uint8_t MT9V032_ImageBuffer[HEIGHT][WIDTH])
{
    _image = std::make_shared<GreyScaleImage>(Size(HEIGHT,WIDTH), &MT9V032_ImageBuffer[0][0]);
    return true;
}

inline Size ImageReceiver::size() const
{
    return _image->size();
}

inline GreyScaleImage::Ptr ImageReceiver::getImage()
{
    return _image->getPtr();
}

#endif // !_IMAGE_RECEIVER_H
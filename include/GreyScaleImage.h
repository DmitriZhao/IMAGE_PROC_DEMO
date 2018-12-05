#ifndef _GREYSCALEIMAGE_H
#define _GREYSCALEIMAGE_H

#include "Types.h"
#include "BmpReader.hpp"
#include "ImageBase.hpp"

class BmpReader;

class GreyScaleImage : public std::enable_shared_from_this<GreyScaleImage>
{
private:
    ImageBase<Scalar>::Ptr _img;
    BYTE    _threshold = 100;
    BYTE    _otsu(COORD topBoundary);
public:
    explicit GreyScaleImage(BmpReader::Ptr bmpReader);
    typedef  std::shared_ptr<GreyScaleImage>    Ptr;
    Ptr      getPtr()                           {return shared_from_this();}
    void     evalThreshold(COORD topBoundary)   {_threshold = _otsu(topBoundary);}
    BYTE     threshold()                        {return _threshold;}
    Size     size()                             {return _img->size();}
    BYTE     read(COORD x, COORD y)             {return _img->read(x,y).value;}
    void     write(COORD x, COORD y, BYTE value){_img->write(x,y,Scalar(value));}
    void     show();
};

#endif
#ifndef _GREYSCALEIMAGE_H
#define _GREYSCALEIMAGE_H

#include "Types.h"
#include "BmpReader.h"

class BmpReader;

class GreyScaleImage
{
private:
    UINT8   **_data;
    Size    _size;
    UINT8   _threshold = 160;
    UINT32  _histrogram[256];
    UINT32  _histrogramIntegrated[256];
    UINT8   _otsu(COORD topBoundary);
public:
    explicit GreyScaleImage(BmpReader *r);
    ~GreyScaleImage();
    void    evalThreshold(COORD topBoundary)    {_threshold = _otsu(topBoundary);}
    UINT8   threshold()                         {return _threshold;}
    void    show    ();
    Size    size    ()                          {return _size;}
    UINT8   read    (COORD x, COORD y)          {return _data[y][x];}
    UINT8   read    (Point2D &p)                {return _data[p.y()][p.x()];}
    void    write   (COORD x, COORD y, UINT8 value)    {_data[y][x] = value;}
    void    write   (Point2D &p, UINT8 value)          {_data[p.y()][p.x()] = value;}
};

#endif
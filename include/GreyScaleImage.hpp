#ifndef _GREYSCALEIMAGE_H
#define _GREYSCALEIMAGE_H

#include "BmpReader.hpp"
#include "ImageBase.hpp"

class BmpReader;

class GreyScaleImage : public ImageBase<BYTE>, public std::enable_shared_from_this<GreyScaleImage>
{
private:
    BYTE    _threshold = 0x7f;
    BYTE    _otsu(COORD topBoundary);
public:
    GreyScaleImage(BmpReader::Ptr bmpReader);
    GreyScaleImage(const Size& size, BYTE defaultVal) : ImageBase<BYTE>(size, defaultVal){};
    typedef  std::shared_ptr<GreyScaleImage>    Ptr;
    Ptr      getPtr()                           {return shared_from_this();}
    BYTE     threshold()	                    {return _threshold;}
    void     setThreshold(BYTE value)           {_threshold = value;}
    void     show(ShowMethod method);
};

inline GreyScaleImage::GreyScaleImage(BmpReader::Ptr bmpReader) : ImageBase<BYTE>(Size(bmpReader->width(),bmpReader->height()), BYTE(0xFF))
{
	Size size(bmpReader->width(),bmpReader->height());

	for (COORD y = 0; y < size.y; y++)
		for (COORD x = 0; x < size.x; x++)
		{
			Pixel px = bmpReader->readPixel(x,y);	
			write(x, y, (px.r + px.g + px.b) / 3);
		}
}
  
inline void GreyScaleImage::show(ShowMethod method)
{
	for (COORD y = 0; y < size().y; y++)
	{
		for (COORD x = 0; x < size().x; x++)
		{
			BYTE byte = read(x, y);
			if(byThreshold == method)
			{
				if (byte > _threshold)
					std::cout << ' ';
				else
					std::cout << '*';
			}
			else if(byHex == method)
				std::cout<<std::hex<<(INT8)byte<<' ';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

#endif
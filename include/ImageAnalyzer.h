#ifndef _IMAGE_ANALYZER_H
#define _IMAGE_ANALYZER_H

#include "GreyScaleImage.hpp"

class ImageAnalyzer
{
private:
    GreyScaleImage::Ptr _img;
    GreyScaleImage::Ptr _gradMap;
    BYTE _otsu(COORD topBoundary);
    BYTE _threshold = 0x7f;
public:
    ImageAnalyzer(GreyScaleImage::Ptr img);
    typedef struct Operator
    {
        ImageBase<INT8> x;
        ImageBase<INT8> y;
    }Operator;
    const static Operator SOBEL;
    const static Operator SCHARR;
    void applyOperator(COORD x, COORD y, const Operator& op);
    void evalThreshold(COORD topBoundary)   {_img->setThreshold(_otsu(topBoundary));}
    void showGradMap(ShowMethod method);
};

#endif // !_IMAGE_ANALYZER_H
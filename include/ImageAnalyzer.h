#ifndef _IMAGE_ANALYZER_H
#define _IMAGE_ANALYZER_H

#include "GreyScaleImage.hpp"

class ImageAnalyzer
{
private:
    GreyScaleImage::Ptr _img;
    BYTE _otsu(COORD topBoundary);
public:
    ImageAnalyzer(GreyScaleImage::Ptr img);
    typedef struct Operator
    {
        ImageBase<INT8> x;
        ImageBase<INT8> y;
    }Operator;
    const static Operator SOBEL;
    const static Operator SCHARR;
    BYTE applyOperator(COORD x, COORD y, const Operator& op);
    void evalThreshold(COORD topBoundary)   {_img->setThreshold(_otsu(topBoundary));}
    GreyScaleImage::Ptr getGradMap(const Operator& op);
};

#endif // !_IMAGE_ANALYZER_H
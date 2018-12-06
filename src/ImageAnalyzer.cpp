#include <cmath>
#include <memory>
#include <iostream>
#include "ImageAnalyzer.h"

ImageAnalyzer::ImageAnalyzer(GreyScaleImage::Ptr img)
{
    _img = img;
    _gradMap = std::make_shared<GreyScaleImage>(_img->size(),0);
}

void ImageAnalyzer::applyOperator(COORD x, COORD y, const Operator& op)
{
	if(x<1 || x>_img->size().x-2 || y<1 || y>_img->size().y-2)
	{
		std::cerr << "out_of_range error"<<std::endl;
		return;
	}
	INT16 Gx = 0, Gy = 0;
	for(COORD i=0; i<3; i++)
	{
		for(COORD j=0; j<3; j++)
		{
			Gx += _img->read(x-1+i, y-1+j) * op.x.read(i,j);
			Gy += _img->read(x-1+i, y-1+j) * op.y.read(i,j);
		}
	}
	_gradMap->write(x, y ,(COORD)sqrt(Gx*Gx+Gy*Gy));
}

void ImageAnalyzer::showGradMap(ShowMethod method)
{
	for (COORD y = 0; y < _gradMap->size().y; y++)
	{
		for (COORD x = 0; x < _gradMap->size().x; x++)
		{
			BYTE byte = _gradMap->read(x, y);
			if(byThreshold == method)
			{
				if (byte < _threshold)
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

BYTE ImageAnalyzer::_otsu(const COORD topBoundary)
{
	UINT32	greyScaleMin = 0u;
	UINT32	greyScaleMax = 0u;
	UINT32	s_all_u = 0u;
	float	s_all_f = 0.0f;
	UINT32	n_0_u = 0u;
	float	n_0_f = 0.0f;
	UINT32	s_0 = 0u;
	float	VAR = 0.0f;
	BYTE	THRESHOLD = 0x00;
	UINT32  histrogram[256];
    UINT32  histrogramIntegrated[256];

	float ALL_PIXEL = (float)((_img->size().y - topBoundary) * _img->size().x);
	float ALL_PIXEL_SMALL = ALL_PIXEL * 0.0001f;

	// STEP1 - get the histrogram and grey-scale limits
	for (UINT32 _CLEAR = 0; _CLEAR < 256; _CLEAR++)
		histrogram[_CLEAR] = 0;

	for (uint_fast8_t y = topBoundary; y < _img->size().y; y++)
		for (uint_fast8_t x = 0; x < _img->size().x; x++)
		{
			UINT8 value = _img->read(x,y);
			histrogram[value]++;
		}

	for (greyScaleMin = 0; greyScaleMin < 256 && histrogram[greyScaleMin] == 0; greyScaleMin++)
		;
	for (greyScaleMax = 255; greyScaleMax > greyScaleMin && histrogram[greyScaleMax] == 0; greyScaleMax--)
		;

	// STEP2 - calculate s_all, the integration of the histrogram over grey-scale. Simultaneously, generate a integration table.
	// Do some scaling here to prevent over-flow
	for (UINT32 _LEVEL = 0; _LEVEL < greyScaleMin; _LEVEL++)
		histrogramIntegrated[_LEVEL] = 0;
	for (UINT32 _LEVEL = greyScaleMin; _LEVEL <= greyScaleMax; _LEVEL++)
	{
		s_all_u += (histrogram[_LEVEL] * _LEVEL);
		histrogramIntegrated[_LEVEL] = s_all_u;
	}
	s_all_f = (float)(s_all_u)*0.0001f;

	// STEP3 - calcaulate t, which is the threshold
	// avoid division to speed-up
	for (UINT32 _TH = greyScaleMin; _TH < greyScaleMax; _TH++)
	{
		n_0_u += histrogram[_TH];
		n_0_f = (float)(n_0_u);
		s_0 = histrogramIntegrated[_TH];

		float VAR_Current = 0.0f;
		VAR_Current = ((ALL_PIXEL_SMALL * (float)(s_0)-n_0_f * s_all_f)) * 0.000001f;
		VAR_Current *= VAR_Current;
		VAR_Current /= (n_0_f * (float)(ALL_PIXEL - n_0_f));

		if (VAR_Current > VAR)
		{
			THRESHOLD = _TH;
			VAR = VAR_Current;
		}
	}
	return THRESHOLD;
}

const ImageAnalyzer::Operator ImageAnalyzer::SOBEL = 
{
    ImageBase<INT8>(Size(3, 3), {-1, 0, 1,  -2, 0, 2,  -1, 0, 1}),
    ImageBase<INT8>(Size(3, 3), {-1,-2,-1,   0, 0, 0,   1, 2, 1})
};

const ImageAnalyzer::Operator ImageAnalyzer::SCHARR = 
{
    ImageBase<INT8>(Size(3, 3), {-3, 0, 3,  -10, 0, 10,  -3, 0, 3}),
    ImageBase<INT8>(Size(3, 3), {-3,-10,-3,   0, 0, 0,   3, 10, 3})
};
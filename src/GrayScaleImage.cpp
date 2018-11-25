#include <iostream>
#include "GreyScaleImage.h"

GreyScaleImage::GreyScaleImage(BmpReader* bmpReader)
{
	Size size(bmpReader->width(),bmpReader->height());

	_img = new ImageBase<Scalar>(size, Scalar(0xFF));

	for (COORD y = 0; y < size.y; y++)
	{
		for (COORD x = 0; x < size.x; x++)
		{
			Pixel px = bmpReader->readPixel(x,y);
			
			_img->write(x, y, (px.r + px.g + px.b) / 3);
		}
	}
	    
}

void GreyScaleImage::show()
{
	for (COORD y = 0; y < size().y; y++)
	{
		for (COORD x = 0; x < size().x; x++)
		{
			BYTE byte = read(x, y);
			//std::cout<<std::hex<<(int)byte<<' ';
			if (byte > _threshold)
				std::cout << ' ';
			else
				std::cout << '*';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

BYTE GreyScaleImage::_otsu(COORD topBoundary)
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

	float ALL_PIXEL = (float)((size().y - topBoundary) * size().x);
	float ALL_PIXEL_SMALL = ALL_PIXEL * 0.0001f;

	// STEP1 - get the histrogram and grey-scale limits
	for (uint32_t _CLEAR = 0; _CLEAR < 256; _CLEAR++)
		histrogram[_CLEAR] = 0;

	for (uint_fast8_t y = topBoundary; y < size().y; y++)
		for (uint_fast8_t x = 0; x < size().x; x++)
		{
			UINT8 value = read(x,y);
			histrogram[value]++;
		}

	for (greyScaleMin = 0; greyScaleMin < 256 && histrogram[greyScaleMin] == 0; greyScaleMin++)
		;
	for (greyScaleMax = 255; greyScaleMax > greyScaleMin && histrogram[greyScaleMax] == 0; greyScaleMax--)
		;

	// STEP2 - calculate s_all, the integration of the histrogram over grey-scale. Simultaneously, generate a integration table.
	// Do some scaling here to prevent over-flow
	for (uint32_t _LEVEL = 0; _LEVEL < greyScaleMin; _LEVEL++)
		histrogramIntegrated[_LEVEL] = 0;
	for (uint32_t _LEVEL = greyScaleMin; _LEVEL <= greyScaleMax; _LEVEL++)
	{
		s_all_u += (histrogram[_LEVEL] * _LEVEL);
		histrogramIntegrated[_LEVEL] = s_all_u;
	}
	s_all_f = (float)(s_all_u)*0.0001f;

	// STEP3 - calcaulate t, which is the threshold
	// avoid division to speed-up
	for (uint32_t _TH = greyScaleMin; _TH < greyScaleMax; _TH++)
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
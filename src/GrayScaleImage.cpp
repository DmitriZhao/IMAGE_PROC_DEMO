#include <iostream>
#include <algorithm>
#include "GreyScaleImage.h"

GreyScaleImage::GreyScaleImage(BmpReader *r)
{
	_size.x = r->width();
	_size.y = r->height();
	_data = new PUINT8[_size.y];

	for (COORD y = 0; y < _size.y; y++)
		_data[y] = new UINT8[_size.x];

	std::fill(_data[0], _data[0] + _size.area(), '*');

	for (int y = 0; y < _size.y; y++)
	{
		for (int x = 0; x < _size.x; x++)
		{
			UINT8 blue = r->readByte(x, y);
			UINT8 green = r->readByte(x, y);
			UINT8 red = r->readByte(x, y);
			_data[y][x] = (blue + green + red) / 3.0;
		}
	}
}

GreyScaleImage::~GreyScaleImage()
{
	for (COORD y = 0; y < _size.y; y++)
		delete[] _data[y];

	_data = NULL;
}

void GreyScaleImage::show()
{
	for (int y = 0; y < _size.y; y++)
	{
		for (int x = 0; x < _size.x; x++)
		{
			int byte = read(x, y);
			if (byte > _threshold)
				std::cout << ' ';
			else
				std::cout << '*';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

UINT8 GreyScaleImage::_otsu(COORD topBoundary)
{
	uint32_t	greyScaleMin = 0u;
	uint32_t	greyScaleMax = 0u;
	uint32_t	s_all_u = 0u;
	float		s_all_f = 0.0f;
	uint32_t	n_0_u = 0u;
	float		n_0_f = 0.0f;
	uint32_t	s_0 = 0u;
	float		VAR = 0.0f;
	uint8_t		THRESHOLD = 0x00;

	float ALL_PIXEL = (float)((_size.y - topBoundary) * _size.x);
	float ALL_PIXEL_SMALL = ALL_PIXEL * 0.0001f;

	// STEP1 - get the histrogram and grey-scale limits
	for (uint32_t _CLEAR = 0; _CLEAR < 256; _CLEAR++)
		_histrogram[_CLEAR] = 0;

	for (uint_fast8_t y = topBoundary; y < _size.y; y++)
		for (uint_fast8_t x = 0; x < _size.x; x++)
		{
			UINT8 value = _data[y][x];
			_histrogram[value]++;
		}

	for (greyScaleMin = 0; greyScaleMin < 256 && _histrogram[greyScaleMin] == 0; greyScaleMin++)
		;
	for (greyScaleMax = 255; greyScaleMax > greyScaleMin && _histrogram[greyScaleMax] == 0; greyScaleMax--)
		;

	// STEP2 - calculate s_all, the integration of the histrogram over grey-scale. Simultaneously, generate a integration table.
	// Do some scaling here to prevent over-flow
	for (uint32_t _LEVEL = 0; _LEVEL < greyScaleMin; _LEVEL++)
		_histrogramIntegrated[_LEVEL] = 0;
	for (uint32_t _LEVEL = greyScaleMin; _LEVEL <= greyScaleMax; _LEVEL++)
	{
		s_all_u += (_histrogram[_LEVEL] * _LEVEL);
		_histrogramIntegrated[_LEVEL] = s_all_u;
	}
	s_all_f = (float)(s_all_u)*0.0001f;

	// STEP3 - calcaulate t, which is the threshold
	// avoid division to speed-up
	for (uint32_t _TH = greyScaleMin; _TH < greyScaleMax; _TH++)
	{
		n_0_u += _histrogram[_TH];
		n_0_f = (float)(n_0_u);
		s_0 = _histrogramIntegrated[_TH];

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
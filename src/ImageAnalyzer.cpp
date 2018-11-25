#include <iostream>
#include "Types.h"
#include "GreyScaleImage.h"
#include "ImageAnalyzer.h"

ImageAnalyzer::ImageAnalyzer(GreyScaleImage *img)
{
    _img = img;
    _result = new ImageBase<BYTE>(_img->size(),' ');
}

void ImageAnalyzer::findPath()
{
    if(_findRoot(_img->size().y - 1))
    {
        Point leftRoot = _left.back();
        Point rightRoot = _right.back();
        _dfs(leftRoot, _left);
        _dfs(rightRoot, _right);
    }

    ELEMENT pathSize = (_right.size()<_left.size()) ? _right.size() : _left.size();

    for(ELEMENT i=0; i<pathSize; i++)
    {
        if(_left[i].y != _right[i].y)
            std::cerr << "ERROR" << std::endl;
        Point midPoint((_left[i].x+_right[i].x)/2, _left[i].y);
        _mid.push_back(midPoint);
    }

    for(Point p : _left)
        _result->write(p.x,p.y,'L');

    for(Point p : _right)
        _result->write(p.x,p.y,'R');
        
    for(Point p : _mid)
        _result->write(p.x,p.y,'M');
}

void ImageAnalyzer::show()
{
    for (int y = 0; y < _img->size().y; y++)
	{
		for (int x = 0; x < _img->size().x; x++)
		{
            // if(!_result->read(x,y))
            //     std::cout<<' ';
			std::cout << _result->read(x,y);
		}
		std::cout << std::endl;
	}
    std::cout << std::endl;
}

void ImageAnalyzer::_dfs(Point &root, std::vector<Point> &edge)
{
    INT8 sequence[] = { 0,1,-1,2,-2,3,-3 };
    BOOL  flag = 1;
    while(flag)
    {
        flag = 0;
        for(INT8 i = 0; i < sizeof(sequence)/sizeof(*sequence); i++)
        {
            Point p(root.x+sequence[i],root.y-1);
            if(_bInImage(p) && _bOnEdge(p) && _img->read(p.x,p.y) > _img->threshold())
            {
                edge.push_back(p);
                root = p;
                flag = 1;
                break;
            }
        }
    }
}

BOOL ImageAnalyzer::_findRoot(COORD bottom)
{
    _left.clear();
    _right.clear();
    for (COORD x = 0; x < _img->size().x; x++)
    {
        if (_img->read(x,bottom) > _img->threshold())
        {
            if (   _img->read(x+1,bottom) > _img->threshold() 
                && _img->read(x-1,bottom) < _img->threshold())
            {
                Point p(x, bottom);
                _left.push_back(p);
            }
            else if (  _img->read(x-1,bottom) > _img->threshold() 
                    && _img->read(x+1,bottom) < _img->threshold())
            {
                Point p(x, bottom);
                _right.push_back(p);
            }
        }
    }
    //TODO: verify by k, crossroads

    if(1)
    {
        return true;
    }
}

BOOL ImageAnalyzer::_bOnEdge(Point &p)
{
	UINT8 counter = 0;
	if (_img->size().x-1 == p.x)     //右边沿
	{
		counter = _img->read(p.x,  p.y+1) > _img->threshold()
                + _img->read(p.x,  p.y-1) > _img->threshold()
                + _img->read(p.x-1,p.y  ) > _img->threshold()
                + _img->read(p.x-1,p.y+1) > _img->threshold()
                + _img->read(p.x-1,p.y-1) > _img->threshold();
                //上+下+左+左上+左下
		//if (counter <= 3)
			return true;
	}
	else if (0 == p.x)		//左边沿
	{
        counter = _img->read(p.x,  p.y+1) > _img->threshold()
                + _img->read(p.x,  p.y-1) > _img->threshold()
                + _img->read(p.x+1,p.y  ) > _img->threshold()
                + _img->read(p.x+1,p.y+1) > _img->threshold()
                + _img->read(p.x+1,p.y-1) > _img->threshold();
                //上+下+右+右上+右下
		//if (counter <= 3)
			return true;
	}
	else
	{
		counter = _img->read(p.x,  p.y+1) > _img->threshold()
                + _img->read(p.x,  p.y-1) > _img->threshold()
                + _img->read(p.x+1,p.y  ) > _img->threshold()
                + _img->read(p.x+1,p.y+1) > _img->threshold()
                + _img->read(p.x+1,p.y-1) > _img->threshold()
                + _img->read(p.x-1,p.y  ) > _img->threshold()
                + _img->read(p.x-1,p.y+1) > _img->threshold()
                + _img->read(p.x-1,p.y-1) > _img->threshold();
		if (counter <= 7)
			return true;
	}
	return false;
}
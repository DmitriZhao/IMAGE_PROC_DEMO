#include <iostream>
#include <algorithm>
#include "Types.h"
#include "GreyScaleImage.h"
#include "ImageAnalyzer.h"

ImageAnalyzer::ImageAnalyzer(GreyScaleImage::Ptr img)
{
    _img = img;
    _result = std::make_shared<ImageBase<BYTE>>(_img->size(),' ');
    for(COORD x = 0; x < _img->size().x; x++)
    {
        for(COORD y = 0; y < _img->size().y; y++)
            _result->write(x,y,_img->read(x,y)>_img->threshold()?' ':'*');
    }
}

BOOL ImageAnalyzer::findPath()
{
    COORD row = 1;
    for(COORD row = 0; row < _img->size().y; row++)
    {
        if(_findRoot(_img->size().y - row - 1))     //write roots into _left & _right if successful
        {
            Vec2D leftRoot = _left.back();
            Vec2D rightRoot = _right.back();
            _dfs(leftRoot, _left,UP);               //extend _left and _right in straight lane if successful
            _dfs(rightRoot, _right,UP);
            //std::vector<Vec2D> left, right, leftTemp, rightTemp;;
            // for(auto left_iter = _left.begin(), right_iter = _right.begin();
            //     left_iter!=_left.end() && right_iter!=_right.end();
            //     left_iter++, right_iter++)
            // {
            //     Vec2D leftRoot = *left_iter;
            //     Vec2D rightRoot = *right_iter;
            //     leftTemp.clear();
            //     rightTemp.clear();
            //     _dfs(leftRoot, leftTemp,UP);
            //     _dfs(rightRoot, rightTemp,UP);
            //     if(leftTemp.size()>left.size())
            //         left = leftTemp;
            //     if(rightTemp.size()>right.size())
            //         right = rightTemp;
            // }
            // _left.insert(_left.end(),leftTemp.begin(),leftTemp.end());
            // _right.insert(_right.end(),rightTemp.begin(),rightTemp.end());
            break;
        }
        // if(row > _img->size().y * 2 / 3)
        //     return false;
    }

    //find midpoint on straight lane
    Point pLeft = _left.begin(), pRight = _right.begin();
    for(; pLeft < _left.end() && pRight < _right.end();
          pLeft++,               pRight++)
        _mid.push_back((*pLeft + *pRight)/2);

    COORD leftInflectionOffset = pLeft - _left.begin();      //record inflection points
    COORD rightInflectionOffset = pRight - _right.begin();

    Path& shorterPath = (_left.size()<_right.size()) ? _left : _right;
    Path& longerPath  = (_left.size()>_right.size()) ? _left : _right;

    _dfs(_left.back(), _left, LEFT);       //extend _left and _right in curved lane if successful
    _dfs(_right.back(), _right, LEFT);

    //find midpoint on curved lane
    Point lastPoint = (_left.size()<_right.size()) ? (_left.begin()+leftInflectionOffset) : (_right.begin()+rightInflectionOffset);
    Point source    = (_left.size()>_right.size()) ? (_left.begin()+leftInflectionOffset) : (_right.begin()+rightInflectionOffset);;

    for(; source != longerPath.end(); source++)
    {
        _mid.push_back((*source + *lastPoint) / 2);

        if(lastPoint + 2 < shorterPath.end())
            lastPoint = _closestPoint(source,{lastPoint,lastPoint+1,lastPoint+2});
        else
            break;
    }

    for(auto p : _left)
        _result->write(p.x,p.y,'L');

    for(auto p : _right)
        _result->write(p.x,p.y,'R');
        
    for(auto p : _mid)
        _result->write(p.x,p.y,'M');

    _result->write(_left.at(leftInflectionOffset).x,_left.at(leftInflectionOffset).y,'X');
    _result->write(_right.at(rightInflectionOffset).x,_right.at(rightInflectionOffset).y,'Y');
    return true;
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

void ImageAnalyzer::_dfs(Vec2D &root, Path &edge, Vec2D direction)
{
    std::vector<SIGNED_COORD> horizontalSearchingDistanceQueue = { 0,1,-1,2,-2,3,-3 };
    if(direction.x)
        horizontalSearchingDistanceQueue =  { 0,1,-1,2,-2 };
    BOOL  continuous = 1;
    while(continuous)
    {
        continuous = 0;
        for(SIGNED_COORD s : horizontalSearchingDistanceQueue)
        {
            BOOL searching = 1;
            for(SIGNED_COORD depth = 1; depth <= 2; depth++)
            {
                //Vec2D p(root.x+(direction.y!=0)*horizontal[i]+direction.x, root.y+(direction.x!=0)*horizontal[i]+direction.y);
                Vec2D p = root + direction * depth + direction.horizontal() * s;
                if(_bInImage(p) && _img->read(p.x,p.y) > _img->threshold() && _bOnEdge(p))
                {
                    edge.push_back(p);
                    root = p;
                    continuous = 1;
                    searching = 0;
                    break;
                }
            }
            if(!searching)
                break;
        }
    }
}

BOOL ImageAnalyzer::_findRoot(COORD bottom)
{
    _left.clear();
    _right.clear();
    for (COORD x = 0; x < _img->size().x; x++)
    {
        BYTE val = _img->read(x,bottom);
        if (val > _img->threshold())
        {
            if (   _img->read(x+1,bottom) > _img->threshold() 
                && _img->read(x-1,bottom) < _img->threshold())
            {
                Vec2D p(x, bottom);
                _left.push_back(p);
            }
            else if (  _img->read(x-1,bottom) > _img->threshold() 
                    && _img->read(x+1,bottom) < _img->threshold())
            {
                Vec2D p(x, bottom);
                _right.push_back(p);
            }
        }
    }
    //TODO: verify by k, crossroads

    if(_left.size() && _right.size())
        return true;
    return false;
}

BOOL ImageAnalyzer::_bOnEdge(Vec2D &p)
{
	UINT8 counter = 0;
	if (_img->size().x-1 == p.x)     //右边沿
	{
		counter = (_img->read(p.x,  p.y+1) > _img->threshold())
                + (_img->read(p.x,  p.y-1) > _img->threshold())
                + (_img->read(p.x-1,p.y  ) > _img->threshold())
                + (_img->read(p.x-1,p.y+1) > _img->threshold())
                + (_img->read(p.x-1,p.y-1) > _img->threshold());
                //上+下+左+左上+左下
		//if (counter <= 3)
			return true;
	}
	else if (0 == p.x)		//左边沿
	{
        counter = (_img->read(p.x,  p.y+1) > _img->threshold())
                + (_img->read(p.x,  p.y-1) > _img->threshold())
                + (_img->read(p.x+1,p.y  ) > _img->threshold())
                + (_img->read(p.x+1,p.y+1) > _img->threshold())
                + (_img->read(p.x+1,p.y-1) > _img->threshold());
                //上+下+右+右上+右下
		//if (counter <= 3)
			return true;
	}
	else
	{
		counter = (_img->read(p.x,  p.y+1) > _img->threshold())
                + (_img->read(p.x,  p.y-1) > _img->threshold())
                + (_img->read(p.x+1,p.y  ) > _img->threshold())
                + (_img->read(p.x+1,p.y+1) > _img->threshold())
                + (_img->read(p.x+1,p.y-1) > _img->threshold())
                + (_img->read(p.x-1,p.y  ) > _img->threshold())
                + (_img->read(p.x-1,p.y+1) > _img->threshold())
                + (_img->read(p.x-1,p.y-1) > _img->threshold());
		if (counter <= 6)
			return true;
	}
	return false;
}

Point ImageAnalyzer::_closestPoint(Point &source, std::initializer_list<Point> init_list)
{
    Point tempPoint;
    ELEMENT tempDistance = ELEMENT(-1); //MAXIUM of unsigned int
    
    for(Point currentPoint : init_list)
    {
        ELEMENT currentDistance = _distanceSquared(*source,*currentPoint);
        if(currentDistance < tempDistance)
        {
            tempDistance = currentDistance;
            tempPoint = currentPoint;
        }
    }
    return tempPoint;
}
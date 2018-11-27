#include <iostream>
#include <algorithm>
#include "Types.h"
#include "GreyScaleImage.h"
#include "ImageAnalyzer.h"

ImageAnalyzer::ImageAnalyzer(GreyScaleImage *img)
{
    _img = img;
    _result = new ImageBase<BYTE>(_img->size(),' ');
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
        if(_findRoot(_img->size().y - row - 1))
        {
            Vec2D leftRoot = _left.back();
            Vec2D rightRoot = _right.back();
            _dfs(leftRoot, _left,UP);
            _dfs(rightRoot, _right,UP);
            _dfs(_left.back(), _left,RIGHT);
            _dfs(_right.back(), _right,RIGHT);

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

    std::vector<Vec2D>& shorterPath = (_right.size()<_left.size()) ? _right : _left;
    std::vector<Vec2D>& longerPath = (_right.size()>_left.size()) ? _right : _left;
    auto lastPoint = shorterPath.begin();
    for(Vec2D source : longerPath)
    {
        _mid.push_back((source + *lastPoint) / 2);

        if(shorterPath.end() != (lastPoint+2))
        {
            Vec2D first  = *(lastPoint);
            Vec2D second = *(lastPoint+1);
            Vec2D third  = *(lastPoint+2);
            lastPoint += _closestPoint(source,{first,second,third});
        }
        else
            break;
    }

    for(Vec2D p : _left)
        _result->write(p.x,p.y,'L');

    for(Vec2D p : _right)
        _result->write(p.x,p.y,'R');
        
    //  for(Vec2D p : _mid)
    //      _result->write(p.x,p.y,'M');
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

void ImageAnalyzer::_dfs(Vec2D &root, std::vector<Vec2D> &edge, Vec2D direction)
{
    std::vector<SIGNED_COORD> sequence = { 0,1,-1,2,-2,3,-3 };
    if(direction.x)
        sequence =  { 0,1,-1,2,-2};
    BOOL  continuous = 1;
    while(continuous)
    {
        continuous = 0;
        for(SIGNED_COORD s : sequence)
        {
            BOOL searchingDepth = 1;
            for(SIGNED_COORD depth = 1; depth <= 2; depth++)
            {
                //Vec2D p(root.x+(direction.y!=0)*sequence[i]+direction.x, root.y+(direction.x!=0)*sequence[i]+direction.y);
                Vec2D p = root + direction * depth + direction.vertical() * s;
                if(_bInImage(p) && _img->read(p.x,p.y) > _img->threshold() && _bOnEdge(p))
                {
                    edge.push_back(p);
                    root = p;
                    continuous = 1;
                    searchingDepth = 0;
                    break;
                }
            }
            if(!searchingDepth)
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
        if (_img->read(x,bottom) > _img->threshold())
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
		if (counter <= 7)
			return true;
	}
	return false;
}

BYTE ImageAnalyzer::_closestPoint(Vec2D &source, std::initializer_list<Vec2D> init_list)
{
    auto tempPoint = init_list.begin();
    Vec2D tempPointValue = *tempPoint;
    ELEMENT tempDistance = _distanceSquared(source, tempPointValue);
    
    for(auto i = init_list.begin()+1; i!=init_list.end(); i++)
    {
        auto currentPoint = i;
        Vec2D currentPointValue = *currentPoint;
        ELEMENT currentDistance = _distanceSquared(source,currentPointValue);
        if(currentDistance < tempDistance)
        {
            tempDistance = currentDistance;
            tempPoint = currentPoint;
        }
    }
    return tempPoint-init_list.begin();
}
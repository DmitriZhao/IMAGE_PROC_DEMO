#include <iostream>
#include <algorithm>
#include "Types.h"
#include "GreyScaleImage.hpp"
#include "LaneAnalyzer.h"

LaneAnalyzer::LaneAnalyzer(GreyScaleImage::Ptr img)
{
    _img = img;
    _analyzer = new ImageAnalyzer(_img);
    _result = std::make_shared<GreyScaleImage>(_img->size(),' ');
    for(COORD x = 0; x < _img->size().x; x++)
    {
        for(COORD y = 0; y < _img->size().y; y++)
            _result->write(x,y,_img->read(x,y)>_img->threshold()?' ':'*');
    }
}

BOOL LaneAnalyzer::findPath()
{
    // COORD row = 1;
    // for(COORD row = 0; row < _img->size().y; row++)
    // {
    //     if(_findRoot(_img->size().y - row - 3))     //write roots into _left & _right if successful
    //     {
    //         Vec4D leftRoot = _left.back();
    //         Vec4D rightRoot = _right.back();
    //         _dfs(leftRoot, _left,UP);               //extend _left and _right in straight lane if successful
    //         _dfs(rightRoot, _right,UP);
    //         // std::vector<Vec2D> left, right, leftTemp, rightTemp;;
    //         // for(auto left_iter = _left.begin(), right_iter = _right.begin();
    //         //     left_iter!=_left.end() && right_iter!=_right.end();
    //         //     left_iter++, right_iter++)
    //         // {
    //         //     Vec2D leftRoot = *left_iter;
    //         //     Vec2D rightRoot = *right_iter;
    //         //     leftTemp.clear();
    //         //     rightTemp.clear();
    //         //     _dfs(leftRoot, leftTemp,UP);
    //         //     _dfs(rightRoot, rightTemp,UP);
    //         //     if(leftTemp.size()>left.size())
    //         //         left = leftTemp;
    //         //     if(rightTemp.size()>right.size())
    //         //         right = rightTemp;
    //         // }
    //         // _left.insert(_left.end(),leftTemp.begin(),leftTemp.end());
    //         // _right.insert(_right.end(),rightTemp.begin(),rightTemp.end());
    //         break;
    //     }
    //     // if(row > _img->size().y * 2 / 3)
    //     //     return false;
    // }

   	// for(COORD y=2; y<_img->size().y; y++)
	// {
	// 	for(COORD x = 0; x < _img->size().x; x++)
	// 	{
	// 		if(_bOnEdge(Vec2D(x,y)))
	// 			std::cout<<'*';
	// 		else
	// 			std::cout<<' ';
	// 	}
	// 	std::cout << std::endl;
	// }

    COORD bottom = _img->size().y;
    do{
        _left_1.clear();
        _right_1.clear();
        do
        {
            bottom -= 3;
        }
        while(!_findRoot(bottom, _left_1, _right_1));

        _dfs(_left_1.at(0), _left_1,UP);               //extend _left and _right in straight lane if successful
        _dfs(_right_1.at(0), _right_1,UP);
    }
    while(_left_1.size()<5 || _right_1.size()<5);
/*
    //find midpoint on straight lane
    Point pLeft = _left.begin(), pRight = _right.begin();
    for(; pLeft < _left.end() && pRight < _right.end();
          pLeft++,               pRight++)
        _mid.push_back((*pLeft + *pRight)/2);

    COORD leftInflectionOffset = pLeft - _left.begin();      //record inflection points
    COORD rightInflectionOffset = pRight - _right.begin();

    Path& shorterPath = (_left.size()<_right.size()) ? _left : _right;
    Path& longerPath  = (_left.size()>_right.size()) ? _left : _right;
*/
    // COORD leftInflectionOffset = _left.size();      //record inflection points
    // COORD rightInflectionOffset = _right.size();

    // for(auto p : _left)
    //     std::cout<<p.direction.x<<' '<<p.direction.y<<std::endl;
    
    // std::cout<<"==================RUA===============\n";
    // for(auto p : _right)
    //     std::cout<<p.direction.x<<' '<<p.direction.y<<std::endl;
    // std::cout << "LEFT VARIANCE: " << (int)_variance(_left) << std::endl;
    // std::cout << "RIGHT VARIANCE: " << (int)_variance(_right) << std::endl;

    COORD leftBoundaryCount, rightBoundaryCount;
    for(auto p : _left_1)
    {
        if(p.point.x <= 2 || p.point.x>=_img->size().x-3)
        leftBoundaryCount++;
    }
    for(auto p : _right_1)
    {
        if(p.point.x <= 2 || p.point.x>=_img->size().x-3)
        rightBoundaryCount++;
    }

    if(_variance(_left_1) + _variance(_right_1) > 6 || leftBoundaryCount >=5 || rightBoundaryCount >=5)
    {
        if(abs(_left_1.back().direction.x + _left_1.back().direction.y)
         + abs(_right_1.back().direction.x + _right_1.back().direction.y) > 10)
            _laneType = rightTurn;
        else
            _laneType = leftTurn;
    }
    else
        _laneType = crossRoad;
    
    if(crossRoad == _laneType)
    {
        COORD bottom = _left_1.back().point.y < _right_1.back().point.y ? _left_1.back().point.y : _right_1.back().point.y;
        do{
            _left_2.clear();
            _right_2.clear();
            do
            {
                bottom -= 3;
            }
            while(!_findRoot(bottom, _left_2, _right_2) && bottom >50);

            if(bottom <= 50)
                break;

            _dfs(_left_2.at(0), _left_2, UP);               //extend _left and _right in straight lane if successful
            _dfs(_right_2.at(0), _right_2, UP);
        }
        while((_left_2.size()<5 || _right_2.size()<5) && bottom >50);
    }
    else
    {
        _left_2.push_back(_left_1.back());
        _right_2.push_back(_right_1.back());  
        if(leftTurn ==_laneType)
        {
            _dfs(_left_1.back(), _left_2, LEFT);
            _dfs(_right_1.back(), _right_2, LEFT);
        }
        else if(rightTurn ==_laneType)
        {
            _dfs(_left_1.back(), _left_2, RIGHT);
            _dfs(_right_1.back(), _right_2, RIGHT);
        }
        BOOL bLeftIsLonger = _left_1.size()+_left_2.size() > _right_1.size()+_right_2.size();
        Path& shorterPath_1 = bLeftIsLonger ? _right_1 : _left_1;
        Path& shorterPath_2 = bLeftIsLonger ? _right_2 : _left_2;
        Path& longerPath_1  = bLeftIsLonger ? _left_1 : _right_1;
        Path& longerPath_2  = bLeftIsLonger ? _left_2 : _right_2;
        
        //find midpoint on straight lane
        Point pShort = shorterPath_1.begin(), pLong = longerPath_1.begin();

        while(pShort < shorterPath_1.end() - 1)
        {
            if(pShort->point.y == pLong->point.y)
            {
                _mid.push_back({(pShort->point+pLong->point)/2, Vec2D(0,0)});
                pShort++;
                pLong++;
            }
            else if(pShort->point.y > pLong->point.y)
                pShort++;
            else
                pLong++;
        } //NOTE: 假设 longerPath_1.back().point.y < shorterPath_1.back().point.y

        INT8 coeff = rightTurn == _laneType ? 1 : -1;
        while((pShort->point.x - pLong->point.x) * coeff > 0)
        {
            _mid.push_back({(pShort->point+pLong->point)/2, Vec2D(0,0)});
            pLong++;
            if(pLong == longerPath_1.end())
                pLong = longerPath_2.begin();
        }
        pShort = shorterPath_2.begin();

        while(pShort < shorterPath_2.end() && pLong < longerPath_2.end())
        {
            if(pShort->point.x == pLong->point.x)
            {
                _mid.push_back({(pShort->point+pLong->point)/2, Vec2D(0,0)});
                pShort++;
                pLong++;
            }
            else if((pShort->point.x - pLong->point.x) * coeff < 0)
                pShort++;
            else
                pLong++;
        }
    }

    // _dfs(_left.back(), _left, LEFT);       //extend _left and _right in curved lane if successful
    // _dfs(_right.back(), _right, LEFT);

/*
    //find midpoint on curved lane
    Point lastPoint = (_left.size()<_right.size()) ? (_left.begin()+leftInflectionOffset) : (_right.begin()+rightInflectionOffset);
    Point source    = (_left.size()>_right.size()) ? (_left.begin()+leftInflectionOffset) : (_right.begin()+rightInflectionOffset);;

    for(; source < longerPath.end() && (*source).x && (*source).y; source++)
    {
        _mid.push_back((*source + *lastPoint) / 2);

        if(lastPoint + 2 < shorterPath.end())
            lastPoint = _closestPoint(source,{lastPoint,lastPoint+1,lastPoint+2});
        else
            break;
    }
*/
    for(auto p : _left_1)
        _result->write(p.point.x, p.point.y, 'L');

    for(auto p : _right_1)
        _result->write(p.point.x, p.point.y, 'R');

    for(auto p : _left_2)
        _result->write(p.point.x, p.point.y, 'X');

    for(auto p : _right_2)
        _result->write(p.point.x, p.point.y, 'Y');
        
    for(auto p : _mid)
        _result->write(p.point.x, p.point.y, 'M');

    // _result->write(_left_1.at(leftInflectionOffset-1).point.x,_left_1.at(leftInflectionOffset-1).point.y,'X');
    // _result->write(_right_1.at(rightInflectionOffset-1).point.x,_right_1.at(rightInflectionOffset-1).point.y,'Y');
    return true;
}

void LaneAnalyzer::show()
{
    for (int y = 0; y < _img->size().y; y++)
	{
		for (int x = 0; x < _img->size().x; x++)
		{
            // if(!_result->read(x,y))
            //     std::cout<<'?';
			std::cout << _result->read(x,y);
		}
		std::cout << std::endl;
	}
    std::cout << std::endl;
}

void LaneAnalyzer::showLaneType()
{
    switch(_laneType)
    {
        case leftTurn:
            std::cout << "Left Turn\n";
            break;
        case rightTurn:
            std::cout << "Right Turn\n";
            break;
        case crossRoad:
            std::cout << "Cross Road\n";
            break;
    }
}

void LaneAnalyzer::_dfs(const Vec4D& root, Path& edge, const Vec2D& direction)
{
    Vec4D rootTemp = root;
    std::vector<SIGNED_COORD> horizontalSearchingDistanceQueue = { 0,1,-1,2,-2,3,-3 };
    if(direction.x)
        horizontalSearchingDistanceQueue =  { 0,1,-1,2,-2 };
    UINT8 counter = 0;
    BOOL  continuous = 1;
    while(continuous)
    {
        continuous = 0; 
        for(SIGNED_COORD depth = 1; depth <= 2; depth++)
        {
            BOOL searching = 1;
            for(SIGNED_COORD s : horizontalSearchingDistanceQueue)
            {
                Vec4D p4D;
                //Vec2D p(root.x+(direction.y!=0)*horizontal[i]+direction.x, root.y+(direction.x!=0)*horizontal[i]+direction.y);
                Vec2D p = rootTemp.point + direction * depth + direction.horizontal() * s;
                if(_bInImage(p) && _img->read(p.x,p.y) > _img->threshold() &&/* p.y >_img->size().y/2 &&*/ _bOnEdge(p))
                {
                    p4D.point = p;
                    p4D.direction = _foo(p);
                    if(abs(p4D.direction.x - rootTemp.direction.x)<= 3
                    && abs(p4D.direction.y - rootTemp.direction.y)<= 3)
                    {
                        edge.push_back(p4D);
                        if((p4D.direction.x - rootTemp.direction.x)*(p4D.direction.x - rootTemp.direction.x)
                         + (p4D.direction.y - rootTemp.direction.y)*(p4D.direction.y - rootTemp.direction.y) >= 3)
                            counter++;
                        rootTemp = p4D;
                        continuous = 1;
                        searching = 0;
                        break;
                    }

                }
            }
            if(!searching)
                break;
        }
    }
    // std::cout<< "counter: " <<(int)counter << std::endl;
}

BOOL LaneAnalyzer::_findRoot(COORD bottom, Path& leftEdge, Path& rightEdge)
{
    /*
    _left.clear();
    _right.clear();
    for (COORD x = 1; x < _img->size().x-1; x++)
    {
        BYTE val = _img->read(x,bottom);
        if (val > _img->threshold())
        {
            if (   _img->read(x+1,bottom) >= _img->threshold() 
                && _img->read(x-1,bottom) < _img->threshold())
            {
                Vec2D p(x, bottom);
                Vec4D p4D = {p, _foo(p)};
                _left.push_back(p4D);
            }
            else if (  _img->read(x-1,bottom) >= _img->threshold() 
                    && _img->read(x+1,bottom) < _img->threshold())
            {
                Vec2D p(x, bottom);
                Vec4D p4D = {p, _foo(p)};
                _right.push_back(p4D);
            }
        }
    }
    //TODO: verify by k, crossroads

    if(_left.size() && _right.size())
        return true;
    size_t ls = _left.size();
    size_t rs = _right.size();
    return false;
    */
    Path root;
    for(COORD x = 2; x < _img->size().x-2; x++)
    {
        Vec2D p = Vec2D(x,bottom);
        if(_bOnEdge(Vec2D(x,bottom)) && _img->read(x,bottom) > _img->threshold())
        {
            root.push_back({p, _foo(p)});
            x+=5;
        }
    }

    if(2 == root.size())
    {
        if(2 ==root.at(0).point.x && _img->size().x-3 == root.at(1).point.x)
            return false;
        else
        {
            leftEdge.push_back(root.at(0));
            rightEdge.push_back(root.at(1));
            return true;
        }
    }
    else if(4 == root.size())
    {
        if(root.at(1).point.x - root.at(0).point.x
         > root.at(3).point.x - root.at(2).point.x)
        {
            leftEdge.push_back(root.at(0));
            rightEdge.push_back(root.at(1));
            return true;
        }
        else
        {
            leftEdge.push_back(root.at(2));
            rightEdge.push_back(root.at(3));
            return true;
        }
    }
    return false;
}

BOOL LaneAnalyzer::_bOnEdge(const Vec2D &p)
{
	UINT8 counter = 0;
	if (_img->size().x-3 == p.x || _img->size().x-2 == p.x)     //右边沿
	{
		// counter = (_img->read(p.x,  p.y+1) > _img->threshold())
        //         + (_img->read(p.x,  p.y-1) > _img->threshold())
        //         + (_img->read(p.x-1,p.y  ) > _img->threshold())
        //         + (_img->read(p.x-1,p.y+1) > _img->threshold())
        //         + (_img->read(p.x-1,p.y-1) > _img->threshold());
                //上+下+左+左上+左下
		//if (counter <= 3)
			return true;
	}
	else if (2 == p.x || 1 == p.x)		//左边沿
	{
        // counter = (_img->read(p.x,  p.y+1) > _img->threshold())
        //         + (_img->read(p.x,  p.y-1) > _img->threshold())
        //         + (_img->read(p.x+1,p.y  ) > _img->threshold())
        //         + (_img->read(p.x+1,p.y+1) > _img->threshold())
        //         + (_img->read(p.x+1,p.y-1) > _img->threshold());
                //上+下+右+右上+右下
		//if (counter <= 3)
			return true;
	}
    else if (_img->size().y-3 == p.y || _img->size().y-2 == p.y)
    {
        return true;
    }
	else
	{
        INT16 threshold;
        INT16 val = 12;
		counter = (_img->read(p.x,  p.y+1) < _img->read(p.x, p.y) - val )
                + (_img->read(p.x,  p.y-1) < _img->read(p.x, p.y) - val )
                + (_img->read(p.x+1,p.y  ) < _img->read(p.x, p.y) - val )
                + (_img->read(p.x+1,p.y+1) < _img->read(p.x, p.y) - val )
                + (_img->read(p.x+1,p.y-1) < _img->read(p.x, p.y) - val )
                + (_img->read(p.x-1,p.y  ) < _img->read(p.x, p.y) - val )
                + (_img->read(p.x-1,p.y+1) < _img->read(p.x, p.y) - val )
                + (_img->read(p.x-1,p.y-1) < _img->read(p.x, p.y) - val );
		if (counter >= 2)
			return true;
        // BYTE grad = _analyzer->applyOperator(p.x, p.y, ImageAnalyzer::SOBEL);
        //if (grad > 40)
        //   return true;
	}
	return false;
}

LaneAnalyzer::Point LaneAnalyzer::_closestPoint(const Point& source, std::initializer_list<Point> init_list)
{
    Point tempPoint;
    ELEMENT tempDistance = ELEMENT(-1); //MAXIUM of unsigned int
    
    for(Point currentPoint : init_list)
    {
        ELEMENT currentDistance = _distanceSquared(source->point,currentPoint->point);
        if(currentDistance < tempDistance)
        {
            tempDistance = currentDistance;
            tempPoint = currentPoint;
        }
    }
    return tempPoint;
}

Vec2D LaneAnalyzer::_foo(Vec2D p)
{
    if(p.x<2 || p.x>_img->size().x-3 || p.y<2 || p.y>_img->size().y-3)
	{
		std::cerr << "foo(): out_of_range error at "<< p.x << ", "<< p.y <<std::endl;
		return 0;
	}
    SIGNED_COORD horizontal = 0, vertical = 0;
    SIGNED_COORD biao[5]={-1,-1,0,1,1};
	for(SIGNED_COORD i=-2; i<=2; i++)
	{
		for(SIGNED_COORD j=-2; j<=2; j++)
		{
			horizontal += (_img->read(p.x+i, p.y+j) < _img->read(p.x, p.y)-12) * (-biao[i+2]);
			vertical += (_img->read(p.x+i, p.y+j) > _img->read(p.x, p.y)-12) * biao[j+2];
		}
	}
	return Vec2D(horizontal, vertical);
}

COORD LaneAnalyzer::_variance(const Path& edge)
{
    SIGNED_COORD meanX = 0;
    SIGNED_COORD meanY = 0;
    Point begin = edge.size()-3>=15 ? (edge.end()-3-15) : edge.begin();
    for(Point p = edge.end()-1-3; p>=begin; p--)
    {
        meanX += p->direction.x;
        meanY += p->direction.y;
    }
    meanX /= (edge.end()-3-begin);
    meanY /= (edge.end()-3-begin);

    SIGNED_COORD varianceX = 0;
    SIGNED_COORD varianceY = 0;
    for(Point p = edge.end()-1-3; p>=begin; p--)
    {
        varianceX += (p->direction.x - meanX) * (p->direction.x - meanX);
        varianceY += (p->direction.y - meanY) * (p->direction.y - meanY);
    }
    return (varianceX + varianceY) / (edge.end()-3-begin);
}

const Vec2D LaneAnalyzer::UP    = Vec2D( 0,-1);
const Vec2D LaneAnalyzer::DOWN  = Vec2D( 0, 1);
const Vec2D LaneAnalyzer::LEFT  = Vec2D(-1, 0);
const Vec2D LaneAnalyzer::RIGHT = Vec2D( 1, 0);
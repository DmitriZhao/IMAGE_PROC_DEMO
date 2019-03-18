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
            _result->write(x,y,_img->read(x,y)>(_img->threshold()+24)?' ':'*');
    }
}

BOOL LaneAnalyzer::findPath()
{
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

    if(_left_1.size()==0 || _right_1.size()==0)
    {
        std::cout<<"Error\n";
        return false;
    }

    //determine lane type
    COORD leftBoundaryCount = 0, rightBoundaryCount = 0;
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

    //std::cout<<(uint32_t)_countBlackPoint(_left_1.rbegin()->point)<<"  !  "<< (uint32_t)_countBlackPoint(_right_1.rbegin()->point)<<std::endl;

    // if(leftBoundaryCount >=5 || rightBoundaryCount >= 5             //delete variance
    // || _countBlackPoint(_left_1.rbegin()->point) + _countBlackPoint(_right_1.rbegin()->point) > 0x0A)
    // {
    //     if( _countBlackPoint(_left_1.rbegin()->point) > 0x06 && _countBlackPoint(_right_1.rbegin()->point) > 0x06)
    //         _laneType = normal;
    //     else
    //     if(abs(_left_1.rbegin()->direction.x + _left_1.rbegin()->direction.y)
    //      + abs(_right_1.rbegin()->direction.x + _right_1.rbegin()->direction.y) > 10)
    //         _laneType = rightTurn;
    //     else
    //         _laneType = leftTurn;
    // }
    // else
    //     _laneType = crossRoad;
    
    COORD lbkpoint = _countBlackPoint(_left_1.rbegin()->point), rbkpoint = _countBlackPoint(_right_1.rbegin()->point);
    std::cout<<(uint32_t)lbkpoint<<"  .  "<<(uint32_t)rbkpoint<<std::endl;
    if(_left_1.rbegin()->point.y < _img->size().y/2 && _right_1.rbegin()->point.y < _img->size().y/2)
        _laneType = normal;
    else if(lbkpoint <= 0x03 && rbkpoint <= 0x03 )
        _laneType = crossRoad;
    else 
    {
        if(lbkpoint <= 0x04)    _laneType = leftTurn;
        else if(rbkpoint <= 0x04)   _laneType = rightTurn;
        else    _laneType = normal;
    }


    if(crossRoad == _laneType)
    {
        COORD bottom = _left_1.rbegin()->point.y < _right_1.rbegin()->point.y ? _left_1.rbegin()->point.y : _right_1.rbegin()->point.y;
        do{
            _left_2.clear();
            _right_2.clear();
            do
            {
                bottom -= 3;
            }
            while(!_findRoot(bottom, _left_2, _right_2) && bottom > _img->size().y/3);

            if(bottom <= _img->size().y/3)
                break;

            if(_left_2.begin()->point.x < _left_1.rbegin()->point.x
            || _right_2.begin()->point.x > _right_2.rbegin()->point.x)
                continue;

            _dfs(_left_2.front(), _left_2, UP);               //extend _left and _right in straight lane if successful
            _dfs(_right_2.front(), _right_2, UP);
        }
        while((_left_2.size()<5 || _right_2.size()<5) && bottom >_img->size().y/3);

        if(bottom <= _img->size().y/3 + 3)
        {
            _left_2.clear();
            _right_2.clear();
        }
        Line l1 = _leastSquares(_left_1);
        Line l2 = _leastSquares(_right_1);
        if(_left_1.begin()->point.y > _img->size().y-10 || _right_1.begin()->point.y > _img->size().y-10 )
        {
            if(bottom <= _img->size().y/3 + 3)     //_2缺失，向上延长
            {
                _drawLine(l1, _left_1, _img->size().y/2, _left_1.rbegin()->point.y);
                _drawLine(l2, _right_1, _img->size().y/2, _right_1.rbegin()->point.y);
            }
            else                //_1, _2之间补全
            {
                _drawLine(l1, _left_1, _left_2.begin()->point.y, _left_1.rbegin()->point.y);
                _drawLine(l2, _right_1, _right_2.begin()->point.y, _right_1.rbegin()->point.y);
            }
        }
        else                    //下缺失，向下延长
        {
            _drawLine(l1, _left_2, _left_1.begin()->point.y, _img->size().y-3);
            _drawLine(l2, _right_2, _right_1.begin()->point.y, _img->size().y-3);
        }
        if(_left_2.empty() || _left_1.begin()->point.y > _left_2.begin()->point.y)
        {
            Point pLeft = _left_1.begin(), pRight = _right_1.begin();
            while(pLeft != _left_2.end() && pRight != _right_2.end())
            {
                if(pLeft->point.y == pRight->point.y)
                {
                    _mid.push_back({(pLeft->point + pRight->point) / 2, {0,0}});
                    pLeft++;
                    pRight++;
                }
                else if(pLeft->point.y > pRight->point.y)
                    pLeft++;
                else
                    pRight++;
                
                if(pLeft == _left_1.end())
                    pLeft = _left_2.begin();
                if(pRight == _right_1.end())
                    pRight = _right_2.begin();
            }
        }
        else
        {
            Point pLeft = _left_2.begin(), pRight = _right_2.begin();
            while(pLeft != _left_1.end() && pRight != _right_1.end())
            {
                if(pLeft->point.y == pRight->point.y)
                {
                    _mid.push_back({(pLeft->point + pRight->point) / 2, {0,0}});
                    pLeft++;
                    pRight++;
                }
                else if(pLeft->point.y > pRight->point.y)
                    pLeft++;
                else
                    pRight++;
                
                if(pLeft == _left_2.end())
                    pLeft = _left_1.begin();
                if(pRight == _right_2.end())
                    pRight = _right_1.begin();
            }
        }
    }
    else if(normal == _laneType)
    {
        Point pleft = _left_1.begin(), pright = _right_1.begin();
        while(pleft < _left_1.end()-1 && pright < _right_1.end()-1)
        {
            if(pleft->point.y == pright->point.y)
            {
                _mid.push_back({(pleft->point+pright->point)/2, Vec2D(0,0)});
                pleft++;
                pright++;
            }
            else if(pleft->point.y > pright->point.y)
                pleft++;
            else
                pright++;
        }
    }
    else
    {
        _left_2.push_back(_left_1.back());
        _right_2.push_back(_right_1.back());  
        if(leftTurn ==_laneType)
        {
            _dfs(_left_1.back(), _left_2, LEFT);            //extend _left and _right in curved lane if successful
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
        } //NOTE: Assume that longerPath_1.rbegin()->point.y < shorterPath_1.rbegin()->point.y

        //find midpoint on curved lane
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
        case normal:
            std::cout << "Normal\n";
            break;
        case circle:
            std::cout << "Circle\n";
            break;
    }
}

void LaneAnalyzer::_dfs(const Vec4D& root, Path& edge, const Vec2D& direction)
{
    Vec4D rootTemp = root;
    std::vector<SIGNED_COORD> horizontalSearchingDistanceQueue = { 0,1,-1,2,-2,3,-3 };
    if(direction.x)
        horizontalSearchingDistanceQueue =  { 0,-1,-2,1,2 };
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
                    p4D.direction = _grad(p);
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
    Path root;
    for(COORD x = 2; x < _img->size().x-2; x++)
    {
        Vec2D p = Vec2D(x,bottom);
        if(_img->read(x,bottom) > _img->threshold() && _bOnEdge(Vec2D(x,bottom)))
        {
            root.push_back({p, _grad(p)});
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

COORD LaneAnalyzer::_countBlackPoint(const Vec2D& p)
{
    COORD counter = 0;
    for(SIGNED_COORD x = -2; x<= 2; x++)
        for(SIGNED_COORD y = -1; y>= -3; y--)
        {
            Vec2D current(p.x+x, p.y+y);
            if(_bInImage(p))
            {
                if(_img->read(current.x, current.y) < _img->threshold()+24)   
                    counter++;
            }
            else
                return 0x0F;
        }
    return counter;
}

BOOL LaneAnalyzer::_bOnEdge(const Vec2D &p)
{
    if (_img->size().x-3 == p.x || _img->size().x-2 == p.x ||   //右边沿
           2 == p.x || 1 == p.x ||	                            //左边沿   
        _img->size().y-2 == p.y || _img->size().y-1 == p.y)     //下边沿
        return true;
    else
    {
        BYTE counter = 0;
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

Vec2D LaneAnalyzer::_grad(Vec2D p)
{
    if(p.x<2 || p.x>_img->size().x-3 || p.y<2 || p.y>_img->size().y-3)
    {
        std::cerr << "LaneAnalyzer::_grad(): out_of_range error at "<< p.x << ", "<< p.y <<std::endl;
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

Line LaneAnalyzer::_leastSquares(const Path& edge)
{
    COORD nDropPoint = 4;
    float a, b, temp;
    float A = 0.0;
    float B = 0.0;
    float C = 0.0;
    float D = 0.0;
    float E = 0.0;
    float F = 0.0;
    for(Point p = edge.begin(); p < edge.end() - nDropPoint; p++)
    {
        A += p->point.y * p->point.y;
        B += p->point.y;
        C += p->point.x * p->point.y;
        D += p->point.x;
    }
    if( 0 != (temp = ((edge.size() - nDropPoint)*A - B*B)))// 判断分母不为0
    {
        a = ((edge.size() - nDropPoint)*C - B*D) / temp;
        b = (A*D - B*C) / temp;
    }
    else
    {
        a = 1;
        b = 0;
    }
    return {a,b};
}

void LaneAnalyzer::_drawLine(const Line& line, Path& edge, COORD yMin, COORD yMax)
{
    if(yMax - yMin < 5)
        return;
    for(COORD y = yMax -1; y > yMin; y--)
    {
        float x = line.a * y + line.b;
        edge.push_back({{COORD(x),COORD(y)},{0,0}});
    }
}

const Vec2D LaneAnalyzer::UP    = Vec2D( 0,-1);
const Vec2D LaneAnalyzer::DOWN  = Vec2D( 0, 1);
const Vec2D LaneAnalyzer::LEFT  = Vec2D(-1, 0);
const Vec2D LaneAnalyzer::RIGHT = Vec2D( 1, 0);
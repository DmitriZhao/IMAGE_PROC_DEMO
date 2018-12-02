#ifndef _TYPES_H
#define _TYPES_H

#include <vector>

#define MT9V032

typedef bool                BOOL, *PBOOL;
typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
//typedef signed __int64      INT64, *PINT64;
typedef unsigned char       UINT8, *PUINT8, BYTE;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
//typedef unsigned __int64    UINT64, *PUINT64;


#if defined MT9V032
    typedef UINT8  COORD;           //图像坐标系
    typedef UINT16 ELEMENT;         //面积元
    typedef INT16  SIGNED_COORD;    //带符号坐标
#endif

#if defined MT9V032
    const COORD     WIDTH   = 188;
    const COORD     HEIGHT  = 120;
#endif


typedef struct Size
{
    COORD   x;
    COORD   y;
    ELEMENT  area()  {return x*y;}
    Size(COORD x=0, COORD y=0)
    {
        this->x = x;
        this->y = y;
    }
}Size;

typedef struct Pixel        //RGB888
{
    BYTE   b;
    BYTE   g;
    BYTE   r;
    Pixel(UINT8 b=0, UINT8 g=0, UINT8 r=0)
    {
        this->b = b;
        this->g = g;
        this->r = r;
    }
}Pixel;

typedef struct Vec2D        //二维向量
{
    SIGNED_COORD   x;
    SIGNED_COORD   y;
    Vec2D(SIGNED_COORD x = 0, SIGNED_COORD y = 0)
    {
        this->x = x;
        this->y = y;
    }
    Vec2D horizontal(void)
    {
        if(0 == this->x)
            return Vec2D(1,0);
        else if(0 == this->y)
            return Vec2D(0,1);
        else
            return Vec2D(0,0);
    }
    Vec2D& operator = (const Vec2D& vec)
    {
        this->x = vec.x;
        this->y = vec.y;
        return *this;
    }
    Vec2D operator + (const Vec2D& vec)
    {
        return Vec2D(this->x+vec.x, this->y+vec.y);
    }
    Vec2D operator * (const SIGNED_COORD& c)
    {
        return Vec2D(c* this->x, c* this->y);
    }
    Vec2D operator / (const SIGNED_COORD& c)
    {
        return Vec2D(this->x/c, this->y/c);
    }    
}Vec2D;

const Vec2D UP    = Vec2D( 0,-1);
const Vec2D DOWN  = Vec2D( 0, 1);
const Vec2D LEFT  = Vec2D(-1, 0);
const Vec2D RIGHT = Vec2D( 1, 0);

typedef std::vector<Vec2D> Path;    //路径
typedef Path::iterator Point;       //路径上的点(因此是(迭)指(代)针(器)，解引用得到位置向量)

typedef struct Scalar
{
    BYTE value;
    Scalar(BYTE value = 0) {this->value = value;}
}Scalar;

class Line
{
private:
    //TODO
};

#endif
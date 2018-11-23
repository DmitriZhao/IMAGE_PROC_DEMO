#ifndef _TYPES_H
#define _TYPES_H

#define MT9V032

typedef bool                BOOL, *PBOOL;
typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
//typedef signed __int64      INT64, *PINT64;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
//typedef unsigned __int64    UINT64, *PUINT64;


#if defined MT9V032
    typedef UINT8 COORD;
#endif

#if defined MT9V032
    const COORD     WIDTH   = 188;
    const COORD     HEIGHT  = 120;
#endif


typedef struct Size
{
    COORD   x;
    COORD   y;
    UINT32  area()  {return x*y;}
}Size;


class Point2D
{
private:
    COORD   _x;
    COORD   _y;
public:
    Point2D(COORD x = 0, COORD y = 0)
    {
        _x = x;
        _y = y;
    }
    COORD   x() {return _x;}
    COORD   y() {return _y;}
};

#endif
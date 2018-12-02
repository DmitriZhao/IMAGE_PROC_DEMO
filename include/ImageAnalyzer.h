#ifndef _IMAGE_ANALYZER_H
#define _IMAGE_ANALYZER_H
#include "Types.h"
#include "GreyScaleImage.h"

class ImageAnalyzer
{
private:
	ImageBase<BYTE>* _result;
	GreyScaleImage*	 _img;
	Path _left;
	Path _right;
	Path _mid;
    BOOL _bInImage(Vec2D &p)    {return p.x>=0 && p.x<_img->size().x && p.y>=0 && p.y<_img->size().y;}
	BOOL _bOnEdge (Vec2D &p);
    BOOL _findRoot(COORD bottom);
    void _dfs(Vec2D &root, Path &edge, Vec2D direction);
	ELEMENT _distanceSquared(Vec2D &a, Vec2D &b)	{return (ELEMENT)((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));}
	Point _closestPoint(Point &source, std::initializer_list<Point> init_list);
public:
	explicit ImageAnalyzer(GreyScaleImage* img);
	~ImageAnalyzer()	{delete _result;}
    BOOL findPath();
	void show();
};

#endif // !_IMAGE_ANALYZER_H
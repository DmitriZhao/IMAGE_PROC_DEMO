#ifndef _IMAGE_ANALYZER_H
#define _IMAGE_ANALYZER_H
#include <vector>
#include "Types.h"
#include "GreyScaleImage.h"

class ImageAnalyzer
{
private:
	ImageBase<BYTE>* _result;
	GreyScaleImage*	 _img;
	std::vector<Vec2D> _left;
	std::vector<Vec2D> _right;
	std::vector<Vec2D> _mid;
    BOOL _bInImage(Vec2D &p)    {return p.x>=0 && p.x<_img->size().x && p.y>=0 && p.y<_img->size().y;}
	BOOL _bOnEdge (Vec2D &p);
    BOOL _findRoot(COORD bottom);
    void _dfs(Vec2D &root, std::vector<Vec2D> &edge, Vec2D direction);
	ELEMENT _distanceSquared(Vec2D &a, Vec2D &b)	{return (ELEMENT)((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));}
	BYTE _closestPoint(Vec2D &source, std::initializer_list<Vec2D> init_list);
public:
	explicit ImageAnalyzer(GreyScaleImage* img);
    BOOL findPath();
	void show();
};

#endif // !_IMAGE_ANALYZER_H

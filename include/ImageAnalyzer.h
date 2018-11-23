#ifndef _IMAGE_ANALYZER_H
#define _IMAGE_ANALYZER_H
#include <vector>
#include "Types.h"
#include "GreyScaleImage.h"

class ImageAnalyzer
{
private:
	UINT8 **_result;
	GreyScaleImage*	_img;
    Size _size;
	std::vector<Point2D> _left;
	std::vector<Point2D> _right;
	std::vector<Point2D> _mid;
    BOOL _bInImage(Point2D &p)    {return p.x()>=0 && p.x()<_size.x && p.y()>=0 && p.y()<_size.y;}
	BOOL _bOnEdge (Point2D &p);
    BOOL _findRoot(COORD bottom);
    void _dfs(Point2D &root, std::vector<Point2D> &edge);
public:
	explicit ImageAnalyzer(GreyScaleImage* img);
    void findPath();
	void show();
};

#endif // !_IMAGE_ANALYZER_H

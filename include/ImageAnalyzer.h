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
	std::vector<Point> _left;
	std::vector<Point> _right;
	std::vector<Point> _mid;
    BOOL _bInImage(Point &p)    {return p.x>=0 && p.x<_img->size().x && p.y>=0 && p.y<_img->size().y;}
	BOOL _bOnEdge (Point &p);
    BOOL _findRoot(COORD bottom);
    void _dfs(Point &root, std::vector<Point> &edge);
public:
	explicit ImageAnalyzer(GreyScaleImage* img);
    void findPath();
	void show();
};

#endif // !_IMAGE_ANALYZER_H

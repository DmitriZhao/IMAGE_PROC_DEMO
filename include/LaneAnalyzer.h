#ifndef _LANE_ANALYZER_H
#define _LANE_ANALYZER_H
#include "Types.h"
#include "GreyScaleImage.hpp"
#include "ImageAnalyzer.h"

class LaneAnalyzer
{
public:
	explicit LaneAnalyzer(GreyScaleImage::Ptr img);
	~LaneAnalyzer()	{delete _analyzer;}
    BOOL	 findPath();
	void 	 show();
	void	 showLaneType();
	typedef  std::vector<Vec4D>   Path;    //路径
	typedef  Path::const_iterator Point;   //路径上的点(因此是(迭)指(代)针(器)，解引用得到位置向量)
	const static Vec2D UP;
	const static Vec2D DOWN;
	const static Vec2D LEFT;
	const static Vec2D RIGHT;
private:
	GreyScaleImage::Ptr	_img;
	GreyScaleImage::Ptr _result;
	ImageAnalyzer* _analyzer;
	Path _left_1;
	Path _left_2;
	Path _right_1;
	Path _right_2;
	Path _mid;
	LaneType _laneType;
	Line _leastSquares(const Path& edge);
	void _drawLine(const Line& line, Path& edge, COORD yMin, COORD yMax);
	COORD _variance(const Path& edge);
	Vec2D _grad(Vec2D p);
    BOOL _bInImage(const Vec2D& p)    {return p.x>=2 && p.x<_img->size().x-2 && p.y>=2 && p.y<_img->size().y-2;}
	BOOL _bOnEdge (const Vec2D& p);
    BOOL _findRoot (COORD bottom, Path& leftEdge, Path& rightEdge);
    void _dfs(const Vec4D& root, Path& edge, const Vec2D& direction);
	Point _closestPoint(const Point& source, std::initializer_list<Point> init_list);
	ELEMENT _distanceSquared(const Vec2D& a, const Vec2D& b)	{return (ELEMENT)((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));}
};
#endif // !_LANE_ANALYZER_H
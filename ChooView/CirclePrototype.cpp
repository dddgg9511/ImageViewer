#include "stdafx.h"
#include "CirclePrototype.h"
#include "Annotation.h"
#include "Circle.h"


CCirclePrototype::CCirclePrototype() :
	IAnnotationPrototype()
{

}

CCirclePrototype::~CCirclePrototype(void)
{
}


void CCirclePrototype::Draw(Graphics* g)
{
	Pen newPen(m_color, m_fWidth);

	std::pair<int, int> xPair = std::minmax<int>(m_point1.x, m_point2.x);
	std::pair<int, int> yPair = std::minmax<int>(m_point1.y, m_point2.y);

	g->DrawEllipse(&newPen, xPair.first, yPair.first, xPair.second - xPair.first,
								 yPair.second - yPair.first);
}


IAnnotation* CCirclePrototype::Convert(const float& zoomRate,
																			 const float& orgRate,
																			 const PointF& viewPoint,
																			 const RectF& orgRect,
																			 const bool& flipVertical,
																			 const bool& flipHorizontal,
																			 const int& rotateState, const CPoint& size,
																			 const CPoint& picPoint)
{
	PointF pt1 = Client2Img(m_point1, zoomRate, orgRate, viewPoint, orgRect,
													flipVertical, flipHorizontal, rotateState, size,
													picPoint);
	PointF pt2 = Client2Img(m_point2, zoomRate, orgRate, viewPoint, orgRect,
													flipVertical, flipHorizontal, rotateState, size,
													picPoint);
	CCircle* pCircle = new CCircle(pt1, pt2, m_color, m_fWidth);
	return pCircle;
}


void CCirclePrototype::Update(const CPoint& point)
{
	int dX = point.x - m_point1.x;
	int dY = point.y - m_point1.y;

	PointF pointf = CPoint2PointF(point);

	if (dY == 0)
	{
		dY = 1;
	}
	if (dX == 0)
	{
		dX = 1;
	}
	float distance = sqrt(pow(m_point1.x - pointf.X, 2) +
											  pow(m_point1.y - pointf.Y, 2));

	m_point2.x = static_cast<int>(m_point1.x + distance * abs(dX) / dX);
	m_point2.y = static_cast<int>(m_point1.y + distance * abs(dY) / dY);
}

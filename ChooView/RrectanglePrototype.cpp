#include "stdafx.h"
#include "RrectanglePrototype.h"
#include "Rectangle.h"
#include "Annotation.h"

CRrectanglePrototype::CRrectanglePrototype() :
	IAnnotationPrototype()
{

}

CRrectanglePrototype::~CRrectanglePrototype(void)
{
}

void CRrectanglePrototype::Draw(Graphics* g)
{
	Pen newPen(m_color, m_fWidth);

	std::pair<int, int> xPair = std::minmax<int>(m_point1.x, m_point2.x);
	std::pair<int, int> yPair = std::minmax<int>(m_point1.y, m_point2.y);

	g->DrawRectangle(&newPen, xPair.first, yPair.first, xPair.second - xPair.first,
									 yPair.second - yPair.first);
}

IAnnotation* CRrectanglePrototype::Convert(const float& zoomRate,
																					 const float& orgRate,
																					 const PointF& viewPoint,
																					 const RectF& orgRect,
																					 const bool& flipVertical,
																					 const bool& flipHorizontal,
																					 const int& rotateState,
																					 const CPoint& size,
																					 const CPoint& picPoint)
{
	PointF pt1 = Client2Img(m_point1, zoomRate, orgRate, viewPoint, orgRect,
												  flipVertical, flipHorizontal, rotateState, size, 
													picPoint);
	PointF pt2 = Client2Img(m_point2, zoomRate, orgRate, viewPoint, orgRect,
													flipVertical, flipHorizontal, rotateState, size, 
													picPoint);
	CRectangle* pRectangle = new CRectangle(pt1, pt2, m_color, m_fWidth);
	return pRectangle;
}

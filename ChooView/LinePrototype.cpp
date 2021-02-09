#include "stdafx.h"
#include "LinePrototype.h"
#include "Annotation.h"
#include "Line.h"


CLinePrototype::CLinePrototype() :
	IAnnotationPrototype()
{

}


CLinePrototype::~CLinePrototype(void)
{
}

void CLinePrototype::Draw(Graphics* g)
{
	Pen newPen(m_color, m_fWidth);
	g->DrawLine(&newPen, CPoint2PointF(m_point1), CPoint2PointF(m_point2));
}


IAnnotation* CLinePrototype::Convert(const float& zoomRate, const float& orgRate,
																		 const PointF& viewPoint, const RectF& orgRect,
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
	CLine* pLine = new CLine(pt1, pt2, m_color, m_fWidth);
	return pLine;
}

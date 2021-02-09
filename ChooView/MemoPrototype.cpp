#include "stdafx.h"
#include "MemoPrototype.h"
#include "Annotation.h"
#include "Memo.h"

/**
@CirclePrototype의 생성자
@color  : Circle의 선 색
@nWidth : Circle의 선 굵기
@pt1		: Circle의 좌표
@pt2		: Circle의 좌표
*/
CMemoPrototype::CMemoPrototype() :
	IAnnotationPrototype()
{
}


CMemoPrototype::~CMemoPrototype(void)
{
}


void CMemoPrototype::Draw(Graphics* g)
{
	Pen newPen(Color(0, 0, 0), m_fWidth);
	newPen.SetDashStyle(DashStyleDashDot);
	std::pair<int, int> xPair = std::minmax<int>(m_point1.x, m_point2.x);
	std::pair<int, int> yPair = std::minmax<int>(m_point1.y, m_point2.y);

	g->DrawRectangle(&newPen, xPair.first, yPair.first, xPair.second - xPair.first,
									 yPair.second - yPair.first);

}


IAnnotation* CMemoPrototype::Convert(const float& zoomRate, const float& orgRate,
																		 const PointF& viewPoint, const RectF& orgRect,
																		 const bool& flipVertical,
																		 const bool& flipHorizontal,
																		 const int& rotateState,
																		 const CPoint& size, const CPoint& picPoint)
{
	PointF pt1 = Client2Img(m_point1, zoomRate, orgRate, viewPoint, orgRect,
													flipVertical, flipHorizontal, rotateState, size,
													picPoint);
	PointF pt2 = Client2Img(m_point2, zoomRate, orgRate, viewPoint, orgRect,
													flipVertical, flipHorizontal, rotateState, size, 
													picPoint);
	CMemo* pMemo = new CMemo(pt1, pt2, m_color, m_fWidth);
	return pMemo;
}

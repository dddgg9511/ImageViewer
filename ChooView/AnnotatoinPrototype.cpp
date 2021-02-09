#include "stdafx.h"
#include "Annotatoinprototype.h"


IAnnotationPrototype::IAnnotationPrototype() :
	m_color(Color(0, 0, 0)),
	m_fWidth(-1),
	m_point1(CPoint(-1, -1)),
	m_point2(CPoint(-1, -1))
{

}

IAnnotationPrototype::~IAnnotationPrototype(void)
{
}

void IAnnotationPrototype::SetProperty(const Color& color, const float& width,
																			 const CPoint& pt1)
{
	m_color = color;
	m_fWidth = width;
	m_point1 = pt1;
	m_point2 = pt1;
}


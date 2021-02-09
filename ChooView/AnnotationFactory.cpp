#include "stdafx.h"
#include "AnnotationFactory.h"
#include "DataBaseStruct.h"
#include "Annotation.h"
#include "Line.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Memo.h"
#include "RrectanglePrototype.h"
#include "CirclePrototype.h"
#include "LinePrototype.h"
#include "MemoPrototype.h"

AnnotationFactory::AnnotationFactory(void)
{
	//Prototype 생성함수 등록
	m_arrPrototypeCreators[DrawRectangle - 3] = &AnnotationFactory::CreateRectanglePrototype;
	m_arrPrototypeCreators[DrawCircle - 3] = &AnnotationFactory::CreateCirclePrototype;
	m_arrPrototypeCreators[DrawLine - 3] = &AnnotationFactory::CreateLinePrototype;
	m_arrPrototypeCreators[DrawMemo - 3] = &AnnotationFactory::CreateMemoPrototype;

	//Annotation 생성함수 등록 
	m_arrAnnotationCreators[DrawRectangle - 3] = &AnnotationFactory::CreateRectangle;
	m_arrAnnotationCreators[DrawCircle - 3] = &AnnotationFactory::CreateCircle;
	m_arrAnnotationCreators[DrawLine - 3] = &AnnotationFactory::CreateLine;
	m_arrAnnotationCreators[DrawMemo - 3] = &AnnotationFactory::CreateMemo;


	m_pRectanglePrototype = new CRrectanglePrototype();
	m_pLinePrototype = new CLinePrototype();
	m_pCirclePrototype = new CCirclePrototype();
	m_pMemoPrototype = new CMemoPrototype();
}


AnnotationFactory::~AnnotationFactory(void)
{
	delete m_pCirclePrototype;
	delete m_pLinePrototype;
	delete m_pRectanglePrototype;
	delete m_pMemoPrototype;
}


IAnnotation* AnnotationFactory::CreateAnnotation(const AnnotationData& data)
{
	if (data.m_nAnnotationType >= DrawRectangle &&
		data.m_nAnnotationType <= DrawLine)
	{
		return (this->*m_arrAnnotationCreators[data.m_nAnnotationType - 3])(data);
	}
	return nullptr;
}


IAnnotationPrototype* AnnotationFactory::CreatePrototype
		(const int& AnnotationType,
		 const Color& color,
		 const float& fPenWidth,
		 const CPoint& point)
{
	if (AnnotationType >= DrawRectangle && AnnotationType <= DrawLine)
	{
		return (this->*m_arrPrototypeCreators[AnnotationType - 3])
			(color, fPenWidth, point);
	}
	return nullptr;
}

IAnnotationPrototype* AnnotationFactory::CreateRectanglePrototype
		(const Color& color,
		 const float& fPenWidth,
		 const CPoint& point)
{
	m_pRectanglePrototype->SetProperty(color, fPenWidth, point);
	return m_pRectanglePrototype;
}

IAnnotationPrototype* AnnotationFactory::CreateCirclePrototype
		(const Color& color,
		 const float& fPenWidth,
		 const CPoint& point)
{
	m_pCirclePrototype->SetProperty(color, fPenWidth, point);
	return m_pCirclePrototype;
}

IAnnotationPrototype* AnnotationFactory::CreateLinePrototype
		(const Color& color,
		 const float& fPenWidth,
		 const CPoint& point)
{
	m_pLinePrototype->SetProperty(color, fPenWidth, point);
	return m_pLinePrototype;
}

IAnnotationPrototype* AnnotationFactory::CreateMemoPrototype
		(const Color& color,
		 const float& fPenWidth,
		 const CPoint& point)
{
	m_pMemoPrototype->SetProperty(color, fPenWidth, point);
	return m_pMemoPrototype;
}

inline IAnnotation* AnnotationFactory::CreateRectangle
		(const AnnotationData& data)
{
	return new CRectangle(data);
}

inline IAnnotation* AnnotationFactory::CreateCircle(const AnnotationData& data)
{
	return new CCircle(data);
}

inline IAnnotation* AnnotationFactory::CreateLine(const AnnotationData& data)
{
	return new CLine(data);
}

inline IAnnotation* AnnotationFactory::CreateMemo(const AnnotationData& data)
{
	return new CMemo(data);
}

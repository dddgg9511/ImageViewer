#include "stdafx.h"
#include "AnnotationManager.h"
#include "Annotation.h"
#include "Line.h"
#include "Rectangle.h"
#include "Memo.h"
#include "Circle.h"
#include "Annotatoinprototype.h"
#include "LinePrototype.h"
#include "RrectanglePrototype.h"
#include "MemoPrototype.h"
#include "CirclePrototype.h"
#include "DBManager.h"
#include "DataBaseStruct.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AnnotationFactory CAnnotationManager::m_annotationFactory = AnnotationFactory();

//std::list<IAnnotation*>ÀÇ Iterator Type
typedef std::list<IAnnotation*>::iterator AnnotationIter;

CAnnotationManager::CAnnotationManager() :
	m_selectAnnotation(nullptr),
	m_pPrototype(nullptr)
{

}


void CAnnotationManager::AnnotationDraw(const int& actionState,
																				const float& orgRate,
																				const float& zoomRate,
																				const PointF& orgPoint,
																				const PointF& viewPoint,
																				const CPoint& size,
																				const int& rotateState,
																				const bool& flipVertical,
																				const bool& flipHorizontal,
																				const CPoint& PicPoint,
																				const bool& isSelect, Graphics* g)
{
	for (AnnotationIter iter = m_arrAnnotation.begin();
			 iter != m_arrAnnotation.end(); iter++)
	{
		if (isSelect && (actionState == Actions::SelectMode ||
			  actionState == Actions::MemoUpdate ||
			  actionState == Actions::AnnotatiaonUpdate))
		{
			if (m_selectAnnotation == (*iter))
			{
				(*iter)->SelectDraw(orgRate, zoomRate, orgPoint, viewPoint, size, 
														rotateState, flipVertical, flipHorizontal, PicPoint,
														g);
			}
			else
			{
				(*iter)->Draw(orgRate, zoomRate, orgPoint, viewPoint, size, rotateState,
											flipVertical, flipHorizontal, PicPoint, g);
			}
		}
		else
		{
			(*iter)->Draw(orgRate, zoomRate, orgPoint, viewPoint, size, rotateState,
										flipVertical, flipHorizontal, PicPoint, g);
		}
	}
	if (m_pPrototype != nullptr)
	{
		m_pPrototype->Draw(g);
	}
}


bool CAnnotationManager::HitTest(const PointF& imgPoint,
																 const CPoint& clientPoint,
																 int* actionState)
{
	if (m_selectAnnotation != nullptr)
	{
		if (m_selectAnnotation->HitTest(imgPoint, clientPoint, actionState))
		{
			return true;
		}
	}
	m_selectAnnotation = nullptr;
	for (AnnotationIter iter = m_arrAnnotation.begin();
			 iter != m_arrAnnotation.end(); iter++)
	{
		if (m_selectAnnotation == *iter)
		{
			continue;
		}
		if ((*iter)->HitTest(imgPoint, clientPoint, actionState))
		{
			m_selectAnnotation = (*iter);
			return true;
		}
	}
	return false;
}


void CAnnotationManager::AnnotationUpdate(const PointF& downPoint,
																					const PointF& firstPoint,
																					const PointF& secondPoint)
{
	if (m_selectAnnotation != nullptr)
	{
		m_selectAnnotation->Update(downPoint, firstPoint, secondPoint);
	}
}


void CAnnotationManager::annotationModify(void)
{
	if (m_selectAnnotation != nullptr)
	{
		m_selectAnnotation->Modify();
	}
}


void CAnnotationManager::UpdateMemo(const UINT& nChar)
{
	if (m_selectAnnotation != nullptr)
	{
		m_selectAnnotation->UpdateMemoString(nChar);
	}
}


void CAnnotationManager::SaveDB(const int& imageID, const bool& isSaveNew)
{
	for (AnnotationIter iter = m_arrAnnotation.begin();
			 iter != m_arrAnnotation.end(); iter++)
	{
		(*iter)->SaveDB(imageID, isSaveNew);
	}
	if (isSaveNew)
	{
		m_arrDeletedIndex.clear();
	}
	else
	{
		for (unsigned int i : m_arrDeletedIndex)
		{
			CDBManager::DeletePointInAnnotation(i);
			CDBManager::DeleteAnnotationById(i);
		}
	}
}


void CAnnotationManager::LoadAnnotation(const int& imageId)
{
	std::vector<AnnotationData> dataList;
	CDBManager::SelectAnnotationInImage(imageId, &dataList);
	for (AnnotationData data : dataList)
	{
		IAnnotation* pAnnotation = m_annotationFactory.CreateAnnotation(data);
		if (pAnnotation != nullptr)
		{
			m_arrAnnotation.push_back(pAnnotation);
		}
	}
}

CAnnotationManager::~CAnnotationManager()
{
	for_each(m_arrAnnotation.begin(),m_arrAnnotation.end(),Delete());
}


void CAnnotationManager::ListReset()
{
	for (AnnotationIter iter = m_arrAnnotation.begin();
			 iter != m_arrAnnotation.end(); iter++)
	{
		int idx = (*iter)->GetIndex();
		if (idx != -1)
		{
			m_arrDeletedIndex.push_back(idx);
		}
		delete (*iter);
	}
	m_arrAnnotation.clear();
}


void CAnnotationManager::DeleteAnnotation()
{
	if (m_selectAnnotation != nullptr)
	{
		int idx = m_selectAnnotation->GetIndex();
		if (idx != -1)
		{
			m_arrDeletedIndex.push_back(idx);
		}
		delete m_selectAnnotation;
		m_arrAnnotation.remove(m_selectAnnotation);
		m_selectAnnotation = nullptr;
	}
}


void CAnnotationManager::SetAnnotationProperty()
{
	if (m_selectAnnotation != nullptr)
	{
		m_selectAnnotation->SetAnnotationProperty();
	}
}


void CAnnotationManager::CreatePrototype(const Color& color,
																				 const float& penWidth,
																				 const CPoint& point,
																				 const int& actionState)
{

	m_pPrototype = m_annotationFactory.CreatePrototype(actionState, color,
																										 penWidth, point);
}


void CAnnotationManager::UpdatePrototype(const CPoint& point)
{
	if (m_pPrototype != nullptr)
	{
		m_pPrototype->Update(point);
	}
}


void CAnnotationManager::ConvertPrototype(const float& zoomRate,
																					const float& orgRate,
																					const PointF& viewPoint,
																					const RectF& orgRect,
																					const bool& flipVertical, 
																					const bool& flipHorizontal,
																					const int& rotateState,
																					const CPoint& size,
																					const CPoint& picPoint)
{
	if (m_pPrototype != nullptr)
	{
		IAnnotation* pAnnot = m_pPrototype->Convert(zoomRate, orgRate, viewPoint,
																								orgRect, flipVertical,
																								flipHorizontal, rotateState,
																								size, picPoint);
		m_arrAnnotation.push_back(pAnnot);
		m_selectAnnotation = pAnnot;
		m_pPrototype = nullptr;
	}
}



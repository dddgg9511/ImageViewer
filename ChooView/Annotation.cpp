#include "stdafx.h"
#include "Annotation.h"
#include "AnnotationPropertyDlg.h"
#include "DataBaseStruct.h"
#include "DBManager.h"

IAnnotation::IAnnotation(const Color& color, const float& fPenWidth) :
	m_Color(color),
	m_fPenWidth(fPenWidth),
	m_nClickState(0),
	m_fTheta(0),
	m_fUpdateTheta(0),
	m_nIndex(-1)
{

}

IAnnotation::IAnnotation(const AnnotationData& annotationData) :
	m_arrPoint(annotationData.m_arrPoints),
	m_arrUpdatePoint(annotationData.m_arrPoints),
	m_Color(annotationData.m_penColor),
	m_fPenWidth(annotationData.m_fPenWidth),
	m_nClickState(-1),
	m_fTheta(annotationData.m_fTheta),
	m_fUpdateTheta(annotationData.m_fTheta),
	m_nIndex(annotationData.m_nAnnotationID)
{
	m_arrSizeBoxes.resize(m_arrPoint.size());
	float xSum = 0;
	float ySum = 0;
	for (unsigned int i = 0; i < m_arrPoint.size(); i++)
	{
		xSum += m_arrPoint[i].X;
		ySum += m_arrPoint[i].Y;
	}
	m_Center.X = xSum / m_arrPoint.size();
	m_Center.Y = ySum / m_arrPoint.size();
}

void IAnnotation::SetAnnotationProperty()
{
	CAnnotationPropertyDlg dlg(m_Color, m_fPenWidth);
	if (dlg.DoModal() == IDOK)
	{
		m_Color = dlg.m_pencolor;
		m_fPenWidth = dlg.m_EditpenWidth;
	}
}

void IAnnotation::DeleteDB()
{
	if (m_nIndex != -1)
	{
		CDBManager::DeleteAnnotationById(m_nIndex);
	}
}


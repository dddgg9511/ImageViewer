#include "stdafx.h"
#include "Annotation.h"
#include "Line.h"
#include "DBManager.h"
#include "DataBaseStruct.h"


CLine::CLine(const PointF& downPoint, const PointF& upPoint, 
						 const Color& penColor, const float& penWidth) :
		IAnnotation(penColor, penWidth)
{
	m_arrPoint.reserve(2);
	if (downPoint.Y > upPoint.Y)
	{
		m_arrPoint.push_back(downPoint);
		m_arrPoint.push_back(upPoint);
	}
	else
	{
		m_arrPoint.push_back(upPoint);
		m_arrPoint.push_back(downPoint);
	}
	m_arrSizeBoxes.resize(2);

	m_Center.X = (m_arrPoint[0].X + m_arrPoint[1].X) / 2;
	m_Center.Y = (m_arrPoint[0].Y + m_arrPoint[1].Y) / 2;
	float minY = min(downPoint.Y, upPoint.Y);
	m_RotatePoint = PointF(m_Center.X, minY - 20);

	m_arrUpdatePoint = m_arrPoint;
	float dx = m_arrPoint[1].X - m_arrPoint[0].X;
	float dy = m_arrPoint[0].Y - m_arrPoint[1].Y;

	const float pi = 3.14159265359f;

	m_fUpdateTheta = m_fTheta = atan2(dx, dy) - (pi / 2);
}


CLine::CLine(const AnnotationData& annotationData) :
	IAnnotation(annotationData)
{
	float dx = m_arrPoint[1].X - m_arrPoint[0].X;
	float dy = m_arrPoint[0].Y - m_arrPoint[1].Y;

	const float pi = 3.14159265359f;

	m_fUpdateTheta = m_fTheta = atan2(dx, dy) - pi / 2;
}


void CLine::Draw(const float& orgRate, const float& zoomRate,
								 const PointF& orgRect, const PointF& viewPoint,
								 const CPoint& size, const int& rotateState,
								 const bool& flipVertical, const bool& flipHorizontal,
								 const CPoint& picPoint, Graphics* g)
{
	PointF ptList[2];


	for (unsigned int i = 0; i < m_arrPoint.size(); i++)
	{
		ptList[i] = Img2Client(m_arrPoint[i], zoomRate, orgRate, viewPoint, orgRect,
													 flipVertical, flipHorizontal, rotateState, size);
		ptList[i].X += picPoint.x;
		ptList[i].Y += picPoint.y;
	}
	Pen newPen(m_Color, m_fPenWidth);
	g->DrawLine(&newPen, ptList[0], ptList[1]);
}


void CLine::SelectDraw(const float& orgRate, const float& zoomRate,
											 const PointF& orgRect, const PointF& viewPoint,
											 const CPoint& size, const int& rotateState,
											 const bool& flipVertical, const bool& flipHorizontal,
											 const CPoint& picPoint, Graphics* g)
{
	PointF ptList[2];
	for (unsigned int i = 0; i < m_arrPoint.size(); i++)
	{
		ptList[i] = Img2Client(m_arrPoint[i], zoomRate, orgRate, viewPoint, orgRect,
													 flipVertical, flipHorizontal, rotateState, size);
		ptList[i].X += picPoint.x;
		ptList[i].Y += picPoint.y;
	}

	PointF updatePtList[2];
	for (unsigned int i = 0; i < m_arrPoint.size(); i++)
	{
		updatePtList[i] = Img2Client(m_arrUpdatePoint[i], zoomRate, orgRate,
																 viewPoint, orgRect, flipVertical, flipHorizontal,
																 rotateState, size);
		updatePtList[i].X += picPoint.x;
		updatePtList[i].Y += picPoint.y;
	}
	Pen newPen(Color(0, 0, 0), 1);
	newPen.SetDashStyle(DashStyleDashDot);

	g->DrawLine(&newPen, ptList[0], ptList[1]);
	g->DrawLine(&newPen, updatePtList[0], updatePtList[1]);
	newPen.SetDashStyle(DashStyleSolid);
	SolidBrush sizeBrush(Color(255, 255, 255));
	for (int i = 0; i < 2; i++)
	{
		m_arrSizeBoxes[i] = RectF(ptList[i].X - 5, ptList[i].Y - 5, 10, 10);
		g->FillRectangle(&sizeBrush, m_arrSizeBoxes[i]);
		g->DrawRectangle(&newPen, m_arrSizeBoxes[i]);
	}

	PointF* pts;
	if (m_nClickState == 5)
	{
		pts = &updatePtList[0];
	}
	else
	{
		pts = &ptList[0];
	}
	float X = (pts[0].X + pts[1].X) / 2;
	float Y = (pts[0].Y + pts[1].Y) / 2;

	m_RotatePoint.X = X - 20 * sin(-m_fUpdateTheta);
	m_RotatePoint.Y = Y - 20 * cos(-m_fUpdateTheta);
	SolidBrush rotateBrush(Color(100, 255, 0));
	g->FillEllipse(&rotateBrush, m_RotatePoint.X - 5, m_RotatePoint.Y - 5,
								 10.f, 10.f);
	g->DrawEllipse(&newPen, m_RotatePoint.X - 5, m_RotatePoint.Y - 5,
								 10.f, 10.f);
}


void CLine::Move(const PointF& downPoint, const PointF& upPoint)
{
	float difX = upPoint.X - downPoint.X;
	float difY = upPoint.Y - downPoint.Y;
	for (int i = 0; i < 2; i++)
	{
		m_arrUpdatePoint[i].X += difX;
		m_arrUpdatePoint[i].Y += difY;
	}
}


void CLine::reSize(const PointF& downPoint, const PointF& upPoint)
{
	float difX = upPoint.X - downPoint.X;
	float difY = upPoint.Y - downPoint.Y;
	m_arrUpdatePoint[m_nClickState - 1].X += difX;
	m_arrUpdatePoint[m_nClickState - 1].Y += difY;
}


bool CLine::HitTest(const PointF& imgPoint, const CPoint& clientPoint,
					int* actionState)
{
	if (m_nClickState != -1)
	{
		for (int i = 0; i < 2; i++)
		{
			if (m_arrSizeBoxes[i].Contains(CPoint2PointF(clientPoint)))
			{
				m_nClickState = i + 1;
				*actionState = Actions::AnnotatiaonUpdate;
				return true;
			}
		}
		float rad = 5;
		float distance = sqrtf(powf(clientPoint.x - m_RotatePoint.X, 2) +
													 powf(clientPoint.y - m_RotatePoint.Y, 2));
		if (distance <= rad)
		{
			m_nClickState = 5;
			*actionState = Actions::AnnotatiaonUpdate;
			return true;
		}
	}
	if (isInLine(imgPoint, m_arrPoint[0], m_arrPoint[1]))
	{
		m_nClickState = 0;
		*actionState = Actions::AnnotatiaonUpdate;
		return true;
	}
	else
	{
		*actionState = Actions::SelectMode;
		m_nClickState = -1;
		return false;
	}
}


void CLine::Modify(void)
{
	std::copy(m_arrUpdatePoint.begin(), m_arrUpdatePoint.end(), m_arrPoint.begin());

	for (int i = 0; i < 2; i++)
	{
		m_arrSizeBoxes[i].X = m_arrPoint[i].X - m_arrSizeBoxes[i].Width;
		m_arrSizeBoxes[i].Y = m_arrPoint[i].Y - m_arrSizeBoxes[i].Width;
	}
	if (1 <= m_nClickState && m_nClickState <= 4)
	{
		float dx = m_arrPoint[1].X - m_arrPoint[0].X;
		float dy = m_arrPoint[0].Y - m_arrPoint[1].Y;

		const float pi = 3.14159265359f;

		m_fTheta = atan2(dx, dy) - (pi / 2);

		m_fUpdateTheta = m_fTheta;
	}
	m_Center.X = (m_arrPoint[0].X + m_arrPoint[1].X) / m_arrPoint.size();
	m_Center.Y = (m_arrPoint[0].Y + m_arrPoint[1].Y) / m_arrPoint.size();
	m_fTheta = m_fUpdateTheta;
}



void CLine::Rotate(const PointF& downPoint, const PointF& movePoint)
{
	PointF dif = m_Center - movePoint;

	m_fUpdateTheta = -atan2(dif.X, dif.Y);
	float rad = m_fUpdateTheta - m_fTheta;
	for (unsigned int i = 0; i < m_arrPoint.size(); i++)
	{
		m_arrUpdatePoint[i].X = (m_arrPoint[i].X - m_Center.X) * cos(rad) -
														(m_arrPoint[i].Y - m_Center.Y) * sin(rad) + m_Center.X;
		m_arrUpdatePoint[i].Y = (m_arrPoint[i].X - m_Center.X) * sin(rad) +
														(m_arrPoint[i].Y - m_Center.Y) * cos(rad) + m_Center.Y;
	}
}


void CLine::Update(const PointF& downPoint, const PointF& firstPoint,
				   const PointF& secondPoint)
{
	if (m_nClickState == 0)
	{
		Move(firstPoint, secondPoint);
	}
	else if (1 <= m_nClickState && m_nClickState <= 4)
	{
		reSize(firstPoint, secondPoint);
	}
	else if (m_nClickState == 5)
	{
		Rotate(downPoint, secondPoint);
	}
}


void CLine::SaveDB(const int& imageID, const bool& isSaveNew)
{
	if (isSaveNew)
	{
		m_nIndex = -1;
	}
	AnnotationData annotationData(m_nIndex, m_fPenWidth, DrawLine, imageID, m_fTheta,
															  m_Color, _T(""), _T(""), _T(""), m_arrPoint);
	int idx = CDBManager::AddAnnotation(annotationData);
	if (idx != -1)
	{
		m_nIndex = idx;
	}
}

void CLine::UpdateMemoString(const UINT& nChar)
{
	throw std::logic_error("The method or operation is not implemented.");
}
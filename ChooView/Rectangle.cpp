#include "stdafx.h"
#include "Rectangle.h"
#include "DBManager.h"
#include "DataBaseStruct.h"

CRectangle::CRectangle(const PointF& downPoint, const PointF& upPoint,
											 const Color& penColor, const float& penWidth) :
IAnnotation(penColor, penWidth)
{

	std::pair<float, float> xPair = std::minmax<float>(downPoint.X, upPoint.X);
	std::pair<float, float> yPair = std::minmax<float>(downPoint.Y, upPoint.Y);
	m_arrPoint.reserve(4);
	m_arrSizeBoxes.resize(4);
	m_arrPoint.push_back(PointF(xPair.first, yPair.first));
	m_arrPoint.push_back(PointF(xPair.second, yPair.first));
	m_arrPoint.push_back(PointF(xPair.second, yPair.second));
	m_arrPoint.push_back(PointF(xPair.first, yPair.second));

	m_Center.X = (xPair.first + xPair.second) / 2;
	m_Center.Y = (yPair.first + yPair.second) / 2;

	m_RotatePoint = PointF(m_Center.X, yPair.first - 20);
	m_arrUpdatePoint = m_arrPoint;
}

CRectangle::CRectangle(const AnnotationData& annotationData) :
	IAnnotation(annotationData)
{

}

void CRectangle::Draw(const float& orgRate, const float& zoomRate,
											const PointF& orgPoint, const PointF& viewPoint,
											const CPoint& size, const int& rotateState,
											const bool& flipVertical, const bool& flipHorizontal,
											const CPoint& picPoint, Graphics* g)
{
	PointF ptList[4];

	Pen newPen(m_Color, m_fPenWidth);

	for (unsigned int i = 0; i < m_arrPoint.size(); i++)
	{
		ptList[i] = Img2Client(m_arrPoint[i], zoomRate, orgRate, viewPoint, orgPoint,
													 flipVertical, flipHorizontal, rotateState, size);
		ptList[i].X += picPoint.x;
		ptList[i].Y += picPoint.y;
	}
	g->DrawPolygon(&newPen, ptList, 4);
}


void CRectangle::SelectDraw(const float& orgRate, const float& zoomRate,
														const PointF& orgPoint, const PointF& viewPoint,
														const CPoint& size, const int& rotateState,
														const bool& flipVertical, const bool& flipHorizontal,
														const CPoint& picPoint, Graphics* g)
{
	PointF ptList[4];


	for (unsigned int i = 0; i < m_arrPoint.size(); i++)
	{
		ptList[i] = Img2Client(m_arrPoint[i], zoomRate, orgRate, viewPoint, orgPoint,
													 flipVertical, flipHorizontal, rotateState, size);
		ptList[i].X += picPoint.x;
		ptList[i].Y += picPoint.y;
	}

	Pen newPen(Color(0, 0, 0), 1);
	newPen.SetDashStyle(DashStyleDashDot);

	PointF updatePtList[4];
	for (unsigned int i = 0; i < m_arrPoint.size(); i++)
	{
		updatePtList[i] = Img2Client(m_arrUpdatePoint[i], zoomRate, orgRate,
																 viewPoint, orgPoint, flipVertical,
																 flipHorizontal, rotateState, size);
		updatePtList[i].X += picPoint.x;
		updatePtList[i].Y += picPoint.y;
	}

	g->DrawPolygon(&newPen, ptList, 4);
	g->DrawPolygon(&newPen, updatePtList, 4);
	newPen.SetDashStyle(DashStyleSolid);
	SolidBrush sizeBrush(Color(255, 255, 255));
	for (int i = 0; i < 4; i++)
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

	float xSum = 0;
	float ySum = 0;

	for (int i = 0; i < 4; i++)
	{
		xSum += pts[i].X;
		ySum += pts[i].Y;
	}

	float centerX = xSum / m_arrPoint.size();
	float centerY = ySum / m_arrPoint.size();


	float X = (pts[0].X + pts[1].X) / 2;
	float Y = (pts[0].Y + pts[1].Y) / 2;
	float dx = X - centerX;
	float dy = Y - centerY;

	float t = atan2(dx, dy);
	m_RotatePoint.X = X + 20 * sin(t);
	m_RotatePoint.Y = Y + 20 * cos(t);
	SolidBrush rotateBrush(Color(100, 255, 0));
	g->FillEllipse(&rotateBrush, m_RotatePoint.X - 5, m_RotatePoint.Y - 5, 10.f, 10.f);
	g->DrawEllipse(&newPen, m_RotatePoint.X - 5, m_RotatePoint.Y - 5, 10.f, 10.f);
}

void CRectangle::Move(const PointF& downPoint, const PointF& upPoint)
{
	float difX = upPoint.X - downPoint.X;
	float difY = upPoint.Y - downPoint.Y;
	for (int i = 0; i < 4; i++)
	{
		m_arrUpdatePoint[i].X += difX;
		m_arrUpdatePoint[i].Y += difY;
	}
}

void CRectangle::reSize(const PointF& downPoint, const PointF& upPoint)
{
	PointF dif = upPoint - downPoint;
	m_arrUpdatePoint[m_nClickState - 1] = m_arrUpdatePoint[m_nClickState - 1] + dif;

	if(m_fTheta == 0)
	{
		if (m_nClickState == 1)
		{
			m_arrUpdatePoint[1].Y += dif.Y;
			m_arrUpdatePoint[3].X += dif.X;
		}
		else if (m_nClickState == 2)
		{
			m_arrUpdatePoint[0].Y += dif.Y;
			m_arrUpdatePoint[2].X += dif.X;
		}
		else if (m_nClickState == 3)
		{
			m_arrUpdatePoint[3].Y += dif.Y;
			m_arrUpdatePoint[1].X += dif.X;
		}
		else if (m_nClickState == 4)
		{
			m_arrUpdatePoint[2].Y += dif.Y;
			m_arrUpdatePoint[0].X += dif.X;
		}
	}
	else 
	{
		if (m_nClickState == 1)
		{
			m_arrUpdatePoint[1] = Intersection(m_arrUpdatePoint[0],
																				 m_arrUpdatePoint[1],
																				 m_arrUpdatePoint[2]);
			m_arrUpdatePoint[3] = Intersection(m_arrUpdatePoint[0],
																				 m_arrUpdatePoint[3],
																				 m_arrUpdatePoint[2]);
		}
		else if (m_nClickState == 2)
		{
			m_arrUpdatePoint[0] = Intersection(m_arrUpdatePoint[1],
																				 m_arrUpdatePoint[0],
																				 m_arrUpdatePoint[3]);
			m_arrUpdatePoint[2] = Intersection(m_arrUpdatePoint[1],
																				 m_arrUpdatePoint[2],
																				 m_arrUpdatePoint[3]);
		}
		else if (m_nClickState == 3)
		{
			m_arrUpdatePoint[1] = Intersection(m_arrUpdatePoint[2],
																				 m_arrUpdatePoint[1],
																				 m_arrUpdatePoint[0]);
			m_arrUpdatePoint[3] = Intersection(m_arrUpdatePoint[2],
																				 m_arrUpdatePoint[3],
																				 m_arrUpdatePoint[0]);
		}
		else if (m_nClickState == 4)
		{
			m_arrUpdatePoint[0] = Intersection(m_arrUpdatePoint[3],
																				 m_arrUpdatePoint[0],
																				 m_arrUpdatePoint[1]);
			m_arrUpdatePoint[2] = Intersection(m_arrUpdatePoint[3],
																				 m_arrUpdatePoint[2],
																				 m_arrUpdatePoint[1]);
		}
	}
}

bool CRectangle::HitTest(const PointF& imgPoint, const CPoint& clientPoint,
												 int* actionState)
{
	if (m_nClickState != -1)
	{
		for (int i = 0; i < 4; i++)
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
	if (isContainRect(imgPoint, m_arrPoint))
	{
		m_nClickState = 0;
		*actionState = Actions::AnnotatiaonUpdate;
		return true;
	}
	else
	{
		m_nClickState = -1;
		*actionState = Actions::SelectMode;
		return false;
	}

}

void CRectangle::Update(const PointF& downPoint, const PointF& firstPoint,
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

void CRectangle::Modify(void)
{
	if (1 <= m_nClickState && m_nClickState <= 4)
	{
		//업데이트이전의 0번째 점과 3번째 점의 각도
		float theta = atan2(m_arrPoint[0].X - m_arrPoint[3].X,
												m_arrPoint[0].Y - m_arrPoint[3].Y);
		
		//업데이트이후의 0번째 점과 3번째 점의 각도
		float theta1 = atan2(m_arrUpdatePoint[0].X - m_arrUpdatePoint[3].X,
												 m_arrUpdatePoint[0].Y - m_arrUpdatePoint[3].Y);
	
		//음수가 나올 경우 두 선의 위치관계가 변동
		if (theta * theta1 <= 0)
		{
			std::swap(m_arrUpdatePoint[0], m_arrUpdatePoint[3]);
			std::swap(m_arrUpdatePoint[1], m_arrUpdatePoint[2]);
		}
	}
	m_arrPoint = m_arrUpdatePoint;
	for (int i = 0; i < 4; i++)
	{
		m_arrSizeBoxes[i].X = m_arrPoint[i].X - m_arrSizeBoxes[i].Width;
		m_arrSizeBoxes[i].Y = m_arrPoint[i].Y - m_arrSizeBoxes[i].Width;
	}
	float xSum = 0, ySum = 0;
	for (unsigned i = 0; i < m_arrPoint.size(); i++)
	{
		xSum += m_arrPoint[i].X;
		ySum += m_arrPoint[i].Y;
	}

	m_Center.X = xSum / m_arrPoint.size();
	m_Center.Y = ySum / m_arrPoint.size();

	m_fTheta = m_fUpdateTheta;
}

void CRectangle::Rotate(const PointF& downPoint, const PointF& movePoint)
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

void CRectangle::SaveDB(const int& imageID, const bool& isSaveNew)
{
	if (isSaveNew)
	{
		m_nIndex = -1;
	}
	AnnotationData annotationData(m_nIndex, m_fPenWidth, DrawRectangle, imageID,
															  m_fTheta, m_Color, _T(""), _T(""), _T(""), 
																m_arrPoint);
	int idx = CDBManager::AddAnnotation(annotationData);
	if (idx != -1)
	{
		m_nIndex = idx;
	}
}

void CRectangle::UpdateMemoString(const UINT& nChar)
{
	throw std::logic_error("The method or operation is not implemented.");
}
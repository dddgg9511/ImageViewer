#include "stdafx.h"
#include "Annotation.h"
#include "Circle.h"
#include "DBManager.h"
#include "DataBaseStruct.h"

/**
@생성자
@downPoint	 : LbuttonDwon 좌표
@upPoint		 : 변경된 마우스의 좌표
@penColor		 : 현재 펜의 색상
@PenWidth		 : 현재 펜의 굵기
@ActionState : 추가될 Annotation의 종류
*/
CCircle::CCircle(const PointF& downPoint, const PointF& upPoint,
								 const Color& penColor, const float& penWidth) :
		IAnnotation(penColor, penWidth)
{
	std::pair<float, float> xPair = std::minmax<float>(downPoint.X, upPoint.X);
	std::pair<float, float> yPair = std::minmax<float>(downPoint.Y, upPoint.Y);

	float width = 0;
	float height = 0;
	//가로가 길 경우
	if (abs(xPair.first - xPair.second) > abs(yPair.first - yPair.second))
	{
		width = ((xPair.first - xPair.second) / 2) - 
						((yPair.first - yPair.second) / 2);
		xPair.second += width;
		xPair.first -= width;
	}
	//세로가 길 경우
	else
	{
		height = ((yPair.first - yPair.second) / 2) -
						 ((xPair.first - xPair.second) / 2);
		yPair.second += height;
		yPair.first -= height;
	}
	m_Center.X = (xPair.first + xPair.second) / 2;
	m_Center.Y = (yPair.first + yPair.second) / 2;
	m_arrPoint.reserve(4);
	m_arrSizeBoxes.resize(4);
	m_arrPoint.push_back(PointF(xPair.first, yPair.first));
	m_arrPoint.push_back(PointF(xPair.second, yPair.first));
	m_arrPoint.push_back(PointF(xPair.second, yPair.second));
	m_arrPoint.push_back(PointF(xPair.first, yPair.second));


	m_arrUpdatePoint = m_arrPoint;
}


CCircle::CCircle(const AnnotationData& annotationData) :
	IAnnotation(annotationData)
{

}


void CCircle::Draw(const float& orgRate, const float& zoomRate,
									 const PointF& orgPoint, const PointF& viewPoint,
									 const CPoint& size, const int& rotateState,
									 const bool& flipVertical, const bool& flipHorizontal,
									 const CPoint& picPoint, Graphics* g)
{
	PointF ptList[4];

	Pen newPen(m_Color, m_fPenWidth);
	for (unsigned int i = 0; i < m_arrPoint.size(); i++) {
		ptList[i] = Img2Client(m_arrPoint[i], zoomRate, orgRate, viewPoint, orgPoint,
													 flipVertical, flipHorizontal, rotateState, size);
		ptList[i].X += picPoint.x;
		ptList[i].Y += picPoint.y;
	}
	g->DrawEllipse(&newPen, ptList[0].X, ptList[0].Y, ptList[2].X - ptList[0].X,
							   ptList[2].Y - ptList[0].Y);
}


void CCircle::SelectDraw(const float& orgRate, const float& zoomRate,
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

	PointF updatePtList[4];
	for (unsigned int i = 0; i < m_arrPoint.size(); i++)
	{
		updatePtList[i] = Img2Client(m_arrUpdatePoint[i], zoomRate, orgRate,
																 viewPoint, orgPoint, flipVertical,
																 flipHorizontal, rotateState, size);
		updatePtList[i].X += picPoint.x;
		updatePtList[i].Y += picPoint.y;
	}
	Pen newPen(Color(0, 0, 0), 1);
	newPen.SetDashStyle(DashStyleDashDot);
	g->DrawEllipse(&newPen, updatePtList[0].X, updatePtList[0].Y,
								 updatePtList[2].X - updatePtList[0].X,
								 updatePtList[2].Y - updatePtList[0].Y);
	g->DrawEllipse(&newPen, ptList[0].X, ptList[0].Y, ptList[2].X - ptList[0].X,
								 ptList[2].Y - ptList[0].Y);
	newPen.SetDashStyle(DashStyleSolid);
	SolidBrush newBrush(Color(255, 255, 255));


	PointF* pts;

	if (m_nClickState == 5)
	{
		pts = &updatePtList[0];
	}
	else
	{
		pts = &ptList[0];
	}


	m_arrSizeBoxes[0] = RectF(m_Center.X, m_arrPoint[0].Y, 10, 10);
	m_arrSizeBoxes[1] = RectF(m_arrPoint[1].X, m_Center.Y, 10, 10);
	m_arrSizeBoxes[2] = RectF(m_Center.X, m_arrPoint[2].Y, 10, 10);
	m_arrSizeBoxes[3] = RectF(m_arrPoint[3].X, m_Center.Y, 10, 10);
	for (int i = 0; i < 4; i++)
	{
		PointF TL = Img2Client(PointF(m_arrSizeBoxes[i].X, m_arrSizeBoxes[i].Y),
													 zoomRate, orgRate, viewPoint, orgPoint, flipVertical,
													 flipHorizontal, rotateState, size);
		m_arrSizeBoxes[i].X = TL.X - 5 + picPoint.x;
		m_arrSizeBoxes[i].Y = TL.Y - 5 + picPoint.y;
		StringFormat SF;

		g->FillRectangle(&newBrush, m_arrSizeBoxes[i]);
		g->DrawRectangle(&newPen, m_arrSizeBoxes[i]);
	}
}


void CCircle::Move(const PointF& downPoint, const PointF& upPoint)
{
	float difX = upPoint.X - downPoint.X;
	float difY = upPoint.Y - downPoint.Y;
	for (unsigned int i = 0; i < 4; i++) {
		m_arrUpdatePoint[i].X += difX;
		m_arrUpdatePoint[i].Y += difY;
	}
}


void CCircle::reSize(const PointF& downPoint, const PointF& upPoint)
{
	float difX = upPoint.X - downPoint.X;
	float difY = upPoint.Y - downPoint.Y;
	float _move;

	if (abs(difX) > abs(difY))
	{
		_move = difX;
	}
	else
	{
		_move = difY;
	}
	if (m_nClickState == 1)
	{
		m_arrUpdatePoint[0].Y += difY;
		m_arrUpdatePoint[1].Y += difY;

		m_arrUpdatePoint[0].X += difY / 2;
		m_arrUpdatePoint[1].X -= difY / 2;
		m_arrUpdatePoint[2].X -= difY / 2;
		m_arrUpdatePoint[3].X += difY / 2;
	}
	else if (m_nClickState == 2)
	{
		m_arrUpdatePoint[1].X += difX;
		m_arrUpdatePoint[2].X += difX;

		m_arrUpdatePoint[0].Y -= difX / 2;
		m_arrUpdatePoint[1].Y -= difX / 2;
		m_arrUpdatePoint[2].Y += difX / 2;
		m_arrUpdatePoint[3].Y += difX / 2;
	}
	else if (m_nClickState == 3)
	{
		m_arrUpdatePoint[2].Y += difY;
		m_arrUpdatePoint[3].Y += difY;

		m_arrUpdatePoint[0].X -= difY / 2;
		m_arrUpdatePoint[1].X += difY / 2;
		m_arrUpdatePoint[2].X += difY / 2;
		m_arrUpdatePoint[3].X -= difY / 2;
	}
	else if (m_nClickState == 4)
	{
		m_arrUpdatePoint[0].X += difX;
		m_arrUpdatePoint[3].X += difX;

		m_arrUpdatePoint[0].Y += difX / 2;
		m_arrUpdatePoint[1].Y += difX / 2;
		m_arrUpdatePoint[2].Y -= difX / 2;
		m_arrUpdatePoint[3].Y -= difX / 2;
	}

}


bool CCircle::HitTest(const PointF& imgPoint, const CPoint& clientPoint,
										  int* actionState)
{
	if (m_nClickState != -1)
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			if (m_arrSizeBoxes[i].Contains(CPoint2PointF(clientPoint)))
			{
				m_nClickState = i + 1;
				*actionState = Actions::AnnotatiaonUpdate;
				return true;
			}
		}
	}
	const float centerX = (m_arrPoint[0].X + m_arrPoint[1].X) / 2;
	const float centerY = (m_arrPoint[0].Y + m_arrPoint[2].Y) / 2;
	float rad = fabs(centerX - m_arrPoint[0].X);
	float distance = sqrt(pow(imgPoint.X - centerX, 2) + 
									 pow(imgPoint.Y - centerY, 2));
	if (distance <= rad)
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

void CCircle::Update(const PointF& downPoint, const PointF& firstPoint,
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
}


void CCircle::Modify(void)
{
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
}


void CCircle::SaveDB(const int& imageID, const bool& isSaveNew)
{
	if (isSaveNew)
	{
		m_nIndex = -1;
	}
	AnnotationData annotationData(m_nIndex, m_fPenWidth, DrawCircle, imageID,
																m_fTheta,	m_Color, _T(""), _T(""), _T(""),
																m_arrPoint);
	int idx = CDBManager::AddAnnotation(annotationData);
	if (idx != -1)
	{
		m_nIndex = idx;
	}
}

void CCircle::UpdateMemoString(const UINT& nChar)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void CCircle::Rotate(const PointF& downPoint, const PointF& movePoint)
{
	throw std::logic_error("The method or operation is not implemented.");
}
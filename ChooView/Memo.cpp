#include "stdafx.h"
#include "Annotation.h"
#include "Memo.h"
#include "AnnotationPropertyDlg.h"
#include "DBManager.h"
#include "DataBaseStruct.h"

CMemo::CMemo(const PointF& downPoint, const PointF& upPoint, 
						 const Color& penColor,	const float& penWidth) :
		IAnnotation(penColor, penWidth),
		m_strMemo(_T("")),
		m_fFontWidth(20),
		m_fontColor(penColor)
{
	std::pair<float, float> xPair = std::minmax<float>(downPoint.X, upPoint.X);
	std::pair<float, float> yPair = std::minmax<float>(downPoint.Y, upPoint.Y);
	m_arrPoint.reserve(4);

	m_arrPoint.push_back(PointF(xPair.first, yPair.first));
	m_arrPoint.push_back(PointF(xPair.second, yPair.first));
	m_arrPoint.push_back(PointF(xPair.second, yPair.second));
	m_arrPoint.push_back(PointF(xPair.first, yPair.second));

	m_arrSizeBoxes.resize(4);
	m_Center.X = (xPair.first + xPair.second) / 2;
	m_Center.Y = (yPair.first + yPair.second) / 2;
	m_arrUpdatePoint = m_arrPoint;
}


CMemo::CMemo(const AnnotationData& annotationData) :
		IAnnotation(annotationData),
		m_strMemo(annotationData.m_strMemoString),
		m_fFontWidth(annotationData.m_fPenWidth),
		m_fontColor(annotationData.m_penColor)
{

}

void CMemo::Draw(const float& orgRate, const float& zoomRate,
								 const PointF& orgPoint, const PointF& viewPoint,
								 const CPoint& size, const int& rotateState,
								 const bool& flipVertical, const bool& flipHorizontal,
								 const CPoint& picPoint, Graphics* g)
{
	PointF ptList[4];

	Pen newPen(Color(0, 0, 0), m_fPenWidth);

	for (unsigned int i = 0; i < m_arrPoint.size(); i++) {
		ptList[i] = Img2Client(m_arrPoint[i], zoomRate, orgRate, viewPoint, orgPoint,
													 flipVertical, flipHorizontal, rotateState, size);
		ptList[i].X += picPoint.x;
		ptList[i].Y += picPoint.y;
	}

	float minX = FLOAT_MAX;
	float minY = FLOAT_MAX;
	float maxX = -1.f;
	float maxY = -1.f;
	for (unsigned int i = 0; i < 4; i++) {
		if (ptList[i].X < minX) 
		{
			minX = ptList[i].X;
		}
		if (ptList[i].X > maxX) 
		{
			maxX = ptList[i].X;
		}
		if (ptList[i].Y < minY) 
		{
			minY = ptList[i].Y;
		}
		if (ptList[i].Y > maxY) 
		{
			maxY = ptList[i].Y;
		}
	}

	StringFormat SF;
	SF.SetAlignment(StringAlignmentCenter);
	SF.SetLineAlignment(StringAlignmentCenter);

	SolidBrush brush(m_fontColor);
	RectF rect(minX, minY, maxX - minX, maxY - minY);
	Gdiplus::Font font(_T("Arial"), m_fFontWidth * zoomRate * orgRate,
									   FontStyleRegular, UnitPixel);
	g->DrawString(m_strMemo, -1, &font, rect, &SF, &brush);
}


void CMemo::SelectDraw(const float& orgRate, const float& zoomRate,
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
	SolidBrush newBrush(Color(255, 255, 255));
	for (int i = 0; i < 4; i++)
	{
		m_arrSizeBoxes[i] = RectF(ptList[i].X - 5, ptList[i].Y - 5, 10, 10);
		g->FillRectangle(&newBrush, m_arrSizeBoxes[i]);
		g->DrawRectangle(&newPen, m_arrSizeBoxes[i]);
	}
	PointF rotateBoxTL = Img2Client(m_RotatePoint, zoomRate, orgRate, viewPoint,
																	orgPoint, flipVertical, flipHorizontal,
																	rotateState, size);
	float minX = FLOAT_MAX;
	float minY = FLOAT_MAX;
	float maxX = -1;
	float maxY = -1;
	for (int i = 0; i < 4; i++)
	{
		if (ptList[i].X < minX) 
		{
			minX = ptList[i].X;
		}
		if (ptList[i].X > maxX) 
		{
			maxX = ptList[i].X;
		}
		if (ptList[i].Y < minY) 
		{
			minY = ptList[i].Y;
		}
		if (ptList[i].Y > maxY) 
		{
			maxY = ptList[i].Y;
		}
	}

	StringFormat SF;
	SF.SetAlignment(StringAlignmentCenter);
	SF.SetLineAlignment(StringAlignmentCenter);

	SolidBrush brush(m_fontColor);
	RectF rect(minX, minY, maxX - minX, maxY - minY);
	Gdiplus::Font font(_T("Arial"), m_fFontWidth * zoomRate * orgRate,
									 	FontStyleRegular, UnitPixel);
	g->DrawString(m_strMemo, -1, &font, rect, &SF, &brush);
}

void CMemo::Update(const PointF& downPoint, const PointF& firstPoint,
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

void CMemo::Move(const PointF& downPoint, const PointF& upPoint)
{
	float difX = upPoint.X - downPoint.X;
	float difY = upPoint.Y - downPoint.Y;
	for (int i = 0; i < 4; i++)
	{
		m_arrUpdatePoint[i].X += difX;
		m_arrUpdatePoint[i].Y += difY;
	}
}

void CMemo::reSize(const PointF& downPoint, const PointF& upPoint)
{
	float difX = upPoint.X - downPoint.X;
	float difY = upPoint.Y - downPoint.Y;

	m_arrUpdatePoint[m_nClickState - 1].X += difX;
	m_arrUpdatePoint[m_nClickState - 1].Y += difY;
	if (m_nClickState == 1)
	{
		m_arrUpdatePoint[3].X += difX;
		m_arrUpdatePoint[1].Y += difY;
	}
	else if (m_nClickState == 2)
	{

		m_arrUpdatePoint[2].X += difX;
		m_arrUpdatePoint[0].Y += difY;
	}
	else if (m_nClickState == 3)
	{
		m_arrUpdatePoint[1].X += difX;
		m_arrUpdatePoint[3].Y += difY;
	}
	else if (m_nClickState == 4)
	{
		m_arrUpdatePoint[0].X += difX;
		m_arrUpdatePoint[2].Y += difY;
	}
}

bool CMemo::HitTest(const PointF& imgPoint, const CPoint& clientPoint,
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
	}
	if (isInLine(imgPoint, m_arrPoint[0], m_arrPoint[1]) ||
			isInLine(imgPoint, m_arrPoint[1], m_arrPoint[2]) ||
			isInLine(imgPoint, m_arrPoint[2], m_arrPoint[3]) ||
			isInLine(imgPoint, m_arrPoint[0], m_arrPoint[3]))
	{
		m_nClickState = 0;
		*actionState = Actions::AnnotatiaonUpdate;
		return true;
	}
	if (isContainRect(imgPoint, this->m_arrPoint))
	{
		m_nClickState = 0;
		*actionState = Actions::MemoUpdate;
		return true;
	}
	else
	{
		*actionState = Actions::SelectMode;
		m_nClickState = -1;
		return false;
	}
}

void CMemo::Modify(void)
{
	std::copy(m_arrUpdatePoint.begin(), m_arrUpdatePoint.end(), 
						m_arrPoint.begin());
	for (int i = 0; i < 4; i++)
	{
		m_arrSizeBoxes[i].X = m_arrPoint[i].X - m_arrSizeBoxes[i].Width;
		m_arrSizeBoxes[i].Y = m_arrPoint[i].Y - m_arrSizeBoxes[i].Width;
	}
	float xSum = 0, ySum = 0;
	for (unsigned int i = 0; i < m_arrPoint.size(); i++)
	{
		xSum += m_arrPoint[i].X;
		ySum += m_arrPoint[i].Y;

	}
	m_Center.X = xSum / m_arrPoint.size();
	m_Center.Y = ySum / m_arrPoint.size();
}

void CMemo::Rotate(const PointF& downPoint, const PointF& movePoint)
{
	throw std::logic_error("The method or operation is not implemented.");
}


void CMemo::UpdateMemoString(const UINT& nChar)
{
	int strIndex = m_strMemo.GetLength();
	if (nChar == VK_BACK)
	{
		m_strMemo.Delete(strIndex - 1, 1);
	}
	else if (nChar == '\r')
	{
		m_strMemo.AppendChar('\n');
	}
	else
	{
		m_strMemo.AppendChar(nChar);
	}
}

void CMemo::SaveDB(const int& imageID, const bool& isSaveNew)
{
	if (isSaveNew)
	{
		m_nIndex = -1;
	}
	AnnotationData annotationData(m_nIndex, m_fFontWidth, DrawMemo, imageID, m_fTheta,
														    m_Color, m_strMemo, _T(""), _T(""), m_arrPoint);
	int idx = CDBManager::AddAnnotation(annotationData);
	if (idx != -1)
	{
		m_nIndex = idx;
	}
}

CMemo::~CMemo()
{

}

void CMemo::SetAnnotationProperty()
{
	CAnnotationPropertyDlg dlg(m_Color, m_fPenWidth, m_fontColor, m_fFontWidth);
	if (dlg.DoModal() == IDOK) 
	{
		m_Color = dlg.m_fontColor;
		m_fPenWidth = dlg.m_EditFontWidth;
		m_fontColor = dlg.m_fontColor;
		m_fFontWidth = dlg.m_EditFontWidth;
	}
}

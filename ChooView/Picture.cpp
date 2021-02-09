#include "stdafx.h"
#include "Picture.h"
#include "AnnotationManager.h"
#include "DBManager.h"
#include "DataBaseStruct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RotateFlipType FlipNoneArr[4] = { RotateNoneFlipNone,Rotate90FlipNone,
																  Rotate180FlipNone,Rotate270FlipNone };
RotateFlipType FlipXArr[4] = { RotateNoneFlipX,Rotate90FlipX,Rotate180FlipX,
															 Rotate270FlipX };
RotateFlipType FlipYArr[4] = { Rotate180FlipX,Rotate270FlipX,RotateNoneFlipX,
															 Rotate90FlipX };
RotateFlipType FlipXYArr[4] = { Rotate180FlipNone,Rotate270FlipNone,
																RotateNoneFlipNone,Rotate90FlipNone };

int round(double val) { return static_cast<int>(floor(val + 0.5)); }
int round(float val) { return static_cast<int>(floor(val + 0.5)); }


CPicture::CPicture(const CString& fileName) :
	m_fZoomRate(1.f),
	m_nRotateState(0),
	m_bFlipVertical(false),
	m_bFlipHorizontal(false),
	m_nWindowWidth(256),
	m_nWindowCenter(128),
	m_nIndex(-1),
	m_strFileName(fileName)
{
	m_annotationManager = new CAnnotationManager();
}


CPicture::CPicture(const ImageData& imageData) :
	m_fZoomRate(imageData.m_fZoomRate),
	m_viewPoint(imageData.m_pan),
	m_nRotateState(imageData.m_nRotateState),
	m_bFlipHorizontal(imageData.m_bFlipHorizontal),
	m_bFlipVertical(imageData.m_bFlipVertical),
	m_nWindowCenter(imageData.m_nWindowCenter),
	m_nWindowWidth(imageData.m_nWindowWidth),
	m_cropTl(imageData.m_cropTl),
	m_cropBr(imageData.m_cropBr),
	m_nIndex(imageData.m_nImageID),
	m_strFileName(imageData.m_strFileName)
{
	m_annotationManager = new CAnnotationManager();
}


void CPicture::PicDraw(const int& actionState, const Rect& PicRect,
											 const bool& isSelect, CDC* mem)
{
	m_picRect = PicRect;
	//원본이미지를 조작하기 위한 임시 객체
	Image* temp;
	temp = m_image->Clone();

	if (m_bFlipVertical & m_bFlipHorizontal)
	{
		temp->RotateFlip(FlipXYArr[m_nRotateState]);
	}
	else if (m_bFlipVertical)
	{
		temp->RotateFlip(FlipXArr[m_nRotateState]);
	}
	else if (m_bFlipHorizontal)
	{
		temp->RotateFlip(FlipYArr[m_nRotateState]);
	}
	else
	{
		temp->RotateFlip(FlipNoneArr[m_nRotateState]);
	}
	m_rotateRect = RectF(0, 0, static_cast<float>(temp->GetWidth()),
		static_cast<float>(temp->GetHeight()));

	setOrgRect();
#pragma region Image Control

	//Image와 orgRect의 비율
	float orgRate = m_picRect.Width / m_orgRect.Width;

	//Crop 영역 TopLeft,bottomRight 좌표
	PointF tl = Img2Client(m_cropTl, m_fZoomRate, orgRate, m_viewPoint,
												PointF(m_orgRect.X, m_orgRect.Y), m_bFlipVertical,
												m_bFlipHorizontal, m_nRotateState, m_size);
	PointF br = Img2Client(m_cropBr, m_fZoomRate, orgRate, m_viewPoint,
												PointF(m_orgRect.X, m_orgRect.Y), m_bFlipVertical,
												m_bFlipHorizontal, m_nRotateState, m_size);
	std::pair<float, float> xPair = std::minmax<float>(tl.X, br.X);
	std::pair<float, float> yPair = std::minmax<float>(tl.Y, br.Y);

	//Crop 영역
	RectF CropRect(m_picRect.X + xPair.first, m_picRect.Y + yPair.first,
								(xPair.second - xPair.first), (yPair.second - yPair.first));

	windowLevelAction(temp);

	Graphics pictureGraphic(mem->m_hDC);
	if (m_fZoomRate * orgRate > 1.f)
	{
		pictureGraphic.SetSmoothingMode(SmoothingModeHighQuality);
		pictureGraphic.SetInterpolationMode(InterpolationModeBilinear);
	}
	else
	{
		pictureGraphic.SetSmoothingMode(SmoothingModeHighSpeed);
		pictureGraphic.SetInterpolationMode(InterpolationModeInvalid);
	}

	RectF picRectF(m_picRect.X * 1.f, m_picRect.Y * 1.f, m_picRect.Width * 1.f,
								 m_picRect.Height * 1.f);
	pictureGraphic.IntersectClip(CropRect);
	pictureGraphic.DrawImage(temp, picRectF,
													 m_orgRect.X + m_viewPoint.X,
													 m_orgRect.Y + m_viewPoint.Y,
													 m_orgRect.Width / m_fZoomRate,
													 m_orgRect.Height / m_fZoomRate, UnitPixel);
	pictureGraphic.SetClip(picRectF);
	m_annotationManager->AnnotationDraw(actionState, orgRate, m_fZoomRate,
																			PointF(m_orgRect.X, m_orgRect.Y),
																			m_viewPoint, m_size, m_nRotateState,
																			m_bFlipVertical, m_bFlipHorizontal,
																			CPoint(m_picRect.X, m_picRect.Y),
																			isSelect, &pictureGraphic);
#pragma endregion Image Control
	OverlayText(mem);
	delete temp;
}



bool CPicture::HitTest(const CPoint& point, int* actionState)
{
	if (*actionState == Actions::SelectMode || *actionState == Actions::MemoUpdate ||
		*actionState == Actions::AnnotatiaonUpdate)
	{
		float orgRate = m_picRect.Width / m_orgRect.Width;
		PointF dPoint = Client2Img(point, m_fZoomRate, orgRate, m_viewPoint, m_orgRect,
															 m_bFlipVertical, m_bFlipHorizontal,
															 m_nRotateState, m_size,
															 CPoint(m_picRect.X, m_picRect.Y));
		m_annotationManager->HitTest(dPoint, point, actionState);
	}
	if (m_picRect.X < point.x && point.x <= m_picRect.X + m_picRect.Width)
	{
		if (m_picRect.Y < point.y && point.y <= m_picRect.Y + m_picRect.Height)
		{
			return true;
		}
	}
	return false;
}




void CPicture::PicZoom(const CPoint& zoomPoint, const CPoint& firstPoint,
											 const CPoint& secondPoint)
{
	PointF zoomPointF = CPoint2PointF(zoomPoint);
	const float prevZoomrate = m_fZoomRate;
	if (secondPoint.y == firstPoint.y)
	{
		return;
	}
	m_fZoomRate += 0.01f * (firstPoint.y - secondPoint.y);

	if (m_fZoomRate <= 0.02f)
	{
		m_fZoomRate = 0.01f;
	}
	const float orgRate = m_picRect.Width / m_orgRect.Width;
	m_viewPoint.X += ((zoomPointF.X - m_picRect.X) / orgRate) / prevZoomrate -
										((zoomPointF.X - m_picRect.X) / orgRate) / m_fZoomRate;
	m_viewPoint.Y += ((zoomPointF.Y - m_picRect.Y) / orgRate) / prevZoomrate -
										((zoomPointF.Y - m_picRect.Y) / orgRate) / m_fZoomRate;
}



void CPicture::PicPan(const CPoint& downPoint, const CPoint& upPoint)
{
	const float orgRate = m_picRect.Width / m_orgRect.Width;
	m_viewPoint.X += ((downPoint.x - m_picRect.X) / orgRate) / m_fZoomRate -
									 ((upPoint.x - m_picRect.X) / orgRate) / m_fZoomRate;
	m_viewPoint.Y += ((downPoint.y - m_picRect.Y) / orgRate) / m_fZoomRate -
									 ((upPoint.y - m_picRect.Y) / orgRate) / m_fZoomRate;
}


void CPicture::clear()
{
	m_fZoomRate = 1.f;
	m_viewPoint.X = 0;
	m_viewPoint.Y = 0;
	m_nRotateState = 0;
	m_bFlipVertical = false;
	m_bFlipHorizontal = false;
	m_nWindowWidth = 256;
	m_nWindowCenter = 128;
	m_cropBr.X = static_cast<float>(m_image->GetWidth());
	m_cropBr.Y = static_cast<float>(m_image->GetHeight());
	m_cropTl.X = 0;
	m_cropTl.Y = 0;
	m_annotationManager->ListReset();
}





void CPicture::RotateRight(void)
{
	if ((m_bFlipHorizontal || m_bFlipVertical) && 
		 !(m_bFlipHorizontal & m_bFlipVertical))
	{
		m_nRotateState = (m_nRotateState + 3) % 4;
	}
	else
	{
		m_nRotateState = (m_nRotateState + 1) % 4;
	}
}


void CPicture::RotateLeft(void)
{
	if ((m_bFlipHorizontal || m_bFlipVertical) && 
		 !(m_bFlipHorizontal & m_bFlipVertical))
	{
		m_nRotateState = (m_nRotateState + 1) % 4;
	}
	else
	{
		m_nRotateState = (m_nRotateState + 3) % 4;
	}
}


void CPicture::FlipVertical(void)
{
	m_bFlipVertical = !m_bFlipVertical;
}


void CPicture::FlipHorizontal(void)
{
	m_bFlipHorizontal = !m_bFlipHorizontal;
}


void CPicture::AnnotationUpdate(const CPoint& downPoint,
								const CPoint& firstPoint,
								const CPoint& secondPoint)
{
	const float orgRate = m_picRect.Width / m_orgRect.Width;
	const PointF dPoint = Client2Img(downPoint, m_fZoomRate, orgRate, m_viewPoint,
																	 m_orgRect, m_bFlipVertical, m_bFlipHorizontal,
																	 m_nRotateState,
																	 CPoint(m_image->GetWidth(), m_image->GetHeight()),
																	 CPoint(m_picRect.X, m_picRect.Y));
	const PointF fPoint = Client2Img(firstPoint, m_fZoomRate, orgRate, m_viewPoint,
			   													 m_orgRect, m_bFlipVertical, m_bFlipHorizontal,
			   													 m_nRotateState,
			   													 CPoint(m_image->GetWidth(), m_image->GetHeight()),
			   													 CPoint(m_picRect.X, m_picRect.Y));
	const PointF sPoint = Client2Img(secondPoint, m_fZoomRate, orgRate, m_viewPoint,
																	 m_orgRect, m_bFlipVertical, m_bFlipHorizontal,
																	 m_nRotateState,
																	 CPoint(m_image->GetWidth(), m_image->GetHeight()),
																	 CPoint(m_picRect.X, m_picRect.Y));
	m_annotationManager->AnnotationUpdate(dPoint, fPoint, sPoint);
}


void CPicture::annotationModify(void)
{
	m_annotationManager->annotationModify();
}



void CPicture::WindowLeveling(const CPoint& downPoint, const CPoint& upPoint)
{
	m_nWindowWidth += upPoint.x - downPoint.x;
	if (m_nWindowWidth <= 0)
	{
		m_nWindowWidth = 1;
	}
	m_nWindowCenter -= upPoint.y - downPoint.y;
}


void CPicture::PicCrop(const CPoint& downPoint, const CPoint& upPoint)
{
	if (downPoint == upPoint)
	{
		return;
	}
	std::pair<int, int> xPair = std::minmax<int>(downPoint.x, upPoint.x);
	std::pair<int, int> yPair = std::minmax<int>(downPoint.y, upPoint.y);
	float orgRate = m_picRect.Width / m_orgRect.Width;

	m_cropBr = Client2Img(CPoint(xPair.first, yPair.first), m_fZoomRate, orgRate,
												m_viewPoint, m_orgRect, m_bFlipVertical, m_bFlipHorizontal,
												m_nRotateState, CPoint(m_image->GetWidth(),
												m_image->GetHeight()), CPoint(m_picRect.X, m_picRect.Y));
	m_cropTl = Client2Img(CPoint(xPair.second, yPair.second), m_fZoomRate, orgRate,
												m_viewPoint, m_orgRect, m_bFlipVertical, m_bFlipHorizontal,
												m_nRotateState, CPoint(m_image->GetWidth(),
												m_image->GetHeight()), CPoint(m_picRect.X, m_picRect.Y));
}



void CPicture::OverlayText(CDC* dc)
{
	dc->SetBkMode(TRANSPARENT);
	Graphics textGraphic(*dc);
	FontFamily fontFamily(_T("Arial"));
	StringFormat strformat;
	CString str;
	float orgRate = m_picRect.Width / m_orgRect.Width;
	str.Format(_T("WC : %d\nWW : %d\nZoom : %d %%"), m_nWindowCenter, m_nWindowWidth,
						static_cast<int>(m_fZoomRate * orgRate * 100));
	GraphicsPath path;
	path.AddString(str, wcslen(str), &fontFamily, FontStyleRegular,
								 20, Point(m_picRect.X, m_picRect.Y), &strformat);
	Pen pen(Color(255, 255, 255), 2);
	textGraphic.DrawPath(&pen, &path);
	SolidBrush brush(Color(0, 0, 0));
	textGraphic.FillPath(&brush, &path);
}



Rect CPicture::Draw(const int& actionState, const Rect& clientRect, CDC* dc)
{
	//원본이미지를 조작하기 위한 임시 객체
	Image* temp;
	temp = m_image->Clone();

	if (m_bFlipVertical & m_bFlipHorizontal)
	{
		temp->RotateFlip(FlipXYArr[m_nRotateState]);
	}
	else if (m_bFlipVertical)
	{
		temp->RotateFlip(FlipXArr[m_nRotateState]);
	}
	else if (m_bFlipHorizontal)
	{
		temp->RotateFlip(FlipYArr[m_nRotateState]);
	}
	else
	{
		temp->RotateFlip(FlipNoneArr[m_nRotateState]);
	}
	m_rotateRect = RectF(0, 0, static_cast<float>(temp->GetWidth()),
											 static_cast<float>(temp->GetHeight()));
	setOrgRect();


#pragma region Image Control
	//Image와 orgRect의 비율
	float orgRate = m_picRect.Width / m_orgRect.Width;
	//Image의 크기

	//Crop 영역 TopLeft,bottomRight 좌표
	PointF tl = Img2Client(m_cropTl, m_fZoomRate, orgRate, m_viewPoint,
												 PointF(m_orgRect.X, m_orgRect.Y), m_bFlipVertical,
												 m_bFlipHorizontal, m_nRotateState, m_size);
	PointF br = Img2Client(m_cropBr, m_fZoomRate, orgRate, m_viewPoint,
												 PointF(m_orgRect.X, m_orgRect.Y), m_bFlipVertical,
												 m_bFlipHorizontal, m_nRotateState, m_size);

	std::pair<float, float> xPair = std::minmax<float>(tl.X, br.X);
	std::pair<float, float> yPair = std::minmax<float>(tl.Y, br.Y);

	//Crop 영역
	RectF CropRect(m_picRect.X + xPair.first, m_picRect.Y + yPair.first,
								(xPair.second - xPair.first), (yPair.second - yPair.first));

	windowLevelAction(temp);


	//Image를 Draw하기 위한 그래픽 객체
	Graphics pictureGraphic(dc->m_hDC);
	if (m_fZoomRate * orgRate > 1.f)
	{
		pictureGraphic.SetSmoothingMode(SmoothingModeHighQuality);
		pictureGraphic.SetInterpolationMode(InterpolationModeBilinear);
	}
	else
	{
		pictureGraphic.SetSmoothingMode(SmoothingModeHighSpeed);
		pictureGraphic.SetInterpolationMode(InterpolationModeInvalid);
	}
	RectF picRectF(m_picRect.X * 1.f, m_picRect.Y * 1.f, m_picRect.Width * 1.f,
								 m_picRect.Height * 1.f);
	pictureGraphic.IntersectClip(CropRect);
	pictureGraphic.DrawImage(temp, picRectF,
													 m_orgRect.X + m_viewPoint.X,
													 m_orgRect.Y + m_viewPoint.Y,
													 m_orgRect.Width / m_fZoomRate,
													 m_orgRect.Height / m_fZoomRate, UnitPixel);

	dc->SetStretchBltMode(COLORONCOLOR);
	pictureGraphic.SetClip(picRectF);
	m_annotationManager->AnnotationDraw(actionState, orgRate, m_fZoomRate,
																			PointF(m_orgRect.X, m_orgRect.Y),
																			PointF(m_viewPoint.X, m_viewPoint.Y),
																			m_size, m_nRotateState, m_bFlipVertical,
																			m_bFlipHorizontal,
																			CPoint(m_picRect.X, m_picRect.Y),
																			true, &pictureGraphic);
#pragma endregion Image Control
	OverlayText(dc);
	delete temp;
	return m_picRect;
}



void CPicture::windowLevelAction(Image* image)
{
	//Image의 byte array을 얻기 위한 Bitmap 객체
	Bitmap* bmp = static_cast<Bitmap*>(image);
	//Bitmap의 특성이 저장된 변수
	BitmapData bmpData;
	Rect lockbitRt = Rect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	PixelFormat pf = bmp->GetPixelFormat();

	float bpp = 0;
	bmp->LockBits(&lockbitRt, ImageLockModeRead || ImageLockModeWrite, pf, &bmpData);

	int arrSize = bmpData.Stride * bmp->GetHeight();
	void* scan0 = bmpData.Scan0;
	//image의 Byte 배열 데이터
	byte* p = (byte*)scan0;

	for (int i = 0; i < arrSize; i += 3) {
		for (int j = 0; j < 3; j++) {
			int nVal = static_cast<int>(p[i + j]);
			float normValue = (((nVal - (m_nWindowCenter - 0.5f)) /
													(m_nWindowWidth - 1) + 0.5f) * 255.f);
			if (normValue < m_nWindowCenter - m_nWindowWidth / 2)
			{
				nVal = 0;
			}
			if (normValue > m_nWindowCenter + m_nWindowWidth / 2)
			{
				nVal = 255;
			}
			p[i + j] = (byte)nVal;
		}
	}
	bmp->UnlockBits(&bmpData);
}



void CPicture::setOrgRect()
{
	m_orgRect.X = 0;
	m_orgRect.Y = 0;
	m_orgRect.Width = m_rotateRect.Width;
	m_orgRect.Height = m_rotateRect.Height;

	//이미지의 비율
	const float imageRatio = m_rotateRect.Width * 1.f / m_rotateRect.Height * 1.f;
	//PicRect의 비율
	const float rectRatio = m_picRect.Width * 1.f / m_picRect.Height * 1.f;
	//이미지와 PicRect의 비율
	float scaleFactor;

	if (imageRatio < 1)
	{
		scaleFactor = m_rotateRect.Height / m_picRect.Height * 1.f;
		float rightWithRatio = m_picRect.Width * scaleFactor;
		float halfOfDif = (rightWithRatio - m_rotateRect.Width) / 2.f;
		m_orgRect.X = -halfOfDif;
		m_orgRect.Width = rightWithRatio;
		if (m_orgRect.Width < m_rotateRect.Width)
		{
			m_orgRect.X = 0;
			m_orgRect.Width = m_rotateRect.Width;
			scaleFactor = m_rotateRect.Width * 1.f / m_picRect.Width * 1.f;
			float bottomWithRatio = m_picRect.Height * scaleFactor;
			float halfOfDif = (bottomWithRatio - m_rotateRect.Height) / 2.f;
			m_orgRect.Y = -halfOfDif;
			m_orgRect.Height = bottomWithRatio;
		}
	}
	else if (imageRatio > 1)
	{
		scaleFactor = m_rotateRect.Width * 1.f / m_picRect.Width * 1.f;
		float bottomWithRatio = m_picRect.Height * scaleFactor;
		float halfOfDif = (bottomWithRatio - m_rotateRect.Height) / 2.f;
		m_orgRect.Y = -halfOfDif;
		m_orgRect.Height = bottomWithRatio;
		if (m_orgRect.Height < m_rotateRect.Height)
		{
			m_orgRect.Y = 0;
			m_orgRect.Height = m_rotateRect.Height;
			scaleFactor = m_rotateRect.Height * 1.f / m_picRect.Height * 1.f;
			float rightWithRatio = m_picRect.Width * scaleFactor;
			float halfOfDif = (rightWithRatio - m_rotateRect.Width) / 2.f;
			m_orgRect.X = -halfOfDif;
			m_orgRect.Width = rightWithRatio;
		}
	}
	else
	{
		if (rectRatio > 1)
		{
			scaleFactor = m_rotateRect.Height * 1.f / m_picRect.Height * 1.f;
			float rightWithRatio = m_picRect.Width * scaleFactor;
			float halfOfDif = (rightWithRatio - m_rotateRect.Width) / 2.f;
			m_orgRect.X = -halfOfDif;
			m_orgRect.Width = rightWithRatio;
		}
		else
		{
			scaleFactor = m_rotateRect.Width * 1.f / m_picRect.Width * 1.f;
			float bottomWithRatio = m_picRect.Height * scaleFactor;
			float halfOfDif = (bottomWithRatio - m_rotateRect.Height) / 2.f;
			m_orgRect.Y = -halfOfDif;
			m_orgRect.Height = bottomWithRatio;
		}
	}
	int a = 0;
}


void CPicture::UpdateMemo(const UINT& nChar)
{
	m_annotationManager->UpdateMemo(nChar);
}

void CPicture::SaveDB(const int& categoryID, const bool& isSaveNew)
{
	if (isSaveNew)
	{
		m_nIndex = -1;
	}
	ImageData imagedata(m_nIndex, categoryID, m_nWindowWidth, m_nWindowCenter,
										  m_nRotateState, m_bFlipVertical, m_bFlipHorizontal,
										  m_fZoomRate, _T(""), _T(""), m_strFileName, m_cropTl, 
											m_cropBr, m_viewPoint);
	int idx = CDBManager::AddImage(imagedata);

	if (idx != -1)
	{
		m_nIndex = idx;
		m_annotationManager->SaveDB(m_nIndex, isSaveNew);
	}
}


CPicture::~CPicture()
{
	delete m_annotationManager;
	delete m_image;
}


bool CPicture::InitPictureLoadDB()
{
	m_image = Image::FromFile(m_strFileName);
	if (m_image->GetLastStatus() != Ok)
	{
		AfxMessageBox(_T("Image Load Fail"));
		return false;
	}
	m_size.x = m_image->GetWidth();
	m_size.y = m_image->GetHeight();
	float orgRate = m_picRect.Width / m_orgRect.Width;
	m_annotationManager->LoadAnnotation(m_nIndex);
	return true;
}


bool CPicture::InitPicture()
{
	m_image = Image::FromFile(m_strFileName);
	if (m_image->GetLastStatus() != Ok)
	{
		AfxMessageBox(_T("Image Load Fail"));
		return false;
	}
	m_size.x = m_image->GetWidth();
	m_size.y = m_image->GetHeight();
	m_viewPoint.X = 0;
	m_viewPoint.Y = 0;
	m_cropBr = CPoint2PointF(m_size);
	m_cropTl.X = 0;
	m_cropTl.Y = 0;
	return true;
}

void CPicture::DeleteAnnotation()
{
	m_annotationManager->DeleteAnnotation();
}


void CPicture::SetAnnotationProperty()
{
	m_annotationManager->SetAnnotationProperty();
}


void CPicture::CreatePrototype(const Color& color, const float& fPenWidth,
															 const CPoint& point, const int& nActionState)
{
	m_annotationManager->CreatePrototype(color, fPenWidth, point, nActionState);
}


void CPicture::UpdatePrototype(const CPoint& point)
{
	m_annotationManager->UpdatePrototype(point);
}


void CPicture::ConvertPrototype()
{
	float orgRate = m_picRect.Width / m_orgRect.Width;
	m_annotationManager->ConvertPrototype(m_fZoomRate, orgRate, m_viewPoint,
																			  m_orgRect, m_bFlipVertical,
																			  m_bFlipHorizontal, m_nRotateState,
																			  CPoint(m_image->GetWidth(),
																			  m_image->GetHeight()),
																			  CPoint(m_picRect.X, m_picRect.Y));
}


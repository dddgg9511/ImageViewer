#include "stdafx.h"
#include "PicManager.h"
#include "Picture.h"
#include "DBManager.h"
#include "DataBaseStruct.h"

CPicManager::CPicManager() :
		m_nSelectIndex(-1),
		m_bDbClick(false),
		m_nRow(1),
		m_nCol(1),
		m_nPage(0),
		m_bIsChanged(false)
{
}


void CPicManager::AddImage(const CString& fileName)
{
	CPicture* picture = new CPicture(fileName);
	if (!picture->InitPicture())
	{
		delete picture;
		return;
	}
	m_arrPictures.push_back(picture);
	m_bIsChanged = true;
}




void CPicManager::Draw(const Rect& ClientRect, const int& nActionState, CDC* mem)
{
	if (m_bDbClick)
	{
		DbClickDraw(nActionState, ClientRect, mem);
		return;
	}
	Rect selectRect;
	bool Select = false;
	if (m_nRow * m_nCol == 1)
	{
		m_nSelectIndex = m_nPage;
	}
	unsigned int i = m_nRow * m_nCol * m_nPage;
	const int width = ClientRect.Width;
	const int height = ClientRect.Height;
	for (unsigned int r = 0; r < m_nRow; r++)
	{
		for (unsigned int c = 0; c < m_nCol; c++)
		{
			Rect rt(width * c / m_nCol, height * r / m_nRow, width / m_nCol, 
							height / m_nRow);
			if (i < m_arrPictures.size())
			{
				if (i == m_nSelectIndex)
				{
					m_arrPictures[i]->PicDraw(nActionState, rt, true, mem);
					if (m_nRow * m_nCol != 1)
					{
						selectRect = rt;
						Select = true;
					}
				}
				else
				{
					m_arrPictures[i]->PicDraw(nActionState, rt, false, mem);
				}
			}
			i++;
		}
	}
	if (Select)
	{
		CPen newPen(PS_SOLID, 1, RGB(255, 255, 0));
		CBrush NewBrush;
		LOGBRUSH lplb;
		lplb.lbStyle = BS_HOLLOW;
		lplb.lbHatch = 0;
		lplb.lbColor = RGB(0, 0, 0);
		NewBrush.CreateBrushIndirect(&lplb);
		CBrush* pOldBrush = mem->SelectObject(&NewBrush);
		CPen* pOldPen = mem->SelectObject(&newPen);
		mem->Rectangle(selectRect.X, selectRect.Y, selectRect.X + selectRect.Width,
									 selectRect.Y + selectRect.Height);
		mem->SelectObject(pOldBrush);
		mem->SelectObject(pOldPen);
	}
}


void CPicManager::HitTest(const CPoint& point, int* nActionState)
{
	bool flag = false;
	if (m_bDbClick)
	{
		m_arrPictures[m_nSelectIndex]->HitTest(point, nActionState);
		return;
	}
	for (unsigned int i = m_nRow * m_nCol * m_nPage; i < m_arrPictures.size() &&
			 i < m_nRow * m_nCol * (m_nPage + 1); i++)
	{
		if (flag = m_arrPictures[i]->HitTest(point, nActionState))
		{
			m_nSelectIndex = i;
			break;
		}
	}
	if (!m_bDbClick && !flag)
	{
		m_nSelectIndex = -1;
	}
}


void CPicManager::DbClickDraw(const int& nActionState, const Rect& ClientRect, 
															CDC* mem)
{
	if (!m_arrPictures.empty() && m_nSelectIndex != -1)
	{
		m_arrPictures[m_nSelectIndex]->PicDraw(nActionState, ClientRect,
																				 	 true, mem);
	}
}



void CPicManager::Zoom(const CPoint& zoomPoint, const CPoint& firstPoint,
											 const CPoint& secondPoint)
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->PicZoom(zoomPoint, firstPoint, secondPoint);
	}
}


void CPicManager::Pan(const CPoint& firstPoint, const CPoint& secondPoint)
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->PicPan(firstPoint, secondPoint);
	}
}


void CPicManager::PicClear()
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->clear();
	}
}



void CPicManager::RotateRight(void)
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->RotateRight();
	}
}


void CPicManager::RotateLeft(void)
{
	if (m_nSelectIndex != -1)
	{
		m_arrPictures[m_nSelectIndex]->RotateLeft();
	}
}



void CPicManager::FlipVertical(void)
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->FlipVertical();
	}
}



void CPicManager::FlipHorizontal(void)
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->FlipHorizontal();
	}
}



void CPicManager::AnnotationUpdate(const CPoint& downPoint,
								   const CPoint& firstPoint,
								   const CPoint& secondPoint)
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->AnnotationUpdate(downPoint, firstPoint,
																										secondPoint);
	}
}



void CPicManager::annotaionModify(void)
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->annotationModify();
	}
}



void CPicManager::WindowLeveling(const CPoint& firstPoint,
																 const CPoint& secondPoint)
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->WindowLeveling(firstPoint, secondPoint);
	}
}


void CPicManager::Crop(const CPoint& downPoint, const CPoint& upPoint)
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->PicCrop(downPoint, upPoint);
	}
}


void CPicManager::CloseAll()
{
	if(!m_arrPictures.empty())
	{
		for_each(m_arrPictures.begin(),m_arrPictures.end(),Delete());
		m_nSelectIndex = -1;
		m_arrPictures.clear();
	}
	return;
}



void CPicManager::UpdatePictureDraw(const int& nActionState,
																		const Rect& clientRect, CDC* dc)
{
	CBitmap memBitmap;
	//이미지를 Draw하기 위한 DC
	CDC MemDC;
	if (m_nSelectIndex == -1)
	{
		return;
	}
	if (m_nSelectIndex < m_nRow * m_nCol * m_nPage ||
			m_nSelectIndex >= m_nRow * m_nCol * (m_nPage + 1))
	{
		return;
	}
	MemDC.CreateCompatibleDC(dc);
	memBitmap.CreateCompatibleBitmap(dc, clientRect.Width, clientRect.Height);
	CBitmap* pOldBitmap = MemDC.SelectObject(&memBitmap);
	const Rect picRect = m_arrPictures[m_nSelectIndex]->
		Draw(nActionState, clientRect, &MemDC);

	CPen newPen(PS_SOLID, 1, RGB(255, 255, 0));
	CBrush NewBrush;
	LOGBRUSH lplb;
	lplb.lbStyle = BS_HOLLOW;
	lplb.lbHatch = 0;
	lplb.lbColor = RGB(0, 0, 0);
	NewBrush.CreateBrushIndirect(&lplb);
	CBrush* pOldBrush = MemDC.SelectObject(&NewBrush);
	CPen* pOldPen = MemDC.SelectObject(&newPen);

	MemDC.Rectangle(picRect.X, picRect.Y, picRect.GetRight(), picRect.GetBottom());
	dc->StretchBlt(picRect.X, picRect.Y, picRect.Width, picRect.Height, &MemDC,
								 picRect.X, picRect.Y, picRect.Width, picRect.Height, SRCCOPY);
	MemDC.SelectObject(pOldBrush);
	MemDC.SelectObject(pOldPen);
	MemDC.SelectObject(pOldBitmap);
	DeleteObject(memBitmap);
	MemDC.DeleteDC();
}


void CPicManager::UpdateMemo(const UINT& uChar)
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->UpdateMemo(uChar);
	}

}


void CPicManager::saveDataBase(const int& nCategoryID, const bool& bIsSaveNew)
{
	for (unsigned int i = 0; i < m_arrPictures.size(); i++)
	{
		m_arrPictures[i]->SaveDB(nCategoryID, bIsSaveNew);
	}
	if (bIsSaveNew)
	{
		m_arrDeletedPictures.clear();
	}
	else
	{
		for (unsigned int i = 0; i < m_arrDeletedPictures.size(); i++)
		{
			std::vector<int> AnnotationList;
			CDBManager::SelectAnnotationIdInImage(m_arrDeletedPictures[i],
																						&AnnotationList);
			for (unsigned int j = 0; j < AnnotationList.size(); j++)
			{
				CDBManager::DeletePointInAnnotation(AnnotationList[j]);
			}
			CDBManager::DeleteAnnotationInImage(m_arrDeletedPictures[i]);
			CDBManager::DeleteImageByID(m_arrDeletedPictures[i]);
		}
	}
	m_bIsChanged = false;
}


void CPicManager::loadImageInCategory(const int& nCategoryID)
{
	m_bIsChanged = false;
	std::vector<ImageData> imageList;
	CDBManager::SelectImageInCategory(nCategoryID, &imageList);
	m_arrPictures.reserve(imageList.size());
	for (unsigned int i = 0; i < imageList.size(); i++)
	{
		CPicture* picture = new CPicture(imageList[i]);
		if (!picture->InitPictureLoadDB())
		{
			delete picture;
			m_arrDeletedPictures.push_back(imageList[i].m_nImageID);
			m_bIsChanged = true;
			continue;
		}
		m_arrPictures.push_back(picture);
	}
	m_nSelectIndex = -1;
	SetMaxPage();
}


void CPicManager::LoadImageDatabase(const int& nImageID)
{
	ImageData imagedata;
	CDBManager::SelectImageByID(nImageID, &imagedata);
	CPicture* picture = new CPicture(imagedata);
	m_bIsChanged = true;
	if (!picture->InitPictureLoadDB())
	{
		delete picture;
		return;
	}
	m_arrPictures.push_back(picture);
	SetMaxPage();
}

void CPicManager::SetDbClickState()
{
	if (m_nSelectIndex != -1)
	{
		if (m_bDbClick)
		{
			m_nPage = m_nSelectIndex / (m_nCol * m_nRow);
		}
		m_bDbClick = !m_bDbClick;
	}
}


void CPicManager::PageUp()
{
	if (m_bDbClick) {
		if (m_nSelectIndex + 1 < m_arrPictures.size())
		{
			m_nSelectIndex++;
		}
		return;
	}
	if (m_nPage + 1 < m_nMaxPage)
	{
		m_nPage++;
	}
}


void CPicManager::PageDown()
{
	if (m_bDbClick)
	{
		if (m_nSelectIndex > 0)
		{
			m_nSelectIndex--;
		}
		return;
	}
	if (m_nPage > 0)
	{
		m_nPage--;
	}
}


void CPicManager::SetMaxPage()
{
	m_nMaxPage = static_cast<int>(ceil(m_arrPictures.size() * 1.f / 
																		(m_nRow * m_nCol) * 1.f));
	m_bDbClick = false;
	m_nPage = 0;
}


void CPicManager::CloseImage()
{
	if (m_nSelectIndex != -1 && !m_arrPictures.empty()) 
	{
		int imageID = m_arrPictures[m_nSelectIndex]->getIndex();
		if (imageID != -1)
		{
			m_arrDeletedPictures.push_back(imageID);
		}
		delete m_arrPictures[m_nSelectIndex];
		m_arrPictures.erase(m_arrPictures.begin() + m_nSelectIndex);
		m_nSelectIndex = -1;
		SetMaxPage();
	}
	m_bIsChanged = true;
}

CPicManager::~CPicManager()
{
	for_each(m_arrPictures.begin(),m_arrPictures.end(),Delete());
}

void CPicManager::DeleteAnnotation()
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->DeleteAnnotation();
	}
}

void CPicManager::SetAnnotationProperty()
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->SetAnnotationProperty();
	}
}


void CPicManager::CreatePrototype(const Color& color, const float& fPenWidth,
																	const CPoint& point, const int& nActionState)
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->CreatePrototype(color, fPenWidth, point,
																									 nActionState);
	};
}


void CPicManager::UpdatePrototype(const CPoint& point)
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->UpdatePrototype(point);
	};
}

void CPicManager::ConvertPrototype()
{
	if (m_nSelectIndex != -1)
	{
		m_bIsChanged = true;
		m_arrPictures[m_nSelectIndex]->ConvertPrototype();
	};
}


void CPicManager::AddImageList(const std::vector<CString>& fileList)
{
	m_arrPictures.reserve(m_arrPictures.size() + fileList.size());
	for (CString fileName : fileList)
	{
		AddImage(fileName);
	}
}

void CPicManager::SetLayOut(const int& row, const int& col)
{
	m_nRow = row;
	m_nCol = col;
	if (m_nSelectIndex < m_nRow * m_nCol * m_nPage ||
			m_nSelectIndex >= m_nRow * m_nCol * (m_nPage + 1))
	{
		m_nSelectIndex = -1;
	}
	SetMaxPage();
}

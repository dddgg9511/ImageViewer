
// ChildView.cpp : CChildView 클래스의 구현
//
#include "stdafx.h"
#include "ChooView.h"
#include "ChildView.h"
#include "PicManager.h"
#include "LayoutSetingDlg.h"
#include "PenWidthDlg.h"
#include "SaveDlg.h"
#include "OpenDlg.h"
#include "MainFrm.h"
#include "AnnotationPropertyDlg.h"
#include "AskSaveCategoryDlg.h"
#include "DBManager.h"
#include "DataBaseStruct.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CChildView::CChildView() :
	m_nActionState(Actions::SelectMode),
	m_movePoint(0),
	m_nCategoryID(-1),
	m_bRClick(false),
	m_bMenu(false)
{
	m_picManager = new CPicManager();
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_IMG_OPEN, &CChildView::OnImgOpen)
	ON_COMMAND(ID_Layout, &CChildView::OnLayout)
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_DRAW_LINE, &CChildView::OnDrawLine)
	ON_COMMAND(ID_DRAW_RECT, &CChildView::OnDrawRect)
	ON_COMMAND(ID_DRAW_CIRCLE, &CChildView::OnDrawCircle)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_TOOL_ZOOM, &CChildView::OnToolZoom)
	ON_COMMAND(ID_TOOL_PAN, &CChildView::OnToolPan)
	ON_COMMAND(ID_TOOL_CLEAR, &CChildView::OnToolClear)
	ON_COMMAND(ID_TOOL_ROTATERIGHT, &CChildView::OnToolRotateRight)
	ON_COMMAND(ID_TOOL_ROTATELEFT, &CChildView::OnToolRotateleft)
	ON_COMMAND(ID_TOOL_FLIPX, &CChildView::OnToolFlipVertical)
	ON_COMMAND(ID_TOOL_FLIPY, &CChildView::OnToolFlipHorizontal)
	ON_COMMAND(ID_ANNOTATION_SELECT, &CChildView::OnAnnotationSelect)
	ON_COMMAND(ID_TOOL_WINDOWLEVELING, &CChildView::OnToolWindowleveling)
	ON_COMMAND(ID_TOOL_CROP, &CChildView::OnToolCrop)
	ON_COMMAND(ID_ANNOTATION_PENCOLOR, &CChildView::OnAnnotationPencolor)
	ON_COMMAND(ID_ANNOTATION_PENWIDTH, &CChildView::OnAnnotationPenwidth)
	ON_COMMAND(ID_FILE_CLOSE32794, &CChildView::OnAllImageClose)
	ON_WM_DESTROY()
	ON_COMMAND(ID_ANNOTATION_MEMO, &CChildView::OnAnnotationMemo)
	ON_WM_CHAR()
	ON_COMMAND(ID_FILE_SAVE32796, &CChildView::OnFileSave)
	ON_COMMAND(ID_FILE_SAVEAS, &CChildView::OnFileSaveas)
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_TOOL_CLOSEIMAGE, &CChildView::OnToolCloseimage)
	ON_COMMAND(ID_ANNOTATION_ANNOTATIONDELETE, &CChildView::OnAnnotationAnnotationdelete)
	ON_COMMAND(ID_ANNOTATION_ANNOTATIONPROPERTY, &CChildView::OnAnnotationAnnotationproperty)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE32796, &CChildView::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEAS, &CChildView::OnUpdateFileSaveas)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE32794, &CChildView::OnUpdateFileClose)
END_MESSAGE_MAP()




// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}

void CChildView::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(&dc, m_ClientRect.Width, m_ClientRect.Height);
	CBitmap* pOldBitmap = MemDC.SelectObject(&memBitmap);
	MemDC.SetStretchBltMode(COLORONCOLOR);

	m_picManager->Draw(m_ClientRect, m_nActionState, &MemDC);
	dc.BitBlt(0, 0, m_ClientRect.Width, m_ClientRect.Height, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap);
	DeleteDC(MemDC);
	DeleteObject(memBitmap);
}

void CChildView::OnImgOpen()
{
	COpenDlg dlg(this);
	dlg.DoModal();
}


void CChildView::OnLayout()
{
	LayoutSetingDlg dlg(3, 3, m_picManager->GetCol(), m_picManager->GetRow());

	if (IDOK == dlg.DoModal())
	{
		m_picManager->SetLayOut(dlg.m_nRow, dlg.m_nColumn);
	}
	Invalidate(TRUE);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_curPenColor = Color(0, 0, 0);
	m_fPenWidth = 1;

	int nRow = 1;
	int nCol = 1;
	if (CDBManager::SelectUserProfile(&nRow, &nCol, &m_fPenWidth, &m_curPenColor))
	{
		m_picManager->SetLayOut(nRow, nCol);
	}
	//GDI+ 객체 초기화
	GdiplusStartup(&gpToken, &gpsi, NULL);

	return 0;
}

bool CChildView::LoadImage()
{
	static TCHAR BASED_CODE szFilter[] =
		_T("이미지 파일(*.BMP, *.GIF, *.JPG, *.PNG) | \
		   *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif;*.PNG;*.png |모든파일(*.*)|*.*||");
	CString File;
	CString strFileList;
	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"),
		OFN_ALLOWMULTISELECT, szFilter);
	const int c_cMaxFiles = 400;
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
	dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);

	dlg.GetOFN().nMaxFile = c_cbBuffSize;
	bool flag;
	if (flag = dlg.DoModal() == IDOK)
	{
		std::vector<CString> fileList;
		for (POSITION pos = dlg.GetStartPosition(); pos != NULL;)
		{
			File = dlg.GetNextPathName(pos);
			fileList.push_back(File);
		}
		m_picManager->AddImageList(fileList);
	}
	m_picManager->SetMaxPage();
	Invalidate(TRUE);
	return flag;
}

BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if (zDelta < 0)
	{
		m_picManager->PageUp();
	}
	else
	{
		m_picManager->PageDown();
	}
	Invalidate(TRUE);
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}


void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	RECT ClientRect;
	GetClientRect(&ClientRect);
	m_ClientRect = Rect(ClientRect.left, ClientRect.top, ClientRect.right,
											ClientRect.bottom);
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CChildView::OnDrawLine()
{
	m_nActionState = Actions::DrawLine;
}


void CChildView::OnDrawRect()
{
	m_nActionState = Actions::DrawRectangle;
}


void CChildView::OnDrawCircle()
{
	m_nActionState = Actions::DrawCircle;
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	::SetCapture(this->GetSafeHwnd());
	m_downPoint = point;
	m_picManager->HitTest(m_downPoint, &m_nActionState);
	if (Actions::DrawRectangle <= m_nActionState &&
			m_nActionState <= Actions::DrawLine)
	{
		m_picManager->CreatePrototype(m_curPenColor, m_fPenWidth, m_downPoint,
																	m_nActionState);
	}
	Invalidate(TRUE);
	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_picManager->SetDbClickState();
	Invalidate(TRUE);
	CWnd::OnLButtonDblClk(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	::ReleaseCapture();
	if (m_downPoint == point)
	{
		return;
	}
	if (Actions::DrawRectangle <= m_nActionState &&
		m_nActionState <= Actions::DrawLine)
	{
		m_picManager->ConvertPrototype();
		//Memo를 생성한 경우, Memo 문자열 작성
		if (m_nActionState == Actions::DrawMemo)
		{
			m_nActionState = Actions::MemoUpdate;
		}
		UpdateDraw();
	}
	else if (Actions::AnnotatiaonUpdate == m_nActionState)
	{
		m_picManager->annotaionModify();
		UpdateDraw();
		m_nActionState = Actions::SelectMode;
	}
	else if (Actions::Crop == m_nActionState)
	{
		m_picManager->Crop(m_downPoint, point);
		UpdateDraw();
	}
	Invalidate(TRUE);
	CWnd::OnLButtonUp(nFlags, point);
}



void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bMenu)
	{
		m_movePoint = point;
		m_bMenu = false;
		return;
	}
	if (nFlags & MK_LBUTTON)
	{
		switch (m_nActionState)
		{
		case Actions::Pan:
		{
			m_picManager->Pan(m_movePoint, point);
			break;
		}
		case Actions::DrawRectangle:
		case Actions::DrawCircle:
		case Actions::DrawLine:
		case Actions::DrawMemo:
		{
			m_picManager->UpdatePrototype(point);
			break;
		}
		case Actions::AnnotatiaonUpdate:
		{
			m_picManager->AnnotationUpdate(m_downPoint, m_movePoint, point);
			break;
		}
		case Actions::Zoom:
		{
			m_picManager->Zoom(m_downPoint, m_movePoint, point);
			break;
		}
		case Actions::WindowLeveling:
		{
			m_picManager->WindowLeveling(m_movePoint, point);
			break;
		}
		default:
			break;
		}
	}
	if (m_bRClick)
	{
		m_picManager->WindowLeveling(m_movePoint, point);
	}
	m_movePoint = point;
	UpdateDraw();

	CWnd::OnMouseMove(nFlags, point);
}


void CChildView::OnToolZoom()
{
	// TODO: Add your command handler code here
	m_nActionState = Actions::Zoom;
	Invalidate(TRUE);
}


void CChildView::OnToolPan()
{
	m_nActionState = Actions::Pan;
	Invalidate(TRUE);
}



void CChildView::OnToolClear()
{
	m_picManager->PicClear();
	Invalidate(TRUE);
}


void CChildView::OnToolRotateRight()
{
	m_picManager->RotateRight();
	Invalidate(TRUE);
}


void CChildView::OnToolRotateleft()
{
	m_picManager->RotateLeft();
	Invalidate(TRUE);
}


void CChildView::OnToolFlipVertical()
{
	// TODO: Add your command handler code here
	m_picManager->FlipVertical();
	Invalidate(TRUE);
}


void CChildView::OnToolFlipHorizontal()
{
	// TODO: Add your command handler code here
	m_picManager->FlipHorizontal();
	Invalidate(TRUE);
}


void CChildView::OnAnnotationSelect()
{
	// TODO: Add your command handler code here
	m_nActionState = Actions::SelectMode;
	Invalidate(TRUE);
}


void CChildView::OnToolWindowleveling()
{
	// TODO: Add your command handler code here
	m_nActionState = Actions::WindowLeveling;
	Invalidate(TRUE);
}


void CChildView::OnToolCrop()
{
	// TODO: Add your command handler code here
	m_nActionState = Actions::Crop;
	Invalidate(TRUE);
}


void CChildView::OnAnnotationPencolor()
{
	// TODO: Add your command handler code here
	CColorDialog dlg(RGB(0, 0, 0));
	CColorDialog colorDlg;
	if (colorDlg.DoModal() == IDOK)
	{
		m_curPenColor.SetFromCOLORREF(colorDlg.GetColor());
	}
}

void CChildView::OnAnnotationPenwidth()
{
	// TODO: Add your command handler code here
	CPenWidthDlg dlg(m_fPenWidth);
	if (IDOK == dlg.DoModal())
	{
		m_fPenWidth = dlg.m_fPenWidth;
	}
}

afx_msg void CChildView::OnAllImageClose()
{
	// TODO: Add your command handler code here
	if (!SaveCheck())
	{
		return;
	}
	m_picManager->CloseAll();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->SetWindowTextW(_T("ChooView"));
	m_nCategoryID = -1;
	Invalidate(TRUE);
}

void CChildView::OnDestroy()
{
	CWnd::OnDestroy();
	CDBManager::UpdateUserProfile(m_picManager->GetRow(), m_picManager->GetCol(),
																m_fPenWidth, m_curPenColor);
	delete m_picManager;
	GdiplusShutdown(gpToken);
}


void CChildView::OnAnnotationMemo()
{
	// TODO: Add your command handler code here
	m_nActionState = DrawMemo;
}


void CChildView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_nActionState != Actions::MemoUpdate)
		return;
	m_picManager->UpdateMemo(nChar);
	UpdateDraw();
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CChildView::OnFileSave()
{
	// TODO: Add your command handler code here
	CSaveDlg dlg;
	//이미 Save를 했거나 Load한 경우
	if (m_nCategoryID != -1)
	{
		m_picManager->saveDataBase(m_nCategoryID, false);
	}
	//새로운 카테고리를 만들어야 하는 경우
	else if (dlg.DoModal() == IDOK)
	{
		if (dlg.CategoryID != -1)
		{
			m_nCategoryID = dlg.CategoryID;
			m_picManager->saveDataBase(dlg.CategoryID, true);
		}
	}
}

void CChildView::LoadCategory(const int& nCategoryID)
{
	if(m_nCategoryID == nCategoryID)
	{
		AfxMessageBox(_T("이미 열려있는 Category를 열 수 없습니다."));
		return;
	}
	if (m_nCategoryID != -1 && !SaveCheck())
	{
		return;
	}
	m_picManager->CloseAll();

	m_nCategoryID = nCategoryID;
	CString strCategoryName = CDBManager::SelectCategoryNameById(nCategoryID);
	CString strWindowText;

	strWindowText.Format(_T("ChooView_%s"), strCategoryName);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->SetWindowTextW(strWindowText);

	m_picManager->loadImageInCategory(nCategoryID);
	Invalidate(TRUE);
}

void CChildView::LoadImageInDataBase(const int& nImageID)
{
	m_picManager->LoadImageDatabase(nImageID);
	Invalidate(TRUE);
}


void CChildView::OnFileSaveas()
{
	// TODO: Add your command handler code here
	CSaveDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.CategoryID != -1)
		{
			m_nCategoryID = dlg.CategoryID;
			m_picManager->saveDataBase(dlg.CategoryID, true);
		}
	}
}


void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	m_bRClick = false;

	if (point != m_rPoint)
	{
		return;
	}
	int nActionState = m_nActionState;
	m_picManager->HitTest(m_rPoint, &nActionState);
	Invalidate(TRUE);
	if (nActionState == Actions::AnnotatiaonUpdate ||
		nActionState == Actions::MemoUpdate)
	{
		CMenu menu;
		menu.LoadMenu(IDR_ANNOTATION);
		CRect windowRect;
		GetWindowRect(&windowRect);
		CMenu* pMenu = menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x + windowRect.left,
													point.y + windowRect.top, AfxGetMainWnd());
		m_bMenu = true;
	}
	else
	{
		CMenu menu;
		menu.LoadMenu(IDR_MAINFRAME);
		CRect windowRect;
		GetWindowRect(&windowRect);
		CMenu* pMenu = menu.GetSubMenu(3);

		pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x + windowRect.left,
													point.y + windowRect.top, AfxGetMainWnd());
		m_bMenu = true;
	}

	CWnd::OnRButtonUp(nFlags, point);
}


void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	::SetCapture(this->GetSafeHwnd());
	m_rPoint = point;
	m_bRClick = true;
	CWnd::OnRButtonDown(nFlags, point);
}


void CChildView::OnToolCloseimage()
{
	// TODO: Add your command handler code here
	m_picManager->CloseImage();
	Invalidate(TRUE);
}

void CChildView::UpdateDraw()
{
	if (m_picManager->GetRow() * m_picManager->GetCol() != 1 &&
		!m_picManager->GetDbClickState())
	{
		CDC* updateDC = GetDC();
		m_picManager->UpdatePictureDraw(m_nActionState, m_ClientRect, updateDC);
		ReleaseDC(updateDC);
	}
	else
	{
		Invalidate(TRUE);
	}
}

void CChildView::SetMenuState(bool isMenuOpened)
{
	m_bMenu = isMenuOpened;
}

void CChildView::OnAnnotationAnnotationdelete()
{
	// TODO: Add your command handler code here
	m_picManager->DeleteAnnotation();
	Invalidate(TRUE);
}


void CChildView::OnAnnotationAnnotationproperty()
{
	// TODO: Add your command handler code here 
	m_picManager->SetAnnotationProperty();
	Invalidate(TRUE);
}

/**
@변경사항 저장 설정
*/
bool CChildView::SaveCheck()
{
	if (m_nCategoryID == -1 && m_picManager->GetPictureCount() == 0)
	{
		return true;
	}
	if (m_picManager->GetChangeState())
	{
		if (m_picManager->GetPictureCount() == 0)
		{
			if (MessageBox(_T("Category의 Image가 없습니다.\n삭제하시겠습니까? "),
				_T("삭제"), MB_YESNO) == IDYES)
			{
				CDBManager::DeleteCategoryById(m_nCategoryID);
			}
			return true;
		}
		CAskSaveCategoryDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			int nSaveState = dlg.m_nSaveState;
			if (nSaveState == SaveState::SAVE)
			{
				OnFileSave();
			}
			else if (nSaveState == SaveState::SAVEAS)
			{
				OnFileSaveas();
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}


void CChildView::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	if (m_picManager->GetPictureCount() == 0)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}


void CChildView::OnUpdateFileSaveas(CCmdUI* pCmdUI)
{
	if (m_picManager->GetPictureCount() == 0)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}


void CChildView::OnUpdateFileClose(CCmdUI* pCmdUI)
{
	if (m_nCategoryID == -1)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

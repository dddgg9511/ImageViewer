
// ChildView.h : CChildView 클래스의 인터페이스
//
#pragma once
class CPicManager;


// CChildView 창
class CChildView : public CWnd
{
public:
	CChildView();
	virtual ~CChildView();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	/**
	@File Open Dialog를 통해 Image Open
	*/
	bool LoadImage();
	/**
	@DataBase에 저장된 CategoryLoad
	@nCategoryID : DataBase PrimaryKey
	*/
	void LoadCategory(const int& nCategoryID);
	/**
	@변경 사항이 생긴 Picture만 Draw
	*/
	void UpdateDraw();
	/**
	@DataBase PrimaryKey로 Image Load
	@nImageID : DataBase Primary Key
	*/
	void LoadImageInDataBase(const int& nImageID);
	/**
	@MenuOpen상태 설정
	*/
	void SetMenuState(bool isMenuOpened);
	/**
	@Open되어 있는 CategoryId 반환
	*/
	int GetCategoryID() const {return m_nCategoryID;}
	/**
	@저장 여부 체크
	*/
	bool SaveCheck();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnImgOpen();
	afx_msg void OnLayout();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDrawLine();
	afx_msg void OnDrawRect();
	afx_msg void OnDrawCircle();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnToolZoom();
	afx_msg void OnToolPan();
	afx_msg void OnToolClear();
	afx_msg void OnToolRotateRight();
	afx_msg void OnToolRotateleft();
	afx_msg void OnToolFlipVertical();
	afx_msg void OnToolFlipHorizontal();
	afx_msg void OnAnnotationSelect();
	afx_msg void OnToolWindowleveling();
	afx_msg void OnToolCrop();
	afx_msg void OnAnnotationPencolor();
	afx_msg void OnAnnotationPenwidth();
	afx_msg void OnAllImageClose();
	afx_msg void OnDestroy();
	afx_msg void OnAnnotationMemo();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveas();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnToolCloseimage();
	afx_msg void OnPaint();
	afx_msg void OnAnnotationAnnotationdelete();
	afx_msg void OnAnnotationAnnotationproperty();
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveas(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileClose(CCmdUI *pCmdUI);
private:
	float								m_fPenWidth;		//현재 펜의 굵기
	int									m_nCategoryID;	//Open된 Category DataBase Primary Key
	int									m_nActionState;	//마우스 동작
	bool								m_bRClick;	  	//Rbutton Click 상태
	bool								m_bMenu;	  		//Menu Open 상태
	CPoint							m_downPoint;  	//Lbutton Down 위치
	CPoint							m_rPoint;	  		//Rbutton Down 위치
	CPoint							m_movePoint;  	//Mouse move 위치
	Rect								m_ClientRect; 	//클라이언트의 크기
	Color								m_curPenColor;	//현재 펜의 색
	CPicManager*				m_picManager; 	//PicList의 Manager
	ULONG_PTR						gpToken;	  		//GDI+ 객체
	GdiplusStartupInput gpsi;			  		//GDI+ 객체
};


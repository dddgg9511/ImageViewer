
// ChildView.h : CChildView Ŭ������ �������̽�
//
#pragma once
class CPicManager;


// CChildView â
class CChildView : public CWnd
{
public:
	CChildView();
	virtual ~CChildView();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	/**
	@File Open Dialog�� ���� Image Open
	*/
	bool LoadImage();
	/**
	@DataBase�� ����� CategoryLoad
	@nCategoryID : DataBase PrimaryKey
	*/
	void LoadCategory(const int& nCategoryID);
	/**
	@���� ������ ���� Picture�� Draw
	*/
	void UpdateDraw();
	/**
	@DataBase PrimaryKey�� Image Load
	@nImageID : DataBase Primary Key
	*/
	void LoadImageInDataBase(const int& nImageID);
	/**
	@MenuOpen���� ����
	*/
	void SetMenuState(bool isMenuOpened);
	/**
	@Open�Ǿ� �ִ� CategoryId ��ȯ
	*/
	int GetCategoryID() const {return m_nCategoryID;}
	/**
	@���� ���� üũ
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
	float								m_fPenWidth;		//���� ���� ����
	int									m_nCategoryID;	//Open�� Category DataBase Primary Key
	int									m_nActionState;	//���콺 ����
	bool								m_bRClick;	  	//Rbutton Click ����
	bool								m_bMenu;	  		//Menu Open ����
	CPoint							m_downPoint;  	//Lbutton Down ��ġ
	CPoint							m_rPoint;	  		//Rbutton Down ��ġ
	CPoint							m_movePoint;  	//Mouse move ��ġ
	Rect								m_ClientRect; 	//Ŭ���̾�Ʈ�� ũ��
	Color								m_curPenColor;	//���� ���� ��
	CPicManager*				m_picManager; 	//PicList�� Manager
	ULONG_PTR						gpToken;	  		//GDI+ ��ü
	GdiplusStartupInput gpsi;			  		//GDI+ ��ü
};


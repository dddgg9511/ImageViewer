#pragma once
#include "afxbutton.h"
#include "resource.h"


// CAnnotationProperty dialog

class CAnnotationPropertyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnnotationPropertyDlg)
	afx_msg void OnBnClickedBtnFontcolor();
	afx_msg void OnBnClickedMfcbutton2();
public:
	// standard constructor
	CAnnotationPropertyDlg(Color color, float penWidth, CWnd* pParent = NULL);  
	CAnnotationPropertyDlg(Color color, float penWidth, Color fontColor,
												 float fontWidth, CWnd* pParent = NULL);
	virtual ~CAnnotationPropertyDlg();

// Dialog Data
	enum { IDD = IDD_ANNOTATION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CMFCButton	m_btnPenColor;		//PenColor ���� ��ư
	CMFCButton	m_btnFontColor;		//FontColor ���� ��ư
	Color				m_pencolor;				//Pen�� ����
	Color				m_fontColor;			//Font�� ����
	float				m_EditpenWidth;		//PenWidth ���� Edit Control
	float				m_EditFontWidth;	//FontWidth	���� Edit Control
	bool				m_bIsMemo;				//Annotation�� ������ �޸�����
};

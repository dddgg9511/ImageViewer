// AnnotationProperty.cpp : implementation file
//

#include "stdafx.h"
#include "ChooView.h"
#include "AnnotationPropertyDlg.h"
#include "afxdialogex.h"


// CAnnotationProperty dialog

IMPLEMENT_DYNAMIC(CAnnotationPropertyDlg, CDialogEx)


CAnnotationPropertyDlg::CAnnotationPropertyDlg(Color color, float penWidth,
																							 CWnd* pParent /*= NULL*/) :
		CDialogEx(CAnnotationPropertyDlg::IDD, pParent),
		m_pencolor(color),
		m_EditpenWidth(penWidth),
		m_fontColor(Color(0, 0, 0)),
		m_bIsMemo(FALSE),
		m_EditFontWidth(0)
{

}

CAnnotationPropertyDlg::CAnnotationPropertyDlg(Color color, float penWidth,
																							 Color fontColor, float fontWidth,
																							 CWnd* pParent /*= NULL*/) :
		CDialogEx(CAnnotationPropertyDlg::IDD, pParent),
		m_pencolor(color),
		m_EditpenWidth(penWidth),
		m_fontColor(fontColor),
		m_bIsMemo(TRUE),
		m_EditFontWidth(fontWidth)
{

}

CAnnotationPropertyDlg::~CAnnotationPropertyDlg()
{
}

void CAnnotationPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_PENCOLOR, m_btnPenColor);
	DDX_Text(pDX, IDC_penWidth, m_EditpenWidth);
	DDX_Control(pDX, IDC_BTN_FONTCOLOR, m_btnFontColor);
	DDX_Text(pDX, IDC_FONTWIDTH, m_EditFontWidth);
}


BEGIN_MESSAGE_MAP(CAnnotationPropertyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_PENCOLOR, &CAnnotationPropertyDlg::OnBnClickedMfcbutton2)
	ON_BN_CLICKED(IDC_BTN_FONTCOLOR, &CAnnotationPropertyDlg::OnBnClickedBtnFontcolor)
END_MESSAGE_MAP()


// CAnnotationProperty message handlers


BOOL CAnnotationPropertyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (!m_bIsMemo)
	{
		m_btnFontColor.EnableWindow(FALSE);
		GetDlgItem(IDC_FONTWIDTH)->EnableWindow(FALSE);
		m_btnPenColor.EnableWindowsTheming(FALSE);
		COLORREF lineColor(RGB(m_pencolor.GetR(), m_pencolor.GetG(),
													 m_pencolor.GetB()));
		m_btnPenColor.SetFaceColor(lineColor);
	}
	else
	{
		m_btnPenColor.EnableWindow(FALSE);
		GetDlgItem(IDC_penWidth)->EnableWindow(FALSE);
		COLORREF fontColor(RGB(m_fontColor.GetR(), m_fontColor.GetG(),
													 m_fontColor.GetB()));
		m_btnFontColor.EnableWindowsTheming(FALSE);
		m_btnFontColor.SetFaceColor(fontColor);
	}
	return true;
}


void CAnnotationPropertyDlg::OnBnClickedMfcbutton2()
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg(RGB(0, 0, 0));
	CColorDialog colorDlg;

	if (colorDlg.DoModal() == IDOK)
	{
		m_pencolor.SetFromCOLORREF(colorDlg.GetColor());
		m_btnPenColor.SetFaceColor(colorDlg.GetColor());
	}
}


void CAnnotationPropertyDlg::OnBnClickedBtnFontcolor()
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg(RGB(m_fontColor.GetR(), m_fontColor.GetG(),
											 m_fontColor.GetB()));
	m_btnFontColor.SendMessage(WM_KILLFOCUS, NULL);
	CColorDialog colorDlg;
	if (colorDlg.DoModal() == IDOK)
	{
		m_fontColor.SetFromCOLORREF(colorDlg.GetColor());
		m_btnFontColor.SetFaceColor(colorDlg.GetColor());
	}
}

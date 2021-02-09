// PenWidthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChooView.h"
#include "PenWidthDlg.h"
#include "afxdialogex.h"


// CPenWidthDlg dialog

IMPLEMENT_DYNAMIC(CPenWidthDlg, CDialogEx)

	CPenWidthDlg::CPenWidthDlg(float penWidth, CWnd* pParent /*= NULL*/) :
		CDialogEx(CPenWidthDlg::IDD, pParent),
		m_fPenWidth(penWidth)
{

}

CPenWidthDlg::~CPenWidthDlg()
{
}

void CPenWidthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fPenWidth);
}


BEGIN_MESSAGE_MAP(CPenWidthDlg, CDialogEx)
END_MESSAGE_MAP()


// CPenWidthDlg message handlers

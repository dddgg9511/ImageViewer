// LayoutSetingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChooView.h"
#include "LayoutSetingDlg.h"
#include "afxdialogex.h"


// LayoutSetingDlg dialog

IMPLEMENT_DYNAMIC(LayoutSetingDlg, CDialogEx)

	LayoutSetingDlg::LayoutSetingDlg(const int& nMaxColumn, const int& nMaxRow, 
																	 const int& nColumn, const int& nRow, 
																	 CWnd* pParent /*= NULL*/ ) :
	CDialogEx(LayoutSetingDlg::IDD, pParent),
	m_nMaxColumn(nMaxColumn),
	m_nMaxRow(nMaxRow),
	m_nColumn(nColumn),
	m_nRow(nRow)
{

}

LayoutSetingDlg::~LayoutSetingDlg()
{
}

void LayoutSetingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_COLUMN, m_cbColumn);
	DDX_Control(pDX, IDC_CB_ROW, m_cbRow);
}


BEGIN_MESSAGE_MAP(LayoutSetingDlg, CDialogEx)
END_MESSAGE_MAP()


// LayoutSetingDlg message handlers


BOOL LayoutSetingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cbColumn.Clear();
	m_cbRow.Clear();
	for (int i = 1; i <= m_nMaxColumn; i++)
	{
		CString str;
		str.Format(_T("%d"), i);
		m_cbColumn.AddString(str);
	}

	for (int i = 1; i <= m_nMaxRow; i++)
	{
		CString str;
		str.Format(_T("%d"), i);
		m_cbRow.AddString(str);
	}
	m_cbColumn.SetCurSel(m_nColumn - 1);
	m_cbRow.SetCurSel(m_nRow - 1);
	return TRUE;
}


void LayoutSetingDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogEx::OnOK();
	m_nColumn = m_cbColumn.GetCurSel() + 1;
	m_nRow = m_cbRow.GetCurSel() + 1;
}

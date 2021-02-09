// SaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChooView.h"
#include "SaveDlg.h"
#include "afxdialogex.h"
#include "DBManager.h"
#include "DataBaseStruct.h"

// CSaveDlg dialog

IMPLEMENT_DYNAMIC(CSaveDlg, CDialogEx)

	CSaveDlg::CSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSaveDlg::IDD, pParent)
	, CategoryName(_T(""))
	, Comment(_T(""))
{

}

CSaveDlg::~CSaveDlg()
{
}

void CSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, CategoryName);
	DDX_Text(pDX, IDC_EDIT2, Comment);
}


BEGIN_MESSAGE_MAP(CSaveDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSaveDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSaveDlg message handlers


void CSaveDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CategoryID = CDBManager::AddCategory(CategoryName, Comment);
	if (CategoryID == -1)
		return;
	CDialogEx::OnOK();
}




BOOL CSaveDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if (pMsg->hwnd == GetDlgItem(IDC_EDIT1)->GetSafeHwnd())
			{
				GetDlgItem(IDC_EDIT2)->SetFocus();
			}
			return FALSE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

// ModifyCategoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChooView.h"
#include "ModifyCategoryDlg.h"
#include "afxdialogex.h"
#include "DBManager.h"
#include "DataBaseStruct.h"

// CModifyCategoryDlg dialog

IMPLEMENT_DYNAMIC(CModifyCategoryDlg, CDialogEx)

	CModifyCategoryDlg::CModifyCategoryDlg(int nCategoryID,
																			   CString strCategoryName,
																			   CString strComment,
																			   CWnd* pParent /*= NULL*/) :
		CDialogEx(CModifyCategoryDlg::IDD, pParent),
		m_nCategoryID(nCategoryID),
		m_strCategoryName(strCategoryName),
		m_strComment(strComment)
{

}

CModifyCategoryDlg::~CModifyCategoryDlg()
{
}

void CModifyCategoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strCategoryName);
	DDX_Text(pDX, IDC_EDIT2, m_strComment);
}


BEGIN_MESSAGE_MAP(CModifyCategoryDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CModifyCategoryDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CModifyCategoryDlg message handlers


void CModifyCategoryDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_nCategoryID = CDBManager::UpdateCategory(m_nCategoryID, m_strCategoryName,
																						 m_strComment);
	if (m_nCategoryID == -1)
	{
			return;
}
	CDialogEx::OnOK();
}


BOOL CModifyCategoryDlg::PreTranslateMessage(MSG* pMsg)
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

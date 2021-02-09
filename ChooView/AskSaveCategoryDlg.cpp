// AskSaveCategoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChooView.h"
#include "AskSaveCategoryDlg.h"
#include "afxdialogex.h"


// CAskSaveCategoryDlg dialog

IMPLEMENT_DYNAMIC(CAskSaveCategoryDlg, CDialogEx)

CAskSaveCategoryDlg::CAskSaveCategoryDlg(CWnd* pParent /*=NULL*/) :
		CDialogEx(CAskSaveCategoryDlg::IDD, pParent),
		m_nSaveState(-1)
{

}

CAskSaveCategoryDlg::~CAskSaveCategoryDlg()
{
}

void CAskSaveCategoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAskSaveCategoryDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CAskSaveCategoryDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_SAVEAS, &CAskSaveCategoryDlg::OnBnClickedBtnSaveas)
	ON_BN_CLICKED(IDC_BTN_DONSAVE, &CAskSaveCategoryDlg::OnBnClickedBtnDonsave)
END_MESSAGE_MAP()


// CAskSaveCategoryDlg message handlers


BOOL CAskSaveCategoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CFont font;
	LOGFONT lf;
	lf.lfHeight = 20;
	::ZeroMemory(&lf, sizeof(lf));
	wsprintf(lf.lfFaceName, TEXT("%s"), TEXT("Arial"));
	font.CreateFontIndirect(&lf);
	GetDlgItem(IDC_STATIC)->SetFont(&font);
	return TRUE;
}


void CAskSaveCategoryDlg::OnBnClickedBtnSave()
{
	// TODO: Add your control notification handler code here
	m_nSaveState = SaveState::SAVE;
	OnOK();
}


void CAskSaveCategoryDlg::OnBnClickedBtnSaveas()
{
	// TODO: Add your control notification handler code here
	m_nSaveState = SaveState::SAVEAS;
	OnOK();
}


void CAskSaveCategoryDlg::OnBnClickedBtnDonsave()
{
	m_nSaveState = SaveState::DONTSAVE;
	OnOK();
	// TODO: Add your control notification handler code here
}

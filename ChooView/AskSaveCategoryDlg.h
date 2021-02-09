#pragma once

// CAskSaveCategoryDlg dialog

enum SaveState{SAVE,SAVEAS,DONTSAVE};

class CAskSaveCategoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAskSaveCategoryDlg)

public:
	CAskSaveCategoryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAskSaveCategoryDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSave();
	
	
	afx_msg void OnBnClickedBtnSaveas();
	afx_msg void OnBnClickedBtnDonsave();
	int m_nSaveState;	//저장 상태
};

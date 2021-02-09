#pragma once
#include "afxwin.h"


// CModifyCategoryDlg dialog

class CModifyCategoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyCategoryDlg)

public:
	CModifyCategoryDlg(int nCategoryID,CString strCategoryName,
										 CString strComment,CWnd* pParent = NULL);   // standard constructor
	virtual ~CModifyCategoryDlg();

// Dialog Data
	enum { IDD = IDD_MODIFY_CATEGORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	
	int			m_nCategoryID;					//Category DataBase PrimaryKey
	CString m_strCategoryName;			//Category Name
	CString m_strComment;						//Category Comment
	
};

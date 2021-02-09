#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// COpenDlg dialog

class COpenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpenDlg)

public:
	COpenDlg(CWnd* pParent);   // standard constructor
	virtual ~COpenDlg();
	enum { IDD = IDD_OPEN };
	virtual BOOL OnInitDialog();
	CTreeCtrl m_TreeCtrl;		//Tree Control 
	CListCtrl m_ListCtrl;		//List Control
	CWnd*			m_pParent;		//ºÎ¸ð
	void InitTree();
	void InitList();
	void AddMemoList(const int& imageID);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedDelete();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTvnItemexpandingTree1(NMHDR *pNMHDR, LRESULT *pResult);
};

#pragma once
#include "afxwin.h"


// LayoutSetingDlg dialog

class LayoutSetingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LayoutSetingDlg)

public:
	LayoutSetingDlg(const int& nMaxColumn, const int& nMaxRow, const int& nColumn,
									const int& nRow, CWnd* pParent = NULL);   // standard constructor
	virtual ~LayoutSetingDlg();

// Dialog Data
	enum { IDD = IDD_LAYOUT_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual void OnOK();

public:
	CComboBox m_cbColumn;				//Column ComboBox
	CComboBox m_cbRow;					//Row ComboBox
	int				m_nMaxColumn;			//�ִ� Column
	int				m_nMaxRow;				//�ִ�	 Row
	int				m_nColumn;				//Column ������
	int				m_nRow;						//Row ������
	
};

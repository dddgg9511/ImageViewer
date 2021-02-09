#pragma once


// CSaveDlg dialog

class CSaveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSaveDlg)

public:
	CSaveDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSaveDlg();

// Dialog Data
	enum { IDD = IDD_SAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedOk();
	CString CategoryName;		//������ Category�� �̸�
	CString Comment;				//������ Category�� Comment
	int			CategoryID;			//�߰��� Cagegory�� PrimaryKey
};

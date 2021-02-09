#pragma once
#include "afxwin.h"


// CPenWidthDlg dialog

class CPenWidthDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPenWidthDlg)

public:
	CPenWidthDlg(float penWidth,CWnd* pParent = NULL);   // standard constructor
	virtual ~CPenWidthDlg();

// Dialog Data
	enum { IDD = IDD_PENWIDTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float m_fPenWidth;	//∆Ê¿« ±Ω±‚
};


// ChooView.h : ChooView ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CChooViewApp:
// �� Ŭ������ ������ ���ؼ��� ChooView.cpp�� �����Ͻʽÿ�.
//

class CChooViewApp : public CWinApp
{
public:
	CChooViewApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CChooViewApp theApp;


// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

// debug_new.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_new.cpp
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new

#ifdef _DEBUG

#endif

#endif


#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.
#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����
#include <gdiplus.h>
#include <vector>
#include <algorithm>
#include <utility>
#pragma comment(lib, "gdiplus")
#pragma comment(lib,"sqlite3.lib")

using namespace Gdiplus;

#include "sqlite3.h"


class Delete
{
public:
	template <typename T>
	void operator() (const T* ptr) const
	{
		delete ptr;
	}
};


/**
@���� ���� ��ġ ���� ��ȯ
@linePt1 : ������ ������ ���� ��ǥ
@linePt2 : ������ ������ ���� ��ǥ
@pos		 : ���� ��ǥ
*/
float isLeft(const PointF& linePt1,
						 const PointF& linePt2,
						 const PointF& pos);
/**
@�ٰ��� ���ο� ���� ��ġ���� Ȯ��
@Pos			: ���� ��ǥ
@Vertices : �ٰ����� ���������� ��ǥ
*/
int isContainRect(const PointF& pos, const std::vector<PointF>& vertices);
/**
@Client ��ǥ���� Image ��ǥ�� ��ǥ ��ȯ
@point					: ������ ��ǥ
@zoomRate				: Zoom ����
@orgRate				: image�� ���� ������ ������ ä Ȯ���� ����
@ViewPointRect	:image�� Pan ��ǥ
@orgRect				: picRect���� ��µ� Image�� ��ǥ
@flipoVertical	: image�� ���� ����
@flipHorizontal : image�� ���� ����
@rotateState		: image�� ȸ�� ����
@Size						: image�� ũ��
@picPoint				: picRect�� TopLeft ��ǥ
@return					: ����� ��ǥ
*/
PointF Client2Img(const CPoint& point, const float& zoomRate,
									const float& orgRate, const PointF& viewPoint,
									const RectF& orgRect, const bool& flipVertical,
									const bool& flipHorizontal, const int& rotateState,
									const CPoint& size, const CPoint& picPoint);
/**
@Image��ǥ���� Client��ǥ�� ��ǥ ��ȯ
@point					: ������ ��ǥ
@zoomRate				: Zoom ����
@orgRate				: image�� ���� ������ ������ ä Ȯ���� ����
@ViewPointRect  : image�� Pan ��ǥ
@orgPoint				: picRect���� ��µ� Image�� ��ǥ
@flipoVertical  : image�� ���� ����
@flipHorizontal : image�� ���� ����
@rotateState		: image�� ȸ�� ����
@Size						: image�� ũ��
@picPoint				: picRect�� TopLeft ��ǥ
@return					:  ����� ��ǥ
*/
PointF Img2Client(const PointF& point, const float& zoomRate,
									const float& orgRate, const PointF& viewPoint,
									const PointF& orgPoint, const bool& flipVertical,
									const bool& flipHorizontal, const int& rotateState,
									const CPoint& size);
/**
@���� ���� ���� ��ǥ
@pt		 : ���� ��ǥ
@linePt1 : ������ ������ ���� ��ǥ
@linePt2 : ������ ������ ���� ��ǥ
*/
PointF Intersection(const PointF& pt, const PointF& LinePt1,
										const PointF& LinePt2);
PointF CPoint2PointF(const CPoint& point);
/**
@Lint�� ���� ��ġ���� Ȯ��
@point	 : ���� ��ǥ
@LinePt1 : ���� ��ǥ
@LinePt2 : ���� ��ǥ
*/
bool isInLine(const PointF& pt, const PointF& LinePt1, const PointF& LinePt2);



//Mouse Event�� ���� ���� ����
enum Actions
{
	SelectMode,Zoom,Pan,DrawRectangle,DrawCircle,DrawMemo,DrawLine,Crop,
	WindowLeveling,AnnotatiaonUpdate,MemoUpdate
};

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


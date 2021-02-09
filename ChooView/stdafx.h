
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
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


#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.
#include <afxdisp.h>        // MFC 자동화 클래스입니다.


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원
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
@선과 점의 위치 관계 반환
@linePt1 : 직선을 지나는 점의 좌표
@linePt2 : 직선을 지나는 점의 좌표
@pos		 : 점의 좌표
*/
float isLeft(const PointF& linePt1,
						 const PointF& linePt2,
						 const PointF& pos);
/**
@다각형 내부에 점이 위치한지 확인
@Pos			: 점의 좌표
@Vertices : 다각형의 꼭짓점들의 좌표
*/
int isContainRect(const PointF& pos, const std::vector<PointF>& vertices);
/**
@Client 좌표에서 Image 좌표로 좌표 변환
@point					: 변경할 좌표
@zoomRate				: Zoom 비율
@orgRate				: image의 원본 비율을 유지한 채 확대한 비율
@ViewPointRect	:image의 Pan 좌표
@orgRect				: picRect에서 출력될 Image의 좌표
@flipoVertical	: image의 수직 반전
@flipHorizontal : image의 수평 반전
@rotateState		: image의 회전 각도
@Size						: image의 크기
@picPoint				: picRect의 TopLeft 좌표
@return					: 변경된 좌표
*/
PointF Client2Img(const CPoint& point, const float& zoomRate,
									const float& orgRate, const PointF& viewPoint,
									const RectF& orgRect, const bool& flipVertical,
									const bool& flipHorizontal, const int& rotateState,
									const CPoint& size, const CPoint& picPoint);
/**
@Image좌표에서 Client좌표로 좌표 변환
@point					: 변경할 좌표
@zoomRate				: Zoom 비율
@orgRate				: image의 원본 비율을 유지한 채 확대한 비율
@ViewPointRect  : image의 Pan 좌표
@orgPoint				: picRect에서 출력될 Image의 좌표
@flipoVertical  : image의 수직 반전
@flipHorizontal : image의 수평 반전
@rotateState		: image의 회전 각도
@Size						: image의 크기
@picPoint				: picRect의 TopLeft 좌표
@return					:  변경된 좌표
*/
PointF Img2Client(const PointF& point, const float& zoomRate,
									const float& orgRate, const PointF& viewPoint,
									const PointF& orgPoint, const bool& flipVertical,
									const bool& flipHorizontal, const int& rotateState,
									const CPoint& size);
/**
@선과 점의 직교 좌표
@pt		 : 점의 좌표
@linePt1 : 직선을 지나는 점의 좌표
@linePt2 : 직선을 지나는 점의 좌표
*/
PointF Intersection(const PointF& pt, const PointF& LinePt1,
										const PointF& LinePt2);
PointF CPoint2PointF(const CPoint& point);
/**
@Lint에 점이 위치한지 확인
@point	 : 점의 좌표
@LinePt1 : 선의 좌표
@LinePt2 : 선의 좌표
*/
bool isInLine(const PointF& pt, const PointF& LinePt1, const PointF& LinePt2);



//Mouse Event의 동작 상태 정의
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


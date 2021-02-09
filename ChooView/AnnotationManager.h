#pragma once
#include <list>
#include <map>
#include "AnnotationFactory.h"
#define DISALLOW_COPY_AND_ASSIGN(CAnnotationManager) \
				CAnnotationManager(const CAnnotationManager&); \
				void operator=(const CAnnotationManager&);

class IAnnotation;
class IAnnotationPrototype;
class AbstractFactory;
class CAnnotationManager
{
public:
	CAnnotationManager();
	~CAnnotationManager();
	/**
	@Annotation에 Draw Event 전달
	@actionState		:	Mouse 동작 상태
	@orgRate				: Image와 orgRect의 비율
	@zoomRate				: Zoom 비율
	@orgPoint				: 원본 비율을 유지하기 위한 그리기 영역의 TopLeft 좌표
	@viewPoint			: 현재 Pan 영역의 TopLeft 좌표
	@size						: Image의 크기
	@rotateState		: 회전 각도
	@flipvertical		: 수직 반전
	@flipHorizontal : 수평 반전
	@picPoint				: PicRect의 TopLeft 좌표
	@isSelect				: Picture의 선택여부
	@g							: Graphic 객체
	*/
	void AnnotationDraw(const int& actionState, const float& orgRate, 
											const float& zoomRate, const PointF& orgPoint,
											const PointF& viewPoint, const CPoint& size,
											const int& rotateState, const bool& flipVertical,
											const bool& flipHorizontal, const CPoint& PicPoint,
											const bool& isSelect, Graphics* g);
	/**
	@Annotation에 Update Event 전달
	@downPoint	 : LbuttonDown 좌표
	@firstPoint	 : old mouseMove 좌표
	@secondPoint : new mouseMove 좌표
	*/
	void AnnotationUpdate(const PointF& downPoint, const PointF& firstPoint,
												const PointF& secondPoint);
	/**
	@MouseDownEvent 전달
	@imgPoint		 : Mouse의 Image 좌표
	@clientPoint : Mouse의 Client 좌표
	@ActionState : 현재 mouseEvent시 동작
	@return			 : Annotation 선택 여부 반환
	*/
	bool HitTest(const PointF& imgPoint, const CPoint& clientPoint,
							 int *actionState);
	/**
	@Annotation 변경사항 적용
	*/
	void annotationModify(void);
	/**
	@MemoString Update
	@nChar : 사용자의 키보드 입력
	*/
	void UpdateMemo(const UINT& nChar);
	/**
	@Database Save or Update
	@imageID	 : image Table의 Primary Key
	@isSaveNew : Annotation 저장 여부
	*/
	void SaveDB(const int& imageID, const bool& isSaveNew);
	/**
	@Image에 저장된 Annotation Load
	@imageId : image Table의 Primary Key
	*/
	void LoadAnnotation(const int& imageId);
	/**
	@AnnotationList 초기화
	*/
	void ListReset();
	/**
	@Annotation 제거
	*/
	void DeleteAnnotation();
	/**
	@Annotation의 설정 변경
	*/
	void SetAnnotationProperty();
	/**
	@Annotation Prototype 생성
	@color				: pen의 색상
	@penWidth			: pen의 굵기
	@point				: Annotation의 좌표
	@actionState	: 마우스 동작 상태
	*/
	void CreatePrototype(const Color& color, const float& penWidth,
											 const CPoint& point, const int& actionState);
	/**
	@Prototype의 좌표 변경
	@point	: 변경될 좌표
	*/
	void UpdatePrototype(const CPoint& point);
	/**
	@Prototype을 Annotation으로 변환
	@ZoomRate				 : Image의 Zoom 비율
	@OrgRate				 : Image의 원본 비율을 유지한 채 확대한 비율
	@viewPoint			 : Image의 Pan 좌표
	@orgRect				 : picRect에서 출력될 Image의 좌표
	@FlipoVertical	 : Image의 수직 반전
	@FlipHorizontal	 : Image의 수평 반전
	@RotateState		 : Image의 회전 각도
	@Size						 : Image의 크기
	@picPoint				 : picRect의 TopLeft 좌표
	@return					 : 변환된 Annotation 객체 포인터
	*/
	void ConvertPrototype(const float& zoomRate, const float& orgRate,
												const PointF& viewPoint, const RectF& orgRect,
												const bool& flipVertical, const bool& flipHorizontal,
												const int& rotateState, const CPoint& size,
												const CPoint& picPoint);
private:
	IAnnotationPrototype*			m_pPrototype;					//Annotation의 Prototype
	std::list<IAnnotation*>		m_arrAnnotation;			//Annotation의 배열
	std::vector<int>					m_arrDeletedIndex;		//삭제된 Annotation의 PKList
	IAnnotation*							m_selectAnnotation;		//선택된 Annotation의 포인터
	static AnnotationFactory	m_annotationFactory;	//Annotation Factory 
};

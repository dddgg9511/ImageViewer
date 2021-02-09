#pragma once
struct AnnotationData;

#define DISALLOW_COPY_AND_ASSIGN(IAnnotation) \
	IAnnotation(const IAnnotation&); \
	void operator=(const IAnnotation&);

const float FLOAT_MAX = 3.402823466e+38f;

//Annotation의 Interface
class IAnnotation
{
public:
	/**
	@Annotation의 생성자, Image Load
	@color			: 현재 Pen의 색상
	@penWidth	: 현재 Pen의 굵기
	*/
	IAnnotation(const Color& color, const float& penWidth);
	/**
	@생성자, DBLoad
	@annotationData : DataBase에서 Load한 Annotation Data
	*/
	IAnnotation(const AnnotationData& annotationData);
	virtual ~IAnnotation(){}
	/**
	@Annotation Draw
	@orgRate				: Image와 orgRect의 비율
	@zoomRate				: Zoom 비율
	@orgRect				: 원본 비율을 유지하기 위한 그리기 영역
	@viewPoint			: 현재 Pan 영역의 TopLeft 좌표
	@size						: Image의 크기
	@rotateState		: 회전 각도
	@flipVertical		: 수직 반전
	@flipHorizontal : 수평 반전
	@picPoint				: PicRect의 TopLeft 좌표
	@g							: Graphic 객체
	*/
	virtual void Draw(const float& orgRate, const float& zoomRate,
										const PointF& orgRect, const PointF& viewPoint,
										const CPoint& size, const int& rotateState,
										const bool& flipVertical, const bool& flipHorizontal,
										const CPoint& picPoint, Graphics* g) = 0;
	/**
	@선택된 Annotation Draw (확대 영역,회전영역 출력)
	@orgRate				: Image와 orgRect의 비율
	@zoomRate				: Zoom 비율
	@orgRect				: 원본 비율을 유지하기 위한 그리기 영역
	@viewPoint			: 현재 Pan 영역의 TopLeft 좌표
	@size						: Image의 크기
	@rotateState		: 회전 각도
	@flipVertical		: 수직 반전
	@flipHorizontal : 수평 반전
	@picPoint				: PicRect의 TopLeft 좌표
	@g							: Graphic 객체
	*/
	virtual void SelectDraw(const float& orgRate, const float& zoomRate,
													const PointF& orgRect, const PointF& viewPoint,
													const CPoint& size, const int& rotateState,
													const bool& flipVertical, const bool& flipHorizontal,
													const CPoint& picPoint, Graphics* g) = 0;
	/**
	@Annotation의 좌표 변경
	@downPoint	 : LbuttonDown 좌표
	@firstPoint  : old mouseMove 좌표
	@secondPoint : new mouseMove 좌표
	*/
	virtual void Update(const PointF& downPoint, const PointF& firstPoint,
											const PointF& secondPoint) = 0;
	/**
	@Annotation의 위치 변경
	@downPoint	: old mouseMove 좌표
	@upPoint		: new mouseMove 좌표
	*/
	virtual void Move(const PointF& downPoint, const PointF& upPoint) = 0;
	/**
	@Annotation의 크기 변환
	@downPoint : LbuttonDown 좌표
	@upPoint	 : LbuttonUp 좌표
	*/
	virtual void reSize(const PointF& downPoint, const PointF& upPoint) = 0;
	/**
	@MouseDownEvent 전달
	@imgPoint		 : Mouse의 Image 좌표
	@clientPoint : Mouse의 Client 좌표
	@ActionState : 현재 mouseEvent시 동작
	@return			 : Annotation 선택 여부 반환
	*/
	virtual bool HitTest(const PointF& imgPoint, const CPoint& clientPoint,
											 int* actionState) = 0;
	/**
	@Annotation의 변경사항 적용
	*/
	virtual void Modify(void) = 0;
	/**
	@Annotation 회전
	@downPoint	 : LbuttonDown 좌표
	@movePoint	 : mouseMove 좌표
	*/
	virtual void Rotate(const PointF& downPoint, const PointF& movePoint) = 0;
	/**
	@MemoString Update
	@nChar : 사용자의 키보드 입력
	*/
	virtual void UpdateMemoString(const UINT& nChar) = 0;
	/**
	@Database Save or Update
	@imageID		: image Table의 Primary Key
	@bIsSaveNew	: True시 Save, False시 Update
	*/
	virtual void SaveDB(const int& imageID, const bool& isSaveNew) = 0;
	/**
	@DataBase에서 Annotation 제거
	*/
	void DeleteDB();
	/**
	@Annotation의 Primary Key 반환
	*/
	int GetIndex() const {return m_nIndex;}
	/**
	@Annotation의 Property 변경
	*/
	virtual void SetAnnotationProperty();
protected:
	std::vector<PointF> m_arrPoint;				//Annotation의 꼭짓점 좌표
	std::vector<PointF> m_arrUpdatePoint;	//Annotatin의 변경된 꼭짓점 좌표
	Color								m_Color;					//Annotation의 색상
	float								m_fPenWidth;			//Anotation의 굵기
	int									m_nClickState;		//Annotation의 클릭 상태
	std::vector<RectF>	m_arrSizeBoxes;		//확대를 위한 영역
	PointF							m_RotatePoint;		//회전을 위한 영역
	PointF							m_Center;					//Annotation의 중심점 좌표
	float								m_fTheta;					//Annotation의 회전 각도
	float								m_fUpdateTheta;		//Annotation의 변경된 회전 각도
	int									m_nIndex;					//DataBase PK
};

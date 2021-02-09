#pragma once
#define DISALLOW_COPY_AND_ASSIGN(IAnnotation) \
	IAnnotation(const IAnnotation&); \
	void operator=(const IAnnotation&);
class IAnnotation;

class IAnnotationPrototype
{
public:
	/**
	@Prototype의 생성자
	@color  : Circle의 선 색
	@nWidth : Circle의 선 굵기
	@pt1		: Circle의 좌표
	@pt2		: Circle의 좌표
	*/
	IAnnotationPrototype();
	~IAnnotationPrototype(void);
	/**
	@Prototype을 화면에 그리기
	@g : Graphic 객체
	*/
	virtual void Draw(Graphics* g)=0;
	/**
	@Prototype을 Annotation으로 변환
	@dZoomRate			 : Image의 Zoom 비율
	@dOrgRate				 : image의 원본 비율을 유지한 채 확대한 비율
	@viewPoint			 : image의 Pan 좌표
	@orgRect				 : picRect에서 출력될 Image의 좌표
	@bFlipoVertical	 : image의 수직 반전
	@bFlipHorizontal : image의 수평 반전
	@nRotateState		 : image의 회전 각도
	@Size						 : image의 크기
	@picPoint				 : picRect의 TopLeft 좌표
	@return					 : 변환된 Annotation 객체 포인터
	*/
	virtual IAnnotation* Convert(const float& zoomRate, const float& orgRate,
															 const PointF& viewPoint, const RectF& orgRect, 
															 const bool& flipVertical,
															 const bool& flipHorizontal, const int& rotateState,
															 const CPoint& size, const CPoint& picPoint) = 0;
	/**
	@Prototype의 좌표 변경
	@point : MouseMove 좌표
	*/
	virtual void Update(const CPoint& point)
	{
		m_point2 = point;
	}
	/**
	@Prototype Property 설정
	@color  : Circle의 선 색
	@nWidth : Circle의 선 굵기
	@pt1		: Circle의 좌표
	@pt2		: Circle의 좌표
	*/
	virtual void SetProperty(const Color& color, const float& width,
													 const CPoint& pt1);
protected:
	Color m_color;		//현재 펜의 색상
	float	m_fWidth;		//현재 펜의 굵기
	CPoint m_point1;	//Lbutton Down 좌표
	CPoint m_point2;	//Lbutton Up 좌표
};


#pragma once
#define DISALLOW_COPY_AND_ASSIGN(IAnnotation) \
	IAnnotation(const IAnnotation&); \
	void operator=(const IAnnotation&);
class IAnnotation;

class IAnnotationPrototype
{
public:
	/**
	@Prototype�� ������
	@color  : Circle�� �� ��
	@nWidth : Circle�� �� ����
	@pt1		: Circle�� ��ǥ
	@pt2		: Circle�� ��ǥ
	*/
	IAnnotationPrototype();
	~IAnnotationPrototype(void);
	/**
	@Prototype�� ȭ�鿡 �׸���
	@g : Graphic ��ü
	*/
	virtual void Draw(Graphics* g)=0;
	/**
	@Prototype�� Annotation���� ��ȯ
	@dZoomRate			 : Image�� Zoom ����
	@dOrgRate				 : image�� ���� ������ ������ ä Ȯ���� ����
	@viewPoint			 : image�� Pan ��ǥ
	@orgRect				 : picRect���� ��µ� Image�� ��ǥ
	@bFlipoVertical	 : image�� ���� ����
	@bFlipHorizontal : image�� ���� ����
	@nRotateState		 : image�� ȸ�� ����
	@Size						 : image�� ũ��
	@picPoint				 : picRect�� TopLeft ��ǥ
	@return					 : ��ȯ�� Annotation ��ü ������
	*/
	virtual IAnnotation* Convert(const float& zoomRate, const float& orgRate,
															 const PointF& viewPoint, const RectF& orgRect, 
															 const bool& flipVertical,
															 const bool& flipHorizontal, const int& rotateState,
															 const CPoint& size, const CPoint& picPoint) = 0;
	/**
	@Prototype�� ��ǥ ����
	@point : MouseMove ��ǥ
	*/
	virtual void Update(const CPoint& point)
	{
		m_point2 = point;
	}
	/**
	@Prototype Property ����
	@color  : Circle�� �� ��
	@nWidth : Circle�� �� ����
	@pt1		: Circle�� ��ǥ
	@pt2		: Circle�� ��ǥ
	*/
	virtual void SetProperty(const Color& color, const float& width,
													 const CPoint& pt1);
protected:
	Color m_color;		//���� ���� ����
	float	m_fWidth;		//���� ���� ����
	CPoint m_point1;	//Lbutton Down ��ǥ
	CPoint m_point2;	//Lbutton Up ��ǥ
};


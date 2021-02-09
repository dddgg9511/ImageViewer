#pragma once
struct AnnotationData;

#define DISALLOW_COPY_AND_ASSIGN(IAnnotation) \
	IAnnotation(const IAnnotation&); \
	void operator=(const IAnnotation&);

const float FLOAT_MAX = 3.402823466e+38f;

//Annotation�� Interface
class IAnnotation
{
public:
	/**
	@Annotation�� ������, Image Load
	@color			: ���� Pen�� ����
	@penWidth	: ���� Pen�� ����
	*/
	IAnnotation(const Color& color, const float& penWidth);
	/**
	@������, DBLoad
	@annotationData : DataBase���� Load�� Annotation Data
	*/
	IAnnotation(const AnnotationData& annotationData);
	virtual ~IAnnotation(){}
	/**
	@Annotation Draw
	@orgRate				: Image�� orgRect�� ����
	@zoomRate				: Zoom ����
	@orgRect				: ���� ������ �����ϱ� ���� �׸��� ����
	@viewPoint			: ���� Pan ������ TopLeft ��ǥ
	@size						: Image�� ũ��
	@rotateState		: ȸ�� ����
	@flipVertical		: ���� ����
	@flipHorizontal : ���� ����
	@picPoint				: PicRect�� TopLeft ��ǥ
	@g							: Graphic ��ü
	*/
	virtual void Draw(const float& orgRate, const float& zoomRate,
										const PointF& orgRect, const PointF& viewPoint,
										const CPoint& size, const int& rotateState,
										const bool& flipVertical, const bool& flipHorizontal,
										const CPoint& picPoint, Graphics* g) = 0;
	/**
	@���õ� Annotation Draw (Ȯ�� ����,ȸ������ ���)
	@orgRate				: Image�� orgRect�� ����
	@zoomRate				: Zoom ����
	@orgRect				: ���� ������ �����ϱ� ���� �׸��� ����
	@viewPoint			: ���� Pan ������ TopLeft ��ǥ
	@size						: Image�� ũ��
	@rotateState		: ȸ�� ����
	@flipVertical		: ���� ����
	@flipHorizontal : ���� ����
	@picPoint				: PicRect�� TopLeft ��ǥ
	@g							: Graphic ��ü
	*/
	virtual void SelectDraw(const float& orgRate, const float& zoomRate,
													const PointF& orgRect, const PointF& viewPoint,
													const CPoint& size, const int& rotateState,
													const bool& flipVertical, const bool& flipHorizontal,
													const CPoint& picPoint, Graphics* g) = 0;
	/**
	@Annotation�� ��ǥ ����
	@downPoint	 : LbuttonDown ��ǥ
	@firstPoint  : old mouseMove ��ǥ
	@secondPoint : new mouseMove ��ǥ
	*/
	virtual void Update(const PointF& downPoint, const PointF& firstPoint,
											const PointF& secondPoint) = 0;
	/**
	@Annotation�� ��ġ ����
	@downPoint	: old mouseMove ��ǥ
	@upPoint		: new mouseMove ��ǥ
	*/
	virtual void Move(const PointF& downPoint, const PointF& upPoint) = 0;
	/**
	@Annotation�� ũ�� ��ȯ
	@downPoint : LbuttonDown ��ǥ
	@upPoint	 : LbuttonUp ��ǥ
	*/
	virtual void reSize(const PointF& downPoint, const PointF& upPoint) = 0;
	/**
	@MouseDownEvent ����
	@imgPoint		 : Mouse�� Image ��ǥ
	@clientPoint : Mouse�� Client ��ǥ
	@ActionState : ���� mouseEvent�� ����
	@return			 : Annotation ���� ���� ��ȯ
	*/
	virtual bool HitTest(const PointF& imgPoint, const CPoint& clientPoint,
											 int* actionState) = 0;
	/**
	@Annotation�� ������� ����
	*/
	virtual void Modify(void) = 0;
	/**
	@Annotation ȸ��
	@downPoint	 : LbuttonDown ��ǥ
	@movePoint	 : mouseMove ��ǥ
	*/
	virtual void Rotate(const PointF& downPoint, const PointF& movePoint) = 0;
	/**
	@MemoString Update
	@nChar : ������� Ű���� �Է�
	*/
	virtual void UpdateMemoString(const UINT& nChar) = 0;
	/**
	@Database Save or Update
	@imageID		: image Table�� Primary Key
	@bIsSaveNew	: True�� Save, False�� Update
	*/
	virtual void SaveDB(const int& imageID, const bool& isSaveNew) = 0;
	/**
	@DataBase���� Annotation ����
	*/
	void DeleteDB();
	/**
	@Annotation�� Primary Key ��ȯ
	*/
	int GetIndex() const {return m_nIndex;}
	/**
	@Annotation�� Property ����
	*/
	virtual void SetAnnotationProperty();
protected:
	std::vector<PointF> m_arrPoint;				//Annotation�� ������ ��ǥ
	std::vector<PointF> m_arrUpdatePoint;	//Annotatin�� ����� ������ ��ǥ
	Color								m_Color;					//Annotation�� ����
	float								m_fPenWidth;			//Anotation�� ����
	int									m_nClickState;		//Annotation�� Ŭ�� ����
	std::vector<RectF>	m_arrSizeBoxes;		//Ȯ�븦 ���� ����
	PointF							m_RotatePoint;		//ȸ���� ���� ����
	PointF							m_Center;					//Annotation�� �߽��� ��ǥ
	float								m_fTheta;					//Annotation�� ȸ�� ����
	float								m_fUpdateTheta;		//Annotation�� ����� ȸ�� ����
	int									m_nIndex;					//DataBase PK
};

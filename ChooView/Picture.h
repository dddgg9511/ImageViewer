#pragma once
struct ImageData;

#define DISALLOW_COPY_AND_ASSIGN(CPicture) \
	CPicture(const CPicture&); \
	void operator=(const CPicture&);
class CAnnotationManager;

class CPicture
{
public:
	/**
	@CPicture ������, Image Load
	@fileName : �о�� Image�� ��� + ���� �̸�
	*/
	CPicture(const CString& fileName);
	/**
	@CPicture ������, DBLoad
	@imageData : Database���� Load�� Picure�� Data
	*/
	CPicture(const ImageData& imageData);

	~CPicture();
	/**
	@Picture ���� �ʱ�ȭ, ImageLoad  
	@return : �ʱ�ȭ ���� ����
	*/
	bool InitPicture();
	/**
	@Picture ���� �ʱ�ȭ, DBLoad
	@return : �ʱ�ȭ ���� ����
	*/
	bool InitPictureLoadDB();
	/**
	@Image Draw
	@actionState : MouseEvent ���� ����
	@PicRect			: ���� ���̾ƿ����� ��ü�� �Ҵ�� �׸��� ����
	@isSelect		: Picture��ü�� ���� ����
	@mem					: Buffering�� ���� memoryDC
	*/
	void PicDraw(const int& actionState,const Rect& PicRect,const bool& isSelect,
							 CDC* mem);
	/**
	@ZoomRate ����
	@zoomPoint	 : Zoom�� �߽� ��ǥ(LbuttonDown)
	@firstPoint	 : ���� ���콺�� ��ǥ
	@secondPoint : ����� ���콺�� ��ǥ
	*/
	void PicZoom(const CPoint& zoomPoint,const CPoint& firstPoint,
							 const CPoint& secondPoint);
	/**
	@Picture��ü�� ��ġ ����
	@downPoint  : ���� ���콺�� ��ǥ
	@upPoint		: ����� ���콺�� ��ǥ
	*/
	void PicPan(const CPoint& downPoint,const CPoint& upPoint);
	/**
	@Annotation�� ��ǥ ����
	@downPoint   : LbuttonDown ��ġ
	@firstPoint  : ���� ���콺�� ��ǥ
	@secondPoint : ����� ���콺�� ��ǥ
	*/
	void AnnotationUpdate(const CPoint& downPoint,const CPoint& firstPoint,
											  const CPoint& secondPoint);

	/**
	@Image Crop ��ǥ ����
	@downPoint : ���� ���콺�� ��ǥ
	@upPoint   : ����� ���콺�� ��ǥ
	*/
	void PicCrop(const CPoint& downPoint,const CPoint& upPoint);
	/**
	@Image�� ������ ���
	@dc :Text�� �׸��� ���� DC
	*/
	void OverlayText(CDC* dc);
	/**
	@WindowLeveling ���� ���� 
	@downPoint : ���� ���콺�� ��ǥ
	@upPoint   : ����� ���콺�� ��ǥ
	*/
	void WindowLeveling(const CPoint& downPoint,const CPoint& upPoint);
	/**
	@point			: ���콺�� ��ǥ
	@actionSate : MouseEvent ���� ����
	@return			: ��ü�� Ŭ������
	*/
	bool HitTest(const CPoint& point,int* actionState);
	/**
	@Picture��ü �ʱ�ȭ
	*/
	void clear();
	/**
	@Image �������� 90�� ȸ��
	*/
	void RotateRight(void);
	/**
	@Image �������� 90�� ȸ��
	*/
	void RotateLeft(void);
	/**
	@Image ���� ����
	*/
	void FlipVertical(void);
	/**
	@Image ���� ����
	*/
	void FlipHorizontal(void);
	/**
	@Annotation�� ������� ����
	*/
	void annotationModify(void);
	/**
	@actionState	: Mouse ���� ����
	@clientRect		: Ŭ���̾�Ʈ ��ü ����
	@dc						: ClientDC
	@return				: PicRect�� ��ȯ 
	*/
	Rect Draw(const int& actionState,const Rect& clientRect,CDC* dc);
	/**
	@WindowLevel�� ���� byteArray ���� �Լ�
	@image : ����� Image ��ü
	*/
	void windowLevelAction(Image * image);
	/**
	@OrgRect setting �Լ�
	*/
	void setOrgRect();
	/**
	@MemoSting ����
	@nChar : ������� Ű���� �Է�
	*/
	void UpdateMemo(const UINT& nChar);
	/**
	@DataBase ���� or Update  
	@categoryID : Category�� Primary Key
	@bIsSaveNew	: True�� Save, False�� Update
	*/
	void SaveDB(const int& categoryID,const bool& isSaveNew);
	/**
	@Picture�� Database PK ��ȯ
	*/
	int getIndex() const {return m_nIndex;}
	/**
	@Annotation ����
	*/
	void DeleteAnnotation();
	/**
	@Annotation ���� ����
	*/
	void SetAnnotationProperty();
	/**
	@Annotation Prototype ����
	@color				: Line ����
	@fPenwidth		: Line ����
	@point				: Lbutton Down ��ǥ
	@nAcitonState : MouseEvent ���� ����
	*/
	void CreatePrototype(const Color& color,const float& fPenWidth,
											 const CPoint& point,const int& nActionState);
	/**
	@Prototype ��ǥ ����
	@point : ����� ��ǥ
	*/
	void UpdatePrototype(const CPoint& point);
	/**
	@Prototype�� Annotation���� ��ȯ
	*/
	void ConvertPrototype();
private:
	Image*							m_image;							//���� �̹���
	float								m_fZoomRate;					//���� Zoom ����
	PointF							m_viewPoint;					//Pan�� ��ġ�� ����
	int									m_nRotateState;				//���� ȸ�� ����
	bool								m_bFlipVertical;			//���� ���� ����
	bool								m_bFlipHorizontal;		//���� ���� ����
	CAnnotationManager* m_annotationManager;	//Annotation�� ����Ʈ ����
	Rect								m_picRect;						//���� ���̾ƿ��� �Ҵ�� �׸��� ����
	RectF								m_orgRect;						//���� ������ ������ �׸��� ����
	RectF								m_rotateRect;					//������ �̹����� ũ��
	int									m_nWindowCenter;			//Window Center
	int									m_nWindowWidth;				//Window Width
	PointF							m_cropTl;							//Crop ������ TopLeft
	PointF							m_cropBr;							//Crop ������ Bottom Right
	CPoint							m_size;								//image�� ũ��
	int									m_nIndex;							//Database PrimaryKey
	const CString				m_strFileName;				//FileName
	
};


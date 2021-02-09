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
	@CPicture 생성자, Image Load
	@fileName : 읽어올 Image의 경로 + 파일 이름
	*/
	CPicture(const CString& fileName);
	/**
	@CPicture 생성자, DBLoad
	@imageData : Database에서 Load한 Picure의 Data
	*/
	CPicture(const ImageData& imageData);

	~CPicture();
	/**
	@Picture 변수 초기화, ImageLoad  
	@return : 초기화 성공 여부
	*/
	bool InitPicture();
	/**
	@Picture 변수 초기화, DBLoad
	@return : 초기화 성공 여부
	*/
	bool InitPictureLoadDB();
	/**
	@Image Draw
	@actionState : MouseEvent 동작 상태
	@PicRect			: 현제 레이아웃에서 객체에 할당된 그리기 영역
	@isSelect		: Picture객체의 선택 여부
	@mem					: Buffering을 위한 memoryDC
	*/
	void PicDraw(const int& actionState,const Rect& PicRect,const bool& isSelect,
							 CDC* mem);
	/**
	@ZoomRate 변경
	@zoomPoint	 : Zoom의 중심 좌표(LbuttonDown)
	@firstPoint	 : 이전 마우스의 좌표
	@secondPoint : 변경된 마우스의 좌표
	*/
	void PicZoom(const CPoint& zoomPoint,const CPoint& firstPoint,
							 const CPoint& secondPoint);
	/**
	@Picture객체의 위치 변경
	@downPoint  : 이전 마우스의 좌표
	@upPoint		: 변경된 마우스의 좌표
	*/
	void PicPan(const CPoint& downPoint,const CPoint& upPoint);
	/**
	@Annotation의 좌표 수정
	@downPoint   : LbuttonDown 위치
	@firstPoint  : 이전 마우스의 좌표
	@secondPoint : 변경된 마우스의 좌표
	*/
	void AnnotationUpdate(const CPoint& downPoint,const CPoint& firstPoint,
											  const CPoint& secondPoint);

	/**
	@Image Crop 좌표 변경
	@downPoint : 이전 마우스의 좌표
	@upPoint   : 변경된 마우스의 좌표
	*/
	void PicCrop(const CPoint& downPoint,const CPoint& upPoint);
	/**
	@Image의 데이터 출력
	@dc :Text를 그리기 위한 DC
	*/
	void OverlayText(CDC* dc);
	/**
	@WindowLeveling 변수 변경 
	@downPoint : 이전 마우스의 좌표
	@upPoint   : 변경된 마우스의 좌표
	*/
	void WindowLeveling(const CPoint& downPoint,const CPoint& upPoint);
	/**
	@point			: 마우스의 좌표
	@actionSate : MouseEvent 동작 상태
	@return			: 객체의 클릭상태
	*/
	bool HitTest(const CPoint& point,int* actionState);
	/**
	@Picture객체 초기화
	*/
	void clear();
	/**
	@Image 우측으로 90도 회전
	*/
	void RotateRight(void);
	/**
	@Image 좌측으로 90도 회전
	*/
	void RotateLeft(void);
	/**
	@Image 수직 반전
	*/
	void FlipVertical(void);
	/**
	@Image 수평 반전
	*/
	void FlipHorizontal(void);
	/**
	@Annotation의 변경사항 적용
	*/
	void annotationModify(void);
	/**
	@actionState	: Mouse 동작 상태
	@clientRect		: 클라이언트 전체 영역
	@dc						: ClientDC
	@return				: PicRect를 반환 
	*/
	Rect Draw(const int& actionState,const Rect& clientRect,CDC* dc);
	/**
	@WindowLevel을 위한 byteArray 조작 함수
	@image : 변경될 Image 객체
	*/
	void windowLevelAction(Image * image);
	/**
	@OrgRect setting 함수
	*/
	void setOrgRect();
	/**
	@MemoSting 수정
	@nChar : 사용자의 키보드 입력
	*/
	void UpdateMemo(const UINT& nChar);
	/**
	@DataBase 저장 or Update  
	@categoryID : Category의 Primary Key
	@bIsSaveNew	: True시 Save, False시 Update
	*/
	void SaveDB(const int& categoryID,const bool& isSaveNew);
	/**
	@Picture의 Database PK 반환
	*/
	int getIndex() const {return m_nIndex;}
	/**
	@Annotation 제거
	*/
	void DeleteAnnotation();
	/**
	@Annotation 설정 변경
	*/
	void SetAnnotationProperty();
	/**
	@Annotation Prototype 생성
	@color				: Line 색상
	@fPenwidth		: Line 굵기
	@point				: Lbutton Down 좌표
	@nAcitonState : MouseEvent 동작 상태
	*/
	void CreatePrototype(const Color& color,const float& fPenWidth,
											 const CPoint& point,const int& nActionState);
	/**
	@Prototype 좌표 변경
	@point : 변경될 좌표
	*/
	void UpdatePrototype(const CPoint& point);
	/**
	@Prototype을 Annotation으로 변환
	*/
	void ConvertPrototype();
private:
	Image*							m_image;							//원본 이미지
	float								m_fZoomRate;					//현재 Zoom 비율
	PointF							m_viewPoint;					//Pan의 위치를 저장
	int									m_nRotateState;				//현재 회전 상태
	bool								m_bFlipVertical;			//수직 반전 여부
	bool								m_bFlipHorizontal;		//수평 반전 여부
	CAnnotationManager* m_annotationManager;	//Annotation의 리스트 관리
	Rect								m_picRect;						//현재 레이아웃에 할당된 그리기 영역
	RectF								m_orgRect;						//원본 비율을 유지한 그리기 영역
	RectF								m_rotateRect;					//변형된 이미지의 크기
	int									m_nWindowCenter;			//Window Center
	int									m_nWindowWidth;				//Window Width
	PointF							m_cropTl;							//Crop 영역의 TopLeft
	PointF							m_cropBr;							//Crop 영역의 Bottom Right
	CPoint							m_size;								//image의 크기
	int									m_nIndex;							//Database PrimaryKey
	const CString				m_strFileName;				//FileName
	
};


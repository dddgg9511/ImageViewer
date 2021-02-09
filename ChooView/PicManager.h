#pragma once
//#include "Picture.h"
class CPicture;

class CPicManager
{
public:
	CPicManager();
	~CPicManager();
	/**
	@Picture의 갯수 반환
	@@eturn : 현재 Picture의 갯수
	*/
	int GetPictureCount() const {return m_arrPictures.size();}
	/**
	@Row 설정값 반환
	@return : 현재 Row 값
	*/
	int GetRow() const {return m_nRow;}
	/**
	@Col 설정값 반환
	@return : 현재 Col 값
	*/
	int GetCol() const {return m_nCol;}
	/**
	@Layout 설정
	@row	: Row 설정 값
	@col	: Col	설정 값
	*/
	void SetLayOut(const int& row, const int& col);
	/**
	@Page 이동
	*/
	void PageUp();
	/**
	@Page 이동
	*/
	void PageDown();
	/**
	@Picture 객체 추가
	@fileName : load할 image의 파일 이름
	*/
	void AddImage(const CString& fileName);
	/**
	@Open된 모든 Image Close
	*/
	void CloseAll();
	/**
	@현재 레이아웃 설정을 반영한 Image Draw
	@ClientRect   : 클라이언트 전체의 영역
	@nActionState : 현재 mouseEvent시 동작
	@DC						: 무효화 영역 DC
	@mem					: image를 draw하기 위한 memoryDC
	*/
	void Draw(const Rect& ClientRect,const int& nActionState,CDC *mem);
	/**
	@Annotation의 변경사항을 전달
	@downPoint	 : LbuttonDown 좌표
	@firstPoint  : old mouseMove 좌표
	@secondPoint : new mouseMove 좌표
	*/
	void AnnotationUpdate(const CPoint& downPoint,const CPoint& firstPoint,
											  const CPoint& secondPoint);
	/**
	//Picture객체에 Clear Event 전달
	*/
	void PicClear();
	/**
	@Picture객체에 Zoom Event 전달
	@zoomPoint	 : LbuttonDown 좌표
	@firstPoint  : old mouseMove 좌표
	@secondPoint : new mouseMove 좌표
	*/
	void Zoom(const CPoint& zoomPoint,const CPoint& firstPoint,
						const CPoint& secondPoint);
	/**
	@Picture객체에 Pan Event 전달
	@firstPoint	 : old mouseMove 좌표
	@secondPoint : new mouseMove 좌표
	*/
	void Pan(const CPoint& firstPoint,const CPoint& secondPoint);
	/**
	@Picture객체에 Crop Event 전달
	@downPoint  : LbuttonDwon 좌표
	@upPoint	: LbuttonUp 좌표
	*/
	void Crop(const CPoint& downPoint,const CPoint& upPoint);
	/**
	@DownEvent 전달
	@Point			  : mouseDown 좌표
	@nActionState : 현재 mouseEvent시 동작상태
	@return			  : DownEvent 결과 반환
	*/
	void HitTest(const CPoint& point,int* nActionState);
	/**
	@더블클릭 동작으로 1*1 레이아웃 설정 전달
	@nActionState : MouseEvent 동작 상태
	@ClientRect   : 클라이언트 전체 영역
	@mem					: image를 draw하기 위한 memoryDC
	@DC						: 무효화 영역 DC
	*/
	void DbClickDraw(const int& nActionState,const Rect& ClientRect,CDC *mem);
	/**
	@Picture 객체에 RotateRight Event 전달
	*/
	void RotateRight();
	/**
	//Picture 객체에 RotateLeft Event 전달
	*/
	void RotateLeft();
	/**
	@Picture 객체에 FlipVertical Event 전달
	*/
	void FlipVertical();
	/**
	@Picture 객체에 FlipHorizontal Event 전달
	*/
	void FlipHorizontal();
	/**
	@Annotatin의 변경사항 적용
	*/
	void annotaionModify();
	/**
	@Picture객체에 WindowLeveling Event 전달
	@firstPoint	 : old mouseMove 좌표
	@secondPoint : new mouseMove 좌표
	*/
	void WindowLeveling(const CPoint& firstPoint,const CPoint& secondPoint);
	/**
	@변경사항이 생긴 Picture 객체 Draw
	@dc					: Client DC
	@updateRect : 변경사항이 생긴 Picture객체의 영역
	*/
	void UpdatePictureDraw(const int& nActionState,
												 const Rect& updateRect,CDC* dc);
	/**
	@MemoString 수정
	@uChar : 사용자 키보드 입력
	*/
	void UpdateMemo(const UINT& uChar);
	/**
	@DataBase에 현재 Category Save or Update
	@nCategoryID : Category의 Primary Key
	@bIsSaveNew  : Category의 저장 유무
	*/
	void saveDataBase(const int& nCategoryID,const bool& bIsSaveNew);
	/**
	@Load한 Category에 Image Load
	@nCategoryID : Category의 Primary Key
	*/
	void loadImageInCategory(const int& nCategoryID);
	/**
	@DataBase PrimaryKey로 ImageLoad
	@nImageID : Load할 Image의 DataBase PrimaryKey
	*/
	void LoadImageDatabase(const int& nImageID);
	/**
	@더블클릭상태 반환
	@return : doubleClick 상태
	*/
	bool GetDbClickState() const {return m_bDbClick;}

	/**
	@MousefloatClick 상태
	*/
	void SetDbClickState();
	/**
	@최대 페이지 설정
	*/
	void SetMaxPage();
	/**
	@선택된 Image Close
	*/
	void CloseImage();
	/**
	@Annotation 제거
	*/
	void DeleteAnnotation();
	/**
	@Annotation 설정 변경
	*/
	void SetAnnotationProperty();
	//Category 변경 상태 반환
	int GetChangeState() const {return m_bIsChanged;}
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
	/**
	@Picture 객체 추가
	@fileList : 추가할 Image의 파일이름의 배열
	*/
	void AddImageList( const std::vector<CString>& fileList );
private:
	std::vector<CPicture*>	m_arrPictures;				//Picture 객체의 배열
	std::vector<int>				m_arrDeletedPictures;	//삭제된 Pucture의 배열
	unsigned int						m_nSelectIndex;				//선택된 Picture의 index
	unsigned int						m_nRow;								//현재 Row
	unsigned int						m_nCol;								//현재 Column
	unsigned int						m_nPage;							//현재 Page
	unsigned int						m_nMaxPage;						//최대 페이지 
	bool										m_bDbClick;						//float Click 상태
	bool										m_bIsChanged;					//Category 변경 상태
};


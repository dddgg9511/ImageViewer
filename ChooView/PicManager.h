#pragma once
//#include "Picture.h"
class CPicture;

class CPicManager
{
public:
	CPicManager();
	~CPicManager();
	/**
	@Picture�� ���� ��ȯ
	@@eturn : ���� Picture�� ����
	*/
	int GetPictureCount() const {return m_arrPictures.size();}
	/**
	@Row ������ ��ȯ
	@return : ���� Row ��
	*/
	int GetRow() const {return m_nRow;}
	/**
	@Col ������ ��ȯ
	@return : ���� Col ��
	*/
	int GetCol() const {return m_nCol;}
	/**
	@Layout ����
	@row	: Row ���� ��
	@col	: Col	���� ��
	*/
	void SetLayOut(const int& row, const int& col);
	/**
	@Page �̵�
	*/
	void PageUp();
	/**
	@Page �̵�
	*/
	void PageDown();
	/**
	@Picture ��ü �߰�
	@fileName : load�� image�� ���� �̸�
	*/
	void AddImage(const CString& fileName);
	/**
	@Open�� ��� Image Close
	*/
	void CloseAll();
	/**
	@���� ���̾ƿ� ������ �ݿ��� Image Draw
	@ClientRect   : Ŭ���̾�Ʈ ��ü�� ����
	@nActionState : ���� mouseEvent�� ����
	@DC						: ��ȿȭ ���� DC
	@mem					: image�� draw�ϱ� ���� memoryDC
	*/
	void Draw(const Rect& ClientRect,const int& nActionState,CDC *mem);
	/**
	@Annotation�� ��������� ����
	@downPoint	 : LbuttonDown ��ǥ
	@firstPoint  : old mouseMove ��ǥ
	@secondPoint : new mouseMove ��ǥ
	*/
	void AnnotationUpdate(const CPoint& downPoint,const CPoint& firstPoint,
											  const CPoint& secondPoint);
	/**
	//Picture��ü�� Clear Event ����
	*/
	void PicClear();
	/**
	@Picture��ü�� Zoom Event ����
	@zoomPoint	 : LbuttonDown ��ǥ
	@firstPoint  : old mouseMove ��ǥ
	@secondPoint : new mouseMove ��ǥ
	*/
	void Zoom(const CPoint& zoomPoint,const CPoint& firstPoint,
						const CPoint& secondPoint);
	/**
	@Picture��ü�� Pan Event ����
	@firstPoint	 : old mouseMove ��ǥ
	@secondPoint : new mouseMove ��ǥ
	*/
	void Pan(const CPoint& firstPoint,const CPoint& secondPoint);
	/**
	@Picture��ü�� Crop Event ����
	@downPoint  : LbuttonDwon ��ǥ
	@upPoint	: LbuttonUp ��ǥ
	*/
	void Crop(const CPoint& downPoint,const CPoint& upPoint);
	/**
	@DownEvent ����
	@Point			  : mouseDown ��ǥ
	@nActionState : ���� mouseEvent�� ���ۻ���
	@return			  : DownEvent ��� ��ȯ
	*/
	void HitTest(const CPoint& point,int* nActionState);
	/**
	@����Ŭ�� �������� 1*1 ���̾ƿ� ���� ����
	@nActionState : MouseEvent ���� ����
	@ClientRect   : Ŭ���̾�Ʈ ��ü ����
	@mem					: image�� draw�ϱ� ���� memoryDC
	@DC						: ��ȿȭ ���� DC
	*/
	void DbClickDraw(const int& nActionState,const Rect& ClientRect,CDC *mem);
	/**
	@Picture ��ü�� RotateRight Event ����
	*/
	void RotateRight();
	/**
	//Picture ��ü�� RotateLeft Event ����
	*/
	void RotateLeft();
	/**
	@Picture ��ü�� FlipVertical Event ����
	*/
	void FlipVertical();
	/**
	@Picture ��ü�� FlipHorizontal Event ����
	*/
	void FlipHorizontal();
	/**
	@Annotatin�� ������� ����
	*/
	void annotaionModify();
	/**
	@Picture��ü�� WindowLeveling Event ����
	@firstPoint	 : old mouseMove ��ǥ
	@secondPoint : new mouseMove ��ǥ
	*/
	void WindowLeveling(const CPoint& firstPoint,const CPoint& secondPoint);
	/**
	@��������� ���� Picture ��ü Draw
	@dc					: Client DC
	@updateRect : ��������� ���� Picture��ü�� ����
	*/
	void UpdatePictureDraw(const int& nActionState,
												 const Rect& updateRect,CDC* dc);
	/**
	@MemoString ����
	@uChar : ����� Ű���� �Է�
	*/
	void UpdateMemo(const UINT& uChar);
	/**
	@DataBase�� ���� Category Save or Update
	@nCategoryID : Category�� Primary Key
	@bIsSaveNew  : Category�� ���� ����
	*/
	void saveDataBase(const int& nCategoryID,const bool& bIsSaveNew);
	/**
	@Load�� Category�� Image Load
	@nCategoryID : Category�� Primary Key
	*/
	void loadImageInCategory(const int& nCategoryID);
	/**
	@DataBase PrimaryKey�� ImageLoad
	@nImageID : Load�� Image�� DataBase PrimaryKey
	*/
	void LoadImageDatabase(const int& nImageID);
	/**
	@����Ŭ������ ��ȯ
	@return : doubleClick ����
	*/
	bool GetDbClickState() const {return m_bDbClick;}

	/**
	@MousefloatClick ����
	*/
	void SetDbClickState();
	/**
	@�ִ� ������ ����
	*/
	void SetMaxPage();
	/**
	@���õ� Image Close
	*/
	void CloseImage();
	/**
	@Annotation ����
	*/
	void DeleteAnnotation();
	/**
	@Annotation ���� ����
	*/
	void SetAnnotationProperty();
	//Category ���� ���� ��ȯ
	int GetChangeState() const {return m_bIsChanged;}
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
	/**
	@Picture ��ü �߰�
	@fileList : �߰��� Image�� �����̸��� �迭
	*/
	void AddImageList( const std::vector<CString>& fileList );
private:
	std::vector<CPicture*>	m_arrPictures;				//Picture ��ü�� �迭
	std::vector<int>				m_arrDeletedPictures;	//������ Pucture�� �迭
	unsigned int						m_nSelectIndex;				//���õ� Picture�� index
	unsigned int						m_nRow;								//���� Row
	unsigned int						m_nCol;								//���� Column
	unsigned int						m_nPage;							//���� Page
	unsigned int						m_nMaxPage;						//�ִ� ������ 
	bool										m_bDbClick;						//float Click ����
	bool										m_bIsChanged;					//Category ���� ����
};


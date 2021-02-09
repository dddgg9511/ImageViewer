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
	@Annotation�� Draw Event ����
	@actionState		:	Mouse ���� ����
	@orgRate				: Image�� orgRect�� ����
	@zoomRate				: Zoom ����
	@orgPoint				: ���� ������ �����ϱ� ���� �׸��� ������ TopLeft ��ǥ
	@viewPoint			: ���� Pan ������ TopLeft ��ǥ
	@size						: Image�� ũ��
	@rotateState		: ȸ�� ����
	@flipvertical		: ���� ����
	@flipHorizontal : ���� ����
	@picPoint				: PicRect�� TopLeft ��ǥ
	@isSelect				: Picture�� ���ÿ���
	@g							: Graphic ��ü
	*/
	void AnnotationDraw(const int& actionState, const float& orgRate, 
											const float& zoomRate, const PointF& orgPoint,
											const PointF& viewPoint, const CPoint& size,
											const int& rotateState, const bool& flipVertical,
											const bool& flipHorizontal, const CPoint& PicPoint,
											const bool& isSelect, Graphics* g);
	/**
	@Annotation�� Update Event ����
	@downPoint	 : LbuttonDown ��ǥ
	@firstPoint	 : old mouseMove ��ǥ
	@secondPoint : new mouseMove ��ǥ
	*/
	void AnnotationUpdate(const PointF& downPoint, const PointF& firstPoint,
												const PointF& secondPoint);
	/**
	@MouseDownEvent ����
	@imgPoint		 : Mouse�� Image ��ǥ
	@clientPoint : Mouse�� Client ��ǥ
	@ActionState : ���� mouseEvent�� ����
	@return			 : Annotation ���� ���� ��ȯ
	*/
	bool HitTest(const PointF& imgPoint, const CPoint& clientPoint,
							 int *actionState);
	/**
	@Annotation ������� ����
	*/
	void annotationModify(void);
	/**
	@MemoString Update
	@nChar : ������� Ű���� �Է�
	*/
	void UpdateMemo(const UINT& nChar);
	/**
	@Database Save or Update
	@imageID	 : image Table�� Primary Key
	@isSaveNew : Annotation ���� ����
	*/
	void SaveDB(const int& imageID, const bool& isSaveNew);
	/**
	@Image�� ����� Annotation Load
	@imageId : image Table�� Primary Key
	*/
	void LoadAnnotation(const int& imageId);
	/**
	@AnnotationList �ʱ�ȭ
	*/
	void ListReset();
	/**
	@Annotation ����
	*/
	void DeleteAnnotation();
	/**
	@Annotation�� ���� ����
	*/
	void SetAnnotationProperty();
	/**
	@Annotation Prototype ����
	@color				: pen�� ����
	@penWidth			: pen�� ����
	@point				: Annotation�� ��ǥ
	@actionState	: ���콺 ���� ����
	*/
	void CreatePrototype(const Color& color, const float& penWidth,
											 const CPoint& point, const int& actionState);
	/**
	@Prototype�� ��ǥ ����
	@point	: ����� ��ǥ
	*/
	void UpdatePrototype(const CPoint& point);
	/**
	@Prototype�� Annotation���� ��ȯ
	@ZoomRate				 : Image�� Zoom ����
	@OrgRate				 : Image�� ���� ������ ������ ä Ȯ���� ����
	@viewPoint			 : Image�� Pan ��ǥ
	@orgRect				 : picRect���� ��µ� Image�� ��ǥ
	@FlipoVertical	 : Image�� ���� ����
	@FlipHorizontal	 : Image�� ���� ����
	@RotateState		 : Image�� ȸ�� ����
	@Size						 : Image�� ũ��
	@picPoint				 : picRect�� TopLeft ��ǥ
	@return					 : ��ȯ�� Annotation ��ü ������
	*/
	void ConvertPrototype(const float& zoomRate, const float& orgRate,
												const PointF& viewPoint, const RectF& orgRect,
												const bool& flipVertical, const bool& flipHorizontal,
												const int& rotateState, const CPoint& size,
												const CPoint& picPoint);
private:
	IAnnotationPrototype*			m_pPrototype;					//Annotation�� Prototype
	std::list<IAnnotation*>		m_arrAnnotation;			//Annotation�� �迭
	std::vector<int>					m_arrDeletedIndex;		//������ Annotation�� PKList
	IAnnotation*							m_selectAnnotation;		//���õ� Annotation�� ������
	static AnnotationFactory	m_annotationFactory;	//Annotation Factory 
};

#pragma once
class IAnnotation;
struct AnnotationData;
class IAnnotationPrototype;
class CLinePrototype;
class CRrectanglePrototype;
class CMemoPrototype;
class CCirclePrototype;

class AnnotationFactory
{
public:
	AnnotationFactory(void);
	~AnnotationFactory(void);
	/**
	@Annotation �����Լ��� ������
	@data		: DataBase���� Load�� Annotation Data
	@return : ������ Annotation ��ü�� ������
	*/
	typedef IAnnotation* (AnnotationFactory::*AnnotationCreator)
		(const AnnotationData& data);
	/**
	@Prototype �����Լ��� ������
	@AnnotationType : ������ Prototype�� Type
	@color					: ������ Prototype�� ����
	@fPenWidht			: ������ Prototype�� �� ����
	@point					: ������ Prototype�� ��ǥ
	@return					: ������ Prototype ��ü�� ������ ����
	*/
	typedef IAnnotationPrototype* (AnnotationFactory::*PrototypeCreator)
		(const Color& color,
		 const float& fPenWidth,
		 const CPoint& point);
	/**
	@Annotation ����
	@data		: DataBase���� Load�� Annotation Data
	@return : ������ Annotation ������ ��ȯ
	*/
	IAnnotation* CreateAnnotation(const AnnotationData& data);

	/**
	@Annotation Prototype ����
	@AnnotationType : ������ Prototype�� Type
	@color					: ������ Prototype�� ����
	@fPenWidht			: ������ Prototype�� �� ����
	@point					: ������ Prototype�� ��ǥ
	@return					: ������ Prototype ��ü�� ������ ����
	*/
	IAnnotationPrototype* CreatePrototype(const int& AnnotationType,
																				const Color& color,
																				const float& fPenWidth,
																				const CPoint& point);

private:
	/**
	@Rectangle ���� �Լ�
	@AnnotationType : ������ Prototype�� Type
	@color					: ������ Prototype�� ����
	@fPenWidht			: ������ Prototype�� �� ����
	@point					: ������ Prototype�� ��ǥ
	@return					: ������ Prototype ��ü�� ������ ����
	*/
	inline IAnnotationPrototype* CreateRectanglePrototype(const Color& color,
																												const float& fPenWidth,
																												const CPoint& point);
	/**
	@Circle ���� �Լ�
	@AnnotationType : ������ Prototype�� Type
	@color					: ������ Prototype�� ����
	@fPenWidht			: ������ Prototype�� �� ����
	@point					: ������ Prototype�� ��ǥ
	@return					: ������ Prototype ��ü�� ������ ����
	*/
	inline IAnnotationPrototype* CreateCirclePrototype(const Color& color,
																										 const float& fPenWidth,
																										 const CPoint& point);
	/**
	@Line ���� �Լ�
	@AnnotationType : ������ Prototype�� Type
	@color					: ������ Prototype�� ����
	@fPenWidht			: ������ Prototype�� �� ����
	@point					: ������ Prototype�� ��ǥ
	@return					: ������ Prototype ��ü�� ������ ����
	*/
	inline IAnnotationPrototype* CreateLinePrototype(const Color& color,
																									 const float& fPenWidth,
																									 const CPoint& point);
	/**
	@Memo ���� �Լ�
	@AnnotationType : ������ Prototype�� Type
	@color					: ������ Prototype�� ����
	@fPenWidht			: ������ Prototype�� �� ����
	@point					: ������ Prototype�� ��ǥ
	@return					: ������ Prototype ��ü�� ������ ����
	*/
	inline IAnnotationPrototype* CreateMemoPrototype(const Color& color,
																									 const float& fPenWidth,
																									 const CPoint& point);
	/**
	@Rectangle ����
	@data		: DataBase���� Load�� Annotation Data
	@return : ������ Rectangle ��ü�� ������ ��ȯ
	*/
	inline IAnnotation* CreateRectangle(const AnnotationData& data);
	/**
	@Circle ����
	@data		: DataBase���� Load�� Annotation Data
	@return : ������ Circle ��ü�� ������ ��ȯ
	*/
	inline IAnnotation* CreateCircle(const AnnotationData& data);
	/**
	@Line ����
	@data		: DataBase���� Load�� Annotation Data
	@return : ������ Line ��ü�� ������ ��ȯ
	*/
	inline IAnnotation* CreateLine(const AnnotationData& data);
	/**
	@Memo ����
	@data		: DataBase���� Load�� Annotation Data
	@return : ������ Memo ��ü�� ������ ��ȯ
	*/
	inline IAnnotation* CreateMemo(const AnnotationData& data);
	
	
	AnnotationCreator			m_arrAnnotationCreators[4];	//AnnotationCraetor�� �迭
	
	PrototypeCreator			m_arrPrototypeCreators[4];	//PrototypeCreator�� �迭 
	CLinePrototype*				m_pLinePrototype;						//Line�� Prototype
	CRrectanglePrototype* m_pRectanglePrototype;			//Rectangle�� Prototype
	CCirclePrototype*			m_pCirclePrototype;					//Circle�� Prototype
	CMemoPrototype*				m_pMemoPrototype;						//Memo�� Prototype

};



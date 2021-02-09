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
	@Annotation 생성함수의 포인터
	@data		: DataBase에서 Load한 Annotation Data
	@return : 생성된 Annotation 객체의 포인터
	*/
	typedef IAnnotation* (AnnotationFactory::*AnnotationCreator)
		(const AnnotationData& data);
	/**
	@Prototype 생성함수의 포인터
	@AnnotationType : 생성될 Prototype의 Type
	@color					: 생성될 Prototype의 색상
	@fPenWidht			: 생성될 Prototype의 펜 굵기
	@point					: 생성될 Prototype의 좌표
	@return					: 생성된 Prototype 객체의 포인터 변수
	*/
	typedef IAnnotationPrototype* (AnnotationFactory::*PrototypeCreator)
		(const Color& color,
		 const float& fPenWidth,
		 const CPoint& point);
	/**
	@Annotation 생성
	@data		: DataBase에서 Load한 Annotation Data
	@return : 생성된 Annotation 포인터 반환
	*/
	IAnnotation* CreateAnnotation(const AnnotationData& data);

	/**
	@Annotation Prototype 생성
	@AnnotationType : 생성될 Prototype의 Type
	@color					: 생성될 Prototype의 색상
	@fPenWidht			: 생성될 Prototype의 펜 굵기
	@point					: 생성될 Prototype의 좌표
	@return					: 생성된 Prototype 객체의 포인터 변수
	*/
	IAnnotationPrototype* CreatePrototype(const int& AnnotationType,
																				const Color& color,
																				const float& fPenWidth,
																				const CPoint& point);

private:
	/**
	@Rectangle 생성 함수
	@AnnotationType : 생성될 Prototype의 Type
	@color					: 생성될 Prototype의 색상
	@fPenWidht			: 생성될 Prototype의 펜 굵기
	@point					: 생성될 Prototype의 좌표
	@return					: 생성된 Prototype 객체의 포인터 변수
	*/
	inline IAnnotationPrototype* CreateRectanglePrototype(const Color& color,
																												const float& fPenWidth,
																												const CPoint& point);
	/**
	@Circle 생성 함수
	@AnnotationType : 생성될 Prototype의 Type
	@color					: 생성될 Prototype의 색상
	@fPenWidht			: 생성될 Prototype의 펜 굵기
	@point					: 생성될 Prototype의 좌표
	@return					: 생성된 Prototype 객체의 포인터 변수
	*/
	inline IAnnotationPrototype* CreateCirclePrototype(const Color& color,
																										 const float& fPenWidth,
																										 const CPoint& point);
	/**
	@Line 생성 함수
	@AnnotationType : 생성될 Prototype의 Type
	@color					: 생성될 Prototype의 색상
	@fPenWidht			: 생성될 Prototype의 펜 굵기
	@point					: 생성될 Prototype의 좌표
	@return					: 생성된 Prototype 객체의 포인터 변수
	*/
	inline IAnnotationPrototype* CreateLinePrototype(const Color& color,
																									 const float& fPenWidth,
																									 const CPoint& point);
	/**
	@Memo 생성 함수
	@AnnotationType : 생성될 Prototype의 Type
	@color					: 생성될 Prototype의 색상
	@fPenWidht			: 생성될 Prototype의 펜 굵기
	@point					: 생성될 Prototype의 좌표
	@return					: 생성된 Prototype 객체의 포인터 변수
	*/
	inline IAnnotationPrototype* CreateMemoPrototype(const Color& color,
																									 const float& fPenWidth,
																									 const CPoint& point);
	/**
	@Rectangle 생성
	@data		: DataBase에서 Load한 Annotation Data
	@return : 생성된 Rectangle 객체의 포인터 반환
	*/
	inline IAnnotation* CreateRectangle(const AnnotationData& data);
	/**
	@Circle 생성
	@data		: DataBase에서 Load한 Annotation Data
	@return : 생성된 Circle 객체의 포인터 반환
	*/
	inline IAnnotation* CreateCircle(const AnnotationData& data);
	/**
	@Line 생성
	@data		: DataBase에서 Load한 Annotation Data
	@return : 생성된 Line 객체의 포인터 반환
	*/
	inline IAnnotation* CreateLine(const AnnotationData& data);
	/**
	@Memo 생성
	@data		: DataBase에서 Load한 Annotation Data
	@return : 생성된 Memo 객체의 포인터 반환
	*/
	inline IAnnotation* CreateMemo(const AnnotationData& data);
	
	
	AnnotationCreator			m_arrAnnotationCreators[4];	//AnnotationCraetor의 배열
	
	PrototypeCreator			m_arrPrototypeCreators[4];	//PrototypeCreator의 배열 
	CLinePrototype*				m_pLinePrototype;						//Line의 Prototype
	CRrectanglePrototype* m_pRectanglePrototype;			//Rectangle의 Prototype
	CCirclePrototype*			m_pCirclePrototype;					//Circle의 Prototype
	CMemoPrototype*				m_pMemoPrototype;						//Memo의 Prototype

};



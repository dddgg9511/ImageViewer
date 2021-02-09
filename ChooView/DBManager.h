#pragma once
#include "DataBaseStruct.h"

//SQLite DataBase Handler
class CDBManager
{
private:

	CDBManager() {};
	/**
	@UTF8 형식을 CString으로 변환
	@utf8		: utf8형식의 문자열
	@return	: 변환된 문자열
	*/
	static void UTF82CString(const char* utf8, CString* result);
	/**
	@ANSI 형식을 UTF8 형식으로 변환
	@pszCode : ANSI 형식 문자열
	@RETURN  : UTF8 형식 문자열
	*/
	static char* ANSIToUTF8(char *pszCode);
public:
	/**
	@Category 추가
	@categoryName  : 추가할 Category의 이름 
	@Comment			 : 추가할 Commnet
	@return		     : 추가된 Category의 DataBase PrimaryKey
	*/
	static int AddCategory(const CString& categoryName, const CString& Comment);
	/**
	@Image 추가 or 수정
	@imagedata : 추가나 수정할 Image의 Data
	@return	   : 추가된 Image의 DataBase Primary Key
	*/
	static int AddImage( const ImageData& imagedata );
	/**
	@Annotation 추가 or 수정
	@annotationData : 추가나 변경할 Annotation의 data
	@return					: 추가된 Annotation의 DataBase PrimaryKey
	*/
	static int AddAnnotation(const AnnotationData& annotationData);
	/**
	@Category 전체 검색
	@categoryList : 검색결과를 저장할 Category List
	*/
	static void SelectAllCategory(std::vector<CategoryData>* categoryList);
	/**
	@Category에 포함된 Image 검색
	@categoryId : 검색할 Category의 DataBase PrimaryKey
	@imageList	: 검색 결과를 저장할 Image List
	*/
	static void SelectImageInCategory(const int& categoryID,
																	  std::vector<ImageData>* imageList);
	/**
	@Image에 포함된 Annotation 검색
	@imageId				: 검색할 Image의 DataBase PrimaryKey
	@annotationList	: 검색 결과를 저장할 Annotation List
	*/
	static void SelectAnnotationInImage(const int& imageID,
																	  	std::vector<AnnotationData>* annotationList);
	/**
	@PrimaryKey로 Category 검색
	@categoryID	: 검색할 Category의 DataBase PrimaryKey
	@result			: 검색 결과를 저장할 Category Pointer 변수
	return			: 검색 성공 여부
	*/
	static bool SelectCategoryById(const int& categoryID, CategoryData *result);
	/**
	@Image에 포함된 Memo 검색
	@imageId				: 검색할 Image의 DataBase PrimaryKey
	@annotationList	: 검색결과를 저장할 annotation List
	*/
	static void SelectMemoInImage(const int& imageId,
															  std::vector<AnnotationData>* annotationList);
	/**
	@PrimaryKey로 Category 삭제
	@categoryId : 삭제할 Category의 DataBase PrimaryKey
	@return			: 삭제의 성공 실패 여부
	*/
	static bool DeleteCategoryById(const int& categoryID);
	/**
	@PrimaryKey로 Image 삭제
	@imageId :	삭제할 Image의 DataBase PrimaryKey
	@return	 :	삭제의 성공 실패 여부
	*/
	static bool DeleteImageByID(const int& imageID);
	/**
	@Image에 포함된 Annotation 제거
	@ImageId : 삭제할 Image의 DatabasePrimaryKey
	*/
	static void DeleteAnnotationInImage(const int& ImageId);
	/**
	@Annotation에 포함된 Point 제거
	@AnnotationID : 삭제할 Annotation의 DatabasePrimaryKey
	*/
	static void DeletePointInAnnotation(const int& AnnotationID);
	/**
	@Category에 포함된 Image PrimaryKey 검색
	@CategoryID : 검색할 Category의 DataBase PrimaryKey
	@idList		: 검색결과가 저장될 Int List
	*/
	static void SelectImageIdInCategory(const int& CategoryId,
																			std::vector<int>* idList);
	/**
	@Image에 포함된 Annotation PrimaryKey 검색
	@ImageId : 검색할 Image의 DataBaes PrimaryKey
	@idList	 : 검색결과가 저장될 Int List
	*/
	static void SelectAnnotationIdInImage(const int& ImageID,
																				std::vector<int>* idList);
	/**
	@프로그램 종료시 설정값 저장
	@row			: 현재 Row의 설정값
	@col			: 현재 Col의 설정값
	@penWidth	: 현재 penWidth의 설정값
	@color		: 현재 Color의 설정값
	*/
	static bool UpdateUserProfile(const int&row,const int& col,
																const float& penWidth, Color &color);
	/**
	@프로그램 실행시 저장된 설정값 Load
	@row			: 저장된 Row의 설정값이 담길 포인터 변수
	@col			: 저장된 Col의 설정값이 담길 포인터 변수
	@penWidth	: 저장된 penWidth의 설정값이 담길 포인터 변수
	@color		: 저장된 Color의 설정값이 담길 포인터 변수
	*/
	static bool SelectUserProfile(int* row,int* col, float* penWidth,Color* color);
	/**
	@Category의 Name과 Comment 수정
	@categoryId		: 수정할 Category의 DataBase PrimaryKey
	@categoryName	: 수정할 Category Name
	@comment			: 수정할 Category Comment
	@return				: 수정 성공 실패 여부
	*/
	static bool UpdateCategory(const int& categoryID, const CString& categoryName,
														 const CString& comment);
	/**
	@Image의 DataBase PrimaryKey로 Image검색
	@imageId		: 검색할 Image의 DataBaes PrimaryKey
	@imageData	: 검색결과가 저장될 Image 포인터 변수
	*/
	static void SelectImageByID(const int& imageID, ImageData* imagedata);
	/**
	@Annotation의 DataBase PrimaryKey로 Annotation 제거
	@annotationID : Annotation의 DataBase PrimaryKey
	*/
	static void DeleteAnnotationById(const int& annotationID);
	/**
	@Primary Key로 CategoryName 검색
	@categoryId : Category의 DataBase PrimaryKey
	@return			: 검색된 Category의 이름
	*/
	static CString SelectCategoryNameById(const int& categoryId);
};




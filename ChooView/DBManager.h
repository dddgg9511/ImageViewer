#pragma once
#include "DataBaseStruct.h"

//SQLite DataBase Handler
class CDBManager
{
private:

	CDBManager() {};
	/**
	@UTF8 ������ CString���� ��ȯ
	@utf8		: utf8������ ���ڿ�
	@return	: ��ȯ�� ���ڿ�
	*/
	static void UTF82CString(const char* utf8, CString* result);
	/**
	@ANSI ������ UTF8 �������� ��ȯ
	@pszCode : ANSI ���� ���ڿ�
	@RETURN  : UTF8 ���� ���ڿ�
	*/
	static char* ANSIToUTF8(char *pszCode);
public:
	/**
	@Category �߰�
	@categoryName  : �߰��� Category�� �̸� 
	@Comment			 : �߰��� Commnet
	@return		     : �߰��� Category�� DataBase PrimaryKey
	*/
	static int AddCategory(const CString& categoryName, const CString& Comment);
	/**
	@Image �߰� or ����
	@imagedata : �߰��� ������ Image�� Data
	@return	   : �߰��� Image�� DataBase Primary Key
	*/
	static int AddImage( const ImageData& imagedata );
	/**
	@Annotation �߰� or ����
	@annotationData : �߰��� ������ Annotation�� data
	@return					: �߰��� Annotation�� DataBase PrimaryKey
	*/
	static int AddAnnotation(const AnnotationData& annotationData);
	/**
	@Category ��ü �˻�
	@categoryList : �˻������ ������ Category List
	*/
	static void SelectAllCategory(std::vector<CategoryData>* categoryList);
	/**
	@Category�� ���Ե� Image �˻�
	@categoryId : �˻��� Category�� DataBase PrimaryKey
	@imageList	: �˻� ����� ������ Image List
	*/
	static void SelectImageInCategory(const int& categoryID,
																	  std::vector<ImageData>* imageList);
	/**
	@Image�� ���Ե� Annotation �˻�
	@imageId				: �˻��� Image�� DataBase PrimaryKey
	@annotationList	: �˻� ����� ������ Annotation List
	*/
	static void SelectAnnotationInImage(const int& imageID,
																	  	std::vector<AnnotationData>* annotationList);
	/**
	@PrimaryKey�� Category �˻�
	@categoryID	: �˻��� Category�� DataBase PrimaryKey
	@result			: �˻� ����� ������ Category Pointer ����
	return			: �˻� ���� ����
	*/
	static bool SelectCategoryById(const int& categoryID, CategoryData *result);
	/**
	@Image�� ���Ե� Memo �˻�
	@imageId				: �˻��� Image�� DataBase PrimaryKey
	@annotationList	: �˻������ ������ annotation List
	*/
	static void SelectMemoInImage(const int& imageId,
															  std::vector<AnnotationData>* annotationList);
	/**
	@PrimaryKey�� Category ����
	@categoryId : ������ Category�� DataBase PrimaryKey
	@return			: ������ ���� ���� ����
	*/
	static bool DeleteCategoryById(const int& categoryID);
	/**
	@PrimaryKey�� Image ����
	@imageId :	������ Image�� DataBase PrimaryKey
	@return	 :	������ ���� ���� ����
	*/
	static bool DeleteImageByID(const int& imageID);
	/**
	@Image�� ���Ե� Annotation ����
	@ImageId : ������ Image�� DatabasePrimaryKey
	*/
	static void DeleteAnnotationInImage(const int& ImageId);
	/**
	@Annotation�� ���Ե� Point ����
	@AnnotationID : ������ Annotation�� DatabasePrimaryKey
	*/
	static void DeletePointInAnnotation(const int& AnnotationID);
	/**
	@Category�� ���Ե� Image PrimaryKey �˻�
	@CategoryID : �˻��� Category�� DataBase PrimaryKey
	@idList		: �˻������ ����� Int List
	*/
	static void SelectImageIdInCategory(const int& CategoryId,
																			std::vector<int>* idList);
	/**
	@Image�� ���Ե� Annotation PrimaryKey �˻�
	@ImageId : �˻��� Image�� DataBaes PrimaryKey
	@idList	 : �˻������ ����� Int List
	*/
	static void SelectAnnotationIdInImage(const int& ImageID,
																				std::vector<int>* idList);
	/**
	@���α׷� ����� ������ ����
	@row			: ���� Row�� ������
	@col			: ���� Col�� ������
	@penWidth	: ���� penWidth�� ������
	@color		: ���� Color�� ������
	*/
	static bool UpdateUserProfile(const int&row,const int& col,
																const float& penWidth, Color &color);
	/**
	@���α׷� ����� ����� ������ Load
	@row			: ����� Row�� �������� ��� ������ ����
	@col			: ����� Col�� �������� ��� ������ ����
	@penWidth	: ����� penWidth�� �������� ��� ������ ����
	@color		: ����� Color�� �������� ��� ������ ����
	*/
	static bool SelectUserProfile(int* row,int* col, float* penWidth,Color* color);
	/**
	@Category�� Name�� Comment ����
	@categoryId		: ������ Category�� DataBase PrimaryKey
	@categoryName	: ������ Category Name
	@comment			: ������ Category Comment
	@return				: ���� ���� ���� ����
	*/
	static bool UpdateCategory(const int& categoryID, const CString& categoryName,
														 const CString& comment);
	/**
	@Image�� DataBase PrimaryKey�� Image�˻�
	@imageId		: �˻��� Image�� DataBaes PrimaryKey
	@imageData	: �˻������ ����� Image ������ ����
	*/
	static void SelectImageByID(const int& imageID, ImageData* imagedata);
	/**
	@Annotation�� DataBase PrimaryKey�� Annotation ����
	@annotationID : Annotation�� DataBase PrimaryKey
	*/
	static void DeleteAnnotationById(const int& annotationID);
	/**
	@Primary Key�� CategoryName �˻�
	@categoryId : Category�� DataBase PrimaryKey
	@return			: �˻��� Category�� �̸�
	*/
	static CString SelectCategoryNameById(const int& categoryId);
};




#include "stdafx.h"
#include "DBManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CDBManager::UTF82CString(const char* utf8, CString* result)
{
	int length = MultiByteToWideChar(CP_UTF8, 0, utf8, strlen(utf8) + 1,
																	 NULL, NULL);
	wchar_t* pBuf = new wchar_t[length + 1];
	MultiByteToWideChar(CP_UTF8, 0, utf8, strlen(utf8) + 1, pBuf, length);
	pBuf[length] = 0;
	*result = CString(pBuf);

	delete[] pBuf;
}



int CDBManager::AddCategory(const CString& categoryName, const CString& Comment)
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	CString sql;
	/* Open database */
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		CString errMsg;
		UTF82CString(sqlite3_errmsg(db), &errMsg);
		err.Format(_T("Can't open database: %s\n"), errMsg);
		AfxMessageBox(err);
		exit(0);
	}


	USES_CONVERSION; // CString -> char* 

	sql.Format(_T("INSERT INTO Category (Category_Name,Comment,\
								CreateDate,UpdateDate) VALUES \
								('%s','%s',date('now','localtime'),date('now','localtime'));"),
						 categoryName, Comment);

	char* temp = T2A(sql);

	rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		CString err;
		CString errMsg;
		UTF82CString(zErrMsg, &errMsg);
		err.Format(_T("SQL error: %s"), errMsg);
		AfxMessageBox(err);
		sqlite3_free(zErrMsg);

		sqlite3_close(db);
		return -1;
	}
	int category_id = static_cast<int>(sqlite3_last_insert_rowid(db));


	sqlite3_close(db);

	return category_id;
}

int CDBManager::AddImage(const ImageData& imagedata)
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return -1;
	}

	//새로 생성된 Image 추가
	if (imagedata.m_nImageID == -1)
	{
		SQL.Format(_T("INSERT INTO \
									imageRect(CropTl_X,CropTl_Y,CropBr_X,CropBr_Y,PanX,PanY) \
									VALUES ('%lf','%lf','%lf','%lf','%lf','%lf');"),
							 imagedata.m_cropTl.X, imagedata.m_cropTl.Y,
							 imagedata.m_cropBr.X, imagedata.m_cropBr.Y,
							 imagedata.m_pan.X, imagedata.m_pan.Y);

		USES_CONVERSION; // CString -> char* 
		char* temp = T2A(SQL);
		rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			CString err;
			CString errMsg;
			UTF82CString(zErrMsg, &errMsg);
			err.Format(_T("SQL error: %s"), errMsg);
			AfxMessageBox(err);
			sqlite3_free(zErrMsg);

			sqlite3_close(db);
			return -1;
		}
		int imageRectID = static_cast<int>(sqlite3_last_insert_rowid(db));

		SQL.Format(_T("INSERT INTO Image (Category_ID,ZoomRate,WindowWidth,\
									 WindowCenter,ImageRect_ID,CreateDate,\
									 UpdateDate,RotateState,FlipVertical,FlipHorizontal,fileName)\
									 VALUES ('%d','%lf','%d','%d','%d',\
									 date('now','localtime'),date('now','localtime'),\
									 '%d','%d','%d','%s');"),
							 imagedata.m_nCategoryID, imagedata.m_fZoomRate,
							 imagedata.m_nWindowWidth, imagedata.m_nWindowCenter,
							 imageRectID, imagedata.m_nRotateState, imagedata.m_bFlipVertical,
							 imagedata.m_bFlipVertical, imagedata.m_strFileName);
		char* test = T2A(SQL);
		rc = sqlite3_exec(db, test, 0, 0, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			CString err;
			CString errMsg;
			UTF82CString(zErrMsg, &errMsg);
			err.Format(_T("SQL error: %s"), errMsg);
			AfxMessageBox(err);
			sqlite3_free(zErrMsg);
			sqlite3_close(db);
			return -1;
		}
		int ImageID = static_cast<int>(sqlite3_last_insert_rowid(db));

		sqlite3_close(db);
		return ImageID;
	}
	//기존에 있는 Image 수정
	else
	{
		SQL.Format(_T("UPDATE ImageRect SET CropTl_X=%lf,CropTl_Y=%lf,CropBr_X=%lf,\
									 CropBr_Y=%lf,PanX=%lf,PanY=%lf WHERE ImageRect_ID=%d;"),
							 imagedata.m_cropTl.X, imagedata.m_cropTl.Y, imagedata.m_cropBr.X,
							 imagedata.m_cropBr.Y, imagedata.m_pan.X,
							 imagedata.m_pan.Y, imagedata.m_nImageID);
		USES_CONVERSION; // CString -> char* 
		char* temp = T2A(SQL);
		temp = T2A(SQL);
		rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			CString err;
			CString errMsg;
			UTF82CString(zErrMsg, &errMsg);
			err.Format(_T("SQL error: %s"), errMsg);
			AfxMessageBox(err);
			sqlite3_free(zErrMsg);
			sqlite3_close(db);
			return -1;
		}
		SQL.Format(_T("UPDATE Image SET Category_ID=%d,ZoomRate=%lf,WindowWidth=%d,\
									 WindowCenter=%d,UpdateDate=date('now','localtime'),\
									 RotateState=%d,FlipVertical=%d,FlipHorizontal=%d,\
									 fileName='%s' WHERE Image_ID=%d;"),
							 imagedata.m_nCategoryID, imagedata.m_fZoomRate,
							 imagedata.m_nWindowWidth, imagedata.m_nWindowCenter,
							 imagedata.m_nRotateState, imagedata.m_bFlipVertical,
							 imagedata.m_bFlipHorizontal, imagedata.m_strFileName,
							 imagedata.m_nImageID);
		temp = T2A(SQL);
		rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			CString err;
			CString errMsg;
			UTF82CString(zErrMsg, &errMsg);
			err.Format(_T("SQL error: %s"), errMsg);
			AfxMessageBox(err);
			sqlite3_free(zErrMsg);
			sqlite3_close(db);
			return -1;
		}
		return imagedata.m_nImageID;
	}
}

int CDBManager::AddAnnotation(const AnnotationData& annotationData)
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	int r = annotationData.m_penColor.GetR();
	int g = annotationData.m_penColor.GetG();
	int b = annotationData.m_penColor.GetB();
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return -1;
	}
	//새로 생성된 Annotation 추가
	if (annotationData.m_nAnnotationID == -1)
	{
		SQL.Format(_T("INSERT INTO Annotation (PenWidth,Type,ColorR,ColorG,ColorB,\
									MemoString,CreateDate,UpdateDate,ImageID,Theta)\
									VALUES (%f,%d,%d,%d,%d,'%s',date('now','localtime'),\
									date('now','localtime'),%d,%f);"),
							 annotationData.m_fPenWidth, annotationData.m_nAnnotationType,
							 r, g, b, annotationData.m_strMemoString,
							 annotationData.m_nImageID, annotationData.m_fTheta);
		USES_CONVERSION; // CString -> char* 
		char* temp = T2A(SQL);
		temp = T2A(SQL);
		rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			CString err;
			CString errMsg;
			UTF82CString(zErrMsg, &errMsg);
			err.Format(_T("SQL error: %s"), errMsg);
			AfxMessageBox(err);
			sqlite3_free(zErrMsg);
			sqlite3_close(db);
			return-1;
		}
		int AnnotationID = static_cast<int>(sqlite3_last_insert_rowid(db));
		//Annotation의 Point 추가 
		for (unsigned int i = 0; i < annotationData.m_arrPoints.size(); i++)
		{
			SQL.Format(_T("INSERT INTO Point (AnnotationID,X,Y,idx) \
										 VALUES (%d,%lf,%lf,%d);"), AnnotationID,
								 annotationData.m_arrPoints[i].X,
								 annotationData.m_arrPoints[i].Y, i);
			char* temp = T2A(SQL);
			temp = T2A(SQL);
			rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
			if (rc != SQLITE_OK)
			{
				CString err;
				CString errMsg;
				UTF82CString(zErrMsg, &errMsg);
				err.Format(_T("SQL error: %s"), errMsg);
				AfxMessageBox(err);
				sqlite3_free(zErrMsg);
				sqlite3_close(db);
				return-1;
			}
		}
	}
	//기존에 있는 Annotation 변경
	else {
		SQL.Format(_T("UPDATE Annotation SET PenWidth=%f,Type=%d,\
									 MemoString='%s',ColorR=%d,ColorG=%d,ColorB=%d,\
									 UpdateDate=date('now','localtime'),theta=%f \
									 WHERE AnnotationID=%d;"),
							annotationData.m_fPenWidth, annotationData.m_nAnnotationType,
							annotationData.m_strMemoString, r, g, b,
							annotationData.m_fTheta, annotationData.m_nAnnotationID);
		USES_CONVERSION; // CString -> char* 
		char* temp = T2A(SQL);
		temp = T2A(SQL);
		rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			CString err;
			CString errMsg;
			UTF82CString(zErrMsg, &errMsg);
			err.Format(_T("SQL error: %s"), errMsg);
			AfxMessageBox(err);
			sqlite3_free(zErrMsg);
			sqlite3_close(db);
			return-1;
		}
		int AnnotationID = static_cast<int>(sqlite3_last_insert_rowid(db));
		//Annotation의 포인트 변경
		for (unsigned int i = 0; i < annotationData.m_arrPoints.size(); i++)
		{
			SQL.Format(_T("UPDATE Point SET X=%lf,Y=%lf WHERE idx=%d\
										 and AnnotationID=%d;"),
								annotationData.m_arrPoints[i].X,
								annotationData.m_arrPoints[i].Y, i, 
								annotationData.m_nAnnotationID);
			char* temp = T2A(SQL);
			temp = T2A(SQL);
			rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
			if (rc != SQLITE_OK)
			{
				CString err;
				CString errMsg;
				UTF82CString(zErrMsg, &errMsg);
				err.Format(_T("SQL error: %s"), errMsg);
				AfxMessageBox(err);
				sqlite3_free(zErrMsg);
				sqlite3_close(db);
				return-1;
			}
		}
	}
	return annotationData.m_nAnnotationID;
}

void CDBManager::SelectAllCategory(std::vector<CategoryData>* categoryList)
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return;
	}
	SQL = _T("SELECT * FROM Category;");
	USES_CONVERSION; // CString -> char* 
	char* temp = T2A(SQL);
	sqlite3_prepare_v2(db, temp, -1, &stmt, NULL);
	int count = sqlite3_column_count(stmt);
	while (sqlite3_step(stmt) != SQLITE_DONE)
	{
		int nCategoryID = sqlite3_column_int(stmt, 0);
		char* categoryName = (char*)sqlite3_column_text(stmt, 1);
		char* comment = (char*)sqlite3_column_text(stmt, 2);
		char* createDate = (char*)sqlite3_column_text(stmt, 3);
		char* updateDate = (char*)sqlite3_column_text(stmt, 4);
		CString strCategoryName;
		CString strComment;
		CString strCreateDate;
		CString strUpdateDate;
		strCategoryName = categoryName;
		strComment = comment;
		strCreateDate = createDate;
		strUpdateDate = updateDate;
		CategoryData data(nCategoryID, strCategoryName, strComment, strCreateDate,
											strUpdateDate);
		categoryList->push_back(data);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}


void CDBManager::SelectImageInCategory(const int& categoryID,
																			 std::vector<ImageData>* imageList)
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return;
	}
	SQL.Format(_T("SELECT * FROM IMAGE AS I JOIN IMAGERECT AS RECT\
								 ON I.ImageRect_ID=RECT.ImageRect_ID \
								 WHERE I.Category_ID=%d;"), categoryID);
	USES_CONVERSION; // CString -> char* 
	char* temp = T2A(SQL);
	sqlite3_prepare_v2(db, temp, -1, &stmt, NULL);
	while (sqlite3_step(stmt) != SQLITE_DONE)
	{
		ImageData data;
		data.m_nImageID = sqlite3_column_int(stmt, 0);
		data.m_fZoomRate = sqlite3_column_double(stmt, 2);
		data.m_nWindowWidth = sqlite3_column_int(stmt, 3);
		data.m_nWindowCenter = sqlite3_column_int(stmt, 4);
		char* createDate = (char*)sqlite3_column_text(stmt, 6);
		UTF82CString(createDate, &data.m_strCreateDate);
		char* updateDate = (char*)sqlite3_column_text(stmt, 7);
		UTF82CString(updateDate, &data.m_strUpdateDate);
		char* fileName = (char*)sqlite3_column_text(stmt, 8);
		data.m_strFileName = fileName;
		data.m_bFlipVertical = (sqlite3_column_int(stmt, 9) != 0);
		data.m_bFlipHorizontal = (sqlite3_column_int(stmt, 10) != 0);
		data.m_nRotateState = sqlite3_column_int(stmt, 11);
		data.m_cropTl.X = sqlite3_column_double(stmt, 13);
		data.m_cropTl.Y = sqlite3_column_double(stmt, 14);
		data.m_cropBr.X = sqlite3_column_double(stmt, 15);
		data.m_cropBr.Y = sqlite3_column_double(stmt, 16);
		data.m_pan.X = sqlite3_column_double(stmt, 17);
		data.m_pan.Y = sqlite3_column_double(stmt, 18);
		data.m_nCategoryID = categoryID;
		imageList->push_back(data);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

void CDBManager::SelectAnnotationInImage(
		const int& imageID,
		std::vector<AnnotationData>* annotationList)
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return;
	}
	SQL.Format(_T("SELECT * FROM Annotation WHERE ImageID=%d;"), imageID);
	USES_CONVERSION; // CString -> char* 
	char* temp = T2A(SQL);
	sqlite3_prepare_v2(db, temp, -1, &stmt, NULL);
	while (sqlite3_step(stmt) != SQLITE_DONE)
	{
		AnnotationData annotation;
		annotation.m_nAnnotationID = sqlite3_column_int(stmt, 0);
		annotation.m_fPenWidth = sqlite3_column_double(stmt, 1);
		annotation.m_nAnnotationType = sqlite3_column_int(stmt, 2);
		if (annotation.m_nAnnotationType == DrawMemo)
		{
			annotation.m_strMemoString = (char*)sqlite3_column_text(stmt, 3);
		}
		int r = sqlite3_column_int(stmt, 7);
		int g = sqlite3_column_int(stmt, 8);
		int b = sqlite3_column_int(stmt, 9);
		annotation.m_penColor = Color(r, g, b);
		annotation.m_fTheta = sqlite3_column_double(stmt, 10);
		sqlite3_stmt* stmtPoint;
		SQL.Format(_T("SELECT * FROM Point WHERE AnnotationID=%d;"),
			annotation.m_nAnnotationID);
		temp = T2A(SQL);
		sqlite3_prepare_v2(db, temp, -1, &stmtPoint, NULL);
		while (sqlite3_step(stmtPoint) != SQLITE_DONE)
		{
			float x = sqlite3_column_double(stmtPoint, 2);
			float y = sqlite3_column_double(stmtPoint, 3);

			annotation.m_arrPoints.push_back(PointF(x, y));
		}
		annotationList->push_back(annotation);
		sqlite3_finalize(stmtPoint);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}


char* CDBManager::ANSIToUTF8(char* pszCode)
{
	int nLength, nLength2;
	BSTR bstrCode;
	char* pszUTFCode = NULL;
	nLength = MultiByteToWideChar(CP_ACP, 0, pszCode, strlen(pszCode), NULL, NULL);
	bstrCode = SysAllocStringLen(NULL, nLength);
	MultiByteToWideChar(CP_ACP, 0, pszCode, strlen(pszCode), bstrCode, nLength);


	nLength2 = WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, pszUTFCode, 0, NULL,
																 NULL);
	pszUTFCode = (char*)malloc(nLength2 + 1);
	WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, pszUTFCode, nLength2, NULL,
											NULL);
	return pszUTFCode;
}


bool CDBManager::SelectCategoryById(const int& categoryID, CategoryData* result)
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc) {
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return false;
	}
	SQL.Format(_T("SELECT * FROM Category WHERE Category_ID=%d;"), categoryID);
	USES_CONVERSION; // CString -> char* 
	char* temp = T2A(SQL);
	sqlite3_prepare_v2(db, temp, -1, &stmt, NULL);
	int nCount = 0;
	while (sqlite3_step(stmt) != SQLITE_DONE)
	{
		nCount++;
		result->m_strCategoryName = (char*)sqlite3_column_text(stmt, 1);
		result->m_strComment = (char*)sqlite3_column_text(stmt, 2);
		result->m_strCreateDate = (char*)sqlite3_column_text(stmt, 3);
		result->m_strUpdateDate = (char*)sqlite3_column_text(stmt, 4);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	if (nCount != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void CDBManager::SelectMemoInImage(const int& imageId,
																	 std::vector<AnnotationData>* annotationList)
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return;
	}
	SQL.Format(_T("SELECT * FROM Annotation WHERE ImageID=%d And Type=%d ;"),
		imageId, DrawMemo);
	USES_CONVERSION; // CString -> char* 
	char* temp = T2A(SQL);
	sqlite3_prepare_v2(db, temp, -1, &stmt, NULL);
	while (sqlite3_step(stmt) != SQLITE_DONE)
	{
		AnnotationData annotation;
		char* memoString = (char*)sqlite3_column_text(stmt, 3);
		if (memoString != NULL)
		{
			annotation.m_strMemoString = memoString;
		}
		else
		{
			annotation.m_strMemoString = _T("");
		}
		char* createDate = (char*)sqlite3_column_text(stmt, 4);
		UTF82CString(createDate, &annotation.m_strCreateDate);
		char* updateDate = (char*)sqlite3_column_text(stmt, 5);
		UTF82CString(updateDate, &annotation.m_strUpdateDate);
		annotationList->push_back(annotation);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}


bool CDBManager::DeleteCategoryById(const int& categoryID)
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return false;
	}
	SQL.Format(_T("Delete FROM Category WHERE Category_ID=%d;"), categoryID);
	USES_CONVERSION; // CString -> char* 
	char* temp = T2A(SQL);
	rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		CString err;
		CString errMsg;
		UTF82CString(zErrMsg, &errMsg);
		err.Format(_T("SQL error: %s"), errMsg);
		AfxMessageBox(err);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return false;
	}
	sqlite3_close(db);
	return true;
}


bool CDBManager::DeleteImageByID(const int& imageID)
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return false;
	}
	USES_CONVERSION; // CString -> char* 
	SQL.Format(_T("Delete FROM Image WHERE Image_ID=%d;"), imageID);
	char* temp = T2A(SQL);
	rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		CString err;
		CString errMsg;
		UTF82CString(zErrMsg, &errMsg);
		err.Format(_T("SQL error: %s\nImage DELETE FAIL"), errMsg);
		AfxMessageBox(err);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return false;
	}
	SQL.Format(_T("Delete FROM ImageRect WHERE ImageRect_ID=%d;"), imageID);
	temp = T2A(SQL);
	rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		CString err;
		CString errMsg;
		UTF82CString(zErrMsg, &errMsg);
		err.Format(_T("SQL error: %s\n Image DELETE FAIL"), errMsg);
		AfxMessageBox(err);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return false;
	}
	sqlite3_close(db);
	return true;
}


void CDBManager::DeleteAnnotationInImage(const int& ImageId)
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return;
	}
	USES_CONVERSION; // CString -> char* 
	SQL.Format(_T("Delete FROM Annotation WHERE ImageID=%d;"), ImageId);
	char* temp = T2A(SQL);
	rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		CString err;
		CString errMsg;
		UTF82CString(zErrMsg, &errMsg);
		err.Format(_T("SQL error: %s\n Annotation DELETE FAIL"), errMsg);
		AfxMessageBox(err);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return;
	}
	sqlite3_close(db);
}


void CDBManager::DeletePointInAnnotation(const int& AnnotationID)
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return;
	}
	USES_CONVERSION; // CString -> char* 
	SQL.Format(_T("Delete FROM Point WHERE AnnotationID=%d;"), AnnotationID);
	char* temp = T2A(SQL);
	rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		CString err;
		CString errMsg;
		UTF82CString(zErrMsg, &errMsg);
		err.Format(_T("SQL error: %s\n Point DELETE FAIL"), errMsg);
		AfxMessageBox(err);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return;
	}
	sqlite3_close(db);
}


void CDBManager::SelectImageIdInCategory(const int& CategoryId,
																				 std::vector<int>* idList)
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return;
	}
	SQL.Format(_T("SELECT * FROM Image WHERE Category_ID=%d;"), CategoryId);
	USES_CONVERSION; // CString -> char* 
	char* temp = T2A(SQL);
	sqlite3_prepare_v2(db, temp, -1, &stmt, NULL);
	while (sqlite3_step(stmt) != SQLITE_DONE)
	{
		int i = sqlite3_column_int(stmt, 0);
		idList->push_back(i);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}


void CDBManager::SelectAnnotationIdInImage(const int& ImageID,
																					 std::vector<int>* idList)
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return;
	}
	SQL.Format(_T("SELECT * FROM Annotation WHERE ImageID=%d;"), ImageID);
	USES_CONVERSION; // CString -> char* 
	char* temp = T2A(SQL);
	sqlite3_prepare_v2(db, temp, -1, &stmt, NULL);
	while (sqlite3_step(stmt) != SQLITE_DONE)
	{
		int i = sqlite3_column_int(stmt, 0);
		idList->push_back(i);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}


bool CDBManager::UpdateUserProfile(const int& row, const int& col,
																	 const float& penWidth, Color& color)
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	CString sql;
	/* Open database */
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		CString errMsg;
		UTF82CString(sqlite3_errmsg(db), &errMsg);
		err.Format(_T("Can't open database: %s\n"), errMsg);
		AfxMessageBox(err);
		return false;
	}

	USES_CONVERSION; // CString -> char* 
	int r = color.GetR();
	int g = color.GetG();
	int b = color.GetB();

	sql.Format(_T("UPDATE UserProfile SET row=%d,col=%d,PenWidth=%f,\
								 Red=%d,Green=%d,Blue=%d WHERE UserProfileID=1;"),
						 row, col, penWidth, r, g, b);

	char* temp = T2A(sql);

	rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		CString err;
		CString errMsg;
		UTF82CString(zErrMsg, &errMsg);
		err.Format(_T("SQL error: %s"), errMsg);
		AfxMessageBox(err);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return false;
	}
	sqlite3_close(db);
	return true;
}


bool CDBManager::SelectUserProfile(int* row, int* col, float* penWidth,
																   Color* color)
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return false;
	}
	SQL.Format(_T("SELECT * FROM UserProfile WHERE UserProfileID=1;"));
	USES_CONVERSION; // CString -> char* 
	char* temp = T2A(SQL);
	sqlite3_prepare_v2(db, temp, -1, &stmt, NULL);
	while (sqlite3_step(stmt) != SQLITE_DONE)
	{
		*row = sqlite3_column_int(stmt, 1);
		*col = sqlite3_column_int(stmt, 2);
		*penWidth = sqlite3_column_double(stmt, 3);
		int r = sqlite3_column_int(stmt, 4);
		int g = sqlite3_column_int(stmt, 5);
		int b = sqlite3_column_int(stmt, 6);
		*color = Color(r, g, b);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);

	return true;
}


bool CDBManager::UpdateCategory(const int& categoryID,
								const CString& categoryName,
								const CString& comment)
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	CString sql;
	/* Open database */
	rc = sqlite3_open("viewer.db", &db);
	if (rc) {
		CString err;
		CString errMsg;
		UTF82CString(sqlite3_errmsg(db), &errMsg);
		err.Format(_T("Can't open database: %s\n"), errMsg);
		AfxMessageBox(err);
		return false;
	}

	USES_CONVERSION; // CString -> char* 
	sql.Format(_T("UPDATE Category SET category_Name='%s',comment='%s'\
				  WHERE Category_ID=%d;"),
				  categoryName, comment, categoryID);

	char* temp = T2A(sql);

	rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		CString err;
		CString errMsg;
		UTF82CString(zErrMsg, &errMsg);
		err.Format(_T("SQL error: %s"), errMsg);
		AfxMessageBox(err);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return false;
	}
	sqlite3_close(db);
	return true;
}


void CDBManager::SelectImageByID(const int& imageID, ImageData* imagedata)
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc) {
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return;
	}
	SQL.Format(_T("SELECT * FROM IMAGE AS I JOIN IMAGERECT AS RECT ON \
				  I.ImageRect_ID=RECT.ImageRect_ID\
				  WHERE I.Image_ID=%d;"), imageID);
	USES_CONVERSION; // CString -> char* 
	char* temp = T2A(SQL);
	sqlite3_prepare_v2(db, temp, -1, &stmt, NULL);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		imagedata->m_nImageID = sqlite3_column_int(stmt, 0);
		imagedata->m_fZoomRate = sqlite3_column_double(stmt, 2);
		imagedata->m_nWindowWidth = sqlite3_column_int(stmt, 3);
		imagedata->m_nWindowCenter = sqlite3_column_int(stmt, 4);
		char* createDate = (char*)sqlite3_column_text(stmt, 6);
		UTF82CString(createDate, &imagedata->m_strCreateDate);
		char* updateDate = (char*)sqlite3_column_text(stmt, 7);
		UTF82CString(updateDate, &imagedata->m_strUpdateDate);
		char* fileName = (char*)sqlite3_column_text(stmt, 8);
		imagedata->m_strFileName = fileName;
		imagedata->m_bFlipVertical = (sqlite3_column_int(stmt, 9) != 0);
		imagedata->m_bFlipHorizontal = (sqlite3_column_int(stmt, 10) != 0);
		imagedata->m_nRotateState = sqlite3_column_int(stmt, 11);
		imagedata->m_cropTl.X = sqlite3_column_double(stmt, 13);
		imagedata->m_cropTl.Y = sqlite3_column_double(stmt, 14);
		imagedata->m_cropBr.X = sqlite3_column_double(stmt, 15);
		imagedata->m_cropBr.Y = sqlite3_column_double(stmt, 16);
		imagedata->m_pan.X = sqlite3_column_double(stmt, 17);
		imagedata->m_pan.Y = sqlite3_column_double(stmt, 18);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}


void CDBManager::DeleteAnnotationById(const int& annotationID)
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc)
	{
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return;
	}
	USES_CONVERSION; // CString -> char* 
	SQL.Format(_T("Delete FROM Annotation WHERE AnnotationID=%d;"), annotationID);
	char* temp = T2A(SQL);
	rc = sqlite3_exec(db, temp, 0, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		CString err;
		CString errMsg;
		UTF82CString(zErrMsg, &errMsg);
		err.Format(_T("SQL error: %s\n Annotation DELETE FAIL"), errMsg);
		AfxMessageBox(err);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return;
	}
	sqlite3_close(db);
}

CString CDBManager::SelectCategoryNameById(const int& categoryID)
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	char* zErrMsg = 0;
	int rc;
	CString SQL;
	rc = sqlite3_open("viewer.db", &db);
	if (rc) {
		CString err;
		err.Format(_T("Can't open database: %s\n"), sqlite3_errmsg(db));
		AfxMessageBox(err);
		return _T("");
	}
	SQL.Format(_T("SELECT Category_Name FROM Category WHERE Category_ID=%d;"),
		categoryID);
	USES_CONVERSION; // CString -> char* 
	char* temp = T2A(SQL);
	sqlite3_prepare_v2(db, temp, -1, &stmt, NULL);
	CString strCategoryName;
	while (sqlite3_step(stmt) != SQLITE_DONE)
	{
		strCategoryName = (char*)sqlite3_column_text(stmt, 0);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return strCategoryName;
}


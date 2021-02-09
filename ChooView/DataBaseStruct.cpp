#include "stdafx.h"
#include "DataBaseStruct.h"


CategoryData::CategoryData(const int& nCategoryID,
													 const CString& strCategoryName,
												   const CString& strCommnet,
													 const CString& strCreateDate,
												   const CString& strUpdateDate) :
		m_nCategoryID(nCategoryID),
		m_strCategoryName(strCategoryName),
		m_strComment(strCommnet),
		m_strCreateDate(strCreateDate),
		m_strUpdateDate(strUpdateDate)
{

}
CategoryData::CategoryData() :
		m_nCategoryID(-1),
		m_strCategoryName(_T("")),
		m_strComment(_T("")),
		m_strCreateDate(_T("")),
		m_strUpdateDate(_T(""))
{

}

AnnotationData::AnnotationData() :
		m_nAnnotationID(-1),
		m_fPenWidth(-1),
		m_nAnnotationType(-1),
		m_nImageID(-1),
		m_penColor(Color(0, 0, 0)),
		m_strMemoString(_T("")),
		m_strCreateDate(_T("")),
		m_strUpdateDate(_T(""))
{

}

AnnotationData::AnnotationData(const int& nAnnotationID, const float& fPenWidth,
															 const int& nAnnotationType, const int& nImageID,
															 const float& theta, const Color& penColor,
															 const CString& strMemoString,
															 const CString& strCreaeteDate,
															 const CString& strUpdateDate,
															 const std::vector<PointF>& arrPoints) :
		m_nAnnotationID(nAnnotationID),
		m_fPenWidth(fPenWidth),
		m_fTheta(theta),
		m_nAnnotationType(nAnnotationType),
		m_nImageID(nImageID),
		m_penColor(penColor),
		m_strMemoString(strMemoString),
		m_strCreateDate(strCreaeteDate),
		m_strUpdateDate(strUpdateDate),
		m_arrPoints(arrPoints)
{

}

ImageData::ImageData() :
		m_nImageID(-1),
		m_nCategoryID(-1),
		m_nWindowWidth(-1),
		m_nWindowCenter(-1),
		m_nRotateState(-1),
		m_bFlipVertical(false),
		m_bFlipHorizontal(false),
		m_fZoomRate(1.0f),
		m_strCreateDate(_T("")),
		m_strUpdateDate(_T("")),
		m_strFileName(_T("")),
		m_cropTl(0, 0),
		m_cropBr(0, 0),
		m_pan(0, 0)
{

}

ImageData::ImageData(const int& nImageID, const int& nCategoryID,
										 const int& nWindowWidth, const int& nWindowCenter,
										 const int& nRotateSate, const bool& bFlipVertical,
										 const bool& bFlipHorizontal, const float& dZoomRate,
										 const CString& strCreateDate, const CString& strUpdateDate,
										 const CString& strFileName, const PointF& cropTl,
										 const PointF& cropBr, const PointF& pan) :
		m_nImageID(nImageID),
		m_nCategoryID(nCategoryID),
		m_nWindowWidth(nWindowWidth),
		m_nWindowCenter(nWindowCenter),
		m_nRotateState(nRotateSate),
		m_bFlipHorizontal(bFlipHorizontal),
		m_bFlipVertical(bFlipVertical),
		m_fZoomRate(dZoomRate),
		m_strCreateDate(strCreateDate),
		m_strUpdateDate(strUpdateDate),
		m_strFileName(strFileName),
		m_cropTl(cropTl),
		m_cropBr(cropBr),
		m_pan(pan)
{

}

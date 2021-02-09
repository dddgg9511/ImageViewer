#pragma once
//DataBase의 테이블들을 저장하기 위한 Struct


//CategoryDataBase Struct
struct CategoryData
{
public:
	CategoryData(const int& nCategoryID, const CString& strCategoryName,
							 const CString& strCommnet, const CString& strCreateDate,
							 const CString& strUpdateDate);
	CategoryData();

	int			m_nCategoryID;
	CString	m_strCategoryName;
	CString	m_strComment;
	CString	m_strCreateDate;
	CString	m_strUpdateDate;
};

//AnnotationDataBase Struct
struct AnnotationData
{
public:
	AnnotationData();
	AnnotationData(const int& nAnnotationID, const float& fPenWidth,
								 const int& nAnnotationType, const int& nImageID,
								 const float& theta, const Color& penColor,
								 const CString& strMemoString, const CString& strCreaeteDate,
								 const CString& strUpdateDate,
								 const std::vector<PointF>& arrPoints);
	int									m_nAnnotationID;
	float								m_fPenWidth;
	float								m_fTheta;
	int									m_nAnnotationType;
	int									m_nImageID;
	Color								m_penColor;
	CString							m_strMemoString;
	CString							m_strCreateDate;
	CString							m_strUpdateDate;
	std::vector<PointF> m_arrPoints;
};

//ImageDataBase Struct
struct ImageData
{
public:
	ImageData();
	ImageData(const int& nImageID, const int& nCategoryID, const int& nWindowWidth,
						const int& nWindowCenter, const int& nRotateSate,
						const bool& bFlipVertical, const bool& bFlipHorizontal,
						const float& dZoomRate, const CString& strCreateDate,
						const CString& strUpdateDate, const CString& strFileName,
						const PointF& cropTl, const PointF& cropBr, const PointF& pan);
	int			m_nImageID;
	int			m_nCategoryID;
	int			m_nWindowWidth;
	int			m_nWindowCenter;
	int			m_nRotateState;
	bool		m_bFlipVertical;
	bool		m_bFlipHorizontal;
	float		m_fZoomRate;
	CString	m_strCreateDate;
	CString	m_strUpdateDate;
	CString	m_strFileName;
	PointF	m_cropTl;
	PointF	m_cropBr;
	PointF	m_pan;
};
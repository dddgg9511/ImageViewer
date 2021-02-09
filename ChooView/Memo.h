#pragma once
#define DISALLOW_COPY_AND_ASSIGN(CMemo) \
	CMemo(const CMemo&); \
	void operator=(const CMemo&);
class IAnnotation;
class CMemo : public IAnnotation
{
public:
	CMemo(const PointF& downPoint,const PointF& upPoint,const Color& penColor,
			  const float& penWidth);
	~CMemo();
	CMemo(const AnnotationData& annotationData);
	virtual void Draw(const float& orgRate, const float& zoomRate,
										const PointF& orgPoint, const PointF& viewPoint,
										const CPoint& size,	const int& rotateState, 
										const bool& flipVertical,	const bool& flipHorizontal,
										const CPoint& picPoint, Graphics* g) override;
	virtual void SelectDraw(const float& orgRate, const float& zoomRate,
													const PointF& orgPoint, const PointF& viewPoint,
													const CPoint& size,	const int& rotateState,
													const bool& flipVertical, const bool& flipHorizontal,
													const CPoint& picPoint, Graphics* g) override;
	virtual void Move(const PointF& downPoint, const PointF& upPoint) override;
	virtual void reSize(const PointF& downPoint, const PointF& upPoint) override;
	virtual bool HitTest(const PointF& imgPoint, const CPoint& clientPoint,
											 int* actionState) override;
	virtual void Update(const PointF& downPoint, const PointF& firstPoint,
										  const PointF& secondPoint) override;
	virtual void Modify(void) override;
	virtual void Rotate(const PointF& downPoint,const PointF& movePoint) override;
	virtual void SaveDB(const int& imageID, const bool& IsSaveNew) override;
	virtual void UpdateMemoString(const UINT& nChar) override;
	virtual void SetAnnotationProperty() override;
private:
	CString m_strMemo;		//출력될 문자열
	float		m_fFontWidth;	//폰트 크기
	Color		m_fontColor;	//폰트 색상
};


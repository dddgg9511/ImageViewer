#pragma once
#define DISALLOW_COPY_AND_ASSIGN(CCircle) \
	CCircle(const CCircle&); \
	void operator=(const CCircle&);
class IAnnotation;

class CCircle : public IAnnotation
{
public:
	CCircle(const PointF& downPoint,const PointF& upPoint,const Color& penColor,
					const float& penWidth);
	CCircle(const AnnotationData& annotationData);
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
};


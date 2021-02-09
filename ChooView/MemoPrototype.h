#pragma once
#include "Annotatoinprototype.h"
class CMemoPrototype :
	public IAnnotationPrototype
{
public:
	CMemoPrototype();
	~CMemoPrototype(void);

	virtual void Draw(Graphics* g) override;

	virtual IAnnotation* Convert(const float& zoomRate, const float& orgRate,
															 const PointF& viewPoint, const RectF& orgRect,
															 const bool& flipVertical,
															 const bool& flipHorizontal, 
															 const int& rotateState, const CPoint& size, 
															 const CPoint& picPoint) override;
};


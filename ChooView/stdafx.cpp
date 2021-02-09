
// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// ChooView.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

#include <cmath>


PointF Client2Img(const CPoint& point, const float& zoomRate, 
									const float& orgRate, const PointF& viewPoint, 
									const RectF& orgRect, const bool& flipVertical, 
									const bool& flipHorizontal, const int& rotateState, 
									const CPoint& size, const CPoint& picPoint)
{
	const float pi = 3.14159265359f;

	float x = (point.x - picPoint.x) / zoomRate / orgRate + viewPoint.X + orgRect.X;
	float y = (point.y - picPoint.y) / zoomRate / orgRate + viewPoint.Y + orgRect.Y;

	int sizeX = size.x;
	int sizeY = size.y;

	if (rotateState == 1)
	{
		x -= sizeY;
		std::swap(sizeX, sizeY);
	}
	else if (rotateState == 2)
	{
		x -= sizeX;
		y -= sizeY;

	}
	else if (rotateState == 3)
	{
		y -= sizeX;
		std::swap(sizeX, sizeY);
	}
	float ddX = x * int(cos((-rotateState) * pi / 2)) -
							y * int(sin((-rotateState) * pi / 2));
	float ddY = x * int(sin((-rotateState) * pi / 2)) +
							y * int(cos((-rotateState) * pi / 2));


	if (flipVertical)
	{
		if (rotateState == 1 || rotateState == 3)
		{
			ddY = (sizeX / 2 - ddY) + sizeX / 2;
		}
		else
		{
			ddX = (sizeX / 2 - ddX) + sizeX / 2;
		}
	}
	if (flipHorizontal)
	{
		if (rotateState == 1 || rotateState == 3)
		{
			ddX = (sizeY / 2 - ddX) + sizeY / 2;
		}
		else
		{
			ddY = (sizeY / 2 - ddY) + sizeY / 2;
		}
	}
	return PointF(ddX, ddY);
}



PointF Img2Client(const PointF& point,
									const float& zoomRate, const float& orgRate,
									const PointF& viewPoint, const PointF& orgPoint, 
									const bool& flipVertical,	const bool& flipHorizontal, 
									const int& rotateState,	const CPoint& size)
{
	const float pi = 3.14159265359f;


	float x = point.X * int(cos(rotateState * pi / 2)) - 
						point.Y * int(sin(rotateState * pi / 2));
	float y = point.X * int(sin(rotateState * pi / 2)) + 
						point.Y * int(cos(rotateState * pi / 2));

	int sizeX = size.x;
	int sizeY = size.y;

	if (rotateState == 1)
	{
		x += sizeY;
		std::swap(sizeX, sizeY);
	}
	else if (rotateState == 2)
	{
		x += sizeX;
		y += sizeY;

	}
	else if (rotateState == 3)
	{
		y += sizeX;
		std::swap(sizeX, sizeY);
	}

	if (flipVertical)
	{
		x = (sizeX / 2 - x) + sizeX / 2;
	}
	if (flipHorizontal)
	{
		y = (sizeY / 2 - y) + sizeY / 2;
	}
	x = (x - orgPoint.X - viewPoint.X) * zoomRate * orgRate;
	y = (y - orgPoint.Y - viewPoint.Y) * zoomRate * orgRate;
	return PointF(x, y);

}


float isLeft(const PointF& linePt1, const PointF& linePt2, const PointF& pos)
{
	return ((linePt2.X - linePt1.X) * (pos.Y - linePt1.Y) -
					(pos.X - linePt1.X) * (linePt2.Y - linePt1.Y));
}



int isContainRect(const PointF& pos, const std::vector<PointF>& vertices)
{
	int wideNum = 0;

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		unsigned nextpos = (i + 1 >= vertices.size()) ? 0 : i + 1;
		if (vertices[i].Y <= pos.Y)
		{
			if (vertices[nextpos].Y > pos.Y)
			{
				if (isLeft(vertices[i], vertices[nextpos], pos) > 0)
				{
					++wideNum;
				}
			}
		}
		else
		{
			if (vertices[nextpos].Y <= pos.Y)
			{
				if (isLeft(vertices[i], vertices[nextpos], pos) < 0)
				{
					--wideNum;
				}
			}
		}
	}
	return wideNum;
}





PointF Intersection(const PointF& pt, const PointF& LinePt1,
									  const PointF& LinePt2)
{
	PointF result;
	float A;
	float B;
	float t_A;
	float t_B;
	if ((LinePt1.X - LinePt2.X) != 0 && LinePt1.Y - LinePt2.Y != 0)
	{
		A = (LinePt1.Y - LinePt2.Y) / (LinePt1.X - LinePt2.X);
		B = LinePt1.Y - A * LinePt1.X;
		t_A = -1 / A;
		t_B = pt.Y - t_A * pt.X;

		result.X = (t_B - B) / (A - t_A);
		result.Y = result.X * A + B;
	}
	else if (LinePt1.X - LinePt2.X == 0)
	{
		result.X = LinePt1.X;
		result.Y = pt.Y;
	}
	else if (LinePt1.Y - LinePt2.Y == 0)
	{
		result.X = pt.X;
		result.Y = LinePt1.Y;
	}
	return result;
}

bool isInLine(const PointF& point, const PointF& LinePt1, const PointF& LinePt2)
{
	std::vector<PointF> linePts;

	const float difY = (LinePt1.Y - LinePt2.Y);
	const float difX = (LinePt1.X - LinePt2.X);

	if (difY == 0)
	{
		linePts.push_back(PointF(LinePt1.X, LinePt1.Y - 5));
		linePts.push_back(PointF(LinePt1.X, LinePt1.Y + 5));
		linePts.push_back(PointF(LinePt2.X, LinePt2.Y + 5));
		linePts.push_back(PointF(LinePt2.X, LinePt2.Y - 5));
	}
	else if (difX == 0)
	{
		linePts.push_back(PointF(LinePt1.X - 5, LinePt1.Y));
		linePts.push_back(PointF(LinePt1.X + 5, LinePt1.Y));
		linePts.push_back(PointF(LinePt2.X + 5, LinePt2.Y));
		linePts.push_back(PointF(LinePt2.X - 5, LinePt2.Y));
	}
	else
	{
		const float a = difY / difX;
		const float b = -a * LinePt1.X + LinePt1.Y;
		int c = 1;
		if (a > 0)
		{
			c = -1;
		}
		linePts.push_back(PointF(LinePt1.X - 5, LinePt1.X * a + b - 5 * c));
		linePts.push_back(PointF(LinePt1.X + 5, LinePt1.X * a + b + 5 * c));
		linePts.push_back(PointF(LinePt2.X + 5, LinePt2.X * a + b + 5 * c));
		linePts.push_back(PointF(LinePt2.X - 5, LinePt2.X * a + b - 5 * c));
	}
	if (isContainRect(point, linePts))
	{

		return true;
	}
	return false;
}

PointF CPoint2PointF(const CPoint& point)
{
	return PointF(static_cast<float>(point.x), static_cast<float>(point.y));
}


#pragma once
#include <math.h> 

class Vec2
{
public:
	float x, y;
	Vec2();
	Vec2(float x, float y);

	Vec2 operator + (const Vec2& rhs) const;
	Vec2 operator - (const Vec2& rhs) const;
	Vec2 operator / (const Vec2& rhs) const;
	Vec2 operator / (const float rhs) const;
	float operator * (const Vec2& rhs) const;

	void operator += (const Vec2& rhs);
	void operator -= (const Vec2& rhs);
	void operator /= (const Vec2& rhs);
	void operator *= (const Vec2& rhs);

	double length();
	float normalize();
};
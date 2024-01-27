#include "Vec2.h"

Vec2::Vec2() {};

Vec2::Vec2(float x, float y)
	: x(x)
	, y(y)
{};

Vec2 Vec2::operator + (const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
};

Vec2 Vec2::operator - (const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
};

Vec2 Vec2::operator / (const Vec2& rhs) const
{
	return Vec2(x / rhs.x, y / rhs.y);
};

Vec2 Vec2::operator / (const float rhs) const
{
	return Vec2(x / rhs, y / rhs);
};

float Vec2::operator * (const Vec2& rhs) const
{
	return (x * rhs.y) - (y * rhs.x);
};

void Vec2::operator += (const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
};

void Vec2::operator -= (const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
};

void Vec2::operator /= (const Vec2& rhs)
{
	x /= rhs.x;
	y /= rhs.y;
};

void Vec2::operator *= (const Vec2& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
};

double Vec2::length()
{
	return sqrt(x * x + y * y);
};

float Vec2::normalize()
{
	// TODO
	return 0;
};
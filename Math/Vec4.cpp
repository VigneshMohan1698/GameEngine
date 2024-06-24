#include "Vec4.hpp"
#include "Vec3.hpp"



Vec4::Vec4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
float Vec4::GetLength() const
{
	return 0.0f;
}
float Vec4::GetLengthXY() const
{
	return 0.0f;
}
float Vec4::GetLengthXYSquared() const
{
	return 0.0f;
}
float Vec4::GetLengthSquared() const
{
	return 0.0f;
}
float Vec4::GetAngleAboutZRadians() const
{
	return 0.0f;
}
float Vec4::GetAngleAboutZDegrees() const
{
	return 0.0f;
}

Vec4 const Vec4::GetNormalized() const
{
	float scale = 1.0f / GetLength();
	return Vec4(x * scale, y * scale, z * scale, w* scale);
}
//
//bool Vec4::operator==(Vec4 const& compare) const
//{
//	return false;
//}
//
//bool Vec4::operator!=(Vec4 const& compare) const
//{
//	return false;
//}

Vec4 const Vec4::operator+(Vec4 const& VecToAdd) const
{
	return Vec4(x + VecToAdd.x , y + VecToAdd.y , z + VecToAdd.z, w + VecToAdd.w);
}

Vec4 const Vec4::operator-(Vec4 const& VecToSubtract) const
{
	return Vec4(x - VecToSubtract.x, y - VecToSubtract.y, z - VecToSubtract.z, w - VecToSubtract.w);
}

Vec4 const Vec4::operator*(float uniformScale) const
{
	return Vec4(x * uniformScale, y* uniformScale, z * uniformScale,w* uniformScale);
}

bool Vec4::operator==(Vec4 const& value) const
{
	return x == value.x && y == value.y && z == value.z && w == value.w;
}


//const Vec4 Vec4::operator/(float inverseScale) const
//{
//	return Vec4();
//}
//
//void Vec4::operator+=(const Vec4& vecToAdd)
//{
//}
//
//void Vec4::operator-=(const Vec4& vecToSubtract)
//{
//}

void Vec4::operator*=(const float uniformScale)
{
	x = x * uniformScale;
	y = y * uniformScale;
	z = z * uniformScale;
	w = w * uniformScale;
}

void Vec4::operator/=(const float uniformDivisor)
{
	x /= uniformDivisor;
	y /= uniformDivisor;
	z /= uniformDivisor;
	w /= uniformDivisor;
}

void Vec4::operator=(const Vec4& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
	w = copyFrom.w;
}

Vec4::Vec4(Vec4 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
	w = copyFrom.w;
}

Vec4::Vec4(Vec3 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}
Vec4::Vec4(Vec3 const& copyFrom, float const& wValue)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
	w = wValue;
}
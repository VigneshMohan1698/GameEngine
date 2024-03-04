#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <math.h>
//#include <Game/GameCommon.hpp>
#include "Engine/Core/StringUtils.hpp"

//#include "Engine/Math/MathUtils.hpp"

//#include "Engine/Core/EngineCommon.hpp"


const Vec2 Vec2::ZERO = Vec2(0.0f, 0.0f);
const Vec2 Vec2::ONE = Vec2(1.0f, 1.0f);

//-----------------------------------------------------------------------------------------------
Vec2::Vec2(const Vec2& copy)
{
	x = copy.x;
	y = copy.y;
}
Vec2::Vec2(const IntVec2& copyFrom)
{
	x = (float)copyFrom.x;
	y = (float)copyFrom.y;
}

Vec2::Vec2(const Vec3& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}
//-----------------------------------------------------------------------------------------------
Vec2::Vec2(float initialX, float initialY)
{
	x = initialX;
	y = initialY;
}

Vec2 const Vec2::MakeFromPolarRadians(float orientationRadians, float length)
{
	return Vec2(length * cosf(orientationRadians), length * sinf(orientationRadians));
}

Vec2 const Vec2::MakeFromPolarDegrees(float orientationDegrees, float length)
{
	float orientationRadians = ConvertDegreesToRadians(orientationDegrees);
	return Vec2(length * cosf(orientationRadians), length * sinf(orientationRadians));
}

float Vec2::GetLength() const
{
	return sqrtf(x * x + y * y);
}

float Vec2::GetLengthSquared() const
{
	return (x * x + y * y);
}

float Vec2::GetOrientationRadians() const
{
	return atan2f(y, x);
}

float Vec2::GetOrientationDegrees() const
{
	return Atan2Degrees(y, x);
}


Vec2 const Vec2::GetRotated90Degrees() const
{
	return Vec2(-y, x);
}

Vec2 const Vec2::GetRotatedMinus90Degrees() const
{
	return Vec2(y, -x);
}

Vec2 const Vec2::GetRotatedRadians(float deltaRadians) const
{
	float radians = atan2f(y,x);
	float radius = GetLength();
	radians += deltaRadians;
	return Vec2(radius * CosDegrees(radians * 180.f/3.14159f), radius * SinDegrees(radians * 180.f / 3.14159f));
}

Vec2 const Vec2::GetRotatedDegrees(float deltaDegrees) const
{
	float degrees = Atan2Degrees(y, x);
	float radius = GetLength();
	degrees += deltaDegrees;
	return Vec2(radius * CosDegrees(degrees), radius * SinDegrees(degrees));
}

Vec2 const Vec2::GetClamped(float maxLength) const
{
	float orientationRadians = GetOrientationRadians();
	if (GetLength() > maxLength)
	{
		return Vec2(maxLength * cosf(orientationRadians), maxLength * sinf(orientationRadians));
	}
	else
		return Vec2(x, y);
}

Vec2 const Vec2::GetNormalized() const
{
	if (GetLength() == 0)
	{
		return Vec2(0.0f, 0.0f);
	}
	float scale = 1.0f / GetLength();
	return Vec2(x * scale, y * scale);
}

void Vec2::SetOrientationRadians(float newOrientationRadians)
{
	float radius = GetLength();
	x = radius * cosf(newOrientationRadians);
	y = radius * sinf(newOrientationRadians);
}

void Vec2::SetOrientationDegrees(float newOrientationDegrees)
{
	float radius = GetLength();
	float radians = ConvertDegreesToRadians(newOrientationDegrees);
	x = radius * cosf(radians);
	y = radius * sinf(radians);
}

void Vec2::SetPolarRadians(float newOrientationRadians, float newLength)
{
	x = newLength * cosf(newOrientationRadians);
	y = newLength * sinf(newOrientationRadians);
}

void Vec2::SetPolarDegrees(float newOrientationDegrees, float newLength)
{
	float newOrientationRadians = ConvertDegreesToRadians(newOrientationDegrees);
	x = newLength * cosf(newOrientationRadians);
	y = newLength * sinf(newOrientationRadians);
}

void Vec2::Rotate90Degrees()
{
	float temp = x;
	x = -y;
	y = temp;
}

void Vec2::RotateMinus90Degrees()
{
	float temp = x;
	x = y;
	y = -temp;
}

void Vec2::RotateRadians(float deltaRadians)
{
	float radius = GetLength();
	float radians = (atan2f(y, x));
	radians += deltaRadians;
	x = radius * cosf(radians);
	y = radius * sinf(radians);
}

void Vec2::RotateDegrees(float deltaDegrees)
{
	float radius = GetLength();
	float deg = Atan2Degrees(y, x);
	deg += deltaDegrees;
	x = radius * CosDegrees(deg);
	y = radius * SinDegrees(deg);
}

void Vec2::SetLength(float newLength)
{
	float orientationRadians = GetOrientationRadians();
	x = newLength * cosf(orientationRadians);
	y = newLength * sinf(orientationRadians);
}

void Vec2::ClampLength(float maxLength)
{
	float orientationRadians = GetOrientationRadians();
	if (GetLength() > maxLength)
	{
		x = maxLength * cosf(orientationRadians);
		y = maxLength * sinf(orientationRadians);
	}
}

void Vec2::Normalize()
{
	float scale = 1.0f / GetLength();
	x *= scale;
	y *= scale;
}

float Vec2::NormalizeAndGetPreviousLength()
{
	float scale = 1.0f / GetLength();
	x *= scale;
	y *= scale;
	return 1.0f / scale;
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator + (const Vec2& vecToAdd) const
{
	return Vec2(x + vecToAdd.x, y + vecToAdd.y);
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator-(const Vec2& vecToSubtract) const
{
	return Vec2(x - vecToSubtract.x, y - vecToSubtract.y);
}

const Vec2 Vec2::operator-(const float floatToSubtract) const
{
	return Vec2(x - floatToSubtract, y - floatToSubtract);
}
const Vec2 Vec2::operator+(const float floatToAdd) const
{
	return Vec2(x + floatToAdd, y + floatToAdd);
}

//------------------------------------------------------------------------------------------------
const Vec2 Vec2::operator-() const
{
	return Vec2(-x, -y);
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator*(float uniformScale) const
{
	return Vec2(x * uniformScale, y * uniformScale);
}


//------------------------------------------------------------------------------------------------
const Vec2 Vec2::operator*(const Vec2& vecToMultiply) const
{

	return Vec2(x * vecToMultiply.x, y * vecToMultiply.y);
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator/(float inverseScale) const
{
	return Vec2(x / inverseScale, y / inverseScale);
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator+=(const Vec2& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator-=(const Vec2& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator*=(const float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
}

//-----------------------------------------------------------------------------------------------
void Vec2::operator/=(const float uniformDivisor)
{
	x /= uniformDivisor;
	y /= uniformDivisor;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator=(const Vec2& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}

void Vec2::SetFromText(const char* text)
{
	Strings stringVec2 = SplitStringOnDelimiter(text, ',');
	x = std::stof(stringVec2[0]);
	y = std::stof(stringVec2[1]);
}

void Vec2::SetFromText(const char* text, char DelimiterToSplitOn)
{
	Strings stringVec2 = SplitStringOnDelimiter(text, DelimiterToSplitOn);
	x = std::stof(stringVec2[0]);
	y = std::stof(stringVec2[1]);
}

void Vec2::Reflect(const Vec2& impactSurfacenormal)
{
	Vec2 incomingRayNormal = impactSurfacenormal * DotProduct2D(Vec2(x,y), impactSurfacenormal);
	x -= 2 * incomingRayNormal.x;
	y -= 2 * incomingRayNormal.y;
	//*this -= 2 * incomingRayNormal;
}

Vec2 Vec2::GetReflected(Vec2 impactSurfacenormal) const
{
	Vec2 incomingRayNormal = impactSurfacenormal * DotProduct2D(*this, impactSurfacenormal);
	return Vec2(x,y) -  2 * incomingRayNormal;
}


//-----------------------------------------------------------------------------------------------
const Vec2 operator*(float uniformScale, const Vec2& vecToScale)
{
	return Vec2(vecToScale.x * uniformScale, vecToScale.y * uniformScale);
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator==(const Vec2& compare) const
{
	if (x == compare.x && y == compare.y)
		return true;
	else
		return false;
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator!=(const Vec2& compare) const
{
	if (x != compare.x || y != compare.y)
		return true;
	else
		return false;
}



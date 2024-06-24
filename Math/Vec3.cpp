#include"Engine/Math/Vec3.hpp"
#include"Engine/Math/Vec2.hpp"
#include <math.h>
#include "Engine/Math/MathUtils.hpp"
#include <Engine/Core/StringUtils.hpp>

const Vec3 Vec3::ZERO = Vec3(0.0f,0.0f,0.0f);
	Vec3::Vec3(float x,float y , float z)
	{
		this->x =  x;
		this->y = y;
		this->z = z;
	}
	Vec3::Vec3(Vec2 copyFrom)
	{
		x = copyFrom.x;
		y = copyFrom.y;
		z = 0.0f;
	}
	const Vec3 Vec3::operator-(const float floatToSubtract) const
	{
		return Vec3(x - floatToSubtract,y - floatToSubtract,z - floatToSubtract);
	}

	const Vec3 Vec3::operator+(const float floatToAdd) const
	{
		return Vec3(x + floatToAdd, y+ floatToAdd,z+ floatToAdd);
	}

	//void Vec3::operator=(const Vec3 copyFrom) 
	//{
	//	x = copyFrom.x;
	//	y = copyFrom.y;
	//	z = copyFrom.z;
	//}

	void Vec3::SetFromText(const char* text)
	{
		Strings stringVec2 = SplitStringOnDelimiter(text, ',');
		x = std::stof(stringVec2[0]);
		y = std::stof(stringVec2[1]);
		z = std::stof(stringVec2[2]);
	}

	void Vec3::SetFromTextWithSpace(const char* text)
	{
		Strings stringVec2 = SplitStringOnDelimiter(text, ' ');
		x = std::stof(stringVec2[0]);
		y = std::stof(stringVec2[1]);
		z = std::stof(stringVec2[2]);
	}

	void Vec3::SetFromTextWithDelimiter(const char* text,char delimiter )
	{
		Strings stringVec2 = SplitStringOnDelimiter(text, delimiter);
		x = std::stof(stringVec2[0]);
		y = std::stof(stringVec2[1]);
		z = std::stof(stringVec2[2]);
	}

	float Vec3::GetLength() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	float Vec3::GetLengthXY() const
	{
		return sqrtf(x * x + y * y);
	}

	float Vec3::GetLengthXYSquared() const
	{
		return x * x + y * y;
	}

	float Vec3::GetLengthSquared() const
	{
		return x * x + y * y + z * z;
	}

	float Vec3::GetAngleAboutZRadians() const
	{
		return atan2f(y, x);
	}

	float Vec3::GetAngleAboutZDegrees() const
	{
		return Atan2Degrees(y, x);
	}

	Vec3 const Vec3::GetRotatedAboutZRadians(float deltaRadians) const
	{
		float radians = atan2f(y, x);
		float radius = GetLengthXY();
		radians += deltaRadians;
		return Vec3(radius * cosf(radians), radius * sinf(radians), z);
	}

	Vec3 const Vec3::GetRotatedAboutZDegrees(float deltaDegrees) const
	{
		float degrees = Atan2Degrees(y, x);
		float radius = GetLengthXY();
		degrees += deltaDegrees;
		return Vec3(radius * CosDegrees(degrees), radius * SinDegrees(degrees), z);
	
	}

	Vec3 const Vec3::GetClamped(float maxLength) const
	{
		Mat44 rotationMatrix;
		float length = GetLength();
		if (length > maxLength)
		{
			return Vec3(x,y,z) * (maxLength /length);
		}
		else return Vec3();
	}

	Vec3 const Vec3::GetNormalized() const
	{
		float scale = 1.0f / GetLength();
		return Vec3(x * scale, y * scale, z* scale);
	}

	bool Vec3::operator==(Vec3 const& compare) const
	{
		if (x == compare.x && y == compare.y && z == compare.z)
		{
			return true;
		}
		else
			return false;
	}

	bool Vec3::operator!=(Vec3 const& compare) const
	{
		if (x != compare.x || y != compare.y || z != compare.z)
		{
			return true;
		}
		else
			return false;
	}

	bool Vec3::operator<(Vec3 const& compare) const
	{
		if (x > compare.x || y > compare.y || z > compare.z)
		{
			return false;
		}
		else
			return true;
	}

	bool Vec3::operator>(Vec3 const& compare) const
	{
		if (x < compare.x || y < compare.y || z < compare.z)
		{
			return false;
		}
		else
			return true;
	}

	Vec3 const Vec3::operator+(Vec3 const& VecToAdd) const
	{
		return Vec3(x + VecToAdd.x, y + VecToAdd.y, z + VecToAdd.z);
	}

	Vec3 const Vec3::operator-(Vec3 const& VecToSubtract) const
	{
		return Vec3(x - VecToSubtract.x, y - VecToSubtract.y, z - VecToSubtract.z);
	}

	Vec3 const Vec3::operator*(float uniformScale) const
	{
		return Vec3(x * uniformScale, y * uniformScale, z * uniformScale);
	}

	const Vec3 Vec3::operator/(float inverseScale) const
	{
		return Vec3(x / inverseScale, y / inverseScale, z/inverseScale);
	}

	void Vec3::operator+=(const Vec3& vecToAdd)
	{
		x += vecToAdd.x;
		y += vecToAdd.y;
		z += vecToAdd.z;
	}

	void Vec3::operator-=(const Vec3& vecToSubtract)
	{
		x -= vecToSubtract.x;
		y -= vecToSubtract.y;
		z -= vecToSubtract.z;
	}

	void Vec3::operator*=(const float uniformScale)
	{
		x *= uniformScale;
		y *= uniformScale;
		z *= uniformScale;
	}

	const Vec3 Vec3::operator*(const Vec3& vecToMultiply) const
	{
		return Vec3(x * vecToMultiply.x, y * vecToMultiply.y, z * vecToMultiply.z);
	}

	void Vec3::operator/=(const float uniformDivisor)
	{
		x /= uniformDivisor;
		y /= uniformDivisor;
		z /= uniformDivisor;
	}

	void Vec3::operator=(const Vec3& copyFrom)
	{
		x = copyFrom.x;
		y = copyFrom.y;
		z = copyFrom.z;
	}

	void Vec3::operator+=(const float& floatToAdd)
	{
		x += floatToAdd;
		y += floatToAdd;
		z += floatToAdd;
	}

	void Vec3::operator-=(const float& floatToSubtract)
	{
		x -= floatToSubtract;
		y -= floatToSubtract;
		z -= floatToSubtract;
	}

	const Vec3 Vec3::operator-() const
	{
		return Vec3(-x, -y,-z);
	}


	Vec3 const operator*(float uniformScale, Vec3 const& VecToScale)
	{
		return VecToScale * uniformScale;
	}

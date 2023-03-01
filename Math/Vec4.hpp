#pragma once
struct Vec3;
struct Vec4 {
public:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 0.f;
	Vec4() {};
	Vec4(Vec4 const& copyFrom);
	Vec4(Vec3 const& copyFrom);
	Vec4(Vec3 const& copyFrom, float const& wValue);
	Vec4(float x, float y, float z, float w);



	//Accessors
	float GetLength() const;
	float GetLengthXY() const;
	float GetLengthXYSquared() const;
	float GetLengthSquared() const;
	float GetAngleAboutZRadians() const;
	float GetAngleAboutZDegrees() const;
	Vec4 const GetNormalized() const;

	//operators (Self Mutating Non Const)	
	bool		operator==(Vec4 const& compare) const;
	bool		operator!=(Vec4 const& compare) const;
	Vec4 const	operator+(Vec4 const& VecToAdd) const;
	Vec4 const	operator-(Vec4 const& VecToSubtract) const;
	Vec4 const  operator*(float uniformScale) const;
	const Vec4	operator/(float inverseScale) const;			

	//Operators (self-mutating / non-const)
	void		operator+=(const Vec4& vecToAdd);				
	void		operator-=(const Vec4& vecToSubtract);		
	void		operator*=(const float uniformScale);		
	void		operator/=(const float uniformDivisor);		
	void		operator=(const Vec4& copyFrom);				

	//Standalone friend functions
	friend Vec4 const operator*(float uniformScale, Vec4 const& VecToScale);
};
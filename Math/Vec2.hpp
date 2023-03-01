#pragma once
#include "Engine/Math/IntVec2.hpp"
struct Vec3;
//-----------------------------------------------------------------------------------------------
struct Vec2
{
public: // NOTE: this is one of the few cases where we break both the "m_" naming rule AND the avoid-public-members rule
	float x = 0.f;
	float y = 0.f;
	const static Vec2 ZERO;
	const static Vec2 ONE;
public:
	// Construction/Destruction
	~Vec2() {}												// destructor (do nothing)
	Vec2() {}												// default constructor (do nothing)
	Vec2( const Vec2& copyFrom );							// copy constructor (from another vec2)
	explicit Vec2( float initialX, float initialY );		// explicit constructor (from x, y)
	explicit Vec2(const IntVec2& copyFrom);
	explicit Vec2(const Vec3& copyFrom);

	static Vec2 const MakeFromPolarRadians(float orientationRadians, float length = 1.0f);
	static Vec2 const MakeFromPolarDegrees(float orientationDegrees, float length = 1.0f);
	//Accessors const methods
	
	float GetLength() const;
	float GetLengthSquared() const;
	float GetOrientationRadians() const;
	float GetOrientationDegrees() const;
	Vec2 const GetRotated90Degrees() const;
	Vec2 const GetRotatedMinus90Degrees() const;
	Vec2 const GetRotatedRadians(float deltaRadians) const;
	Vec2 const GetRotatedDegrees(float deltaDegrees) const;
	Vec2 const GetClamped(float maxLength) const;
	Vec2 const GetNormalized() const;

	//Mutators non const methods
	void SetOrientationRadians(float newOrientationRadians);
	void SetOrientationDegrees(float newOrientationDegrees);
	void SetPolarRadians(float newOrientationRadians, float newLength);
	void SetPolarDegrees(float newOrientationDegrees, float newLength);
	void Rotate90Degrees();
	void RotateMinus90Degrees();
	void RotateRadians(float deltaRadians);
	void RotateDegrees(float deltaDegrees);
	void SetLength(float newLength);
	void ClampLength(float maxLength);
	void Normalize();
	float NormalizeAndGetPreviousLength();


	// Operators (const)
	bool		operator==( const Vec2& compare ) const;		// vec2 == vec2
	bool		operator!=( const Vec2& compare ) const;		// vec2 != vec2
	const Vec2	operator+( const Vec2& vecToAdd ) const;		// vec2 + vec2
	const Vec2	operator-( const Vec2& vecToSubtract ) const;	// vec2 - vec2
	const Vec2	operator-(const float floatToSubtract) const;	// vec2 - float
	const Vec2	operator+(const float floatToAdd) const;	// vec2 + float
	const Vec2	operator-() const;								// -vec2, i.e. "unary negation"
	const Vec2	operator*( float uniformScale ) const;			// vec2 * float
	const Vec2	operator*( const Vec2& vecToMultiply ) const;	// vec2 * vec2
	const Vec2	operator/( float inverseScale ) const;			// vec2 / float

	// Operators (self-mutating / non-const)
	void		operator+=( const Vec2& vecToAdd );				// vec2 += vec2
	void		operator-=( const Vec2& vecToSubtract );		// vec2 -= vec2
	void		operator*=( const float uniformScale );			// vec2 *= float
	void		operator/=( const float uniformDivisor );		// vec2 /= float
	void		operator=( const Vec2& copyFrom );				// vec2 = vec2




	void		SetFromText(const char* text);
	void		SetFromText(const char* text, char delim);
	void		Reflect(const Vec2& impactSurfacenormal);
	Vec2		GetReflected(Vec2 impactSurfacenormal)			 const;
	// Standalone "friend" functions that are conceptually, but not actually, part of Vec2::
	friend const Vec2 operator*( float uniformScale, const Vec2& vecToScale );	// float * vec2
};



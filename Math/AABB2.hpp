#pragma once
#include "Engine/Core/Vertex_PCU.hpp"

struct AABB2
{

public:
	Vec2 m_mins;
	Vec2 m_maxs;

public:
	const static AABB2 ZERO_TO_ONE;
	~AABB2();
	AABB2() {}
	AABB2(AABB2 const& copyForm);
	explicit AABB2(float minX, float minY, float maxX, float maxY);
	explicit AABB2(Vec2 const& mins, Vec2 const& maxs);
	explicit AABB2(IntVec2 const& mins, IntVec2 const& maxs);

	//Accessors
	bool IsPointInside(Vec2 const& point) const;
	Vec2 GetCenter() const;
	Vec2 GetDimensions() const;
	Vec2 GetNearestPoint(Vec2 const& referencePoint) const;
	Vec2 GetPointAtUV(Vec2 const& uv) const;
	Vec2 GetUVForPoint(Vec2 const& point) const;
	AABB2 GetFractionalAABB2(Vec2 const& fractionMins, Vec2 const& fractionMaxs) const;

	// Mutators 
	void Translate(Vec2 const& translation);
	void SetCenter(Vec2 const& newCenter);
	void SetDimensions(Vec2 const& newDimensions);
	void StretchToIncludePoint(Vec2 const& point);
	void		operator+(const AABB2& AABBtoAdd);

};


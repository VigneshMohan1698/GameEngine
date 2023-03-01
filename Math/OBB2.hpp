#pragma once
#include "Engine/Math/Vec2.hpp"
class OBB2
{
public:
	OBB2();
	OBB2(Vec2 center, Vec2 normal, Vec2 halfDimensions);
	~OBB2();
	Vec2				m_center;
	Vec2				m_iBasisNormal;
	Vec2				m_halfDimensions;

	void				GetCornerPoints(Vec2* out_fourCornerWorldPositios) const;
	Vec2				GetLocalPosForWorldPos(Vec2 worldPos) const;
	Vec2				GetWorldPosForLocalPos(Vec2 localPos) const;
	void				RotateAboutCenter(float rotationDeltaDegrees);
};


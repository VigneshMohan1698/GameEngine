#pragma  once
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Texture.hpp"

class SpriteSheet;
class SpriteDefinition
{
public:
	explicit SpriteDefinition(SpriteSheet const& spriteSheet, int spriteIndex, Vec2 const& uvAtmins, Vec2 const& uvAtMaxs);
	void							GetUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs) const;
	AABB2							GetUVs() const;
	SpriteSheet const&				GetSpriteSheet() const;
	Texture&						GetTexture() const;
	float							GetAspect() const;
	int								m_spriteIndex = -1;
protected:
	SpriteSheet const&				m_spriteSheet;

	Vec2							m_uvAtMins = Vec2::ZERO;
	Vec2							m_uvAtMaxs = Vec2::ONE;

};
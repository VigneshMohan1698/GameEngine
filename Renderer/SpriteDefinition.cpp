#include "SpriteDefinition.hpp"
#include "SpriteSheet.hpp"

SpriteDefinition::SpriteDefinition(SpriteSheet const& spriteSheet, int spriteIndex, Vec2 const& uvAtmins, Vec2 const& uvAtMaxs): m_spriteSheet(spriteSheet)
{
	m_spriteIndex = spriteIndex;
	m_uvAtMaxs = uvAtMaxs;
	m_uvAtMins = uvAtmins;
}

void SpriteDefinition::GetUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs) const
{
	out_uvAtMaxs = m_uvAtMaxs;
	out_uvAtMins = m_uvAtMins;
}

AABB2 SpriteDefinition::GetUVs() const
{
	return AABB2(m_uvAtMins,m_uvAtMaxs);
}

SpriteSheet const& SpriteDefinition::GetSpriteSheet() const
{
	return m_spriteSheet;
}

//Texture& SpriteDefinition::GetTexture() const
//{
//	return m_spriteSheet.GetTexture();
//}

float SpriteDefinition::GetAspect() const
{
	IntVec2 textureDimesnions = m_spriteSheet.GetTexture().GetDimensions();
	Vec2 UVsize = m_uvAtMaxs - m_uvAtMins;
	float width = textureDimesnions.x * UVsize.x;
	float length = textureDimesnions.y * UVsize.y;
	return width / length;
}

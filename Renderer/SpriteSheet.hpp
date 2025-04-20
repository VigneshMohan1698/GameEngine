#pragma once
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/TextureD12.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include <vector>

class SpriteSheet
{
public: 
   explicit SpriteSheet(IntVec2 const&  textureDimensions, IntVec2 const& simpleGridLayout);
   explicit SpriteSheet(IntVec2 const&  textureDimensions, IntVec2 const& simpleGridLayout, bool textureLoader);

	int								GetNumSprites() const;
	SpriteDefinition const&			GetSpriteDef(int spriteIndex) const;
	SpriteDefinition const&			GetSpriteDef(Vec2 spriteCell, IntVec2 const& simpleGridLayout) const;
	void							GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, int spriteIndex) const;
	AABB2							GetSpriteUVs(int spriteIndex) const;
public: 
	IntVec2							m_textureDimensions;
protected:
	std::vector<SpriteDefinition>	m_spriteDefs;
	
};


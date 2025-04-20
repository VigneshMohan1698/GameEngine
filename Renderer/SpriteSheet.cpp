#include "SpriteSheet.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/EngineCommon.hpp"



SpriteSheet::SpriteSheet(IntVec2 const& textureDimensions, IntVec2 const& simpleGridLayout)
{
	int index = 0;
	for (float yIncrement = 1; yIncrement > 0;)
	{
		for (float xIncrement = 0; xIncrement < 1;)
		{
			float xInc = ((float)1 / simpleGridLayout.x) + (1 / (textureDimensions.x * 100));
			float yInc = ((float)1 / simpleGridLayout.y) + (1 / (textureDimensions.y * 100));
			m_spriteDefs.push_back(SpriteDefinition(*this, index, Vec2(xIncrement, yIncrement), Vec2(xIncrement + xInc, yIncrement - yInc)));
			xIncrement += xInc;
			index++;
		}
		yIncrement -= (float)1 / simpleGridLayout.y;
	}


}


SpriteSheet::SpriteSheet(IntVec2 const& textureDimensions, IntVec2 const& simpleGridLayout, bool textureLoader) 
{
	textureLoader = false;
	m_textureDimensions = textureDimensions;
	//int index = 0;
	//for (float yIncrement = 1; yIncrement > 0;)
	//{
	//	for (float xIncrement = 0; xIncrement < 1;)
	//	{
	//		float xInc = ((float)1 / simpleGridLayout.x) + (1 / (textureDimensions.x * 100));
	//		float yInc = ((float)1 / simpleGridLayout.y) + (1 / (textureDimensions.y * 100));
	//		m_spriteDefs.push_back(SpriteDefinition(*this, index, Vec2(xIncrement, yIncrement), Vec2(xIncrement + xInc, yIncrement - yInc)));
	//		xIncrement += xInc;
	//		index++;
	//	}
	//	yIncrement -= (float)1 / simpleGridLayout.y;
	//}

	int index = 0;
	for (float yIncrement = 0; yIncrement < 1;)
	{
		for (float xIncrement = 0; xIncrement < 1;)
		{
			float xInc = ((float)1 / simpleGridLayout.x) + (1 / (textureDimensions.x * 100));
			float yInc = ((float)1 / simpleGridLayout.y) + (1 / (textureDimensions.y * 100));
			m_spriteDefs.push_back(SpriteDefinition(*this, index, Vec2(xIncrement, yIncrement), Vec2(xIncrement + xInc, yIncrement - yInc)));
			xIncrement += xInc;
			index++;
		}
		yIncrement += (float)1 / simpleGridLayout.y;
	}

}

int SpriteSheet::GetNumSprites() const
{
	return (int)m_spriteDefs.size();
}

SpriteDefinition const& SpriteSheet::GetSpriteDef(int spriteIndex) const
{
	return m_spriteDefs[spriteIndex];
}

SpriteDefinition const& SpriteSheet::GetSpriteDef(Vec2 spriteCell,IntVec2 const& simpleGridLayout) const
{
	int x = (int)spriteCell.x;
	int y = (int)spriteCell.y;
	int spriteIndex = y * simpleGridLayout.y + x;
	return m_spriteDefs[spriteIndex];
}
void SpriteSheet::GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, int spriteIndex) const
{
	UNUSED((void)out_uvAtMins);
	UNUSED((void)out_uvAtMaxs);
	UNUSED((void)spriteIndex);
}

AABB2 SpriteSheet::GetSpriteUVs(int spriteIndex) const
{
	AABB2 returnSpriteUVs = m_spriteDefs[spriteIndex].GetUVs();
	return returnSpriteUVs;
}
	

#include "BitmapFont.hpp"
#include "Engine/Math/VertexUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include <Engine/Math/MathUtils.hpp>
#include "Engine/Core/ErrorWarningAssert.hpp"

extern Renderer* g_theRenderer;


BitmapFont::BitmapFont(char const* fontFilePathNameWithNoExtension, Texture& fontTexture):
	m_fontFilePathNameWithNoExtension(fontFilePathNameWithNoExtension),
	m_fontGlyphsSpriteSheet(fontTexture, IntVec2(16, 16))
{

}

BitmapFont::BitmapFont(char const* fontFilePathNameWithNoExtension, Texture& fontTexture, IntVec2 const& textureDimensions):
m_fontFilePathNameWithNoExtension(fontFilePathNameWithNoExtension),
m_fontGlyphsSpriteSheet(fontTexture, textureDimensions,  IntVec2(16, 16))
{

}

const Texture& BitmapFont::GetTexture() const
{
	return m_fontGlyphsSpriteSheet.GetTexture();
}

void BitmapFont::AddVertsForText2D(std::vector<Vertex_PCU>& vertexArray, Vec2 const& textMins, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspect, float letterWidth)
{
	AABB2 uvBounds;
	Vec2 letterPosition = textMins;
	for (int i = 0; i < text.length(); i++)
	{
		letterPosition.x += letterWidth;
		uvBounds = m_fontGlyphsSpriteSheet.GetSpriteUVs(text[i]);
		AddVertsForTexts2D(vertexArray, letterPosition, uvBounds, cellHeight, text, tint, cellAspect);
		
	}
	
}

float BitmapFont::GetTextWidth(float cellHeight, std::string const& text, float cellAspect)
{
	float textWidth = 0.0f;

	float cellWidth = cellHeight * cellAspect;
	for (int i = 0; i < text.length(); i++)
	{
		textWidth = textWidth + cellWidth;
	}
	return textWidth;
}

float BitmapFont::GetTextHeight(float cellHeight, std::string const& text, float cellAspect)
{
	UNUSED((void)cellAspect);
	float textHeight = 0.0f;
	Strings allstrings = SplitStringOnDelimiter(text, '\n');
	for (int i = 0; i < allstrings.size(); i++)
	{
		textHeight += cellHeight;
	}
	return textHeight;
}

void BitmapFont::AddVertsForTextInBox2D(std::vector<Vertex_PCU>& vertexArray, AABB2 const& box, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspect, Vec2 const& alignment, TextBoxMode mode, int maxGlyphsToDraw, float widthBetweenLetters)
{
	UNUSED((void)widthBetweenLetters);
	float maxTextWidth = 0.0f;
	Strings textLines = SplitStringOnDelimiter(text, '\n');

	for (int i = 0; i < textLines.size(); i++)
	{
		float textWidth = GetTextWidth(cellHeight, textLines[i], cellAspect);
		if (textWidth > maxTextWidth)
		{
			maxTextWidth = textWidth;
		}
	}

	float textHeight = GetTextHeight(cellHeight, text, cellAspect);
	AABB2 uvBounds;
	AABB2 textBounds;
	Vec2 letterStartPosition;
	
	float boxDimensionsX = box.m_maxs.x - box.m_mins.x;
	float boxDimensionsY = box.m_maxs.y - box.m_mins.y;

	float leftPadding = 0.0f;
	float bottomPadding = 0.0f;

	if (maxTextWidth < boxDimensionsX)
	{
		leftPadding = (boxDimensionsX - maxTextWidth) * alignment.x;
	}
	if (textHeight < boxDimensionsY)
	{
		bottomPadding = (boxDimensionsY - textHeight) * alignment.y;
	}

	letterStartPosition = Vec2(box.m_mins.x + leftPadding + (cellHeight * cellAspect), box.m_mins.y + bottomPadding + textHeight - cellHeight);

	if (mode == TextBoxMode::SHRINK_TO_FIT)
	{
		float shrinkFactorX = 0.0f;
		float shrinkFactorY = 0.0f;

		if (maxTextWidth > boxDimensionsX - leftPadding)
		{
			shrinkFactorX = maxTextWidth / (boxDimensionsX - leftPadding);
		}
		if (textHeight > boxDimensionsY - bottomPadding)
		{
			shrinkFactorY = textHeight / (boxDimensionsY - bottomPadding);
		}

		if (shrinkFactorX >= shrinkFactorY && shrinkFactorX != 0)
		{
			cellHeight /= shrinkFactorX;
		}
		else if (shrinkFactorY >= shrinkFactorX && shrinkFactorY != 0)
		{
			cellHeight /= shrinkFactorY;
		}
	}
	
	for (int j = 0; j < textLines.size(); j++)
	{
		for (int i = 0; i < textLines[j].length(); i++)
		{
			if (i < maxGlyphsToDraw)
			{
				uvBounds = m_fontGlyphsSpriteSheet.GetSpriteUVs(textLines[j][i]);
				AddVertsForTexts2D(vertexArray, letterStartPosition, uvBounds, cellHeight, textLines[j], tint, cellAspect);
				letterStartPosition.x += (cellHeight * cellAspect);
			}
		}
		letterStartPosition.x = box.m_mins.x + leftPadding;
		letterStartPosition.y -= cellHeight;

	}

}

float BitmapFont::GetGlyphAspect(int glyphUnicode) const
{
	UNUSED(void(glyphUnicode));
    return 1.0f;
}

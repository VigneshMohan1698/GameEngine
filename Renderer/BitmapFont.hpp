#pragma once
#include "Engine/Renderer/Texture.hpp"
#include <vector>
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Core/Rgba8.hpp"


enum class TextBoxMode
{
	SHRINK_TO_FIT,
	OVERRUN
};
class BitmapFont
{
	friend class Renderer;
	friend class RendererD12;

private:
	BitmapFont(char const* fontFilePathNameWithNoExtension, Texture& fontTexture);
	BitmapFont(char const* fontFilePathNameWithNoExtension, Texture& fontTexture, IntVec2 const& textureDimensions);

public:
	const Texture& GetTexture() const;

	void AddVertsForText2D(std::vector<Vertex_PCU>& vertexArray, Vec2 const& textMins,
		float cellHeight, std::string const& text, Rgba8 const& tint = Rgba8::WHITE, float cellAspect = 1.f, float letterWidth =1.0f);
	float GetTextWidth(float cellHeight, std::string const& text, float cellAspect = 1.f);
	float GetTextHeight(float cellHeight, std::string const& text, float cellAspect);
	void AddVertsForTextInBox2D(std::vector<Vertex_PCU>& vertexArray, AABB2 const& box, float cellHeight,
		std::string const& text, Rgba8 const& tint = Rgba8::WHITE, float cellAspect = 1.f,
		Vec2 const& alignment = Vec2(.5f, .5f), TextBoxMode mode = TextBoxMode::SHRINK_TO_FIT, int maxGlyphsToDraw = 999999, float widthBetweenLetters = 0.0f);

protected:
	float GetGlyphAspect(int glyphUnicode) const; // For now this will always return 1.0f!!!

protected:
	std::string	m_fontFilePathNameWithNoExtension;
	SpriteSheet	m_fontGlyphsSpriteSheet;
};


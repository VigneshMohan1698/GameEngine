
#pragma once
#include "ThirdParty/stb/stb_image.h"
#include <string>
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Core/Rgba8.hpp"
#include <vector>
class Image
{
public:
	Image();
	~Image();
	Image(IntVec2 size, Rgba8 color);
	Image(char const* imageFilePath);
	void						StoreTexelData(unsigned char* texelData, int bytesPerTexel);
	std::string const&			GetImageFilePath() const;
	IntVec2						GetDimensions() const;
	Rgba8						GetTexelColor(IntVec2 const& texelCoords) const;
	Rgba8						GetTexelColorRowMajor(IntVec2 const& texelCoords) const;
	void						SetTexelColor(IntVec2 const& texelCoords, Rgba8 const& newColor);
	const void*					GetRawData() const;

private:
	std::string						m_imageFilePath;
	IntVec2							m_dimensions = IntVec2(0, 0);
	std::vector<Rgba8>				m_rgbaTexels;
};



#define STB_IMAGE_IMPLEMENTATION 
#include "Image.hpp"
#include "Engine/Core/EngineCommon.hpp"

Image::Image()
{
}

Image::~Image()
{
}

Image::Image(IntVec2 size, Rgba8 color)
{
	int imageSize = size.x * size.y;

	for (int i = 0; i < imageSize; i++)
	{
		m_rgbaTexels.push_back(color);
	}
	m_dimensions = size;
}

Image::Image(char const* imageFilePath)
{
    m_imageFilePath = imageFilePath;

	IntVec2 dimensions = IntVec2::ZERO;		// This will be filled in for us to indicate image width & height
	int bytesPerTexel = 0; // This will be filled in for us to indicate how many color components the image had (e.g. 3=RGB=24bit, 4=RGBA=32bit)
	int numComponentsRequested = 0; // don't care; we support 3 (24-bit RGB) or 4 (32-bit RGBA)

									// Load (and decompress) the image RGB(A) bytes from a file on disk into a memory buffer (array of bytes)
	stbi_set_flip_vertically_on_load(1); // We prefer uvTexCoords has origin (0,0) at BOTTOM LEFT
	unsigned char* texelData = stbi_load(imageFilePath, &dimensions.x, &dimensions.y, &bytesPerTexel, numComponentsRequested);

	m_dimensions = dimensions;
	StoreTexelData(texelData, bytesPerTexel);
}
void Image::StoreTexelData(unsigned char* texelData, int bytesPerTexel)
{
	m_rgbaTexels.reserve(m_dimensions.x * m_dimensions.y);
	int imageSize = m_dimensions.x * m_dimensions.y;

	for (int i = 0; i < imageSize; i++)
	{
		int pointerIndex = (i * bytesPerTexel);
		unsigned char r = *(texelData + pointerIndex);
		unsigned char g = *(texelData + pointerIndex + 1);
		unsigned char b = *(texelData + pointerIndex + 2);

		if (g != 0 && r ==0)
		{
			g = g;
		}
		unsigned char a = 255;
		if (bytesPerTexel == 4)
			a = *(texelData + pointerIndex + 3);

		m_rgbaTexels.push_back(Rgba8(r, g, b, a));
	}
	stbi_image_free(texelData);
}
std::string const& Image::GetImageFilePath() const
{
    return m_imageFilePath;
}

IntVec2 Image::GetDimensions() const
{
    return m_dimensions;
}

Rgba8 Image::GetTexelColor(IntVec2 const& texelCoords) const
{
	Rgba8 returnValue;
	int index = 0;
	for (int i = 0; i < m_dimensions.x; i++)
	{
		for (int j = 0; j < m_dimensions.y; j++)
		{
			if (i == texelCoords.x && j == texelCoords.y)
			{
				returnValue = m_rgbaTexels[index];
			}
			index++;
		}
	}
	return returnValue;
}

Rgba8 Image::GetTexelColorRowMajor(IntVec2 const& texelCoords) const
{
	Rgba8 returnValue;
	int index = 0;
	for (int j = 0; j < m_dimensions.y; j++)
	{
		for (int i = 0; i < m_dimensions.x; i++)
		{
			if (i == texelCoords.x && j == texelCoords.y)
			{
				returnValue = m_rgbaTexels[index];
			}
			index++;
		}
	}
	return returnValue;
}

void Image::SetTexelColor(IntVec2 const& texelCoords, Rgba8 const& newColor)
{
	UNUSED((void)texelCoords);
	UNUSED((void)newColor);
}

const void* Image::GetRawData() const
{
	return m_rgbaTexels.data();
}

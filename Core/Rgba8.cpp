#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <Engine/Math/MathUtils.hpp>


const Rgba8 Rgba8::WHITE = Rgba8(255,255,255,255);
const Rgba8 Rgba8::GREY = Rgba8(200,200,200,255);
const Rgba8 Rgba8::BLACK = Rgba8(0,0,0,255);
const Rgba8 Rgba8::NOTHING = Rgba8(0,0,0,0);
const Rgba8 Rgba8::BLACKLOWALPHA = Rgba8(0,0,0,150);
const Rgba8 Rgba8::RED = Rgba8(255,0,0,255);
const Rgba8 Rgba8::LIGHTRED = Rgba8(150,0,0,255);
const Rgba8 Rgba8::GREEN = Rgba8(0, 255,0,255);
const Rgba8 Rgba8::LIGHTGREEN = Rgba8(0, 150,0,255);
const Rgba8 Rgba8::BLUE = Rgba8(0,0, 255,255);
const Rgba8 Rgba8::YELLOW = Rgba8(255, 255, 0, 255);
const Rgba8 Rgba8::CYAN = Rgba8(0, 255, 255, 255);
const Rgba8 Rgba8::MAGENTA = Rgba8(255, 0, 255, 255);
const Rgba8 Rgba8::LIGHTBLUE = Rgba8(135, 206, 235, 255);
const Rgba8 Rgba8::SKYBLUE = Rgba8(8, 143, 143, 255);
const Rgba8 Rgba8::DARKBLUE = Rgba8(8, 143, 200, 255);
const Rgba8 Rgba8::GOLDBROWN = Rgba8(153, 101, 21, 255);
const Rgba8 Rgba8::DARKERBLUE = Rgba8(8, 50, 255, 255);
const Rgba8 Rgba8::ORANGE = Rgba8(242, 151, 53, 255);

Rgba8::Rgba8(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Rgba8::Rgba8(unsigned char r, unsigned char g, unsigned char b)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = (unsigned char)255;
}

bool Rgba8::operator==(const Rgba8& compare) const
{
	if (this->r == compare.r && this->g == compare.g && this->b == compare.b && this->a == compare.a)
	{
		return  true;
	}
	else
		return false;
}


Rgba8 const Rgba8::operator+(Rgba8 const& rgb) const
{
	return Rgba8(r + rgb.r, g+ rgb.g, b+ rgb.b, a + rgb.a);
}

void Rgba8::SetFromText(const char* text)
{
	int count = 0;
	for (int i = 0; i < strlen(text); i++)
	{
		if (text[i] == ',')
		{
			count++;
		}
	}
	if (count == 2)
	{
		Strings rgbaText = SplitStringOnDelimiter(text, ',');
		r = (unsigned char)atof(rgbaText[0].c_str());
		g = (unsigned char)atof(rgbaText[1].c_str());
		b = (unsigned char)atof(rgbaText[2].c_str());
		a = (unsigned char)255;
	}
	else if (count == 3)
	{
		Strings rgbaText = SplitStringOnDelimiter(text, ',');
		r = (unsigned char)atof(rgbaText[0].c_str());
		g = (unsigned char)atof(rgbaText[1].c_str());
		b = (unsigned char)atof(rgbaText[2].c_str());
		a = (unsigned char)atof(rgbaText[3].c_str());
	}
	
}
void Rgba8::GetAsFloats(float* colorAsFloats) const
{
	colorAsFloats[0] = RangeMap(r, 0, 255, 0, 1);
	colorAsFloats[1] = RangeMap(g, 0, 255, 0, 1);
	colorAsFloats[2] = RangeMap(b, 0, 255, 0, 1);
	colorAsFloats[3] = RangeMap(a, 0, 255, 0, 1);
}


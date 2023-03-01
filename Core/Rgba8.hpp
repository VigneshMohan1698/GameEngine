#pragma once
struct Rgba8
{
	public:
		unsigned char r = 255;
		unsigned char g = 255;
		unsigned char b = 255;
		unsigned char a = 255;
	public:
		const static Rgba8 WHITE;
		const static Rgba8 GREY;
		const static Rgba8 BLACK;
		const static Rgba8 NOTHING;
		const static Rgba8 BLACKLOWALPHA;
		const static Rgba8 RED;
		const static Rgba8 LIGHTRED;
		const static Rgba8 GREEN;
		const static Rgba8 LIGHTGREEN;
		const static Rgba8 BLUE;
		const static Rgba8 YELLOW;
		const static Rgba8 CYAN;
		const static Rgba8 MAGENTA;
		const static Rgba8 LIGHTBLUE;
		const static Rgba8 SKYBLUE;
		const static Rgba8 DARKBLUE;
		const static Rgba8 GOLDBROWN;
		const static Rgba8 DARKERBLUE;
		const static Rgba8 ORANGE;
		const static Rgba8 SILVER;
		Rgba8() {}
		explicit Rgba8(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

		Rgba8(unsigned char r, unsigned char g, unsigned char b);

		void 	    GetAsFloats(float* colorAsFloats) const;
		bool		operator==(const Rgba8& compare) const;		
		Rgba8 const operator+(Rgba8 const& rgb) const;
		void		SetFromText(const char* text);
};
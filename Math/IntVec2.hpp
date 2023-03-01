#pragma once
struct Vec2;
struct IntVec2
{
public:
	int x = 0;
	int y = 0;
	

	static IntVec2 const	ZERO;
	static IntVec2 const	ONE;

public:
	~IntVec2() {}
	IntVec2() {}
	IntVec2(IntVec2 const& copyFrom);
	IntVec2(Vec2 const& copyFrom);
	explicit IntVec2(int initialX, int initialY);

	//Accessors
	float GetLength() const;
	int	  GetLengthSquared() const;

	int	  GetTaxicabLength() const;
	IntVec2 const GetRotated90Degrees() const;
	IntVec2 const GetRotatedMinus90Degrees() const;
	float GetOrientationDegrees() const;
	float GetOrientationRadians() const;

	//Mutators
	void Rotate90Degrees();
	void RotateMinus90Degrees();
	void  SetFromTextWithDelimiter(const char* text, char Delimiter);
	bool	operator==(const IntVec2& compare) const;
	bool	operator!=(const IntVec2& compare) const;
	IntVec2	operator+(const IntVec2& IntVec2ToAdd);
	IntVec2	operator-(const IntVec2& IntVec2ToSubtract);
	void SetFromText(const char* text);

};


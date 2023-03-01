#include "IntVec2.hpp"
#include "Vec2.hpp"
#include <math.h>
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"


const IntVec2 IntVec2::ZERO = IntVec2(0, 0);
const IntVec2 IntVec2::ONE = IntVec2(1, 1);

IntVec2::IntVec2(IntVec2 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}

IntVec2::IntVec2(int initialX, int initialY)
{
	x = initialX;
	y = initialY;
}
IntVec2::IntVec2(Vec2 const& copyFrom)
{
	x = RoundDownToInt(copyFrom.x);
	y = RoundDownToInt(copyFrom.y);
}
float IntVec2::GetLength() const
{
    return (float)sqrtf((float)x * x + y * y);
}

int IntVec2::GetLengthSquared() const
{
	return (x * x + y * y);
}
void IntVec2::SetFromTextWithDelimiter(const char* text, char Delimiter)
{
	Strings stringVec3 = SplitStringOnDelimiter(text, Delimiter);
	x = std::stoi(stringVec3[0]);
	y = std::stoi(stringVec3[1]);
}
int IntVec2::GetTaxicabLength() const
{
    return abs(x) + abs(y);
}

IntVec2 const IntVec2::GetRotated90Degrees() const
{
    return IntVec2(-y, x);
}

IntVec2 const IntVec2::GetRotatedMinus90Degrees() const
{
	return IntVec2(y, -x);
}

float IntVec2::GetOrientationDegrees() const
{
	return Atan2Degrees((float)y, (float)x);
}

float IntVec2::GetOrientationRadians() const
{
	return atan2f((float)y, (float)x);
}

void IntVec2::Rotate90Degrees()
{
	IntVec2 rotatatedVec = IntVec2(-y, x);
	x = rotatatedVec.x;
	y = rotatatedVec.y;
}

void IntVec2::RotateMinus90Degrees()
{
	IntVec2 rotatatedVec = IntVec2(y, -x);
	x = rotatatedVec.x;
	y = rotatatedVec.y;
}

bool IntVec2::operator==(const IntVec2& compare) const
{
	if (compare.x == x && compare.y == y)
	{
		return true;
	}
	return false;
}

bool IntVec2::operator!=(const IntVec2& compare) const
{
	if (compare.x == x && compare.y == y)
	{
		return false;
	}
	return true;
}
IntVec2	IntVec2::operator+(const IntVec2& IntVec2ToAdd)
{
	
	return IntVec2(IntVec2ToAdd.x + x, IntVec2ToAdd.y + y);
}
IntVec2	IntVec2::operator-(const IntVec2& IntVec2ToSubtract)
{
	return IntVec2(x - IntVec2ToSubtract.x, y - IntVec2ToSubtract.y);
}
void IntVec2::SetFromText(const char* text)
{
	Strings stringVec2 = SplitStringOnDelimiter(text, ',');
	x = std::stoi(stringVec2[0]);
	y = std::stoi(stringVec2[1]);
}

#include "FloatRange.hpp"
#include <Engine/Core/StringUtils.hpp>


const FloatRange FloatRange::ZERO = FloatRange(0.0f, 0.0f);
const FloatRange FloatRange::ONE = FloatRange(1.0f, 1.0f);
const FloatRange FloatRange::ZERO_TO_ONE = FloatRange(0.0f, 1.0f);

FloatRange::FloatRange()
{
	m_min = 0;
	m_max = 0;
}

FloatRange::FloatRange(float min, float max)
{
	m_min = min;
	m_max = max;
}

bool FloatRange::operator==(const FloatRange& compare) const
{
	return m_min == compare.m_min && m_max == compare.m_max;
}

void FloatRange::operator=(const FloatRange& copyFrom)
{
	m_min = copyFrom.m_min;
	m_max = copyFrom.m_max;
}

bool FloatRange::operator!=(const FloatRange& compare) const
{
	return m_min != compare.m_min && m_max != compare.m_max;
}

bool FloatRange::IsOnRange(const float& value) const
{
	return value <= m_max && value >= m_min;
}

bool FloatRange::IsOverlappingWith(const FloatRange& floatRange) const
{
	return (floatRange.m_min > m_min && floatRange.m_min < m_max) ||
		(floatRange.m_max > m_min && floatRange.m_max < m_max);
}

void FloatRange::SetFromText(const char* text)
{
	Strings rgbaText = SplitStringOnDelimiter(text, '~');
	m_min = (float)atof(rgbaText[0].c_str());
	m_max = (float)atof(rgbaText[1].c_str());

}

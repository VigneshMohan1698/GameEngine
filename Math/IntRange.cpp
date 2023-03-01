#include "IntRange.hpp"

const IntRange IntRange::ZERO = IntRange(0.0f, 0.0f);
const IntRange IntRange::ONE = IntRange(1.0f, 1.0f);
const IntRange IntRange::ZERO_TO_ONE = IntRange(0.0f, 1.0f);

IntRange::IntRange()
{
	m_min = 0;
	m_max = 0;
}

IntRange::IntRange(float min, float max)
{
	m_min = min;
	m_max = max;
}

bool IntRange::operator==(const IntRange& compare) const
{
	return m_min == compare.m_min && m_max == compare.m_max;
}

void IntRange::operator=(const IntRange& copyFrom)
{
	m_min = copyFrom.m_min;
	m_max = copyFrom.m_max;
}

bool IntRange::operator!=(const IntRange& compare) const
{
	return m_min != compare.m_min && m_max != compare.m_max;
}

bool IntRange::IsOnRange(const int& value) const
{
	return value <= m_max && value >= m_min;
}

bool IntRange::IsOverlappingWith(const IntRange& intRange) const
{
	return (intRange.m_min > m_min && intRange.m_min < m_max) ||
		(intRange.m_max > m_min && intRange.m_max < m_max);
}
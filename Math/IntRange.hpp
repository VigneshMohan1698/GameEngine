#pragma once
class IntRange
{
public:
	float m_min;
	float m_max;


	static IntRange const	ZERO;
	static IntRange const	ONE;
	static IntRange const	ZERO_TO_ONE;

	IntRange();
	IntRange(float min, float max);

	bool		operator==(const IntRange& compare) const;
	void		operator=(const IntRange& copyFrom);
	bool		operator!=(const IntRange& compare) const;
	bool		IsOnRange(const int& value) const;
	bool		IsOverlappingWith(const IntRange& floatRange) const;
};


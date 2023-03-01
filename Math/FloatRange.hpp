#pragma once

class FloatRange
{
public:
	float m_min;
	float m_max;

	static FloatRange const	ZERO;
	static FloatRange const	ONE;
	static FloatRange const	ZERO_TO_ONE;

	FloatRange();
	FloatRange(float min,float max);

	bool		operator==(const FloatRange& compare) const;
	void		operator=(const FloatRange& copyFrom);
	bool		operator!=(const FloatRange& compare) const;		
	bool		IsOnRange(const float& value) const;
	bool		IsOverlappingWith(const FloatRange& floatRange) const;
	void		SetFromText(const char* text);
	
};


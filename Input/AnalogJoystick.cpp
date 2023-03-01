#include "AnalogJoystick.hpp"
#include "Engine/Math/MathUtils.hpp"

Vec2 AnalogJoystick::GetPosition() const
{
	return m_correctedPosition;
}

float AnalogJoystick::GetMagnitude() const
{
	return m_correctedPosition.GetLength();
}

float AnalogJoystick::GetOrientationDegrees() const
{
	return m_correctedPosition.GetOrientationDegrees();
}

Vec2 AnalogJoystick::GetRawUncorrectedPosition() const
{
	return this->m_rawPosition;
}

float AnalogJoystick::GetInnerDeadZoneFraction() const
{
	return this->m_innerDeadZoneFraction;
}

float AnalogJoystick::GetOuterDeadZoneFraction() const
{
	return this->m_outerDeadZoneFraction;
}

void AnalogJoystick::Reset()
{
}

void AnalogJoystick::SetDeadZoneThresholds(float normalizedInnerDeadZoneThreshold, float normalizedOuterDeadZoneThreshold)
{
	this->m_innerDeadZoneFraction = normalizedInnerDeadZoneThreshold;
	this->m_outerDeadZoneFraction = normalizedOuterDeadZoneThreshold;
}

void AnalogJoystick::UpdatePosition(float rawNormalizedX, float rawNormalizedY)
{
	this->m_rawPosition = Vec2(rawNormalizedX, rawNormalizedY).GetNormalized();
	float xPosition = RangeMap(rawNormalizedX, -32768.0f, 32767.0f, -1.0f, 1.0f);
	float yPosition = RangeMap(rawNormalizedY, -32768.0f, 32767.0f, -1.0f, 1.0f);
	float radius = Vec2(xPosition, yPosition).GetLength();

	float radiusClamped = RangeMapClamped(radius, GetInnerDeadZoneFraction(), GetOuterDeadZoneFraction(), 0.0f, 1.0f);


	float orientation = m_rawPosition.GetOrientationDegrees();
	this->m_correctedPosition = Vec2::MakeFromPolarDegrees(orientation, radiusClamped);
}

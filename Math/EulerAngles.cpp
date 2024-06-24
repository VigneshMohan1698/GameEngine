#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/MathUtils.hpp"

const EulerAngles EulerAngles::ZERO = EulerAngles(0.0f, 0.0f, 0.0f);
EulerAngles::EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees)
{
	m_yawDegrees = yawDegrees;
	m_pitchDegrees = pitchDegrees;
	m_rollDegrees = rollDegrees;
}

void EulerAngles::GetAsVectors_XFwd_YLeft_ZUp(Vec3& out_forwardIBasis, Vec3& out_leftJbasis, Vec3& out_upKbasis) const
{
	float cy = CosDegrees(m_yawDegrees);
	float sy = SinDegrees(m_yawDegrees);

	float cp = CosDegrees(m_pitchDegrees);
	float sp = SinDegrees(m_pitchDegrees);

	float cr = CosDegrees(m_rollDegrees);
	float sr = SinDegrees(m_rollDegrees);

	Mat44 yaw = Mat44(Vec3(cy,sy,0.0f), Vec3(-sy,cy,0.0f), Vec3(0.0f,0.0f,1.0f),Vec3(0.0f,0.0f,0.0f));
	Mat44 pitch = Mat44(Vec3(cp,0.0f,-sp), Vec3(0.0f,1.0f,0.0f), Vec3(sp,0.0f,cp),Vec3(0.0f,0.0f,0.0f));
	Mat44 roll = Mat44(Vec3(1.0f,0.0f,0.0f), Vec3(0.0f,cr,sr), Vec3(0.0f,-sr,cr),Vec3(0.0f,0.0f,0.0f));

	yaw.Append(pitch);
	yaw.Append(roll);

	out_forwardIBasis = yaw.GetIBasis3D();
	out_leftJbasis = yaw.GetJBasis3D();
	out_upKbasis = yaw.GetKBasis3D();
}

void EulerAngles::GetAsVectors_XRight_YUp_ZForward(Vec3& out_forwardIBasis, Vec3& out_leftJbasis, Vec3& out_upKbasis) const
{
	float cy = CosDegrees(m_rollDegrees);
	float sy = SinDegrees(m_rollDegrees);

	float cp = CosDegrees(-m_yawDegrees);
	float sp = SinDegrees(-m_yawDegrees);

	float cr = CosDegrees(m_pitchDegrees);
	float sr = SinDegrees(m_pitchDegrees);

	Mat44 yaw = Mat44(Vec3(cy, sy, 0.0f), Vec3(-sy, cy, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));
	Mat44 pitch = Mat44(Vec3(cp, 0.0f, -sp), Vec3(0.0f, 1.0f, 0.0f), Vec3(sp, 0.0f, cp), Vec3(0.0f, 0.0f, 0.0f));
	Mat44 roll = Mat44(Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, cr, sr), Vec3(0.0f, -sr, cr), Vec3(0.0f, 0.0f, 0.0f));

	yaw.Append(pitch);
	yaw.Append(roll);

	out_forwardIBasis = yaw.GetIBasis3D();
	out_leftJbasis = yaw.GetJBasis3D();
	out_upKbasis = yaw.GetKBasis3D();
}
Mat44 EulerAngles::GetAsMatrix_XFwd_YLeft_ZUp() const
{
	float cy = CosDegrees(m_yawDegrees);
	float sy = SinDegrees(m_yawDegrees);

	float cp = CosDegrees(m_pitchDegrees);
	float sp = SinDegrees(m_pitchDegrees);

	float cr = CosDegrees(m_rollDegrees);
	float sr = SinDegrees(m_rollDegrees);

	Mat44 yaw = Mat44(Vec3(cy, sy, 0.0f), Vec3(-sy, cy, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));
	Mat44 pitch = Mat44(Vec3(cp, 0.0f, -sp), Vec3(0.0f, 1.0f, 0.0f), Vec3(sp, 0.0f, cp), Vec3(0.0f, 0.0f, 0.0f));
	Mat44 roll = Mat44(Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, cr, sr), Vec3(0.0f, -sr, cr), Vec3(0.0f, 0.0f, 0.0f));

	yaw.Append(pitch);
	yaw.Append(roll);

	return yaw;
}
Mat44 EulerAngles::GetAsMatrix_XRight_YUp_ZForward() const
{
	float cy = CosDegrees(m_rollDegrees);
	float sy = SinDegrees(m_rollDegrees);

	float cp = CosDegrees(-m_yawDegrees);
	float sp = SinDegrees(-m_yawDegrees);

	float cr = CosDegrees(m_pitchDegrees);
	float sr = SinDegrees(m_pitchDegrees);

	Mat44 yaw = Mat44(Vec3(cy, sy, 0.0f), Vec3(-sy, cy, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));
	Mat44 pitch = Mat44(Vec3(cp, 0.0f, -sp), Vec3(0.0f, 1.0f, 0.0f), Vec3(sp, 0.0f, cp), Vec3(0.0f, 0.0f, 0.0f));
	Mat44 roll = Mat44(Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, cr, sr), Vec3(0.0f, -sr, cr), Vec3(0.0f, 0.0f, 0.0f));

	yaw.Append(pitch);
	yaw.Append(roll);

	return yaw;
}
void EulerAngles::operator+=(const EulerAngles& ValueToAdd)
{
	m_yawDegrees += ValueToAdd.m_yawDegrees;
	m_pitchDegrees += ValueToAdd.m_pitchDegrees;
	m_rollDegrees += ValueToAdd.m_rollDegrees;
}
EulerAngles EulerAngles::operator-(const EulerAngles& valueToSubtract)
{
	EulerAngles returnValue;
	returnValue.m_yawDegrees = m_yawDegrees - valueToSubtract.m_yawDegrees;
	returnValue.m_pitchDegrees = m_pitchDegrees - valueToSubtract.m_pitchDegrees;
	returnValue.m_rollDegrees = m_rollDegrees - valueToSubtract.m_rollDegrees;
	return returnValue;
}
EulerAngles EulerAngles::operator+(const EulerAngles& valueToADd)
{
	EulerAngles returnValue;
	returnValue.m_yawDegrees = m_yawDegrees + valueToADd.m_yawDegrees;
	returnValue.m_pitchDegrees = m_pitchDegrees + valueToADd.m_pitchDegrees;
	returnValue.m_rollDegrees = m_rollDegrees + valueToADd.m_rollDegrees;
	return returnValue;
}
EulerAngles EulerAngles::operator*(const EulerAngles& valueToADd)
{
	EulerAngles returnValue;
	returnValue.m_yawDegrees = m_yawDegrees * valueToADd.m_yawDegrees;
	returnValue.m_pitchDegrees = m_pitchDegrees * valueToADd.m_pitchDegrees;
	returnValue.m_rollDegrees = m_rollDegrees * valueToADd.m_rollDegrees;
	return returnValue;
}
void EulerAngles::operator-=(const EulerAngles& valueToSubtract)
{
	m_yawDegrees -= valueToSubtract.m_yawDegrees;
	m_pitchDegrees -= valueToSubtract.m_pitchDegrees;
	m_rollDegrees -= valueToSubtract.m_rollDegrees;
}
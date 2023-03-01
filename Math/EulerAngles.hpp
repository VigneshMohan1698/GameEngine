#pragma  once
struct Vec3;
struct Mat44;
struct EulerAngles
{

public:
	EulerAngles() = default;
	EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees);


	void GetAsVectors_XFwd_YLeft_ZUp(Vec3& out_forwardIBasis, Vec3& out_leftJbasis, Vec3& out_upKbasis) const;
	void GetAsVectors_XRight_YUp_ZForward(Vec3& out_forwardIBasis, Vec3& out_leftJbasis, Vec3& out_upKbasis) const;
	Mat44 GetAsMatrix_XFwd_YLeft_ZUp() const;
	Mat44 GetAsMatrix_XRight_YUp_ZForward() const;

	void			operator+=(const EulerAngles& ValueToAdd);
	EulerAngles		operator-(const EulerAngles& valueToSubtract);
	EulerAngles		operator+(const EulerAngles& valueToAdd);
	EulerAngles		operator*(const EulerAngles& valueToMultiply);
	void			operator-=(const EulerAngles& valueToSubtract);
public:
	const static EulerAngles ZERO;
	float m_yawDegrees = 0.0f;
	float m_pitchDegrees = 0.0f;
	float m_rollDegrees = 0.0f;

};



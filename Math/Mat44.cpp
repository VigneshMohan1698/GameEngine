#include "Mat44.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/MathUtils.hpp"


Mat44::Mat44()
{
	m_values[Ix] =	1.0f;
	m_values[Iy] = 0.0f;
	m_values[Iz] = 0.0f;
	m_values[Iw] = 0.0f;

	m_values[Jx] = 0.0f;
	m_values[Jy] = 1.0f;
	m_values[Jz] = 0.0f;
	m_values[Jw] = 0.0f;

	m_values[Kx] = 0.0f;
	m_values[Ky] = 0.0f;
	m_values[Kz] = 1.0f;
	m_values[Kw] = 0.0f;

	m_values[Tx] = 0.0f;
	m_values[Ty] = 0.0f;
	m_values[Tz] = 0.0f;
	m_values[Tw] = 1.0f;
}
Mat44::Mat44(bool zeroes)
{
	if (zeroes)
	{
		m_values[Ix] = 0.0f;
		m_values[Iy] = 0.0f;
		m_values[Iz] = 0.0f;
		m_values[Iw] = 0.0f;

		m_values[Jx] = 0.0f;
		m_values[Jy] = 0.0f;
		m_values[Jz] = 0.0f;
		m_values[Jw] = 0.0f;

		m_values[Kx] = 0.0f;
		m_values[Ky] = 0.0f;
		m_values[Kz] = 0.0f;
		m_values[Kw] = 0.0f;

		m_values[Tx] = 0.0f;
		m_values[Ty] = 0.0f;
		m_values[Tz] = 0.0f;
		m_values[Tw] = 0.0f;
	}
	else
	{
		m_values[Ix] = 1.0f;
		m_values[Iy] = 1.0f;
		m_values[Iz] = 1.0f;
		m_values[Iw] = 1.0f;
					   
		m_values[Jx] = 1.0f;
		m_values[Jy] = 1.0f;
		m_values[Jz] = 1.0f;
		m_values[Jw] = 1.0f;
					   
		m_values[Kx] = 1.0f;
		m_values[Ky] = 1.0f;
		m_values[Kz] = 1.0f;
		m_values[Kw] = 1.0f;
					   
		m_values[Tx] = 1.0f;
		m_values[Ty] = 1.0f;
		m_values[Tz] = 1.0f;
		m_values[Tw] = 1.0f;
	}

}
Mat44::Mat44(float values[16])
{
	m_values[0] = values[0];
	m_values[1] = values[1];
	m_values[2] = values[2];
	m_values[3] = values[3];
	m_values[4] = values[4];
	m_values[5] = values[5];
	m_values[6] = values[6];
	m_values[7] = values[7];
	m_values[8] = values[8];
	m_values[9] = values[9];
	m_values[10] = values[10];
	m_values[11] = values[11];
	m_values[12] = values[12];
	m_values[13] = values[13];
	m_values[14] = values[14];
	m_values[15] = values[15];
}
Mat44::Mat44(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translation2D)
{
    m_values[Ix] = iBasis2D.x;
    m_values[Iy] = iBasis2D.y;
    m_values[Iz] = 0.0f;
    m_values[Iw] = 0.0f;
    
	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.0f;
	m_values[Jw] = 0.0f;

	m_values[Kx] = 0.0f;
	m_values[Ky] = 0.0f;
	m_values[Kz] = 1.0f;
	m_values[Kw] = 0.0f;

	m_values[Tx] = translation2D.x;
	m_values[Ty] = translation2D.y;
	m_values[Tz] = 0.0f;
	m_values[Tw] = 1.0f;
}

Mat44::Mat44(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translation3D)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.0f;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.0f;

	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0.0f;

	m_values[Tx] = translation3D.x;
	m_values[Ty] = translation3D.y;
	m_values[Tz] = translation3D.z;
	m_values[Tw] = 1.0f;
}

Mat44::Mat44(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D)
{
	m_values[Ix] = iBasis4D.x;
	m_values[Iy] = iBasis4D.y;
	m_values[Iz] = iBasis4D.z;
	m_values[Iw] = iBasis4D.w;

	m_values[Jx] = jBasis4D.x;
	m_values[Jy] = jBasis4D.y;
	m_values[Jz] = jBasis4D.z;
	m_values[Jw] = jBasis4D.w;

	m_values[Kx] = kBasis4D.x;
	m_values[Ky] = kBasis4D.y;
	m_values[Kz] = kBasis4D.z;
	m_values[Kw] = kBasis4D.w;

	m_values[Tx] = translation4D.x;
	m_values[Ty] = translation4D.y;
	m_values[Tz] = translation4D.z;
	m_values[Tw] = translation4D.w;
}
Mat44::Mat44(std::string basisString)
{
	Vec3 ibasis, jbasis,kbasis;
	if (basisString[0] == 'j')
	{
		ibasis = Vec3(0, 1, 0);
	}
	else
	{
		ibasis = Vec3(0, -1, 0);
	}
	if (basisString[2] == 'k')
	{
		jbasis = Vec3(0, 0, 1);
	}
	else 
	{
		jbasis = Vec3(0, 0, -1);
	}
	if (basisString[4] == 'i')
	{
		kbasis = Vec3(1, 0, 0);
	}
	else
	{
		kbasis = Vec3(-1, 0, 0);
	}
	/*if (basisString[0] == 'j' && basisString[2] == 'k' && basisString[4] == 'i')
	{
		ibasis = Vec3(0,1,0);
		jbasis = Vec3(0,0,1);
		kbasis = Vec3(1,0,0);
	}
	else if (basisString[0] == '-j' && basisString[2] == 'k' && basisString[4] == 'i')
	{
		ibasis = Vec3(0, 1, 0);
		jbasis = Vec3(0, 0, -1);
		kbasis = Vec3(1, 0, 0);
	}
	else if (basisString[0] == '-j' && basisString[2] == 'k' && basisString[4] == '-i')
	{
		ibasis = Vec3(0, -1, 0);
		jbasis = Vec3(0, 0, -1);
		kbasis = Vec3(1, 0, 0);
	}
	else if (basisString[0] == 'i' && basisString[2] == 'j' && basisString[4] == 'k')
	{
		ibasis = Vec3(1, 0, 0);
		jbasis = Vec3(0, 1, 0);
		kbasis = Vec3(0, 0, 1);
	}
	else if (basisString[0] == 'k' && basisString[2] == 'j' && basisString[4] == 'i')
	{
		ibasis = Vec3(0, 0, 1);
		jbasis = Vec3(0, 1, 0);
		kbasis = Vec3(1, 0, 0);
	}*/

	m_values[Ix] = ibasis.x;
	m_values[Iy] = ibasis.y;
	m_values[Iz] = ibasis.z;
	m_values[Iw] = 0.0f;

	m_values[Jx] = jbasis.x;
	m_values[Jy] = jbasis.y;
	m_values[Jz] = jbasis.z;
	m_values[Jw] = 0.0f;

	m_values[Kx] = kbasis.x;
	m_values[Ky] = kbasis.y;
	m_values[Kz] = kbasis.z;
	m_values[Kw] = 0.0f;

	m_values[Tx] = 0;
	m_values[Ty] = 0;
	m_values[Tz] = 0;
	m_values[Tw] = 1.0f;
}
Mat44::Mat44(float const* sixteenValuesBasisMajor)
{
	m_values[Ix] = sixteenValuesBasisMajor[0];
	m_values[Iy] = sixteenValuesBasisMajor[1];
	m_values[Iz] = sixteenValuesBasisMajor[2];
	m_values[Iw] = sixteenValuesBasisMajor[3];

	m_values[Jx] = sixteenValuesBasisMajor[4];
	m_values[Jy] = sixteenValuesBasisMajor[5];
	m_values[Jz] = sixteenValuesBasisMajor[6];
	m_values[Jw] = sixteenValuesBasisMajor[7];

	m_values[Kx] = sixteenValuesBasisMajor[8];
	m_values[Ky] = sixteenValuesBasisMajor[9];
	m_values[Kz] = sixteenValuesBasisMajor[10];
	m_values[Kw] = sixteenValuesBasisMajor[11];

	m_values[Tx] = sixteenValuesBasisMajor[12];
	m_values[Ty] = sixteenValuesBasisMajor[13];
	m_values[Tz] = sixteenValuesBasisMajor[14];
	m_values[Tw] = sixteenValuesBasisMajor[15];
}

Mat44 const Mat44::CreateTranslation2D(Vec2 const& translationXY)
{
	Mat44 returnValue;
	returnValue.m_values[Ix] = 1.0f;
	returnValue.m_values[Iy] = 0.0f;
	returnValue.m_values[Iz] = 0.0f;
	returnValue.m_values[Iw] = 0.0f;

	returnValue.m_values[Jx] = 0.0f;
	returnValue.m_values[Jy] = 1.0f;
	returnValue.m_values[Jz] = 0.0f;
	returnValue.m_values[Jw] = 0.0f;

	returnValue.m_values[Kx] = 0.0f;
	returnValue.m_values[Ky] = 0.0f;
	returnValue.m_values[Kz] = 1.0f;
	returnValue.m_values[Kw] = 0.0f;

	returnValue.m_values[Tx] = translationXY.x;
	returnValue.m_values[Ty] = translationXY.y;
	returnValue.m_values[Tz] = 0.0f;
	returnValue.m_values[Tw] = 1.0f;

	return returnValue;
}

Mat44 const Mat44::CreateTranslation3D(Vec3 const& translationXYZ)
{
	Mat44 returnValue;
	returnValue.m_values[Ix] = 1.0f;
	returnValue.m_values[Iy] = 0.0f;
	returnValue.m_values[Iz] = 0.0f;
	returnValue.m_values[Iw] = 0.0f;

	returnValue.m_values[Jx] = 0.0f;
	returnValue.m_values[Jy] = 1.0f;
	returnValue.m_values[Jz] = 0.0f;
	returnValue.m_values[Jw] = 0.0f;

	returnValue.m_values[Kx] = 0.0f;
	returnValue.m_values[Ky] = 0.0f;
	returnValue.m_values[Kz] = 1.0f;
	returnValue.m_values[Kw] = 0.0f;

	returnValue.m_values[Tx] = translationXYZ.x;
	returnValue.m_values[Ty] = translationXYZ.y;
	returnValue.m_values[Tz] = translationXYZ.z;
	returnValue.m_values[Tw] = 1.0f;

	return returnValue;
}

Mat44 const Mat44::CreateUniformScale2D(float uniformScaleXY)
{
	Mat44 returnValue;
	returnValue.m_values[Ix] = uniformScaleXY; returnValue.m_values[Iy] = 0.0f; returnValue.m_values[Iz] = 0.0f; returnValue.m_values[Iw] = 0.0f;

	returnValue.m_values[Jx] = 0.0f; returnValue.m_values[Jy] = uniformScaleXY; returnValue.m_values[Jz] = 0.0f;returnValue.m_values[Jw] = 0.0f;

	returnValue.m_values[Tx] = 0.0f;returnValue.m_values[Ty] = 0.0f;returnValue.m_values[Tz] = 0.0f; returnValue.m_values[Tw] = 1.0f;

	return returnValue;
}

Mat44 const Mat44::CreateUniformScale3D(float uniformScaleXYZ)
{
	Mat44 returnValue;
	returnValue.m_values[Ix] = uniformScaleXYZ;
	returnValue.m_values[Iy] = 0.0f;
	returnValue.m_values[Iz] = 0.0f;
	returnValue.m_values[Iw] = 0.0f;

	returnValue.m_values[Jx] = 0.0f;
	returnValue.m_values[Jy] = uniformScaleXYZ;
	returnValue.m_values[Jz] = 0.0f;
	returnValue.m_values[Jw] = 0.0f;

	returnValue.m_values[Kx] = 0.0f;
	returnValue.m_values[Ky] = 0.0f;
	returnValue.m_values[Kz] = uniformScaleXYZ;
	returnValue.m_values[Kw] = 0.0f;

	returnValue.m_values[Tx] = 0.0f;
	returnValue.m_values[Ty] = 0.0f;
	returnValue.m_values[Tz] = 0.0f;
	returnValue.m_values[Tw] = 1.0f;

	return returnValue;
}

Mat44 const Mat44::CreateNonUniformScale2D(Vec2 const& uniformScaleXY)
{
	Mat44 returnValue;
	returnValue.m_values[Ix] = uniformScaleXY.x;
	returnValue.m_values[Iy] = 0.0f;
	returnValue.m_values[Iz] = 0.0f;
	returnValue.m_values[Iw] = 0.0f;

	returnValue.m_values[Jx] = 0.0f;
	returnValue.m_values[Jy] = uniformScaleXY.y;
	returnValue.m_values[Jz] = 0.0f;
	returnValue.m_values[Jw] = 0.0f;

	returnValue.m_values[Tx] = 0.0f;
	returnValue.m_values[Ty] = 0.0f;
	returnValue.m_values[Tz] = 0.0f;
	returnValue.m_values[Tw] = 1.0f;

	return returnValue;
}

Mat44 const Mat44::CreateNonUniformScale3D(Vec3 const& uniformScaleXYZ)
{
	Mat44 returnValue;
	returnValue.m_values[Ix] = uniformScaleXYZ.x;
	returnValue.m_values[Iy] = 0.0f;
	returnValue.m_values[Iz] = 0.0f;
	returnValue.m_values[Iw] = 0.0f;

	returnValue.m_values[Jx] = 0.0f;
	returnValue.m_values[Jy] = uniformScaleXYZ.y;
	returnValue.m_values[Jz] = 0.0f;
	returnValue.m_values[Jw] = 0.0f;

	returnValue.m_values[Kx] = 0.0f;
	returnValue.m_values[Ky] = 0.0f;
	returnValue.m_values[Kz] = uniformScaleXYZ.z;
	returnValue.m_values[Kw] = 0.0f;

	returnValue.m_values[Tx] = 0.0f;
	returnValue.m_values[Ty] = 0.0f;
	returnValue.m_values[Tz] = 0.0f;
	returnValue.m_values[Tw] = 1.0f;

	return returnValue;
}

Mat44 const Mat44::CreateZRotationDegrees(float rotationDegreesAboutZ)
{
	Mat44 returnValue;
	returnValue.m_values[Ix] = CosDegrees(rotationDegreesAboutZ);
	returnValue.m_values[Iy] = SinDegrees(rotationDegreesAboutZ);
	returnValue.m_values[Iz] = 0.0f;
	returnValue.m_values[Iw] = 0.0f;

	returnValue.m_values[Jx] = -SinDegrees(rotationDegreesAboutZ);
	returnValue.m_values[Jy] = CosDegrees(rotationDegreesAboutZ);
	returnValue.m_values[Jz] = 0.0f;
	returnValue.m_values[Jw] = 0.0f;

	returnValue.m_values[Kx] = 0.0f;
	returnValue.m_values[Ky] = 0.0f;
	returnValue.m_values[Kz] = 1.0f;
	returnValue.m_values[Kw] = 0.0f;

	returnValue.m_values[Tx] = 0.0f;
	returnValue.m_values[Ty] = 0.0f;
	returnValue.m_values[Tz] = 0.0f;
	returnValue.m_values[Tw] = 1.0f;

	return returnValue;
}

Mat44 const Mat44::CreateYRotationDegrees(float rotationDegreesAboutY)
{
	Mat44 returnValue;
	returnValue.m_values[Ix] = CosDegrees(rotationDegreesAboutY);
	returnValue.m_values[Iy] = 0.0f;
	returnValue.m_values[Iz] = -SinDegrees(rotationDegreesAboutY);
	returnValue.m_values[Iw] = 0.0f;

	returnValue.m_values[Jx] = 0.0f;
	returnValue.m_values[Jy] = 1.0f;
	returnValue.m_values[Jz] = 0.0f;
	returnValue.m_values[Jw] = 0.0f;

	returnValue.m_values[Kx] = SinDegrees(rotationDegreesAboutY);
	returnValue.m_values[Ky] = 0.0f;
	returnValue.m_values[Kz] = CosDegrees(rotationDegreesAboutY);
	returnValue.m_values[Kw] = 0.0f;

	returnValue.m_values[Tx] = 0.0f;
	returnValue.m_values[Ty] = 0.0f;
	returnValue.m_values[Tz] = 0.0f;
	returnValue.m_values[Tw] = 1.0f;

	return returnValue;
}

Mat44 const Mat44::CreateXRotationDegrees(float rotationDegreesAboutX)
{
	Mat44 returnValue;
	returnValue.m_values[Ix] = 1.0f;
	returnValue.m_values[Iy] = 0.0f;
	returnValue.m_values[Iz] = 0.0f;
	returnValue.m_values[Iw] = 0.0f;

	returnValue.m_values[Jx] = 0.0f;
	returnValue.m_values[Jy] = CosDegrees(rotationDegreesAboutX);
	returnValue.m_values[Jz] = SinDegrees(rotationDegreesAboutX);
	returnValue.m_values[Jw] = 0.0f;

	returnValue.m_values[Kx] = 0.0f;
	returnValue.m_values[Ky] = -SinDegrees(rotationDegreesAboutX);
	returnValue.m_values[Kz] = CosDegrees(rotationDegreesAboutX);
	returnValue.m_values[Kw] = 0.0f;

	returnValue.m_values[Tx] = 0.0f;
	returnValue.m_values[Ty] = 0.0f;
	returnValue.m_values[Tz] = 0.0f;
	returnValue.m_values[Tw] = 1.0f;

	return returnValue;
}

Vec2 const Mat44::TransformVectorQuantity2D(Vec2 const& vectorQuantityXY) const
{
	Mat44 mat;
	Vec4 dotValue = Vec4(vectorQuantityXY.x, vectorQuantityXY.y, 0.0f, 0.0f);
	float ix = DotProduct4D(Vec4(m_values[Ix], m_values[Jx], m_values[Kx], m_values[Tx]), dotValue);
	float iy = DotProduct4D(Vec4(m_values[Iy], m_values[Jy], m_values[Ky], m_values[Ty]), dotValue);

	return Vec2(ix, iy);
}

Vec3 const Mat44::TransformVectorQuantity3D(Vec3 const& vectorQuantityXYZ) const
{
	Mat44 mat;
	Vec4 dotValue = Vec4(vectorQuantityXYZ.x, vectorQuantityXYZ.y, vectorQuantityXYZ.z, 0.0f);
	float ix = DotProduct4D(Vec4(m_values[Ix], m_values[Jx], m_values[Kx], m_values[Tx]), dotValue);
	float iy = DotProduct4D(Vec4(m_values[Iy], m_values[Jy], m_values[Ky], m_values[Ty]), dotValue);
	float iz = DotProduct4D(Vec4(m_values[Iz], m_values[Jz], m_values[Kz], m_values[Tz]), dotValue);

	return Vec3(ix, iy,iz);
}

Vec2 const Mat44::TransformPosition2D(Vec2 const& vectorQuantityXY) const
{
	Mat44 mat;
	Vec4 dotValue = Vec4(vectorQuantityXY.x, vectorQuantityXY.y, 0.0f, 1.0f);

	float tx = DotProduct4D(Vec4(m_values[Ix], m_values[Jx], m_values[Kx], m_values[Tx]), dotValue);
	float ty = DotProduct4D(Vec4(m_values[Iy], m_values[Jy], m_values[Ky], m_values[Ty]), dotValue);
	return Vec2(tx, ty);
}

Vec3 const Mat44::TransformPosition3D(Vec3 const& vectorQuantityXYZ) const
{
	Mat44 mat;
	Vec4 dotValue = Vec4(vectorQuantityXYZ.x, vectorQuantityXYZ.y, vectorQuantityXYZ.z, 1.0f);
	float tx = DotProduct4D(Vec4(m_values[Ix], m_values[Jx], m_values[Kx], m_values[Tx]), dotValue);
	float ty = DotProduct4D(Vec4(m_values[Iy], m_values[Jy], m_values[Ky], m_values[Ty]), dotValue);
	float tz = DotProduct4D(Vec4(m_values[Iz], m_values[Jz], m_values[Kz], m_values[Tz]), dotValue);

	return Vec3(tx, ty, tz);
}

Vec4 const Mat44::TransformHomogeneous3D(Vec4 const& homogeneousPoint3D) const
{
	Mat44 mat;

	float ix = DotProduct4D(Vec4(m_values[Ix], m_values[Jx], m_values[Kx], m_values[Tx]),homogeneousPoint3D);
	float iy = DotProduct4D(Vec4(m_values[Iy], m_values[Jy], m_values[Ky], m_values[Ty]),homogeneousPoint3D);
	float iz = DotProduct4D(Vec4(m_values[Iz], m_values[Jz], m_values[Kz], m_values[Tz]),homogeneousPoint3D);
	float iw = DotProduct4D(Vec4(m_values[Iw], m_values[Jw], m_values[Kw], m_values[Tw]),homogeneousPoint3D);
																		

	return Vec4(ix, iy, iz,iw);
}

float* Mat44::GetAsFloatArray()
{
    return m_values;
}

float const* Mat44::GetAsFloatArray() const
{
	return m_values;
}

Vec2 const Mat44::GetIBasis2D() const
{
    return Vec2(m_values[0], m_values[1]);
}

Vec2 const Mat44::GetJBasis2D() const
{
	return Vec2(m_values[4], m_values[5]);
}

Vec2 const Mat44::GetTranslation2D() const
{
	return Vec2(m_values[Tx], m_values[Ty]);
}

Vec3 const Mat44::GetIBasis3D() const
{
    return Vec3(m_values[0], m_values[1], m_values[2]);
}

Vec3 const Mat44::GetJBasis3D() const
{
	return Vec3(m_values[4], m_values[5], m_values[6]);
}

Vec3 const Mat44::GetKBasis3D() const
{
	return Vec3(m_values[8], m_values[9], m_values[10]);
}

Vec3 const Mat44::GetTranslation3D() const
{
	return Vec3(m_values[Tx], m_values[Ty], m_values[Tz]);
}

Vec4 const Mat44::GetIBasis4D() const
{
	return Vec4(m_values[Ix], m_values[Iy], m_values[Iz], m_values[Iw]);
}

Vec4 const Mat44::GetJBasis4D() const
{
	return Vec4(m_values[Jx], m_values[Jy], m_values[Jz], m_values[Jw]);
}

Vec4 const Mat44::GetKBasis4D() const
{
	return Vec4(m_values[Kx], m_values[Ky], m_values[Kz], m_values[Kw]);
}

Vec4 const Mat44::GetTranslation4D() const
{
	return Vec4(m_values[Tx], m_values[Ty], m_values[Tz], m_values[Tw]);
}

void Mat44::SetTranslation2D(Vec2 const& translationXY)
{

	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0.0f;
	m_values[Tw] = 1.0f;

}

void Mat44::SetTranslation3D(Vec3 const& translationXYZ)
{
	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1.0f;
}

void Mat44::SetIJ2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.0f;
	m_values[Iw] = 0.0f;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.0f;
	m_values[Jw] = 0.0f;
}

void Mat44::SetIJT2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translationXY)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.0f;
	m_values[Iw] = 0.0f;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.0f;
	m_values[Jw] = 0.0f;

	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0.0f;
	m_values[Tw] = 1.0f;
}

void Mat44::SetIJK3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& KBasis3D)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.0f;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.0f;

	m_values[Kx] = KBasis3D.x;
	m_values[Ky] = KBasis3D.y;
	m_values[Kz] = KBasis3D.z;
	m_values[Kw] = 0.0f;
}

void Mat44::SetIJKT3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& KBasis3D, Vec3 const& translationXYZ)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.0f;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.0f;

	m_values[Kx] = KBasis3D.x;
	m_values[Ky] = KBasis3D.y;
	m_values[Kz] = KBasis3D.z;
	m_values[Kw] = 0.0f;

	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1.0f;
}

void Mat44::SetIJKT4D(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& KBasis4D, Vec4 const& translation4D)
{
	m_values[Ix] = iBasis4D.x;
	m_values[Iy] = iBasis4D.y;
	m_values[Iz] = iBasis4D.z;
	m_values[Iw] = iBasis4D.w;

	m_values[Jx] = jBasis4D.x;
	m_values[Jy] = jBasis4D.y;
	m_values[Jz] = jBasis4D.z;
	m_values[Jw] = jBasis4D.w;

	m_values[Kx] = KBasis4D.x;
	m_values[Ky] = KBasis4D.y;
	m_values[Kz] = KBasis4D.z;
	m_values[Kw] = KBasis4D.w;

	m_values[Tx] = translation4D.x;
	m_values[Ty] = translation4D.y;
	m_values[Tz] = translation4D.z;
	m_values[Tw] = translation4D.w;
}

void Mat44::Append(Mat44 const& appendThis)
{
	
	float ix= DotProduct4D(Vec4(m_values[Ix], m_values[Jx], m_values[Kx], m_values[Tx]), appendThis.GetIBasis4D());
	float iy = DotProduct4D(Vec4(m_values[Iy], m_values[Jy], m_values[Ky], m_values[Ty]), appendThis.GetIBasis4D());
	float iz = DotProduct4D(Vec4(m_values[Iz], m_values[Jz], m_values[Kz], m_values[Tz]), appendThis.GetIBasis4D());
	float iw = DotProduct4D(Vec4(m_values[Iw], m_values[Jw], m_values[Kw], m_values[Tw]), appendThis.GetIBasis4D());

	float jx = DotProduct4D(Vec4(m_values[Ix], m_values[Jx], m_values[Kx], m_values[Tx]), appendThis.GetJBasis4D());
	float jy = DotProduct4D(Vec4(m_values[Iy], m_values[Jy], m_values[Ky], m_values[Ty]), appendThis.GetJBasis4D());
	float jz = DotProduct4D(Vec4(m_values[Iz], m_values[Jz], m_values[Kz], m_values[Tz]), appendThis.GetJBasis4D());
	float jw = DotProduct4D(Vec4(m_values[Iw], m_values[Jw], m_values[Kw], m_values[Tw]), appendThis.GetJBasis4D());

	float kx = DotProduct4D(Vec4(m_values[Ix], m_values[Jx], m_values[Kx], m_values[Tx]), appendThis.GetKBasis4D());
	float ky = DotProduct4D(Vec4(m_values[Iy], m_values[Jy], m_values[Ky], m_values[Ty]), appendThis.GetKBasis4D());
	float kz = DotProduct4D(Vec4(m_values[Iz], m_values[Jz], m_values[Kz], m_values[Tz]), appendThis.GetKBasis4D());
	float kw = DotProduct4D(Vec4(m_values[Iw], m_values[Jw], m_values[Kw], m_values[Tw]), appendThis.GetKBasis4D());

	float tx = DotProduct4D(Vec4(m_values[Ix], m_values[Jx], m_values[Kx], m_values[Tx]), appendThis.GetTranslation4D());
	float ty = DotProduct4D(Vec4(m_values[Iy], m_values[Jy], m_values[Ky], m_values[Ty]), appendThis.GetTranslation4D());
	float tz = DotProduct4D(Vec4(m_values[Iz], m_values[Jz], m_values[Kz], m_values[Tz]), appendThis.GetTranslation4D());
	float tw = DotProduct4D(Vec4(m_values[Iw], m_values[Jw], m_values[Kw], m_values[Tw]), appendThis.GetTranslation4D());

	m_values[Ix] = ix;
	m_values[Iy] = iy;
	m_values[Iz] = iz;
	m_values[Iw] = iw;

	m_values[Jx] = jx;
	m_values[Jy] = jy;
	m_values[Jz] = jz;
	m_values[Jw] = jw;

	m_values[Kx] = kx;
	m_values[Ky] = ky;
	m_values[Kz] = kz;
	m_values[Kw] = kw;

	m_values[Tx] = tx;
	m_values[Ty] = ty;
	m_values[Tz] = tz;
	m_values[Tw] = tw;
}

void Mat44::AppendZRotation(float degreesRotationAboutZ)
{
	this->Append(CreateZRotationDegrees(degreesRotationAboutZ));
}

void Mat44::AppendYRotation(float degreesRotationAboutY)
{
	this->Append(CreateYRotationDegrees(degreesRotationAboutY));
}

void Mat44::AppendXRotation(float degreesRotationAboutX)
{
	this->Append(CreateXRotationDegrees(degreesRotationAboutX));
}

void Mat44::AppendTranslation2D(Vec2 const& translationXY)
{
	this->Append(CreateTranslation2D(translationXY));
}

void Mat44::AppendTranslation3D(Vec3 const& translationXYZ)
{
	this->Append(CreateTranslation3D(translationXYZ));
}

void Mat44::AppendScaleUniform2D(float uniformScaleXY)
{
	this->Append(CreateUniformScale2D(uniformScaleXY));
}

void Mat44::AppendScaleUniform3D(float uniformScaleXYZ)
{
	this->Append(CreateUniformScale3D(uniformScaleXYZ));
}

void Mat44::AppendScaleNonUniform2D(Vec2 const& nonUniformScaleXY)
{
	this->Append(CreateNonUniformScale2D(nonUniformScaleXY));
}

void Mat44::AppendScaleNonUniform3D(Vec3 const& nonUniformScaleXYZ)
{
	this->Append(CreateNonUniformScale3D(nonUniformScaleXYZ));
}

Mat44 const Mat44::CreateOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Mat44 returnValue;
	returnValue.m_values[Ix] = 2 / (right - left);
	returnValue.m_values[Jy] = 2 / (top - bottom);
	returnValue.m_values[Kz] = 1 / (zFar - zNear);
	returnValue.m_values[Tx] = -(right + left) / (right - left);
	returnValue.m_values[Ty] = -(top + bottom) / (top - bottom);
	returnValue.m_values[Tz] = -(zNear) / (zFar - zNear);
	returnValue.m_values[Tw] = 1.0f;
	return returnValue;
}

Mat44 const Mat44::CreatePerspectiveProjection(float fovYDegrees, float aspect, float zNear, float zFar)
{
	Mat44 returnValue;
	float Sy;
	float Sx;
	float Sz;
	float TZ;
	Sy = (CosDegrees(fovYDegrees * 0.5f) / SinDegrees(fovYDegrees * 0.5f));
	Sz = zFar / (zFar - zNear);
	Sx = Sy / aspect;
	TZ = (zNear * zFar) / (zNear - zFar);
	returnValue.m_values[Ix] = Sx;
	returnValue.m_values[Jy] = Sy;
	returnValue.m_values[Kz] = Sz;
	returnValue.m_values[Kw] = 1.0f;
	returnValue.m_values[Tz] = TZ;
	returnValue.m_values[Tw] = 0.0f;
	return returnValue;
}

Mat44 const Mat44::CreateStereoscopicProjectionMatrix(float fovYDegrees, float aspect, float zNear, float zFar, float left, float right)
{
	Mat44 returnValue;
	float Sy;
	float Sx;
	float Sz;
	float TZ, TX;
	Sy = (CosDegrees(fovYDegrees * 0.5f) / SinDegrees(fovYDegrees * 0.5f));
	Sz = zFar / (zFar - zNear);
	Sx = Sy / aspect;
	TX = left + right;
	TZ = (zNear * zFar) / (zNear - zFar);
	returnValue.m_values[Ix] = Sx;
	returnValue.m_values[Jy] = Sy;
	returnValue.m_values[Kz] = Sz;
	returnValue.m_values[Kw] = 1.0f;
	//returnValue.m_values[Tx] = TX;
	returnValue.m_values[Tz] = TZ;
	returnValue.m_values[Tw] = 0.0f;
	return returnValue;
}
Mat44 const Mat44::CreateStereoscopicProjectionMatrix(float eyeDistance, float left,float right,float up,float down, float zNear, float zFar)
{
	Mat44 returnValue;
	//float ix, jy, kx, ky, kz, kw, tz;
	//float eyeDistance = right-left/2.0f;
	returnValue.m_values[Ix] = (2 * eyeDistance) / (right - left);
	returnValue.m_values[Jy] = (2 * eyeDistance) / (up -down);
	returnValue.m_values[Kx] = (right + left) / (right - left);
	returnValue.m_values[Ky] = (up + down) / (up - down);
	returnValue.m_values[Kz] = (zNear + zFar) / (zNear - zFar);
	returnValue.m_values[Kw] = -1;
	returnValue.m_values[Tz] = (2 * zNear * zFar) / (zNear - zFar);

	return returnValue;
}

void Mat44::Transpose()
{
	SwapValues(m_values[Iy], m_values[Jx]);
	SwapValues(m_values[Iz], m_values[Kx]);
	SwapValues(m_values[Iw], m_values[Tx]);

	SwapValues(m_values[Jz], m_values[Ky]);
	SwapValues(m_values[Jw], m_values[Ty]);
	SwapValues(m_values[Kw], m_values[Tz]);
}

Mat44 const Mat44::GetOrthonormalInverse() const
{
	Mat44 returnValue;
	Vec3 translation = GetTranslation3D();
	translation.x = -translation.x;
	translation.y = -translation.y;
	translation.z = -translation.z;
	float translations[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, (float)translation.x, (float)translation.y, (float)translation.z, 1.0f };
	Mat44 translationMatrix = Mat44(translations);
	Mat44 matrix = Mat44(GetIBasis4D(), GetJBasis4D(), GetKBasis4D(), GetTranslation4D());
	matrix.SetTranslation3D(Vec3(0.0f, 0.0f, 0.0f));
	matrix.Transpose();
	matrix.Append(translationMatrix);
	return matrix;
}

void Mat44::Orthonormalize_XFwd_YLeft_ZUp()
{
	Vec3 iBasis;
	Vec3 jBasis;
	Vec3 kBasis;
	kBasis = GetKBasis3D();
	jBasis = GetJBasis3D();
	iBasis = GetIBasis3D();
	kBasis -= GetProjectedOnto3D(kBasis, iBasis.GetNormalized());
	kBasis = kBasis.GetNormalized();	
	jBasis -= GetProjectedOnto3D(jBasis, iBasis.GetNormalized());
	jBasis -= GetProjectedOnto3D(jBasis, kBasis.GetNormalized());
	jBasis = jBasis.GetNormalized();
	SetIJK3D(iBasis.GetNormalized(), jBasis, kBasis);
}
Mat44 Mat44::GetNonOrthonormalInverse()
{
	Mat44 returnInvertedMatrix;
	Mat44 matrix = Mat44(GetIBasis4D(), GetJBasis4D(), GetKBasis4D(), GetTranslation4D());
	matrix.Transpose();
	float MatrixArray[4][4] = {0};
	float InverseArray[4][4] = {0};
	MapMatrixTo2DArray(matrix, MatrixArray);
	Inverse(MatrixArray, InverseArray);
	Map2DArrayToMatrix(InverseArray , returnInvertedMatrix);
	return returnInvertedMatrix;
}
void Mat44::MapMatrixTo2DArray(Mat44 copyFrom, float copyTo[4][4])
{
   int matrixIndex = 0, row = 0,col = 0;
   for (matrixIndex = 0; matrixIndex < 16; matrixIndex++)
   {
	   copyTo[row][col] = copyFrom.m_values[matrixIndex];
	   row = row + 1;
	   if (row == 4) {
		   row = 0;
		   col++;
	   }
   }
}

Mat44 Invert(Mat44 const& mat)
{
	float inv[16];
	float det;
	float m[16];
	int i;

	for (i = 0; i < 16; ++i) {
		m[i] = (float)mat.m_values[i];
	}

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
	det = 1.0f / det;

	Mat44 ret;
	for (i = 0; i < 16; i++) {
		ret.m_values[i] = (float)(inv[i] * det);
	}

	return ret;
}

void Mat44::Map2DArrayToMatrix(float copyFrom[4][4], Mat44& copyTo)
{
	
	int matrixIndex = 0, row = 0, col = 0;
	for (row = 0; row <4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			copyTo.m_values[matrixIndex] = copyFrom[row][col];
			matrixIndex++;
		}
	}

}
void GetCofactor(float A[4][4], float temp[4][4], int p, int q,int n)
{
	int i = 0, j = 0;
	for (int row = 0; row < n; row++) 
	{
		for (int col = 0; col < n; col++) 
		{
			if (row != p && col != q) 
			{
				temp[i][j++] = A[row][col];
				if (j == n - 1) {
					j = 0;
					i++;
				}
			}
		}
	}
}

float Determinant(float A[4][4], int n)
{
	float D = 0; 

	if (n == 1)
		return A[0][0];

	float temp[4][4]; 

	int sign = 1; 
	for (int f = 0; f < n; f++) {

		GetCofactor(A, temp, 0, f, n);
		D += sign * A[0][f] * Determinant(temp, n - 1);

		sign = -sign;
	}

	return D;
}

void Adjoint(float A[4][4], float adj[4][4])
{
	int sign = 1;
	float temp[4][4];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			GetCofactor(A, temp, i, j, 4);
			sign = ((i + j) % 2 == 0) ? 1 : -1;
			adj[j][i] = (sign) * (Determinant(temp, 4 - 1));
		}
	}
}

bool Inverse(float A[4][4], float inverse[4][4])
{
	float det = Determinant(A, 4);

	float adj[4][4];
	Adjoint(A, adj);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			inverse[i][j] = adj[i][j] / float(det);

	return true;
}

void Mat44::SwapValues(float& valuea, float& valueb)
{
	float temp = valuea;
	valuea = valueb;
	valueb = temp;
}
#include "Camera.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/GameCommon.hpp"

Camera::Camera(float bottomX, float bottomY, float topX, float topY)
{
	Vec2 bottomLeft = Vec2(bottomX, bottomY);
	Vec2 topRight = Vec2(topX, topY);
	SetCameraBounds(bottomLeft, topRight);
	SetOrthoView(bottomLeft, topRight);
	m_cameraSettings.viewType = Orthographic;
	m_orientation = EulerAngles(0.0f, 0.0f, 0.0f);
}
void Camera::SetOrthoView(Vec2 const& bottomLeft, Vec2 const& topRight)
{
	m_cameraSettings.viewType = Orthographic;
	m_orthogrphicCameraMatrix = Mat44::CreateOrthoProjection(cameraBounds.m_mins.x, cameraBounds.m_maxs.x, cameraBounds.m_mins.y, cameraBounds.m_maxs.y, 0.0f, 1.0f);
}
void Camera::SetOrthoView()
{
	m_cameraSettings.viewType = Orthographic;
	m_orthogrphicCameraMatrix =  Mat44::CreateOrthoProjection(cameraBounds.m_mins.x, cameraBounds.m_maxs.x, cameraBounds.m_mins.y, cameraBounds.m_maxs.y, 0.0f, 1.0f);
}
void Camera::SetCameraBounds(Vec2 const& bottomLeft, Vec2 const& topRight)
{
	cameraBounds.m_mins = bottomLeft;
	cameraBounds.m_maxs = topRight;
}

void Camera::SetCameraBounds(AABB2 bounds)
{
	Bottomleft = bounds.m_mins;
	TopRight = bounds.m_maxs;
	
	cameraBounds = bounds;
}

AABB2 Camera::GetCameraBounds() const 
{
	return cameraBounds;
}

void Camera::Translate2D(const Vec2& translation2D)
{
	cameraBounds.m_mins += translation2D;
	cameraBounds.m_maxs += translation2D;
}

AABB2 Camera::GetTranslatated2D(const Vec2& translation2D) const
{
	AABB2 returnValue;
	returnValue.m_mins = cameraBounds.m_mins + translation2D;
	returnValue.m_maxs = cameraBounds.m_maxs + translation2D;
	return returnValue;
}

void Camera::CenterCameraAroundPoint(const Vec2& point)
{
	this->cameraBounds.SetCenter(point);
}

void Camera::RestrictCameraWithinBounds(Vec2 lowerBounds, Vec2 UpperBounds)
{
	float distance;
	if (cameraBounds.m_mins.x < lowerBounds.x)
	{
		distance = 0 - cameraBounds.m_mins.x;
		Translate2D(Vec2(distance, 0.0f));
	}
	if (cameraBounds.m_maxs.x > UpperBounds.x)
	{
		distance = cameraBounds.m_maxs.x - UpperBounds.x;
		Translate2D(Vec2(-distance, 0.0f));
	}
	if (cameraBounds.m_mins.y < lowerBounds.y)
	{
		distance = -cameraBounds.m_mins.y;
		Translate2D(Vec2(0.0f, distance));
	}
	if (cameraBounds.m_maxs.y > UpperBounds.y)
	{
		distance = cameraBounds.m_maxs.y - UpperBounds.y;
		Translate2D(Vec2(0.0f, -distance));
	}
}
void Camera::SetFov(float fov) 
{
	m_cameraSettings.fov = fov;
}
void Camera::SetZnearAndFar(float znear, float zfar) 
{
	m_cameraSettings.znear = znear;
	m_cameraSettings.zFar = zfar;
}
void Camera::RecalculateCameraMatrix()
{
	switch (m_cameraSettings.viewType)
	{
	case Orthographic:
		m_orthogrphicCameraMatrix = Mat44::CreateOrthoProjection(cameraBounds.m_mins.x, cameraBounds.m_maxs.x, cameraBounds.m_mins.y, cameraBounds.m_maxs.y, 0.0f, 1.0f);
		break;
	case Perspective:
		m_perspectiveCameraMatrix = Mat44::CreatePerspectiveProjection(m_cameraSettings.fov, m_cameraSettings.aspect, m_cameraSettings.znear, m_cameraSettings.zFar);
		break;
	case Stereoscope:
		break;
	}
}

void Camera::SetPerspectiveView(float aspect, float fov, float znear, float zfar)
{
	m_cameraSettings.viewType = Perspective;
	m_cameraSettings.aspect = aspect;
	m_cameraSettings.fov = fov;
	m_cameraSettings.znear = znear;
	m_cameraSettings.zFar = zfar;

	m_perspectiveCameraMatrix = Mat44::CreatePerspectiveProjection(m_cameraSettings.fov, aspect, znear, zfar);
}

Mat44 Camera::GetOrthoMatrix() const
{
	return m_orthogrphicCameraMatrix;
}

Mat44 Camera::GetProjectionMatrix() const
{
	Mat44 projectionMatrix;
	switch (m_cameraSettings.viewType)
	{
	case Orthographic:
		projectionMatrix = GetOrthoMatrix();
		projectionMatrix.Append(m_viewToRenderMatrix);
		return projectionMatrix;
		break;
	case Perspective:
		projectionMatrix = GetPerspectiveMatrix();
		projectionMatrix.Append(m_viewToRenderMatrix);
		return projectionMatrix;
		break;
	case Stereoscope:
		break;
	}
	return Mat44();
}

void Camera::SetTransform(Vec3 position, EulerAngles orientation)
{
	m_orientation = orientation;
	m_position = position;
}
Mat44 Camera::GetModalMatrix() const
{
	Mat44 translationMatrix = Mat44::CreateTranslation3D(m_position);
	Mat44 rotationMatrix = m_orientation.GetAsMatrix_XFwd_YLeft_ZUp();
	translationMatrix.Append(rotationMatrix);
	return translationMatrix;
}
Mat44 Camera::GetViewMatrix() const
{
	//if the look at matrix is set, just return that and ignore 
	//camera position and orientation.
	if (m_lookAtMatrixSet)
	{
		return m_lookAtMatrix.GetOrthonormalInverse();
	}

	Mat44 translationMatrix = Mat44::CreateTranslation3D(m_position);
	Mat44 rotationMatrix = m_orientation.GetAsMatrix_XFwd_YLeft_ZUp();
	translationMatrix.Append(rotationMatrix);
	return translationMatrix.GetOrthonormalInverse();
}
Mat44 Camera::GetViewMatrixAtOrigin() const
{
	Mat44 translationMatrix = Mat44::CreateTranslation3D(Vec3());
	Mat44 rotationMatrix = m_orientation.GetAsMatrix_XFwd_YLeft_ZUp();
	translationMatrix.Append(rotationMatrix);
	return translationMatrix.GetOrthonormalInverse();
}
void Camera::SetLookAt(const Vec3& lookAt, const Vec3& up)
{
	Mat44 lookAtMatrix = Mat44::CreateLookAtMatrix(m_position, lookAt, up);
	SetLookAtMatrix(lookAtMatrix, true);
}

void Camera::SetLookAtMatrix(const Mat44& matrix, bool setValue)
{
	m_lookAtMatrix = matrix;
	m_lookAtMatrixSet = setValue;
}

Mat44 Camera::GetPerspectiveMatrix() const
{
	return m_perspectiveCameraMatrix;
}

void Camera::SetViewToRenderTransform(Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis)
{
	m_viewToRenderMatrix = Mat44(iBasis, jBasis, kBasis, Vec3());
	//m_viewToRenderMatrix.SetIJK3D(iBasis, jBasis, kBasis);
}

Mat44 Camera::GetViewToRenderMatrix() const
{
	return m_viewToRenderMatrix;
}

EulerAngles Camera::GetCameraOrientation()
{
	return m_orientation;
}

void Camera::SetRenderBasis(Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis)
{
	m_renderI = iBasis;
	m_renderJ = jBasis;
	m_renderK = kBasis;
}

#include "Camera.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Texture.hpp"

Camera::Camera(float bottomX, float bottomY, float topX, float topY)
{
	Vec2 bottomLeft = Vec2(bottomX, bottomY);
	Vec2 topRight = Vec2(topX, topY);
	SetOrthoView(bottomLeft, topRight);
	SetCameraBounds(bottomLeft, topRight);
	m_cameraView = Orthographic;
	m_orientation = EulerAngles(0.0f, 0.0f, 0.0f);
}
void Camera::SetOrthoView(Vec2 const& bottomLeft, Vec2 const& topRight)
{
	this->m_bottomLeft = bottomLeft;
	this->m_topRight = topRight;
	m_cameraView = Orthographic;
}
void Camera::SetOrthoView()
{
	m_cameraView = Orthographic;
}
void Camera::SetCameraBounds(Vec2 const& bottomLeft, Vec2 const& topRight)
{
	this->cameraBounds.m_mins = bottomLeft;
	this->cameraBounds.m_maxs = topRight;
}

void Camera::SetCameraBounds(AABB2 bounds)
{
	Bottomleft = bounds.m_mins;
	TopRight = bounds.m_maxs;
	
	this->cameraBounds = bounds;
}


Vec2 Camera::GetOrthoBottomLeft() const
{
	return this->m_bottomLeft;
}

Vec2 Camera::GetOrthoTopRight() const
{
	return this->m_topRight;
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

void Camera::SetPerspectiveView(float aspect, float fov, float nearz, float farz)
{
	m_cameraView = Perspective;
	m_perspectiveCameraMatrix = Mat44::CreatePerspectiveProjection(fov, aspect, nearz, farz);
}

void Camera::SetStereoscopicView(Mat44 stereoScopicProjectionMatrix, Mat44 viewToRender)
{
	UNUSED((void)stereoScopicProjectionMatrix);
	m_cameraView = Stereoscope;
	m_viewToRenderMatrix = viewToRender;
	m_stereoScopicCameraMatrix = GetPerspectiveMatrix();
}
void Camera::SetStereoscopicView(float aspect, float fov, float nearz, float farz, float left, float right)
{
	m_cameraView = Stereoscope;
	m_stereoScopicCameraMatrix = Mat44::CreateStereoscopicProjectionMatrix(fov, aspect, nearz, farz, left, right);
};
Mat44 Camera::GetOrthoMatrix() const
{
	return Mat44::CreateOrthoProjection(cameraBounds.m_mins.x, cameraBounds.m_maxs.x, cameraBounds.m_mins.y, cameraBounds.m_maxs.y, 0.0f, 1.0f);
}

Mat44 Camera::GetProjectionMatrix() const
{
	Mat44 projectionMatrix;
	switch (m_cameraView)
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
		projectionMatrix = GetStereoScopicMatrix();
		projectionMatrix.Append(m_viewToRenderMatrix);
		//float leftEye;
		//float rightEye;
		//leftEye = g_theRenderer->m_leftStereoscope;
		//rightEye = g_theRenderer->m_rightStereoscope;
		//projectionMatrix.m_values[12] += (leftEye + rightEye);
		return projectionMatrix;
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
Mat44 Camera::SetAndGetLookAtMatrix(const Vec3& positionToLookAt)
{
	Vec3 iBasis, jBasis, kBasis;
	iBasis = (m_position - positionToLookAt).GetNormalized();
	Vec3 worldZBasis = Vec3(0.0f, 0.0, 1.0f);
	jBasis = CrossProduct3D(worldZBasis, iBasis);
	kBasis = CrossProduct3D(iBasis,jBasis);
	m_lookAtMatrix = Mat44(iBasis, kBasis, jBasis, Vec3());
	return m_lookAtMatrix;
}

Mat44 Camera::GetPerspectiveMatrix() const
{
	return m_perspectiveCameraMatrix;
}
Mat44 Camera::GetStereoScopicMatrix() const
{
	return m_stereoScopicCameraMatrix;
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

void Camera::SetColorTarget(Texture* tex)
{
	m_colorTarget = tex;
}

void Camera::SetDepthTarget(Texture* tex)
{
	m_depthTarget = tex;
}

Texture* Camera::GetColorTarget()
{
	return m_colorTarget;
}

Texture* Camera::GetDepthTarget()
{
	return m_depthTarget;
}
void Camera::DestroyTextures()
{
	if (m_colorTarget)
	{
		m_owner->DestroyTexture(m_colorTarget);
	}
	if (m_depthTarget)
	{
		m_owner->DestroyTexture(m_depthTarget);
	}
}
void Camera::SetRenderBasis(Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis)
{
	m_renderI = iBasis;
	m_renderJ = jBasis;
	m_renderK = kBasis;
}

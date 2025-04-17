#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/EulerAngles.hpp"

class Renderer;
class Texture;
 enum CameraView {
	Orthographic,
	Perspective,
	Stereoscope
};

struct CameraSettings {
	CameraView viewType = CameraView::Perspective;
	float	   fov = 90.0f;
	float	   znear = 0.01f;
	float	   zFar =  1000.0f;
	float	   aspect = 2.0f;
};

class OrbitCamera
{
	public:
		OrbitCamera() {} ;
		OrbitCamera(Vec3 focalPoint, EulerAngles yawPichRoll, float radius)
		{
			m_focalPoint = focalPoint;
			m_yawPichRoll = yawPichRoll;
			m_radius = radius;
		}

	public:
		Vec3 m_focalPoint;
		EulerAngles m_yawPichRoll;
		float m_radius;
};
class Camera {

	AABB2 cameraBounds;
	Vec2 Bottomleft;
	Vec2 TopRight;
	public:
		Camera() {};
		Camera(float bottomX, float bottomY, float topX, float topY);
		~Camera() {};
		void				SetCameraBounds(Vec2 const& bottomLeft, Vec2 const& topRight);
		void				SetCameraBounds(AABB2 bounds);
		Vec2				GetOrthoBottomLeft() const;
		Vec2				GetOrthoTopRight() const;
		AABB2				GetCameraBounds() const;
		void				Translate2D(const Vec2& translation2D);
		void				SetRenderBasis(Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis);
		AABB2				GetTranslatated2D(const Vec2& translation2D) const;
		void				CenterCameraAroundPoint(const Vec2& point);
		void				RestrictCameraWithinBounds(Vec2 lowerBounds, Vec2 UpperBounds);

		//--------------------CAMERA VIEWS-------------------------------------------
		void				SetPerspectiveView(float aspect, float fov, float near, float far);
		void				SetOrthoView(Vec2 const& bottomLeft, Vec2 const& topRight);
		void				SetOrthoView();
		Mat44				GetPerspectiveMatrix() const;
		void				SetViewToRenderTransform(Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis);
		Mat44				GetViewToRenderMatrix() const;
		Mat44				GetOrthoMatrix() const;
		Mat44				GetProjectionMatrix() const;
		void				SetTransform(Vec3 position, EulerAngles orientation);
		Mat44				GetModalMatrix() const;
		Mat44				GetViewMatrix() const;
		Mat44				GetViewMatrixAtOrigin() const;
		void				SetLookAt(const Vec3& positionToLookAt, const Vec3& up);
		void				SetLookAtMatrix(const Mat44& matrix, bool setValue);
		EulerAngles			GetCameraOrientation();
		void				SetFov(float fov);
		void				SetZnearAndFar(float znear, float zfar);
		void				SetCameraSettings(const CameraSettings& cameraSettings) {m_cameraSettings = cameraSettings;}
		const CameraSettings&  GetCameraSettings() {return m_cameraSettings;}
		void				RecalculateCameraMatrix();

		Vec3				m_position;
		EulerAngles			m_orientation;
		AABB2				m_viewport = AABB2::ZERO_TO_ONE;
		CameraSettings		m_cameraSettings = {};
	private:
		Mat44				m_viewToRenderMatrix = Mat44();
		Vec3				m_renderI = Vec3(1.0f, 0.0f, 0.0f);
		Vec3				m_renderJ = Vec3(0.0f, 1.0f, 0.0f);
		Vec3				m_renderK = Vec3(0.0f, 0.0f, 1.0f);

		Mat44				m_perspectiveCameraMatrix;
		Mat44				m_orthogrphicCameraMatrix;
		Mat44				m_lookAtMatrix = Mat44();
		bool				m_lookAtMatrixSet = false;

};


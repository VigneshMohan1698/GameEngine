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
		void				SetStereoscopicView(Mat44 stereoScopicProjectionMatrix, Mat44 viewToRender);
		void				SetStereoscopicView(float aspect, float fov, float, float, float left, float right);
		void				SetOrthoView(Vec2 const& bottomLeft, Vec2 const& topRight);
		void				SetOrthoView();
		Mat44				GetPerspectiveMatrix() const;
		Mat44				GetStereoScopicMatrix() const;
		void				SetViewToRenderTransform(Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis);
		Mat44				GetViewToRenderMatrix() const;
		Mat44				GetOrthoMatrix() const;
		Mat44				GetProjectionMatrix() const;
		void				SetTransform(Vec3 position, EulerAngles orientation);
		Mat44				GetModalMatrix() const;
		Mat44				GetViewMatrix() const;
		Mat44				GetViewMatrixAtOrigin() const;
		Mat44				SetAndGetLookAtMatrix(const Vec3& positionToLookAt);
		EulerAngles			GetCameraOrientation();
		void				SetColorTarget(Texture* tex);
		void				SetDepthTarget(Texture* tex);
		Texture*			GetColorTarget();
		Texture*			GetDepthTarget();
		void				DestroyTextures();

		Texture*			m_colorTarget = nullptr;
		Texture*			m_depthTarget = nullptr;
		Vec3				m_position;
		EulerAngles			m_orientation;
		AABB2				m_viewport = AABB2::ZERO_TO_ONE;
		float				m_cameraAspectRatio = 2.0f;
		Renderer*			m_owner = nullptr;
		CameraView			m_cameraView = CameraView::Perspective;
	private:
		Mat44				m_viewToRenderMatrix;
		Vec3				m_renderI = Vec3(1.0f, 0.0f, 0.0f);
		Vec3				m_renderJ = Vec3(0.0f, 1.0f, 0.0f);
		Vec3				m_renderK = Vec3(0.0f, 0.0f, 1.0f);

		Vec2				m_bottomLeft;
		Vec2				m_topRight;

		Mat44				m_perspectiveCameraMatrix;
		Mat44				m_orthogrphicCameraMatrix;
		Mat44				m_stereoScopicCameraMatrix;
		Mat44				m_stereoScopicEyeMatrix;
		Mat44				m_lookAtMatrix;

};


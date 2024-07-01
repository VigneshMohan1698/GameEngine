#pragma once
#include <Engine\Math\Vec3.hpp>
#include <Engine\Math\EulerAngles.hpp>
#include <Engine\Core\Model.hpp>
#include <Engine\Material\Material.hpp>
#include <Engine\Renderer\Camera.hpp>

struct TransformComponent
{
	Vec3 m_position;
	EulerAngles m_orientationDegrees;
	float m_scale;

	//Get model Matrix and do the transform on CPU?
	//Or bind it to a shader and do it on GPU?
	Mat44 GetTransformMatrix() {
		Mat44 translationMatrix = Mat44::CreateTranslation3D(m_position);
		Mat44 rotationMatrix = m_orientationDegrees.GetAsMatrix_XFwd_YLeft_ZUp();
		translationMatrix.Append(rotationMatrix);
		return translationMatrix;
	}
};

struct MeshComponent
{
	Mesh			m_mesh;
	Material		m_material;
};

struct CameraComponent
{
	Camera			m_camera;
	bool			m_mainUICamera = false;
	bool			m_main3DCamera = false;
};
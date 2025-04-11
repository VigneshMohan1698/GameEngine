#pragma once
#include <Engine\Math\Vec3.hpp>
#include <Engine\Math\EulerAngles.hpp>
#include <Engine\Core\Model.hpp>
#include <Engine\Material\Material.hpp>
#include <Engine\Renderer\Camera.hpp>
#include <Engine\Core\FileUtils.hpp>

enum class LightType 
{
	Directional,
	Spot,
	Point,
	Area,
	NotDefined
};

enum class UIType
{
	Text,
	Window,
	Checkbox
};


struct TransformComponent
{
	Vec3 m_position;
	EulerAngles m_orientationDegrees;
	float m_scale = 1.0f;

	//Use this for dirty tracking. 
	Mat44 m_transformMatrix = Mat44();

	//Get model Matrix and do the transform on CPU?
	//Or bind it to a shader and do it on GPU?
	Mat44 GetTransformMatrix() {
		Mat44 translationMatrix = Mat44::CreateTranslation3D(m_position);
		Mat44 scaleMatrix = Mat44::CreateUniformScale3D(m_scale);
		Mat44 rotationMatrix = m_orientationDegrees.GetAsMatrix_XFwd_YLeft_ZUp();

		//-----------------SRT-------------
		rotationMatrix.Append(scaleMatrix);
		translationMatrix.Append(rotationMatrix);
		m_transformMatrix = translationMatrix;
		return translationMatrix;
	}
};

struct UITextComponent
{
	UITextComponent() {};
	~UITextComponent() {};
	std::string m_text = "";
	Rgba8		m_color = Rgba8();
	AABB2		m_bounds = AABB2::ZERO_TO_ONE;
	float		m_textHeight = DefaultUITextHeight;
	bool		m_isDebugRender = false;
};


struct MeshComponent
{
public:

	MeshComponent();
	~MeshComponent();
	Mesh*			m_mesh = new Mesh();
	Material		m_material;

public:
	void			Add3DModelMesh(std::string modelName);
};

struct CameraComponent
{
	Camera			m_camera;
	bool			m_mainUICamera = false;
	bool			m_main3DCamera = false;

	void LookAt(const Vec3& position, const Vec3& up)
	{
		m_camera.SetLookAt(position, up);
	}
};

struct LightComponent
{
	LightType       m_type = LightType::NotDefined;
	bool			m_isShadowed = false;
};
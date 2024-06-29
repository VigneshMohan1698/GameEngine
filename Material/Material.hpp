#pragma once

enum class MaterialType
{
	Basic2D,
	Basic3D,
	Lit3D,
	PBR3D,
	Custom
};

enum class MaterialTextureIndexes
{
	Albedo,
	Normal,
	Roughness,
	Metallic,
	Count
};

class ShaderD12;
class Material
{
	public:
	Material(const MaterialType& type);
	Material(ShaderD12* customShader);
	~Material();

	private:
	MaterialType			m_type;
	int						m_textures[(int)MaterialTextureIndexes::Count];
	ShaderD12*				m_shader;

};
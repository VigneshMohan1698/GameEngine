#pragma once

enum class MaterialType
{
	Basic2D,
	Basic3D,
	Basic3DNoColor,
	Lit3D,
	PBR3D,
	Custom,
	NoMaterial
};

enum class TextureIndexes
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
	Material() {};
	Material(MaterialType type);
	Material(ShaderD12* customShader);
	~Material() { m_shader  = nullptr; };

	ShaderD12*			   GetShader() {return m_shader;}
	MaterialType		   GetMaterialType() {return m_type;}
	int					   GetAlbedoTextureIndex() {  return m_texturesIndexes[(int)TextureIndexes::Albedo]; }

	public:
	MaterialType			m_type = MaterialType::NoMaterial;
	int						m_texturesIndexes[(int)TextureIndexes::Count] = {};
	ShaderD12*				m_shader = nullptr;

};
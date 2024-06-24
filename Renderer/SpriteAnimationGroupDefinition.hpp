#pragma once
#include <string>
#include <ThirdParty/TinyXML2/tinyxml2.h>


#include <Engine/Core/SpriteAnimDefinition.hpp>

//------------------------------------------------------------------------------------------------
class Camera;
class Shader;
class SpriteSheet;
class Texture;
struct Vec3;
typedef tinyxml2::XMLElement XmlElement;
//------------------------------------------------------------------------------------------------
class SpriteAnimationGroupDefinition
{
public:
	bool LoadFromXmlElement( const XmlElement& element, Renderer* renderer);
	bool LoadFromXmlElementDx12( const XmlElement& element, RendererD12* rendererD12);

	const SpriteAnimDefinition& GetAnimationForDirection( const Vec3& direction ) const;
	float GetDuration() const;

	std::string m_name;

	Shader* m_shader;
	SpriteSheet* m_spriteSheet; 
	RendererD12* m_rendererD12 = nullptr;
	Renderer* m_renderer = nullptr;
	float m_fps = 1.0f;
	SpriteAnimPlaybackType m_playbackMode = SpriteAnimPlaybackType::LOOP;
	bool m_scaleBySpeed = false;

	std::vector<Vec3> m_directions;
	std::vector<SpriteAnimDefinition> m_spriteAnimationDefinitions;
};

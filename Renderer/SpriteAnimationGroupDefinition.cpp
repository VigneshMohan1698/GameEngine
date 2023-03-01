#include "Engine/Renderer/SpriteAnimationGroupDefinition.hpp"
#include <Engine/Core/XmlUtils.hpp>
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/RendererD12.hpp"
#include <Engine/Math/MathUtils.hpp>

bool SpriteAnimationGroupDefinition::LoadFromXmlElement(const XmlElement& element, Renderer* renderer)
{
	m_renderer = renderer;
	std::string defaultValue = "";

	m_name = ParseXmlAttribute(element, "name", defaultValue);
	std::string shaderName = ParseXmlAttribute(element, "shader", defaultValue);
	m_shader = m_renderer->CreateOrGetShader(shaderName.c_str());

	m_fps = ParseXmlAttribute(element, "secondsPerFrame", 0.0f);
	std::string textureFilePath = ParseXmlAttribute(element, "spriteSheet", defaultValue);
	Texture* texture = m_renderer->CreateOrGetTextureFromFile(textureFilePath.c_str());
	IntVec2 gridLayout = ParseXmlAttribute(element, "cellCount", IntVec2());

	m_spriteSheet = new SpriteSheet(*texture, gridLayout);
	m_scaleBySpeed = ParseXmlAttribute(element, "scaleBySpeed", false);

	for (const tinyxml2::XMLElement* elements = element.FirstChildElement(); elements; elements = elements->NextSiblingElement())
	{
		Vec3 direction = ParseXmlAttribute(*elements, "vector", Vec3());
		direction.GetNormalized();
		m_directions.push_back(direction);
		int startFrame = ParseXmlAttribute(*elements->FirstChildElement(), "startFrame",0);
		int endFrame = ParseXmlAttribute(*elements->FirstChildElement(), "endFrame",0);
		float duration = (endFrame - startFrame) == 0 ? m_fps : (endFrame - startFrame) * m_fps;
		SpriteAnimDefinition def(*m_spriteSheet, startFrame,endFrame , duration,m_playbackMode);
		m_spriteAnimationDefinitions.push_back(def);
	}

    return false;
}

bool SpriteAnimationGroupDefinition::LoadFromXmlElementDx12(const XmlElement& element, RendererD12* rendererD12)
{
	m_rendererD12 = rendererD12;
	std::string defaultValue = "";

	m_name = ParseXmlAttribute(element, "name", defaultValue);
	std::string shaderName = ParseXmlAttribute(element, "shader", defaultValue);

	m_fps = ParseXmlAttribute(element, "secondsPerFrame", 0.0f);
	std::string textureFilePath = ParseXmlAttribute(element, "spriteSheet", defaultValue);
	Texture* nulltexture = nullptr;
	
	IntVec2 gridLayout = ParseXmlAttribute(element, "cellCount", IntVec2());
	IntVec2 spriteSheetDimensions = ParseXmlAttribute(element, "spriteSheetDimensions", IntVec2());
	m_spriteSheet = new  SpriteSheet(*nulltexture, spriteSheetDimensions, gridLayout);

	m_scaleBySpeed = ParseXmlAttribute(element, "scaleBySpeed", false);

	for (const tinyxml2::XMLElement* elements = element.FirstChildElement(); elements; elements = elements->NextSiblingElement())
	{
		Vec3 direction = ParseXmlAttribute(*elements, "vector", Vec3());
		direction.GetNormalized();
		m_directions.push_back(direction);
		int startFrame = ParseXmlAttribute(*elements->FirstChildElement(), "startFrame", 0);
		int endFrame = ParseXmlAttribute(*elements->FirstChildElement(), "endFrame", 0);
		float duration = (endFrame - startFrame) == 0 ? m_fps : (endFrame - startFrame) * m_fps;
		SpriteAnimDefinition def(*m_spriteSheet, startFrame, endFrame, duration, m_playbackMode);
		m_spriteAnimationDefinitions.push_back(def);
	}

	return false;
}


const SpriteAnimDefinition& SpriteAnimationGroupDefinition::GetAnimationForDirection(const Vec3& direction) const
{
	int directionIndex = 0;
	float maxDot = -100.0f;
	float dot;
	for (int i = 0; i < (int)m_spriteAnimationDefinitions.size(); i++)
	{
		Vec3 animationDirection = m_directions[i].GetNormalized();
		dot = DotProduct3D(direction, animationDirection);
		if (dot > maxDot)
		{
			maxDot = dot;
			directionIndex = i;
		}	
	}
	return m_spriteAnimationDefinitions[directionIndex];
}

float SpriteAnimationGroupDefinition::GetDuration() const
{
	if (&m_spriteAnimationDefinitions == nullptr)
	{
		return 0.0f;
	}
	return m_spriteAnimationDefinitions[0].GetDuration();
}



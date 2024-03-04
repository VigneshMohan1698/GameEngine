
#ifndef __APPLE__
#include "SpriteAnimDefinition.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/XmlUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"


SpriteAnimDefinition::SpriteAnimDefinition(const SpriteSheet& sheet, int startSpriteIndex, int endSpriteIndex, SpriteAnimPlaybackType playbackMode): m_spriteSheet(sheet), m_endSpriteIndex(endSpriteIndex),
m_startSpriteIndex(startSpriteIndex), m_playbackType(playbackMode)
{

}
bool SpriteAnimDefinition::LoadFromXmlElement(const XmlElement& element)
{
    std::string defaultValue = "";
    std::string textureFilePath = ParseXmlAttribute(element, "spriteSheet", defaultValue);
    m_name = ParseXmlAttribute(element, "name", defaultValue);
    m_fps = ParseXmlAttribute(element, "secondsPerFrame", 0.0f);
    m_startSpriteIndex = ParseXmlAttribute(element, "startFrame", 0);
    m_endSpriteIndex = ParseXmlAttribute(element, "endFrame", 0);
    m_playbackType = SpriteAnimPlaybackType::ONCE;
    return false;
}
//
//SpriteAnimDefinition::SpriteAnimDefinition()
//{
//   
//}
SpriteAnimDefinition::SpriteAnimDefinition(const SpriteSheet& sheet, int startSpriteIndex, int endSpriteIndex, float durationSeconds,
    SpriteAnimPlaybackType playbackType): m_spriteSheet(sheet),m_endSpriteIndex(endSpriteIndex),
    m_durationSeconds(durationSeconds),m_startSpriteIndex(startSpriteIndex),m_playbackType(playbackType)
{
    m_numberOfSpritesToPlay = endSpriteIndex - startSpriteIndex;
}

SpriteAnimDefinition::SpriteAnimDefinition(const SpriteSheet& sheet, int startSpriteIndex, int endSpriteIndex, float durationSeconds, Shader* shader, SpriteAnimPlaybackType playbackType) : m_spriteSheet(sheet), m_endSpriteIndex(endSpriteIndex),
m_durationSeconds(durationSeconds), m_startSpriteIndex(startSpriteIndex),m_shader(shader), m_playbackType(playbackType)
{

}

const SpriteDefinition& SpriteAnimDefinition::GetSpriteDefAtTime(float seconds) const
 {
	int numOfFramesThatRepeat = 0;
	if (m_playbackType == SpriteAnimPlaybackType::PINGPONG)
		numOfFramesThatRepeat = (m_endSpriteIndex - m_startSpriteIndex) * 2;
	else
		numOfFramesThatRepeat = m_endSpriteIndex - m_startSpriteIndex + 1;

	if (m_playbackType == SpriteAnimPlaybackType::ONCE && seconds >= m_durationSeconds)
		return m_spriteSheet.GetSpriteDef(m_endSpriteIndex);

	int frameIndexInRepititionSet = 0;
	int indexOfSpriteToRender = 0;
	if (m_durationSeconds > 0.f)
	{
		seconds = fmodf(seconds, m_durationSeconds);
		frameIndexInRepititionSet = static_cast<int>((seconds / m_durationSeconds) * numOfFramesThatRepeat);
	}

	if (m_playbackType == SpriteAnimPlaybackType::LOOP)
		indexOfSpriteToRender = m_startSpriteIndex + frameIndexInRepititionSet;
	else
	{
		if (frameIndexInRepititionSet < (numOfFramesThatRepeat / 2))
			indexOfSpriteToRender = m_startSpriteIndex + frameIndexInRepititionSet;
		else
		{
			if (frameIndexInRepititionSet == 0)
				indexOfSpriteToRender = m_endSpriteIndex;
			else
				indexOfSpriteToRender = m_endSpriteIndex - (frameIndexInRepititionSet % (numOfFramesThatRepeat / 2));
		}
	}

	return m_spriteSheet.GetSpriteDef(indexOfSpriteToRender);
   

}
//
//bool SpriteAnimDefinition::LoadFromXmlElement(const XmlElement& element)
//{
//    return false;
//}

const float SpriteAnimDefinition::GetDuration() const
{
    return m_durationSeconds;
}

const int SpriteAnimDefinition::GetTotalFramesInCycle() const
{
    return 0;
}

void SpriteAnimDefinition::SetTotalDuration(float totalSeconds)
{
    UNUSED((void)totalSeconds);
}

void SpriteAnimDefinition::SetSecondsPerFrame(float secondsPerFrame)
{
    UNUSED((void)secondsPerFrame);
}

void SpriteAnimDefinition::SetFramesPerSecond(float fps)
{
    UNUSED((void)fps);
}

#endif

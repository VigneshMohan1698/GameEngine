#ifndef __APPLE__
#pragma once
#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/Shader.hpp"
#include <ThirdParty/TinyXML2/tinyxml2.h>


typedef tinyxml2::XMLElement XmlElement;

enum class SpriteAnimPlaybackType
{
	ONCE,		// for 5-frame anim, plays 0,1,2,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4...
	LOOP,		// for 5-frame anim, plays 0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0...
	PINGPONG,	// for 5-frame anim, plays 0,1,2,3,4,3,2,1,0,1,2,3,4,3,2,1,0,1,2,3,4,3,2,1,0,1...
};

class SpriteAnimDefinition
{
public:
	SpriteAnimDefinition();

	bool LoadFromXmlElement(const XmlElement& element);
	SpriteAnimDefinition(const SpriteSheet& sheet, int startSpriteIndex, int endSpriteIndex,
		float durationSeconds, SpriteAnimPlaybackType playbackType = SpriteAnimPlaybackType::LOOP);

	SpriteAnimDefinition(const SpriteSheet& sheet, int startSpriteIndex, int endSpriteIndex,
		float durationSeconds,Shader* shader, SpriteAnimPlaybackType playbackType = SpriteAnimPlaybackType::LOOP);

	SpriteAnimDefinition(const SpriteSheet& sheet, int startSpriteIndex, int endSpriteIndex, SpriteAnimPlaybackType playbackMode);

	const SpriteDefinition& GetSpriteDefAtTime(float seconds) const;


	const float GetDuration() const;
	const int GetTotalFramesInCycle() const;

	void SetTotalDuration(float totalSeconds);
	void SetSecondsPerFrame(float secondsPerFrame);
	void SetFramesPerSecond(float fps);
protected:
	int										m_numberOfSpritesToPlay = 0;
private:
	const SpriteSheet&						m_spriteSheet;
	int										m_startSpriteIndex = -1;
	int										m_endSpriteIndex = -1;
	float									m_durationSeconds = 1.f;
	SpriteAnimPlaybackType					m_playbackType = SpriteAnimPlaybackType::LOOP;
	Shader*									m_shader = nullptr;
	float									m_fps = 1.f;
	std::string								m_name;
};


#endif

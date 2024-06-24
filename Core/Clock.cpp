#include "Clock.hpp"
#include "Engine/Core/Time.hpp"

static Clock g_systemClock;
Clock::Clock()
{
	if (this != &g_systemClock)
	{
		SetParent(g_systemClock);
		g_systemClock.AddChild(this);
	}
}

Clock::Clock(Clock& parent)
{
	SetParent(parent);
	parent.AddChild(this);
}

void Clock::SetParent(Clock& newParent)
{
	if (m_parent != nullptr)
	{
		m_parent->RemoveChild(this);
	}
	m_parent = &newParent;
}

Clock::~Clock()
{
	if (m_parent && this)
		m_parent->RemoveChild(this);

	//set parent of all children to system clock
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] != nullptr)
		{
			m_children[i]->SetParent(g_systemClock);
		}
	}
}

void Clock::SystemBeginFrame()
{
	g_systemClock.Tick();
}

Clock& Clock::GetSystemClock()
{
	return g_systemClock;
}
Clock* Clock::GetSystemClockPointer()
{
	return &g_systemClock;
}
void Clock::Tick()
{
	if (this != nullptr)
	{
		double timeNow = GetCurrentTimeSeconds();
		double deltaTime = timeNow - m_lastUpdateTime;
		m_lastUpdateTime = timeNow;
		Advance(deltaTime);
	}
}

void Clock::Advance(double deltaTimeSeconds)
{

	if (m_pauseAfterFrame)
	{
		Pause();
		m_pauseAfterFrame = false;
	}
	else if (m_isPaused)
	{
		m_deltaTime = 0.0;
		return;
	}

	m_deltaTime = deltaTimeSeconds * m_timeDilation;
	m_frameCount++;
	m_totalTime += m_deltaTime;
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] != nullptr)
		{
			m_children[i]->Advance(deltaTimeSeconds);
		}
	}
}

void Clock::AddChild(Clock* childClock)
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] == nullptr)
		{
			m_children[i] = childClock;
			return;
		}
	}
	m_children.push_back(childClock);
}
void Clock::RemoveChild(Clock* childClock)
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] == childClock)
		{
			m_children[i] = nullptr;
		}
	}
}

void Clock::Pause()
{
	m_isPaused = true;
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Pause();
	}
}

void Clock::Unpause()
{
	m_isPaused = true;
}

void Clock::TogglePause()
{
	m_isPaused = !m_isPaused;
}

void Clock::StepFrame()
{
	m_pauseAfterFrame = true;
}

void Clock::SetTimeDilation(double dilationAmount)
{
	m_timeDilation = dilationAmount;
}

double Clock::GetDeltaTime() const
{
	return (m_deltaTime);
}

double Clock::GetTotalTime() const
{
	return m_totalTime;
}

size_t Clock::GetFrameCount() const
{
	return m_frameCount;
}

bool Clock::GetIsPaused() const
{
	return m_isPaused;
}

double Clock::GetTimeDilation() const
{
	return m_timeDilation;
}

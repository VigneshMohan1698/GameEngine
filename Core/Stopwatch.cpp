#include "Stopwatch.hpp"
#include "Clock.hpp"
Stopwatch::Stopwatch()
{
	m_clock = Clock::GetSystemClockPointer();
}

Stopwatch::Stopwatch(double duration)
{
	m_clock = Clock::GetSystemClockPointer();
	Start(duration);
}

Stopwatch::Stopwatch(const Clock* clock, double duration)
{
	Start(clock, duration);
}

void Stopwatch::Start(double duration)
{
	m_duration = duration;
	m_startTime = m_clock->GetTotalTime();
}

void Stopwatch::Start(const Clock* clock, double duration)
{
	m_clock = clock;
	m_duration = duration;
	m_startTime = clock->GetTotalTime();
}

void Stopwatch::Restart()
{
	m_startTime = m_clock->GetTotalTime();
}

void Stopwatch::Stop()
{
	m_duration = 0.0;
	m_isStopped = true;
}

void Stopwatch::Pause()
{
	m_isPaused = true;
	m_pausedTime = m_clock->GetTotalTime();
}

void Stopwatch::Resume()
{
	m_isPaused = false;
}

double Stopwatch::GetElapsedTime() const
{
	double returnValue = 0.0;
	if (m_isStopped)
		returnValue = 0.0;
	else if (m_isPaused)
		returnValue = m_pausedTime - m_startTime;
	else
		returnValue = m_clock->GetTotalTime() - m_startTime;

	return returnValue;
}

float Stopwatch::GetElapsedFraction() const
{
	float returnValue = 0.0f;
	returnValue = (float)(GetElapsedTime() / m_duration);
	return returnValue;
}

bool Stopwatch::IsStopped() const
{
	return m_isStopped;
}

bool Stopwatch::IsPaused() const
{
	return m_isPaused;
}

bool Stopwatch::HasDurationElapsed() const
{
	if (GetElapsedTime() > m_duration)
		return true;
	else
		return false;
}

bool Stopwatch::CheckDurationElapsedAndDecrement()
{
	if (m_isStopped)
	{
		return false;
	}
	if (GetElapsedFraction() < 1.0f)
		return false;
	m_startTime += m_duration;
	return true;


}

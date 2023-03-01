#include "Engine/Core/JobSystem.hpp"


JobSystem::JobSystem(JobSystemConfig config)
{
	m_config = config;
}

JobSystem::~JobSystem()
{
	
}


void JobSystem::Startup()
{
	m_workerThreads.reserve(m_config.m_numWorkerThreads);
	for (int i = 0; i < m_config.m_numWorkerThreads; ++i)
	{
		JobWorker* newWorkerThread = new JobWorker(this, i);
		m_workerThreads.push_back(newWorkerThread);
	}
}

void JobSystem::ShutDown()
{
	for (int i = 0; i < (int)m_workerThreads.size(); i++)
	{
		if (m_workerThreads[i])
		{
			delete m_workerThreads[i];
			m_workerThreads[i] = nullptr;
		}
	}
}

Job* JobSystem::RetrieveJobToExecute()
{
	m_jobsQueuedMutex.lock();
	Job* queudjob = nullptr;
	if (!m_jobsQueued.empty())
	{
		queudjob = m_jobsQueued.front();
		m_jobsQueued.pop_front();
	}
	m_jobsQueuedMutex.unlock();
	return queudjob;
}

Job* JobSystem::RetrieveCompletedJobs()
{
	m_completedJobsMutex.lock();
	Job* completedJob = nullptr;
	if (!m_completedJobs.empty())
	{
		completedJob = m_completedJobs.front();
		m_completedJobs.pop_front();
	}
	m_completedJobsMutex.unlock();
	return completedJob;
}

void JobSystem::MoveToCompletedJobs(Job* job)
{
	m_completedJobsMutex.lock();
	m_completedJobs.push_back(job);
	m_completedJobsMutex.unlock();
}

void JobSystem::MoveToExecutingJobs(Job* job)
{
	m_executingJobsMutex.lock();
	for (int i = 0; i < (int)m_executingJobs.size(); i++)
	{
		if (m_executingJobs[i] == nullptr)
		{
			m_executingJobs[i] = job;
			m_executingJobsMutex.unlock();
			return;
		}
	}
	m_executingJobs.push_back(job);
	m_executingJobsMutex.unlock();
	return;

}

void JobSystem::RemoveFromExecutingJobs(Job* job)
{
	m_executingJobsMutex.lock();
	for (int i = 0 ; i < (int)m_executingJobs.size(); i++)
	{
		if (m_executingJobs[i] == job)
		{
			m_executingJobs[i] = nullptr;
		}
	}
	m_executingJobsMutex.unlock();
}


void JobSystem::BeginFrame()
{
}

void JobSystem::EndFrame()
{
}


bool JobSystem::CheckQueuedJobsOfType(int JobType)
{
	m_jobsQueuedMutex.lock();
	for (const auto& jobIterator : m_jobsQueued)
	{
		if (jobIterator->m_jobType == JobType)
		{
			m_jobsQueuedMutex.unlock();
			return true;
		}
	}
	m_jobsQueuedMutex.unlock();
	return false;
}

bool JobSystem::CheckExecutingJobsOfType(int JobType)
{
	m_executingJobsMutex.lock();
	for (const auto& jobIterator : m_executingJobs)
	{
		if(jobIterator != nullptr && jobIterator->m_jobType == JobType)
		{
			m_executingJobsMutex.unlock();
			return true;
		}
	}
	m_executingJobsMutex.unlock();
	return false;
}

bool JobSystem::CheckCompletedJobsOfType(int JobType)
{
	m_completedJobsMutex.lock();
	for (const auto& jobIterator : m_completedJobs)
	{
		if (jobIterator->m_jobType == JobType)
		{
			m_completedJobsMutex.unlock();
			return true;
		}
	}
	m_completedJobsMutex.unlock();
	return false;
}


void JobSystem::QueueJob(Job* jobToQueue)
{
	m_jobsQueuedMutex.lock();
	m_jobsQueued.push_back(jobToQueue);
	m_jobsQueuedMutex.unlock();
}

JobSystemConfig JobSystem::GetConfig()
{
	return JobSystemConfig();
}

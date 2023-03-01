#include "Engine/Core/JobWorker.hpp"
#include <chrono>
#include "Engine/Core/JobSystem.hpp"
#include "Game/GameCommon.hpp"


Job::Job(int jobType)
{
	m_jobType = jobType;
}

JobWorker::JobWorker(JobSystem* jobsystem, int workerThreadID)
{
	m_JobSystem = jobsystem;
	m_id = workerThreadID;
	m_thread = new std::thread(&JobWorker::JobWorkerMain, this, workerThreadID);
}

JobWorker::~JobWorker()
{
	m_isQuitting = true;
	m_thread->join();
	delete m_thread;
	m_thread = nullptr;
}

void JobWorker::JobWorkerMain(int id)
{
	UNUSED((void)id);
	while (!m_isQuitting)
	{
		Job* jobToExecute = m_JobSystem->RetrieveJobToExecute();
		if (jobToExecute != nullptr)
		{
			m_JobSystem->MoveToExecutingJobs(jobToExecute);
			jobToExecute->Execute();
			m_JobSystem->RemoveFromExecutingJobs(jobToExecute);
			m_JobSystem->MoveToCompletedJobs(jobToExecute);
		}
		else
		{
			std::this_thread::yield();
		}
	}
}


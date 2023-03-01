#pragma once
#include <deque>
#include <mutex>
#include "Engine/Core/JobWorker.hpp"

struct JobSystemConfig
{
	int	m_numWorkerThreads = 0;
};

class JobSystem
{
	public:
		//-------------------CONSTRUCTOR/DESTRUCTOR-------------------
		JobSystem(JobSystemConfig config);
		~JobSystem();
	public:
		//-------------------MAIN FUNCTIONS-------------------------
		void Startup();
		void BeginFrame();
		void EndFrame();
		void ShutDown();
		
		//-------------------JOB SYSTEM FUNCTIONS-------------
		Job* RetrieveJobToExecute();
		Job* RetrieveCompletedJobs();
		void MoveToCompletedJobs(Job* job);
		void MoveToExecutingJobs(Job* job);
		void RemoveFromExecutingJobs(Job* job);
		void QueueJob(Job* jobToQueue);

		bool CheckQueuedJobsOfType(int JobType);
		bool CheckExecutingJobsOfType(int JobType);
		bool CheckCompletedJobsOfType(int JobType);

		JobSystemConfig GetConfig();

	public:
		JobSystemConfig				m_config;
		std::vector<JobWorker*>		m_workerThreads;
		std::deque<Job*>			m_jobsQueued;
		std::mutex					m_jobsQueuedMutex;
		std::deque<Job*>			m_completedJobs;
		std::mutex					m_completedJobsMutex;
		std::vector<Job*>			m_executingJobs;
		std::mutex					m_executingJobsMutex;
};

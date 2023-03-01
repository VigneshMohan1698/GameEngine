#pragma once

#include <atomic>
#include <thread>
#include <vector>

class JobSystem;

class Job
{
	friend class JobWorker;
public:
	//-------------------CONSTRUCTOR/DESTRUCTOR-------------------
	Job(int jobType);
	int			 m_jobType;
private:
	virtual void Execute() = 0;
	virtual void OnFinished() = 0;

};

class JobWorker
{
public:
	//-------------------CONSTRUCTOR/DESTRUCTOR-------------------
	JobWorker(JobSystem* jobsystem, int workerThreadID);
	~JobWorker();

	//------------------MAIN FUNCTIONS---------------------
	void JobWorkerMain(int id);

public:
	std::thread*				m_thread = nullptr;
	int							m_id;
	std::atomic<bool>			m_isQuitting;
	JobSystem*					m_JobSystem = nullptr;
};


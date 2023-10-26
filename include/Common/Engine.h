#pragma once

#include "base.h"

#include "JobSystem.h"

namespace And
{

	struct ThreadsData
	{
		bool m_Stop;
		std::mutex m_MapMutex;
		std::mutex m_QueueMutex;
		std::mutex m_ResourceQueueMutex;
		std::condition_variable m_Condition;
		std::condition_variable m_ResourceCondition;
		std::queue<internal::job> m_JobsQueue;
		std::queue<internal::job> m_ResourceJobsQueue;
		std::unordered_map<size_t, std::vector<internal::job>> m_FutureJobUnion;
	};

	class Engine final
	{		
		NON_COPYABLE_CLASS(Engine)
		NON_MOVABLE_CLASS(Engine)
	public:
		Engine();
		~Engine();

		bool is_initialized() const;

	private:
		bool m_Initialized;
	};
}
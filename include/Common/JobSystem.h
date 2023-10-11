#pragma once

#include "base.h"

namespace And
{

	class JobSystem
	{
		NON_COPYABLE_CLASS(JobSystem)
		NON_MOVABLE_CLASS(JobSystem)
	public:
		JobSystem() : m_Stop(false)
		{
			int num_threads = std::thread::hardware_concurrency();

			printf("Num Threads: %d\n", num_threads);
			for (int i = 0; i < num_threads; i++)
			{
				m_Threads.push_back(std::thread{ [this]() {
						{
							std::lock_guard<std::mutex> lock(m_QueueMutex);
							std::cout << "Thread Created [" << std::this_thread::get_id() << "]" << std::endl;
						}
						while (true)
						{
							std::function<void()> job;
							{
								std::unique_lock<std::mutex> lock{m_QueueMutex};
								m_Condition.wait(lock, [this]() {return !m_JobsQueue.empty() || m_Stop; });
								if (m_JobsQueue.empty() && m_Stop) return;
								job = std::move(m_JobsQueue.front());
								m_JobsQueue.pop();
							}
							job();
						}
					} });
			}
		}

		~JobSystem()
		{
			m_Stop = true;
			for (std::thread& thread : m_Threads)
			{
				if (thread.joinable())
				{
					thread.join();
				}
			}
		}

		template<typename func_t>
		auto add_job(func_t function) -> std::future<decltype(function())>
		{
			using ret_t = decltype(function());

			std::shared_ptr<std::packaged_task<ret_t()>> job = std::make_shared<std::packaged_task<ret_t()>>(function);
			std::future<ret_t> future = job->get_future();

			{
				std::lock_guard<std::mutex> lock{ m_QueueMutex };
				m_JobsQueue.push([job]() { (*job)(); });
			}
			m_Condition.notify_one();
			return future;
		}

	private:
		bool m_Stop;
		std::vector<std::thread> m_Threads;
		std::queue<std::function<void()>> m_JobsQueue;
		std::mutex m_QueueMutex;
		std::condition_variable m_Condition;
	};
}
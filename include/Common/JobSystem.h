#pragma once

#include "base.h"

#include "future.h"

namespace And
{
	class JobSystem;

	namespace internal
	{
		class job_base
		{
		public:
			job_base() = default;
			virtual ~job_base() = default;

			virtual void operator()() = 0;
			virtual bool is_available() const = 0;
			virtual future_availability get_future_availability() = 0;
		};

		template<typename ret_t, typename... args_t>
		class job_imp : public job_base
		{
		public:
			job_imp(std::packaged_task<ret_t(args_t...)>& task, future<args_t>... args) : m_Args(args...)
			{
				m_Task = std::move(task);
				std::future<ret_t> std_f = m_Task.get_future();
				std::shared_ptr<future_imp<ret_t>> f(new future_imp<ret_t>(std_f));
				m_Future.m_Future = f;
			}
			virtual ~job_imp() = default;

			virtual void operator()() override
			{
				return call(std::make_integer_sequence<int, sizeof...(args_t)>{});
			}

			virtual bool is_available() const
			{
				return are_available_all(std::make_integer_sequence<int, sizeof...(args_t)>{});
			}

			virtual future_availability get_future_availability() override
			{
				return future_availability{ m_Future };
			}

			future<ret_t> get_future() const
			{
				return m_Future;
			}

		private:

			template<int... ints>
			void call(std::integer_sequence<int, ints...> int_seq)
			{
				return m_Task((std::get<ints>(m_Args).get())...);
			}

			template<int... ints>
			bool are_available_all(std::integer_sequence<int, ints...> int_seq) const
			{
				return (std::get<ints>(m_Args).is_available() && ...);
			}

			std::packaged_task<ret_t(args_t...)> m_Task;
			std::tuple<future<args_t>...> m_Args;
			future<ret_t> m_Future;
		};

		class job
		{
		private:
			job(bool resource = false) { m_Resource = resource; };

		public:
			job(const job& other) { m_Job = other.m_Job; m_Id = other.m_Id; m_Resource = other.m_Resource; }
			job(job&& other) { m_Job = std::move(other.m_Job); std::swap(m_Id, other.m_Id); std::swap(m_Resource, other.m_Resource); }

			~job() = default;

			job& operator =(const job& other) { if (this != &other) { m_Job = other.m_Job; m_Id = other.m_Id; m_Resource = other.m_Resource; } return *this; }
			job& operator =(job& other) { if (this != &other) { m_Job = std::move(other.m_Job); std::swap(m_Id, other.m_Id); std::swap(m_Resource, other.m_Resource); } return *this; }

			size_t get_id() const
			{
				return m_Id;
			}

			void operator()()
			{
				return (*m_Job)();
			}

			bool is_available() const
			{
				return m_Job->is_available();
			}

			future_availability get_future_availability() const
			{
				return m_Job->get_future_availability();
			}

			bool is_resource() const
			{
				return m_Resource;
			}

			friend class ResourceManager;
			friend class JobSystem;
		private:
			std::shared_ptr<job_base> m_Job;
			size_t m_Id = 0;
			bool m_Resource;
		};
	}

	class JobSystem
	{
	public:
		NON_COPYABLE_CLASS(JobSystem)
		NON_MOVABLE_CLASS(JobSystem)
	public:
		JobSystem() : m_Stop(false)
		{
			int num_threads = std::thread::hardware_concurrency();
			for (int i = 0; i < num_threads; i++)
			{
				m_Threads.push_back(std::thread{ [this]() {
						while (true)
						{
							internal::job j;
							{
								std::unique_lock<std::mutex> lock{m_QueueMutex};
								m_Condition.wait(lock, [this]() {return !m_JobsQueue.empty() || m_Stop; });
								if (m_JobsQueue.empty() && m_Stop) return;
								j = std::move(m_JobsQueue.front());
								m_JobsQueue.pop();
							}
							j();
							future_availability fa = j.get_future_availability();
							check_job_blocked(fa.get_id());
						}
					} });
			}
		}

		~JobSystem()
		{
			m_Stop = true;
			m_Condition.notify_all();
			for (std::thread& thread : m_Threads)
			{
				if (thread.joinable())
				{
					thread.join();
				}
			}
		}

		template<typename func_t, typename... args_t>
		auto add_job(func_t function, future<args_t>... args) -> future<decltype(function((args.get())...))>
		{
			using ret_t = decltype(function((args.get())...));
			std::packaged_task<ret_t(args_t...)> pt(function);
			internal::job j;
			std::shared_ptr<internal::job_imp<ret_t, args_t...>> ji(new internal::job_imp(pt, args...));

			future<ret_t> f = ji->get_future();

			j.m_Job = ji;
			j.m_Id = (size_t)j.m_Job.get();

			if (j.is_available())
			{
				m_JobsQueue.push(j);
			}
			else
			{
				add_job_blocked_to_union(f.get_id(), j);
			}

			return f;
		}

		friend class ResourceManager;
	private:

		void check_job_blocked(size_t future_id)
		{
			std::lock_guard<std::mutex> lock{ m_MapMutex };
			if (m_FutureJobUnion.contains(future_id))
			{
				int num_jobs = (int)m_FutureJobUnion[future_id].size();
				std::vector<internal::job>& jobs = m_FutureJobUnion[future_id];
				for (int i = 0; i < num_jobs; i++)
				{
					if (jobs[i].is_available())
					{
						internal::job j = jobs[i];
						jobs.erase(jobs.begin() + i);
						if (j.is_resource())
						{
							m_ResourceJobsQueue.push(j);
							m_ResourceCondition.notify_one();
						}
						else
						{
							m_JobsQueue.push(j);
							m_Condition.notify_one();
						}
					}
				}
			}
		}

		void add_job_blocked_to_union(size_t future_id, internal::job& j)
		{
			std::lock_guard<std::mutex> lock{ m_MapMutex };
			if (m_FutureJobUnion.contains(future_id))
			{
				m_FutureJobUnion[future_id].push_back(j);
			}
			else
			{
				m_FutureJobUnion.insert({ future_id, {j} });
			}
		}

		bool m_Stop;
		std::vector<std::thread> m_Threads;
		std::mutex m_MapMutex;
		std::mutex m_QueueMutex;
		std::mutex m_ResourceQueueMutex;
		std::condition_variable m_Condition;
		std::condition_variable m_ResourceCondition;
		std::queue<internal::job> m_JobsQueue;
		std::queue<internal::job> m_ResourceJobsQueue;
		std::unordered_map<size_t, std::vector<internal::job>> m_FutureJobUnion;
	};
}
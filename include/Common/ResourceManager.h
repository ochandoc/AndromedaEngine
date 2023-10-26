#pragma once

#include "resource.h"
#include "JobSystem.h"

#include "Common/OpenGLTexture2D.h"

namespace And
{
  class Window;

  class ResourceManager
  {
    NON_COPYABLE_CLASS(ResourceManager)
    NON_MOVABLE_CLASS(ResourceManager)
  public:
    ResourceManager(Window& w, JobSystem& js);

    ~ResourceManager();

		resource<OpenGLTexture2D> new_texture2D(std::string path);

  private:

		template<typename func_t, typename... args_t>
		auto add_resource(func_t function, future<args_t>... args) -> future<decltype(function((args.get())...))>
		{
			using ret_t = decltype(function((args.get())...));
			std::packaged_task<ret_t(args_t...)> pt(function);
			internal::job j(true);
			std::shared_ptr<internal::job_imp<ret_t, args_t...>> ji(new internal::job_imp(pt, args...));

			future<ret_t> f = ji->get_future();

			j.m_Job = ji;
			j.m_Id = (size_t)j.m_Job.get();

			if (j.is_available())
			{
				m_JobSystem.m_ResourceJobsQueue.push(j);
			}

			return f;
		}

    Window& m_Window;
		JobSystem& m_JobSystem;
		std::mutex& m_ResourceQueueMutex;
		std::condition_variable& m_ResourceCondition;
		std::queue<internal::job>& m_ResourceJobsQueue;
		bool& m_Stop;
    std::unique_ptr<struct ResourceManagerData> m_Data;
		std::unordered_map<size_t, std::function<void()>> m_SwapMap;
		std::unordered_map <size_t, std::shared_ptr<internal::resource_base>> m_Resources;
  };
}
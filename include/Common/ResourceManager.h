#pragma once

#include "resource.h"
#include "ResourceGenerator.h"
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

		template<typename T>
		void add_resource_generator()
		{
			size_t type_hash = typeid(T::ResourceType).hash_code();
			assert(!m_Generators.contains(type_hash) && "Generator class already inserted!");
			std::shared_ptr<internal::ResourceGeneratorBase> Generator = std::make_shared<T>();
			m_Generators.insert({type_hash, Generator});
		}

		template<typename T>
		resource<T> new_resource(const std::string& arg)
		{
			size_t type_hash = typeid(T).hash_code();
			ResourceGenerator<T>* Generator = dynamic_cast<ResourceGenerator<T>*>(m_Generators[type_hash].get());

			std::shared_ptr<internal::resource_value<T>> resourceValue = std::make_shared<internal::resource_value<T>>();

			resource<T> r;
			r.m_Resource = resourceValue;
			r.m_Resource->m_Value = Generator->GetDefault();
			r.m_Resource->m_Id = Generator->GenerateId(arg);

			
			if (m_Resources.contains(type_hash))
			{
				std::unordered_map<size_t, std::shared_ptr<internal::resource_base>>& resource_list = m_Resources[type_hash];
				if (resource_list.contains(r.m_Resource->m_Id))
				{
					r.m_Resource = std::static_pointer_cast<internal::resource_value<T>>(resource_list[r.m_Resource->m_Id]);
					return r;
				}
			}
			else
			{
				m_Resources.insert({ type_hash , std::unordered_map<size_t, std::shared_ptr<internal::resource_base>>() });
			}

			auto task = [Generator](const std::string& arguments) -> std::shared_ptr<T>
			{
				return (*Generator)(arguments);
			};

			m_ThreadsData->m_ResourceQueueMutex.lock();

			std::unordered_map<size_t, std::shared_ptr<internal::resource_base>>& resource_list = m_Resources[type_hash];
			resource_list.insert({ r.m_Resource->m_Id , std::static_pointer_cast<internal::resource_base>(resourceValue)});

			future<std::shared_ptr<T>> f = add_resource(task, future<std::string>(arg));
			std::function<void()> swap_function = [f, resourceValue, type_hash, this]()
			{ 
				resourceValue->m_Value = f.get();
			};

			m_SwapMap.insert({ f.get_id(), swap_function });

			m_ThreadsData->m_ResourceQueueMutex.unlock();
			m_ThreadsData->m_ResourceCondition.notify_one();

			return r;
		}

  private:

		template<typename func_t, typename... args_t>
		auto add_resource(func_t function, future<args_t>... args) -> future<decltype(function((args.get())...))>
		{
			using ret_t = decltype(function((args.get())...));
			std::packaged_task<ret_t(args_t...)> pt(function);
			internal::job j(true);
			std::shared_ptr<internal::job_imp<ret_t, args_t...>> ji(new internal::job_imp(pt, args...));

			future<ret_t> f = ji->get_future();

			std::cout << "[Main] Future Id: " << f.get_id() << "\n";

			j.m_Job = ji;
			j.m_Id = (size_t)j.m_Job.get();

			if (j.is_available())
			{
				m_ThreadsData->m_ResourceJobsQueue.push(j);
			}

			return f;
		}

		bool m_Stop;
    Window& m_Window;
		JobSystem& m_JobSystem;
		std::shared_ptr<ThreadsData> m_ThreadsData;
    std::unique_ptr<struct ResourceManagerData> m_Data;
		std::unordered_map<size_t, std::function<void()>> m_SwapMap;
		std::unordered_map<size_t, std::unordered_map<size_t, std::shared_ptr<internal::resource_base>>> m_Resources;
		std::unordered_map<size_t, std::shared_ptr<internal::ResourceGeneratorBase>> m_Generators;
  };
}
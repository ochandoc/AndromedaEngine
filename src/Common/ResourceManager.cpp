#include "Common/ResourceManager.h"

#include "Common/Window.h"

#include "GLFW/glfw3.h"


namespace And
{

  struct ResourceManagerData
  {
    std::unique_ptr<std::thread> thread;
    GLFWwindow* window = nullptr;
  };

  ResourceManager::ResourceManager(Window& w, JobSystem& js) : m_Window(w), m_JobSystem(js), m_Stop(false), m_Data(new ResourceManagerData)
  {
    m_ThreadsData = js.m_ThreadsData;
    GLFWwindow* main_window = (GLFWwindow*)w.get_native_window();

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#ifdef AND_OPENGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Version principal de OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // Version menor de OpenGL
#   ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); // Activar debug
#   endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Perfil de OpenGL
#endif
    m_Data->window = glfwCreateWindow(100, 100, "OpenGL", nullptr, main_window);

    m_Data->thread = std::make_unique<std::thread>([this]() {
      glfwMakeContextCurrent(m_Data->window);
      internal::job j;
      {
        std::unique_lock<std::mutex> lock{ m_ThreadsData->m_ResourceQueueMutex};
        m_ThreadsData->m_ResourceCondition.wait(lock, [this]() {return !m_ThreadsData->m_ResourceJobsQueue.empty() || m_Stop; });
        if (m_ThreadsData->m_ResourceJobsQueue.empty() && m_Stop) return;
        j = std::move(m_ThreadsData->m_ResourceJobsQueue.front());
        m_ThreadsData->m_ResourceJobsQueue.pop();
      }
      j();
      future_availability fa = j.get_future_availability();

      std::function<void()> swap = m_SwapMap[fa.get_id()];
      m_SwapMap.erase(fa.get_id());

      swap();
      });
  }

  ResourceManager::~ResourceManager()
  {
    m_Stop = true;
    m_ThreadsData->m_ResourceCondition.notify_all();
    if (m_Data->thread->joinable())
    {
      m_Data->thread->join();
    }
    glfwDestroyWindow(m_Data->window);
  }

  resource<OpenGLTexture2D> ResourceManager::new_texture2D(std::string path)
  {
    resource<OpenGLTexture2D> r;
    std::shared_ptr<internal::resource_value<OpenGLTexture2D>> value = std::make_shared<internal::resource_value<OpenGLTexture2D>>();
    r.m_Resource = value;
    r.m_Resource->m_Value = std::shared_ptr<OpenGLTexture2D>(new OpenGLTexture2D);
    r.m_Resource->m_Value->m_Id = 0;
    future<std::string> f_path(path);
    future<std::shared_ptr<OpenGLTexture2D>> f = add_resource([](std::string path) {std::this_thread::sleep_for(std::chrono::seconds(5)); return std::shared_ptr<OpenGLTexture2D>(new OpenGLTexture2D(path)); }, f_path);
    m_ThreadsData->m_ResourceCondition.notify_one();

    std::function<void()> swap_f = [f, value]() { value->m_Value = f.get(); };

    m_SwapMap.insert({f.get_id(), swap_f});

    return r;
  }

}
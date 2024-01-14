#include "Common/Resources/ResourceManager.h"

namespace And
{
  ResourceManager::ResourceManager(Window& w, TaskSystem& ts) : m_TaskSystem(ts)
  {
    WorkerCreationInfo creationInfo;
    creationInfo.Name = "Resource Thread";
    creationInfo.UserData = nullptr;
    creationInfo.Function = w.get_worker_function();
    ts.AddWorker(creationInfo);
  }

  ResourceManager::~ResourceManager()
  {

  }
}

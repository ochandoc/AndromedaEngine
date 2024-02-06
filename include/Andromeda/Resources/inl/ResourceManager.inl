namespace And
{
  template<typename T>
  inline void ResourceManager::AddGenerator()
  {
    size_t type_hash = typeid(T::ResourceType).hash_code();
    assert(!m_Generators.contains(type_hash) && "Generator class already inserted!");
    std::shared_ptr<IResourceGeneratorBase> Generator = std::make_shared<T>();
    m_Generators.insert({ type_hash, Generator });
  }

  template<typename T>
  inline Resource<T> ResourceManager::NewResource(const std::string& path)
  {
    size_t type_hash = typeid(T).hash_code();
    ResourceGenerator<T>* Generator = dynamic_cast<ResourceGenerator<T>*>(m_Generators[type_hash].get());

    assert(Generator != nullptr && "Resource Generator not found!");

    std::shared_ptr<ResourceValue<T>> resourceValue = std::make_shared<ResourceValue<T>>();

    resourceValue->m_Resource = Generator->GetDefault();
    resourceValue->m_Id = Generator->GenerateId(path);

    Resource<T> r;
    r.m_ResourceValue = resourceValue;

    if (m_Resources.contains(type_hash))
    {
      std::unordered_map<size_t, std::shared_ptr<IResourceBase>>& resource_list = m_Resources[type_hash];
      if (resource_list.contains(r.m_ResourceValue->m_Id))
      {
        r.m_ResourceValue = std::static_pointer_cast<ResourceValue<T>>(resource_list[r.m_ResourceValue->m_Id]);
        return r;
      }
    }
    else
    {
      m_Resources.insert({ type_hash , std::unordered_map<size_t, std::shared_ptr<IResourceBase>>() });
    }

    auto task = [Generator, resourceValue](const std::string& arg)
    {
      resourceValue->m_Resource = (*Generator)(arg);
    };

    std::unordered_map<size_t, std::shared_ptr<IResourceBase>>& resource_list = m_Resources[type_hash];
    resource_list.insert({ r.m_ResourceValue->m_Id , std::static_pointer_cast<IResourceBase>(resourceValue) });

    m_TaskSystem.AddTaskInThread("Resource Thread", task, path);
    return r;
  }

  template<typename T>
  inline void ResourceManager::ReloadResource(const std::string& path)
  {
    size_t type_hash = typeid(T).hash_code();
    ResourceGenerator<T>* Generator = dynamic_cast<ResourceGenerator<T>*>(m_Generators[type_hash].get());

    assert(Generator != nullptr && "Resource Generator not found!");

    std::unordered_map<size_t, std::shared_ptr<IResourceBase>>& resource_list = m_Resources[type_hash];

    std::shared_ptr<ResourceValue<T>> resourceValue = std::static_pointer_cast<ResourceValue<T>>(resource_list[Generator->GenerateId(path)]);

    auto task = [Generator, resourceValue](const std::string& arg)
    {
      resourceValue->m_Resource = (*Generator)(arg);
    };


    m_TaskSystem.AddTaskInThread("Resource Thread", task, path);
  }
}
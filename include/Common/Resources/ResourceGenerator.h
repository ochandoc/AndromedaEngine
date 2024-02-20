#pragma once

#include "Common/Misc/CoreMiscDefines.h"

namespace And
{
  class IResourceGeneratorBase
  {
    INTERFACE_BODY(IResourceGeneratorBase)
  };

  template<typename T>
  class ResourceGenerator : public IResourceGeneratorBase
  {
  public:
    using ResourceType = T;

    ResourceGenerator() = default;

    virtual ~ResourceGenerator() = default;

    friend class ResourceManager;
  private:
    virtual std::shared_ptr<T> operator()(const std::string& arg) = 0;
    virtual size_t GenerateId(const std::string& arg) { return std::hash<std::string>{}(arg); }
    virtual std::shared_ptr<T> GetDefault() = 0;
  };
}
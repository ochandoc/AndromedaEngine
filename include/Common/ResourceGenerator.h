#pragma once

#include "base.h"

namespace And
{
  namespace internal
  {
    class ResourceGeneratorBase
    {
    public:
      ResourceGeneratorBase() = default;

      virtual ~ResourceGeneratorBase() = default;
    };
  }

  template<typename T>
  class ResourceGenerator : public internal::ResourceGeneratorBase
  {
  public:
    using ResourceType = T;

    ResourceGenerator() = default;

    virtual ~ResourceGenerator() = default;

    friend class ResourceManager;
  private:
    virtual std::shared_ptr<T> operator()(const std::string& arg) = 0;
    virtual uint64 GenerateId(const std::string& arg) = 0;
    virtual std::shared_ptr<T> GetDefault() = 0;
  };
}
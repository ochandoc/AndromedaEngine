#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"

namespace And
{
  class Shader
  {
    NON_COPYABLE_CLASS(Shader)
    NON_MOVABLE_CLASS(Shader)
  public:
    Shader() = default;

    virtual ~Shader() = default;

    /**
     * @brief Use shader (its called automatically)
     * 
     */
    virtual void Use() const = 0;

    /**
     * @brief Stop using shader (its called automatically)
     * 
     */
    virtual void StopUsing() const = 0;

    /**
     * @brief Get the Path of the shader file
     * 
     * @return const std::string& 
     */
    inline const std::string& GetPath() const { return m_Path; }
  private:
    std::string m_Path;
  };

  std::shared_ptr<Shader> MakeShader(const std::string& Path);
}

#include "Andromeda/Graphics/Texture.h"
#include <optional>
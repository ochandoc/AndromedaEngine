#pragma once
#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Backends/OpenGL/OpenGL.h"


namespace And {

  class UniformBuffer {
  public:
    NON_COPYABLE_CLASS(UniformBuffer)

    UniformBuffer(unsigned int block_index, unsigned int size);
    ~UniformBuffer();

    void upload_data(const void* data, unsigned int size);

    void bind() const;
    void unbind() const;


  private:
    GLuint m_Id;
    GLuint m_Size;
    GLuint m_BlockIndex;
  };
}
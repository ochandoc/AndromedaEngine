#pragma once
#include "Common/Misc/CoreMiscDefines.h"

namespace And{

  class UniformBuffer{
    public:
      NON_COPYABLE_CLASS(UniformBuffer)
      
      UniformBuffer(int block_index, int size);
      ~UniformBuffer();
      
      void upload_data(const void* data, int size); 


    private:
      GLuint m_Id;
      GLuint m_Size;
      GLuint m_BlockIndex;
  };
}
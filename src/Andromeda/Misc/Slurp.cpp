#include "Andromeda/Misc/Slurp.h"

#include <utility>
#include <algorithm>

namespace And
{
  Slurp::Slurp(const char* filePath)
  {
    FILE *f;
    f = fopen(filePath, "rb");

    if(f == NULL){
      printf("Error opening file");
      return;
    }
    
    fseek(f, 0L, SEEK_END);
    int size = ftell(f);
    fseek(f, 0L, SEEK_SET);

    
    //printf("Size of file in bytes-> %d\n", (int)m_Size);

    m_Content.resize(size + 1);
    
    fread(m_Content.data(), sizeof(char), size, f);
    m_Content[size] = '\0';
    //printf("\n%s\n",m_Content);

    fclose(f);

  }

  Slurp::Slurp(Slurp&& other)
  {
    std::swap(m_Content, other.m_Content);
  }

  Slurp::~Slurp()
  {
  }

  Slurp& Slurp::operator =(Slurp&& other)
  {
    if (this != &other)
    {
      std::swap(m_Content, other.m_Content);
    }

    return *this;
  }

  char* Slurp::data()
  {
    return m_Content.data();
  }

  size_t Slurp::size() const
  {
    return m_Content.size();
  }

}
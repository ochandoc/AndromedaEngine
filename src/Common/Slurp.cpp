#include "Common/Slurp.h"

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
    m_Size = ftell(f);
    fseek(f, 0L, SEEK_SET);

    
    //printf("Size of file in bytes-> %d\n", (int)m_Size);

    m_Content = new char[m_Size+1];
    
    fread(m_Content, sizeof(char), m_Size, f);
    m_Content[m_Size] = '\0';
    //printf("\n%s\n",m_Content);

    fclose(f);

  }

  Slurp::Slurp(Slurp&& other) : m_Content(other.m_Content), m_Size(other.m_Size) 
  {
    other.m_Content = nullptr;
  }

  Slurp::~Slurp()
  {
    delete[] m_Content;
  }

  Slurp& Slurp::operator =(Slurp&& other)
  {
    if (this != &other)
    {
      std::swap(m_Content, other.m_Content);
      std::swap(m_Size, other.m_Size);
    }

    return *this;
  }

  char* Slurp::data()
  {
    return m_Content;
  }

  size_t Slurp::size() const
  {
    return m_Size;
  }

}
#pragma once

#include <vector>

namespace And
{
  class Slurp
  {
  public:
    Slurp(const char* filePath);
    Slurp(const Slurp&) = delete;
    Slurp(Slurp&& other);

    ~Slurp();

    Slurp& operator =(const Slurp&) = delete;
    Slurp& operator =(Slurp&& other);

    char* data();
    size_t size() const;

  private:
    char* m_Content;
    size_t m_Size;
  };
};
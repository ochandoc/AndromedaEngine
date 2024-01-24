#pragma once

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

    /**
     * @brief file data
     * 
     * @return char* of file data
     */
    char* data();

    /**
     * @brief size in bytes of file
     * 
     * @return size_t 
     */
    size_t size() const;

  private:
    char* m_Content;
    size_t m_Size;
  };
};
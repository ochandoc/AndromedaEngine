#pragma once


class Loader{

  public:
    Loader(const Loader&);
    Loader(Loader&&);
    ~Loader();

    bool LoadFile(const char* path);


  private:
    Loader();

};
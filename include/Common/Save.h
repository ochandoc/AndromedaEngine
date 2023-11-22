#pragma once
#include <format>
#include <type_traits>

namespace And{


template <typename T>
concept jsonFile = requires(T t) {
  t.Save();
  t.Load();
};


/*
requires(T t){
  t.Save();
  t.Load();
};
*/

template <jsonFile T>
class Save{

  public:
    Save(T& t){

    }

    Save(const Save&);
    Save(Save&&);
    ~Save();

  private:

};
}
#pragma once
#include "Slurp.h"
#include "Threw.h"
#include <string>
#include <format>
#include <type_traits>

namespace And{

/*
template <typename T>
concept jsonFile = requires(T t) {
  t.Save();
  t.Load();
};
*/


/*
requires(T t){
  t.Save();
  t.Load();
};
*/

// Guardar cualquier tipo de dato en memoria con un nombre identificador
template <typename T>
class SavedObject{

 public:

  SavedObject(T object){
    m_obj = object;
  }
  SavedObject(const SavedObject&) = delete;
  SavedObject(SavedObject&&) = delete;
  ~SavedObject(){
  }


  bool save(std::string name){

    Threw<T> threw(name.c_str(), m_obj);

    return true;
  }

  bool load(std::string name, T& obj){
    Slurp slurp{name.c_str()};

    if(slurp.size() == sizeof(T)){
      obj = *reinterpret_cast<T*>(slurp.data());
    }else{
      printf("El tipo de dato no coincide");
      return false;
    }

    return true;
  }

 private:

  T m_obj;
};


}
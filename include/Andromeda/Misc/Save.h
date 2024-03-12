#pragma once
#include "Slurp.h"
#include "Threw.h"
#include <string>
#include <format>
#include <type_traits>

namespace And{

// Guardar cualquier tipo de dato en memoria con un nombre identificador
template <typename T>
class SavedObject{

 public:

  /**
   * @brief Construct a new Saved Object
   * 
   * @param object 
   */
  SavedObject(T object){
    m_obj = object;
  }
  SavedObject(const SavedObject&) = delete;
  SavedObject(SavedObject&&) = delete;
  ~SavedObject(){
  }

  /**
   * @brief Save in memory the T parameter recived in constructor
   * 
   * @param name 
   * @return true if save is completed
   * @return false if can't save the file
   */
  bool save(std::string name){

    Threw<T> threw(name.c_str(), m_obj);

    return true;
  }

  /**
   * @brief Load in a T parameter the T object saved previously with a identifier
   * 
   * @param name 
   * @param obj 
   * @return true 
   * @return false 
   */
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
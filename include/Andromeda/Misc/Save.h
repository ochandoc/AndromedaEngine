#pragma once
#include "Slurp.h"
#include "Threw.h"
#include <string>
#include <format>
#include <type_traits>

namespace And{

// Guardar cualquier tipo de dato en memoria con un nombre identificador
class SavedObject{

 public:

  /**
   * @brief Construct a new Saved Object
   * 
   * @param object 
   */
  SavedObject(){
    
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
  template <typename T>
  bool save(std::string name, T obj){

    Threw<T> threw(name.c_str(), obj);

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
  template <typename T>
  bool load(std::string filename, T& obj){
    std::string name = std::string(filename) + ".and";
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
};


}
#pragma once
#include <vector>
#include <optional>
#include <cassert>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <typeinfo>
#include <stdexcept>
#include <stdio.h>


struct Transform {
  float position[3];
};

struct Physics{
  float velocity;
};

struct component_base{
  virtual void grow() = 0;
  virtual size_t size() = 0;
  virtual void remove_all(size_t e) = 0;
};

template <typename T>
struct component_list : component_base {

  // Vector de componentes
  std::vector<std::optional<T>> components_;
  
  // Añadimos algo nuevo
  virtual void grow(){
    // Añadimos uno al final y ademas nos crea un objeto del tipo que sea
    components_.emplace_back();
  }

  virtual void remove_all(size_t e){
    components_.at(e) = std::nullopt;
  }

  template <typename T>
  void add_component();

  virtual size_t size(){
    return components_.size();
  }


};


class ComponentManager{

  public:
  // Unorderer map (clave : valor) de component base. La clave es el hash del nombre del componente y el valor es el vector de cada componente
  std::unordered_map<std::size_t, std::unique_ptr<component_base>> component_classes_;
  std::vector<size_t> component_empty_; 

  // Añadimos al umap todos los componentes que tenemos
  ComponentManager(){
    //typeid(Transform).hash_code();
    add_component_class<Transform>();
    add_component_class<Physics>();
  }

  // Añadimos un componente del tipo que sea
  template <typename T>
  void add_component_class(){
    // Metemos en el unordered map de clave el hash del nombre de la estructura del componente, y como valor un puntero al componente
    component_classes_.emplace(typeid(T).hash_code(), std::make_unique<component_list<T>>());
  }
  
  template <typename T>
  T* get_component(size_t e);

  template <typename T>
  void add_component(size_t e);

  template <typename T>
  void remove_component(size_t e);

  void remove_entity(size_t e);

  size_t new_entity();
  
};


template <typename T>
void ComponentManager::add_component(size_t e){
  // cogemos el hash code del component
  auto hash_code = typeid(T).hash_code();

  // Puntero al vector de ese componente
  auto component_list_it = component_classes_.find(hash_code);

  // Coger posicion de la entidad en ese componente
  auto& comp = *static_cast<component_list<T>*>(component_list_it->second.get());
  auto& new_comp = comp.components_.at(e-1);

  // Si es nullopt le metemos uno nuevo
  if(!new_comp.has_value()){
    new_comp = T{};
  }


}


template <typename T>
T* ComponentManager::get_component(size_t e){

  size_t index = e - 1;

  // Sacamos el hash del nombre del componente
  auto hash_code = typeid(T).hash_code();
  // Salta si el umap esta vacio
  assert(component_classes_.size());
  // Salta si no existe ese componente en el umap
  assert(component_classes_.contains(hash_code));
  //Salta si el index es 0
  assert(e != 0);

  // Buscamos si hay un elemento en el umap con clave el hash del componente que queremos
  // Buscamos el vector del componente que queremos
  auto component_list_it = component_classes_.find(hash_code);

  // component_list_it nos da una tupla (clave : valor) del componente con el hash que hemos buscado
  // clave es el primer elemento
  // valor es el segundo elemento, que es el vector de componentes
  // cogemos el segundo elemento que es el valor y lo casteamos a un puntero de component list (struct) del tipo que hayamos recibido
  // Cogemos el vector del componente que queremos
  auto& comp_list = *static_cast<component_list<T>*>(component_list_it->second.get());

  // salta si el indice es mayor que el size del vector
  assert(index <= comp_list.size());

  // Del vector de componentes, cogemos el componente de la posicion de la entidad
  auto& component_opt = comp_list.components_.at(index);

  // No podemos usar value_or() porque el tipo que devuelve si no tiene valor tiene que ser del mismo tipo que el que contiene el optional
  if(!component_opt.has_value()){
    return nullptr;
  } 

  return &component_opt.value();
}

template <typename T>
void ComponentManager::remove_component(size_t e){

  // Primero sacamos el hash
  auto hash_code = typeid(T).hash_code();

  auto component_list_it = component_classes_.find(hash_code);

  auto& comp_list = *static_cast<component_list<T>*>(component_list_it->second.get());
  auto& component_opt = comp_list.components_.at(e-1);
  if(component_opt.has_value()){
    component_opt = std::nullopt;
  }

}

// Eliminamos todos sus componentes y los dejamos en nullopt
void ComponentManager::remove_entity(size_t e){
  for(auto& pair : component_classes_){
    //auto& comp_list = *static_cast<component_list<Transform>*>(pair->second.get());
    auto& comp = pair.second;
    comp->remove_all(e-1);
  }

  // Metemos en el vector de vacios la posicion que hemos dejado libre
  component_empty_.push_back(e);

}

size_t ComponentManager::new_entity(){
  size_t size;

  // Si hay algo en el vector de vacios devolvemos esa posicion que ya esa libre
  if(component_empty_.size() == 0){
    // Recorremos el umap de vectores
    // A cada vector de componentes le sumamos un nuevo elemento
    for(auto& [key, value] : component_classes_){
      value->grow();
      size = value->size();
    }
  }else{
    size = component_empty_.front();
    component_empty_.erase(component_empty_.begin());
  }

  return size;
}



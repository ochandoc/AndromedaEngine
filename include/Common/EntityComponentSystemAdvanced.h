#pragma once
#include "ID.h"
#include "Entity.h"

namespace And{

struct Physics{
  float speed;
};

struct Transform{
  float position[3];
};

namespace internal{

template <typename T>
struct component{
  ID id;
  T value;
};


struct component_base{
  virtual void grow(ID id) = 0;
  virtual size_t size() = 0;
  virtual void clear_at_index(size_t e) = 0;
};

template <typename T>
struct component_list : component_base{
  // En vez de tener el vector de optinal<T> tenemos un vector de component<T>, osea que siempre va a tener valor y ese valor esta asociado a un ID de una entidad
  std::vector<component<T>> components_;

  virtual void grow(ID id){
    // creamos un nuevo componente del tipo que sea asociado a esa id
    components_.push_back(id);
  }

  virtual void clear_at_index(size_t e){
    //components_[e] = std::nullopt;
  }

  virtual size_t size(){
    return components_.size();
  }

};


class ComponentManagerAdvanced{
  public:
  std::unordered_map<size_t, std::unique_ptr<component_base>> component_classes_;


  ComponentManagerAdvanced(){
    add_component_class<Transform>();
    add_component_class<Physics>();
  }

  Entity new_entity();

  template <typename T>
  void add_component(Entity e);


  private:
  template <typename T>
  void add_component_class(){
    component_classes_.emplace(typeid(T).hash_code(), std::make_unique<component_list<T>>());
  }


};



Entity ComponentManagerAdvanced::new_entity(){
  // Creamos una entidad con todos componentes (o sin componentes?)
  
  Entity entity_tmp();
  for(auto& [key, value] : component_classes_){
    //value->grow(entity_tmp.get_id());
  }
  
}




}
}
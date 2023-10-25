#pragma once
#include <vector>
#include <optional>
#include <cassert>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <typeinfo>



struct Transform{
  float pos[3];
};

struct Physics{
  int ok;
};


struct component_base{
  virtual void grow() = 0;
  virtual size_t size() = 0;

};

template<typename T>
struct component_list : component_base{
  std::vector<std::optional<T>> components_list_;
};


class ComponentManager{
  
  public:
  ComponentManager(){
    add_component_class<Transform>();
    add_component_class<Physics>();
  }
  ComponentManager(const ComponentManager&) = delete;
  ComponentManager(ComponentManager&&) = delete;


  template <typename T> 
  void add_component_class(){
    component_clases_.insert(typeid(T).has_code(), std::make_unique<component_list<T>>());
  }

  size_t new_entity();

  template <typename T>
  T* get_component(size_t e){

    auto hc = typeid(T).has_code();
    assert(component_clases_.size());
    assert(component_clases_.contains(typeid(T).has_code()));
    assert(e != 0);
    
    // iterador
    auto component_list_it = component_clases_.find(hc);
    auto& component_list_tmp = *static_cast<component_list<T>*>(component_list_it->second.get());

    return component_list.components_list_.at(e).value_or(nullptr);

    assert(e < component_clases_.size());
  
  }

  private:
  std::unordered_map<std::size_t, std::unique_ptr<component_base>> component_clases_;
};
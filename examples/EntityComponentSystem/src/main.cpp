#include <assert.h>

#include <stdio.h>
#include <iostream>
#include <fstream>

#include <typeinfo>

#include <functional>
#include <algorithm>
#include <utility>
#include <chrono>

#include <optional>
#include <memory>

#include <string>
#include <optional>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <tuple>

#include "Common/ID.h"
#include "Common/EntityComponentSystem.h"

struct int_comp
{
  int num;
};

template<class ForwardIt, class T>
bool binary_search(ForwardIt first, ForwardIt last, const T& value)
{
  first = std::lower_bound(first, last, value);
  return ;
}

int main(int argc, char** argv){
  And::EntityComponentSystem ecs;

  And::internal::compact_component_list_imp<int_comp> v;

  And::ID id;

  std::cout << "ID: " << id.get() << std::endl;

  //And::internal::component<int_comp> comp{id, 10};

  v.add_empty(id);
  v.add_empty(And::ID());
  v.add_empty(And::ID());
  v.add_empty(And::ID());
  v.add_empty(And::ID());
  v.add_empty(And::ID());
  v.add_empty(And::ID());
  v.add_empty(And::ID());
  v.add_empty(And::ID());
  v.add_empty(And::ID());
  v.add_empty(And::ID());
  v.add_empty(And::ID());
  v.add_empty(And::ID());
  v.add_empty(And::ID());
  v.add_empty(And::ID());
  v.add_empty(And::ID());

  for (auto val : v)
  {
    std::cout << "Hola :" << val.num << std::endl;
    //std::cout << "Id: " << val.id.get() << std::endl;
  }

  v.sort();

  //using comp_t = And::internal::component<int_comp>;

  //auto first = std::lower_bound(v.m_Components.begin(), v.m_Components.end(), comp, [](const comp_t& c1, const comp_t& c2){return c1.id.get() < c2.id.get();});

  int_comp* comp = v.get_component(id);


  return 0;
}
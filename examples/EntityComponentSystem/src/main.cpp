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
#include <tuple>


#include "Common/EntityComponentSystem.h"

struct int_comp
{
  int value;
};



int main(int argc, char** argv){
  And::EntityComponentSystem ecs;

  ecs.add_component_class<int_comp>();

  int_comp c = { 10 };
  And::Entity a = ecs.new_entity(c);

  int_comp* comp = ecs.get_entity_component<int_comp>(a);
  if (comp)
  {
    std::cout << comp->value << std::endl;
  }
  else
  {
    std::cout << "No component added!" << std::endl;
  }
  
  return 0;
}
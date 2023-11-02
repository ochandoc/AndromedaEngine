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
  int value;
};



int main(int argc, char** argv){
  And::EntityComponentSystem ecs;

  And::internal::component_list_imp<int_comp> v;

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
    std::cout << "Id: " << val.id.get() << std::endl;
  }

  return 0;
}
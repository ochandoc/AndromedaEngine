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

#include <Windows.h>

#include "Andromeda.h"

#define NUM_ENTITIES 100000

std::vector<int> test_V;

struct int_comp
{
  int num;
};

struct float_comp 
{
  float fnum;
};

void manolo(int_comp* c1, float_comp* c2)
{
  c2->fnum++;
  c1->num += (int)c2->fnum;
  test_V.push_back(c1->num);
}

void manolo2(int_comp* c1)
{
  c1->num += c1->num;
  test_V.push_back(c1->num);
}

int main(int argc, char** argv)
{
  And::Entity e;

  And::EntityComponentSystem ecs;

  ecs.add_component_class<int_comp>();
  ecs.add_component_class<float_comp>();

  for (int i = 0; i < 100000; i++)
  {
    if ((i % 2) == 0)
    {
      e = ecs.new_entity(int_comp{ i }, float_comp{ (float)i });
    }
    else
    {
      ecs.new_entity(int_comp{ i });
    }
  }

  for (int i = 0; i < 10; i++)
  {
    ecs.execute_system(manolo2);
    printf("Size: %zu\n", test_V.size());
    test_V.clear();

    ecs.execute_system(manolo);

    printf("Size: %zu\n", test_V.size());
    test_V.clear();
  }

  return 0;
}
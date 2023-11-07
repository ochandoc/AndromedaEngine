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

#include "Common/ID.h"
#include "Common/EntityComponentSystem.h"

#define NUM_ENTITIES 100000

struct int_comp
{
  int num;
};

struct float_comp 
{
  float fnum;
};

int system(int_comp* c1, int_comp* c2)
{
  c2->num++;
  c1->num += c2->num;
  return c1->num;
}

LARGE_INTEGER StartTime;
LARGE_INTEGER Freq;

void ResetTimer()
{
  QueryPerformanceCounter(&StartTime);
}

void CheckTimer(const char* Name)
{
  static LARGE_INTEGER CurrentTime;
  QueryPerformanceCounter(&CurrentTime);

  printf("%s: %fms\n", Name, (((float)(CurrentTime.QuadPart - StartTime.QuadPart) / (float)Freq.QuadPart) * 1000.0f));
}

int main(int argc, char** argv)
{
  QueryPerformanceFrequency(&Freq);
  And::EntityComponentSystem ecs;

  And::internal::component_list_imp<int_comp> v;
  And::internal::component_list_imp<float_comp> v2;

  for (int i = 0; i < 10; i++)
  {
    And::ID id;
    if ((i % 2) == 0)
    {
      v2.add_empty(id);
    }
    v.add_empty(id);
  }

  And::internal::tuple_iterator<int_comp, float_comp> ti(v, v2);

  ++ti;

  /*
  ecs.add_component_class<int_comp>();
  ecs.add_component_class<float_comp>();


  int adds = 1;

  for (int it = 0; it < 20; it++)
  {
    printf("\nNum entities %d\n", ((it + 1) * NUM_ENTITIES) + 1);
    int adds = 0;
    ResetTimer();
    for (int i = 0; i < NUM_ENTITIES; i++)
    {
      if ((i % 3) == 0)
      {
        ecs.new_entity(int_comp{ i }, float_comp{ 2.0f });
      }
      else
      {
        ecs.new_entity(int_comp{ i });
      }
      adds++;
    }
    CheckTimer("Add entities");
  }
  */
  /*



  And::ID id;

  std::cout << "ID: " << id.get() << std::endl;

  v.add(id, int_comp{10});

  int adds = 1;

  for (int it = 0; it < 20; it++)
  {
    printf("\nNum entities %d\n", ((it + 1) * NUM_ENTITIES) + 1);
    ResetTimer();
    for (int i = 0; i < NUM_ENTITIES; i++)
    {
      And::ID hola_id;
      if ((i % 3) == 0)
      {
        v.add_empty(hola_id);
      }
      if ((i % 2) == 0)
      {
        v2.add_empty(hola_id);
      }
      adds++;
    }
    CheckTimer("Add entities");
    std::cout << "Comp 1 size: " << v.size() << std::endl;
    std::cout << "Comp 2 size: " << v2.size() << std::endl;

    ResetTimer();
    v.sort();
    CheckTimer("Sort comp 1");

    ResetTimer();
    v2.sort();
    CheckTimer("Sort comp 2");

    ResetTimer();
    int_comp* comp = v.get_component(id);
    CheckTimer("Search one");

    if (!comp)
    {
      std::cout << "[Error] component not found! " << std::endl;
    }

    int err = 0;
    int etts = 0;

    ResetTimer();
    for (auto& c : v2)
    {
      int_comp* c2 = v.get_component(c.id);
      if (!c2)
      {
        err++;
      }
      else
      {
        system(&c.value, c2);
        etts++;
      }
    }
    CheckTimer("System");

    std::cout << "Num entities calculated: " << etts << std::endl;
    std::cout << "Error matching components: " << err << std::endl;

    //std::cout << "Num entities: " << adds << std::endl;
  }

  */


  return 0;
}
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

  ecs.add_component_class<int_comp>();
  ecs.add_component_class<float_comp>();

  And::Entity e;

  ResetTimer();
  for (int i = 0; i < 2000000; i++)
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
  CheckTimer("Insertions");

  std::function<void(int_comp*, float_comp*)> s = manolo;
  ResetTimer();
  ecs.execute_system(s);
  CheckTimer("System");

  test_V.clear();

  ResetTimer();
  ecs.execute_system(s);
  CheckTimer("System");

  printf("Size: %zu\n", test_V.size());
  /*for (auto i : test_V)
  {
    printf("Num: %d\n", i);
  }*/

  return 0;
}
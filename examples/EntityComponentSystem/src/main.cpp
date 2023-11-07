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

  for (int i = 0; i < 4; i++)
  {
    And::ID id;
    //std::cout << "ID: " << id.get() << " Num: " << i << std::endl;
    if ((i % 2) == 0)
    {
      v2.add(id, float_comp{(float)i});
    }
    v.add(id, int_comp{i});
  }

  std::cout << "int_comp" << std::endl;
  for (auto& a : v)
  {
    std::cout << "ID: " << a.id << " Num: " << a.value.num << std::endl;
  }

  std::cout << "float_comp" << std::endl;
  for (auto& a : v2)
  {
    std::cout << "ID: " << a.id << " Num: " << a.value.fnum << std::endl;
  }

  And::internal::tuple_iterator<int_comp, float_comp> ti(v, v2);
  for (int i = 0; i < 4; i++)
  {
    auto& [c1, c2] = *ti;
    ++ti;

  }

  return 0;
}
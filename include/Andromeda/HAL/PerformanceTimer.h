#pragma once

#include <Windows.h>

#include "base.h"

class PerformanceTimer
{
  NON_COPYABLE_CLASS(PerformanceTimer)
  NON_MOVABLE_CLASS(PerformanceTimer)
public:
  PerformanceTimer() { QueryPerformanceFrequency(&Freq); }

  void reset() { QueryPerformanceCounter(&StartTime); }
  void check(const char* Name) 
  {
    static LARGE_INTEGER CurrentTime;
    QueryPerformanceCounter(&CurrentTime);

    printf("%s: %fms\n", Name, (((float)(CurrentTime.QuadPart - StartTime.QuadPart) / (float)Freq.QuadPart) * 1000.0f));
  }
private:
  LARGE_INTEGER StartTime;
  LARGE_INTEGER Freq;
};
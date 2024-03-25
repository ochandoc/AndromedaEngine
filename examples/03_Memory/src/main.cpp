#include <assert.h>

#include <stdio.h>
#include <iostream>
#include <fstream>

#include <functional>
#include <algorithm>
#include <utility>

#include <optional>
#include <memory>

#include <string>
#include <vector>
#include <queue>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

#include "Andromeda.h"

struct SPrueba {
  int uno = 1;
  float dos = 2.0f;
};

int main(int argc, char** argv){

  And::Engine e;

  SPrueba prueba;
  prueba.uno = 3;
  prueba.dos = 3.0f;

  And::SavedObject<SPrueba> save_struct(prueba);
  bool result = save_struct.save("test");
  save_struct.load("test", prueba);

  return 0;
}
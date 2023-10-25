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
#include <optional>
#include <vector>
#include <unordered_map>

#include "Andromeda.h"

int main(int argc, char** argv){
  And::Engine e;

  And::ResourceManager rm;

  And::resource<int> r = rm.new_resource();

  std::cout << *r << std::endl;

  rm.swap();

  std::cout << *r << std::endl;

  return 0;
}
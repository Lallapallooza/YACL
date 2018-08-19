# Yet Another Config Library for C++

### About
YACL is library for operating configs with large and complex hierarchy. Library designed to be as simple as possible to read config and operate with them in code
  
### Installation
YACL using cmake to build project.

Build example provided below
```sh
$ mkdir build
$ cd build
$ cmake .. && cmake --build .
```

### Documentation
All documentation can be found in doc/ folder.
It's generated with doxygen.

### Usage
```cpp 
#include <YACL/config.h>
int main() {
  yacl::Setting *root = yacl::Config::parseConfigFromFile("config.yacl");
  yacl::Config::printConfig(root);

  int var1 = *root->field("var1");
  std::string var2 = *root->field("var2");
  std::string var3 = *root->field("var3");

  yacl::Setting::recDeleteGraph(root);
  return 0;
}
```

Other examples can be found in example/ folder.



### Todo List

 - Add gtest
 - Add write functionality
 - Python, Java, ObjC wrappers
 - Add crypt and convert to binary

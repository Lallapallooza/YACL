
#include <iostream>
#include <YACL/config.h>

// CONFIG_PATH setted in CMakeLists.txt
const std::string config_path = CONFIG_PATH;

int main() {
  
  yacl::Setting *root = yacl::Config::parseConfigFromFile(config_path + "/example.yacl");
  yacl::Config::printConfig(root);

  yacl::Setting::recDeleteGraph(root);
  return 0;
}
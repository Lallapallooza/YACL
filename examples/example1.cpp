#include <iostream>
#include <YACL/config.h>

// CONFIG_PATH setted in CMakeLists.txt
const std::string config_path = CONFIG_PATH;

int main() {

  const yacl::SettingsUniquePtr root =
    yacl::Config::parseConfigFromFile(config_path + "/example.yacl");

  yacl::Config::printConfig(*root);

  return 0;
}

#include <iostream>

#include <YACL/config.h>

// CONFIG_PATH setted in CMakeLists.txt
const std::string config_path = CONFIG_PATH;

int main() {

  // root contain only fields
  const yacl::SettingsUniquePtr root =
    yacl::Config::parseConfigFromFile(config_path + "/simple.yacl");

  // lets print it and assing to out vars
  yacl::Config::printConfig(*root);

  int var1 = *root->field("var1");
  std::string var2 = *root->field("var2");
  std::string var3 = *root->field("var3");

  std::cout << "Our vars we get from config\n"
    "var1 = " << var1 << "\n"
    "var2 = " << var2 << "\n"
    "var3 = " << var3 << std::endl;

  return 0;
}

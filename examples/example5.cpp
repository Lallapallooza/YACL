#include <YACL/config.h>

// CONFIG_PATH setted in CMakeLists.txt
const std::string config_path = CONFIG_PATH;

int main() {

  // root contain only anon fields
  const yacl::SettingsUniquePtr root =
    yacl::Config::parseConfigFromFile(config_path + "/anons.yacl");

  for (yacl::Setting *anon : root->anons()) { yacl::Config::printConfig(anon); }

  // so working with anon fields
  // absolutely same as with named

  const std::string version = *root->anons()[0]->field("version");
  std::cout << version;

  return 0;
}

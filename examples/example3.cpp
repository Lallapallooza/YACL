#include <vector>
#include <YACL/config.h>

// CONFIG_PATH setted in CMakeLists.txt
const std::string config_path = CONFIG_PATH;

int main() {

  const yacl::SettingsUniquePtr root =
    yacl::Config::parseConfigFromFile(config_path + "/example.yacl");

  // get field from config
  const yacl::Field *field = root->setting("main")
                                 ->setting("some_inner")
                                 ->setting("more_inner")
                                 ->field("int_arr");

  // unsafe getting type
  // this will be faster than
  // yacl::int_vector vec = *field;
  // but UB if you're miss type
  yacl::str_vector vec = field->getValue<yacl::str_vector>(); // oops... UB here

  return 0;
}


#include <vector>
#include <YACL/config.h>

// CONFIG_PATH setted in CMakeLists.txt
const std::string config_path = CONFIG_PATH;

int main() {
  
  yacl::Setting *root = yacl::Config::parseConfigFromFile(config_path + "/example.yacl");

  // get field from config
  const yacl::Field *field = root->setting("main")
                                 ->setting("some_inner")
                                 ->setting("more_inner")
                                 ->field("int_arr");

  // print using debugString()
  std::cout << field->debugString() << std::endl;

  // safe getting type
  yacl::int_vector vec = *field;

  for(int e : vec) {
    std::cout << e << ' ';
  }
  std::cout << std::endl;

  // trying to get wrong type 
  // and catch using YACLException
  try {
    yacl::bool_vector vec = *field;
  } catch(yacl::YACLException &e) {
    std::cout << e.what() << std::endl;
  }

  // trying to get wrong type
  // and catch using std::exception
  try {
    yacl::str_vector vec = *field;
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }


  yacl::Setting::recDeleteGraph(root);
  return 0;
}
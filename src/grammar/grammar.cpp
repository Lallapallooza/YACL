
#include <YACL/types.h>
#include <grammar/grammar.h>
#include <helper/utils.h>

yacl::ContentType yacl::getTypeOfRaw(const std::string &str) noexcept {
  if (yacl::isInt(str)) return yacl::ContentType::INT_VALUE;
  if (yacl::isFloat(str)) return yacl::ContentType::FLOAT_VALUE;
  if (yacl::isString(str)) return yacl::ContentType::STRING_VALUE;
  if (yacl::isBool(str)) return yacl::ContentType::BOOL_VALUE;

  return yacl::ContentType::NOT_INIT;
}


bool yacl::isArrayOfSameType(
  const std::string &str,
  yacl::ContentType force_type) noexcept {
  // array must be typeof [ 1, 2, 3, 5 ]
  if (str[0] != '[' || str.back() != ']') return false;

  // get each element of array
  std::vector<std::string> splitted =
    yacl::split(str.substr(1, str.size() - 2), ',');

  // use 0 element as main array type if force_type not used
  yacl::ContentType type;
  if (force_type == yacl::ContentType::NOT_INIT)
    type = getTypeOfRaw(splitted[0]);
  else
    type = force_type;

  for (size_t i = 1; i < splitted.size(); ++i) {
    if (type != getTypeOfRaw(splitted[i])) return false;
  }

  return true;
}


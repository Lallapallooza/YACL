
#include <YACL/types.h>
#include <type_conversation/converter.h>

yacl::int_vector yacl::
raw_to_vector_ints(const std::string &raw_vector) noexcept {
  return rawVectorToVectorOfType<int, decltype(strToInt)>(raw_vector, strToInt);
}

yacl::float_vector yacl::raw_to_vector_floats(
  const std::string &raw_vector) noexcept {
  return rawVectorToVectorOfType<float, decltype(strToFloat)>(raw_vector,
                                                              strToFloat);
}

yacl::str_vector yacl::raw_to_vector_strings(
  const std::string &raw_vector) noexcept {
  return rawVectorToVectorOfType<
    std::string, decltype(strToStr)>(raw_vector, strToStr);
}

yacl::bool_vector yacl::raw_to_vector_bools(
  const std::string &raw_vector) noexcept {
  return rawVectorToVectorOfType<bool, decltype(strToBool)>(raw_vector,
                                                            strToBool);
}

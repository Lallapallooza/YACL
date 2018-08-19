#pragma once

#include <vector>
#include <string>

#include <YACL/types.h>
#include <helper/utils.h>

namespace yacl {

/**
 * \brief Format string "my str" -> my str
 * \param str string for format
 * \return formated string
 */
inline std::string strToStr(const std::string &str) noexcept {
  return str.substr(1, str.size() - 2);
}

/**
 * \brief Convert string to int
 * \param str string for format
 * \return int value from string
 */
inline int strToInt(const std::string &str) noexcept {
  return std::stoi(str);
}

/**
 * \brief Convert string to float
 * \param str string for format
 * \return float value from string
 */
inline float strToFloat(const std::string &str) noexcept {
  return std::stof(str);
}


/**
* \brief Convert string to bool
* \param str string for format
* \return bool value from string
*/
inline bool strToBool(const std::string &str) noexcept {
  return str == "true";
}


/**
 * \brief Generic string to vector of type
 * \tparam RetT return type
 * \tparam SingleValueConverterT functor or function type that can convert single string to type
 * \param raw_vector string to be converted
 * \param converter functor or function
 * \return vector with type RetT
 */
template <class RetT, class SingleValueConverterT>
static std::vector<RetT> rawVectorToVectorOfType(
  const std::string &raw_vector, SingleValueConverterT converter) {

  // TODO: optimize, remove copy
  std::vector<std::string> only_values =
    split(raw_vector.substr(1, raw_vector.size() - 2), ',');

  std::vector<RetT> real_values;
  for (const auto &str : only_values)
    real_values.push_back(converter(str));

  return real_values;
}


/**
 * \brief String to vector int converter
 * \param raw_vector string to be converted
 * \return vector with type int
 */
int_vector raw_to_vector_ints(const std::string &raw_vector) noexcept;


/**
* \brief String to vector float converter
* \param raw_vector string to be converted
* \return vector with type float
*/
float_vector raw_to_vector_floats(const std::string &raw_vector) noexcept;


/**
* \brief String to vector string converter
* \param raw_vector string to be converted
* \return vector with type string
*/
str_vector raw_to_vector_strings(const std::string &raw_vector) noexcept;


/**
* \brief String to vector bool converter
* \param raw_vector string to be converted
* \return vector with type bool
*/
bool_vector raw_to_vector_bools(const std::string &raw_vector) noexcept;


}

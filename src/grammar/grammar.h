#pragma once

#include <string>

#include <YACL/types.h>

namespace yacl {

/**
 * \brief str == int if all chars are digits
 * \param str string for parsing
 * \return true if int, false otherwise
 */
static inline bool isInt(const std::string &str) noexcept {
  for (char c : str)
    if (!isdigit(c) && c != '-')
      return false;

  return true;
}


/**
 * \brief str == float if all chars are digits except 1, 1 must be '.'
 * \param str string for paring
 * \return true if float, false otherwise
 */
static inline bool isFloat(const std::string &str) noexcept {
  bool contain_digit = false, contain_dot = false;

  for (char c : str) {
    if (isdigit(c)) contain_digit = true;

    if (c == '.') {
      if (contain_dot) return false;
      contain_dot = true;
    }
  }

  return contain_dot && contain_digit;
}


/**
 * \brief str == bool if 'true' or 'false'
 * \param str string for paring
 * \return true if bool, false otherwise
 */
static inline bool isBool(const std::string &str) noexcept {
  return (str == "true") || (str == "false");
}


/**
 * \brief str == string if begin and ends "
 * \param str string for paring
 * \return true if string, false otherwise
 */
static inline bool isString(const std::string &str) noexcept {
  return !(str[0] != '"' && str.back() != '"') || !(
           str[0] != '\'' && str.back() != '\'');
}


/**
 * \brief Get simple type from string
 *        simple: int, float, bool, string
 * \param str string for paring
 * \return ContentType of str, if can't parse return ContentType::NOT_INIT
 */
ContentType getTypeOfRaw(const std::string &str) noexcept;

/**
 * \brief Detecting is array contain same type elements
 * \param str string for parsing
 * \param force_type hint for type searching, if value not setted, using 1st elements type
 * \return true if array of same type, false otherwise
 */
bool isArrayOfSameType(const std::string &str,
                       ContentType force_type = ContentType::NOT_INIT) noexcept;


/**
 * \brief Detecting is array contain only int elements
 * \param str string for parsing
 * \return true if array of ints type, false otherwise
 */
static inline bool isArrayOfIntegers(const std::string &str) noexcept {
  return isArrayOfSameType(str, ContentType::INT_VALUE);
}


/**
* \brief Detecting is array contain only float elements
* \param str string for parsing
* \return true if array of floatss type, false otherwise
*/
static inline bool isArrayOfFloats(const std::string &str) noexcept {
  return isArrayOfSameType(str, ContentType::FLOAT_VALUE);
}


/**
* \brief Detecting is array contain only string elements
* \param str string for parsing
* \return true if array of strings type, false otherwise
*/
static inline bool isArrayOfStrings(const std::string &str) noexcept {
  return isArrayOfSameType(str, ContentType::STRING_VALUE);
}


/**
* \brief Detecting is array contain only bool elements
* \param str string for parsing
* \return true if array of boolss type, false otherwise
*/
static inline bool isArrayOfBools(const std::string &str) noexcept {
  return isArrayOfSameType(str, ContentType::BOOL_VALUE);
}

}

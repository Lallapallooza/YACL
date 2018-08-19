#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace yacl {
/**
 * \brief std::vector<int> alias
 */
using int_vector = std::vector<int>;


/**
* \brief std::vector<float> alias
*/
using float_vector = std::vector<float>;


/**
* \brief std::vector<std::string> alias
*/
using str_vector = std::vector<std::string>;


/**
* \brief std::vector<bool> alias
*/
using bool_vector = std::vector<bool>;

/**
 * \brief std::initializer_list<int> alias
 */
using int_initlist = std::initializer_list<int>;


/**
* \brief std::initializer_list<int> alias
*/
using float_initlist = std::initializer_list<float>;


/**
* \brief std::initializer_list<int> alias
*/
using str_initlist = std::initializer_list<std::string>;


/**
* \brief std::initializer_list<int> alias
*/
using bool_initlist = std::initializer_list<bool>;


/**
 * \brief Supported primitive types
 */
enum class ContentType {
  INT_VALUE = 0,
  BOOL_VALUE,
  FLOAT_VALUE,
  STRING_VALUE,
  VECTOR_INT_VALUE,
  VECTOR_FLOAT_VALUE,
  VECTOR_STRING_VALUE,
  VECTOR_BOOL_VALUE,
  NOT_INIT
};

}

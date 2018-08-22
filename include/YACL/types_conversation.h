#pragma once

#include <YACL/types.h>
#include <iostream>

namespace yacl {

#define TEMPLATE_IS_SAME(firstT, secondT)                                 \
  typename std::enable_if<std::is_same<firstT, secondT>::value>::type * = \
      nullptr

#define TEMPLATE_IS_NOT_SAME(firstT, secondT)                              \
  typename std::enable_if<!std::is_same<firstT, secondT>::value>::type * = \
      nullptr

/**
 * \brief int to ContentType converter
 * \return ContentType
 */
template <class T, TEMPLATE_IS_SAME(T, int)>
ContentType typeToContentTypeEnum() noexcept { return ContentType::INT_VALUE; }

/**
 * \brief float to ContentType converter
 * \return ContentType
 */
template <class T, TEMPLATE_IS_SAME(T, float)>
ContentType typeToContentTypeEnum() noexcept {
  return ContentType::FLOAT_VALUE;
}

/**
 * \brief bool to ContentType converter
 * \return ContentType
 */
template <class T, TEMPLATE_IS_SAME(T, bool)>
ContentType typeToContentTypeEnum() noexcept { return ContentType::BOOL_VALUE; }

/**
 * \brief string to ContentType converter
 * \return ContentType
 */
template <class T, TEMPLATE_IS_SAME(T, std::string)>
ContentType typeToContentTypeEnum() noexcept {
  return ContentType::STRING_VALUE;
}

/**
 * \brief int_vector to ContentType converter
 * \return ContentType
 */
template <class T, TEMPLATE_IS_SAME(T, int_vector)>
ContentType typeToContentTypeEnum() noexcept {
  return ContentType::VECTOR_INT_VALUE;
}

/**
 * \brief float_vector to ContentType converter
 * \return ContentType
 */
template <class T, TEMPLATE_IS_SAME(T, float_vector)>
ContentType typeToContentTypeEnum() noexcept {
  return ContentType::VECTOR_FLOAT_VALUE;
}

/**
 * \brief bool_vector to ContentType converter
 * \return ContentType
 */
template <class T, TEMPLATE_IS_SAME(T, bool_vector)>
ContentType typeToContentTypeEnum() noexcept {
  return ContentType::VECTOR_BOOL_VALUE;
}

/**
 * \brief strt_vector to ContentType converter
 * \return ContentType
 */
template <class T, TEMPLATE_IS_SAME(T, str_vector)>
ContentType typeToContentTypeEnum() noexcept {
  return ContentType::VECTOR_STRING_VALUE;
}

/**
 * \brief int to string converter
 * \return type name
 */
template <class T, TEMPLATE_IS_SAME(T, int)>
const char *typeToString() noexcept { return "INT"; }


/**
 * \brief float to string converter
 * \return type name
 */
template <class T, TEMPLATE_IS_SAME(T, float)>
const char *typeToString() noexcept { return "FLOAT"; }

/**
 * \brief bool to string converter
 * \return type name
 */
template <class T, TEMPLATE_IS_SAME(T, bool)>
const char *typeToString() noexcept { return "BOOL"; }

/**
 * \brief string to string converter
 * \return type name
 */
template <class T, TEMPLATE_IS_SAME(T, std::string)>
const char *typeToString() noexcept { return "STRING"; }

/**
 * \brief int_vector to string converter
 * \return type name
 */
template <class T, TEMPLATE_IS_SAME(T, int_vector)>
const char *typeToString() noexcept { return "ARRAY INT"; }

/**
 * \brief float_vector to string converter
 * \return type name
 */
template <class T, TEMPLATE_IS_SAME(T, float_vector)>
const char *typeToString() noexcept { return "ARRAY FLOAT"; }

/**
 * \brief bool_vector to string converter
 * \return type name
 */
template <class T, TEMPLATE_IS_SAME(T, bool_vector)>
const char *typeToString() noexcept { return "ARRAY BOOL"; }

/**
 * \brief str_vector to string converter
 * \return type name
 */
template <class T, TEMPLATE_IS_SAME(T, str_vector)>
const char *typeToString() noexcept { return "ARRAY STRING"; }

/**
 * \brief Generic ContentType to real type converter
 */
template <ContentType T>
struct ContentTypeToType {
  using type = void;
};

/**
 * \brief int ContentType to real type converter
 */
template <>
struct ContentTypeToType<ContentType::INT_VALUE> {
  using type = int;
};

/**
 * \brief float ContentType to real type converter
 */
template <>
struct ContentTypeToType<ContentType::FLOAT_VALUE> {
  using type = float;
};

/**
 * \brief bool ContentType to real type converter
 */
template <>
struct ContentTypeToType<ContentType::BOOL_VALUE> {
  using type = bool;
};

/**
 * \brief string ContentType to real type converter
 */
template <>
struct ContentTypeToType<ContentType::STRING_VALUE> {
  using type = std::string;
};

/**
 * \brief int_vector ContentType to real type converter
 */
template <>
struct ContentTypeToType<ContentType::VECTOR_INT_VALUE> {
  using type = int_vector;
};

/**
 * \brief bool_vector ContentType to real type converter
 */
template <>
struct ContentTypeToType<ContentType::VECTOR_BOOL_VALUE> {
  using type = bool_vector;
};

/**
 * \brief str_vector ContentType to real type converter
 */
template <>
struct ContentTypeToType<ContentType::VECTOR_STRING_VALUE> {
  using type = str_vector;
};

/**
 * \brief float_vector ContentType to real type converter
 */
template <>
struct ContentTypeToType<ContentType::VECTOR_FLOAT_VALUE> {
  using type = float_vector;
};

/**
 * \brief Convert ContentType to string at runtime
 * \param type field content type
 * \return real type name
 */
static inline const char *runtimeContentToString(ContentType type) {
  if (type == ContentType::INT_VALUE) { return typeToString<int>(); }
  if (type == ContentType::FLOAT_VALUE) { return typeToString<float>(); }
  if (type == ContentType::BOOL_VALUE) { return typeToString<bool>(); }
  if (type == ContentType::STRING_VALUE) { return typeToString<std::string>(); }
  if (type == ContentType::VECTOR_INT_VALUE) {
    return typeToString<int_vector>();
  }
  if (type == ContentType::VECTOR_BOOL_VALUE) {
    return typeToString<bool_vector>();
  }
  if (type == ContentType::VECTOR_FLOAT_VALUE) {
    return typeToString<float_vector>();
  }
  if (type == ContentType::VECTOR_STRING_VALUE) {
    return typeToString<str_vector>();
  }

  return "FAILED TO GET TYPE IN RUNTIME";
}
}

#pragma once

#include <YACL/types.h>

namespace yacl {


  #define TEMPLATE_IS_SAME(firstT, secondT)                                 \
  typename std::enable_if<std::is_same<firstT, secondT>::value>::type * = \
      nullptr

  #define TEMPLATE_IS_NOT_SAME(firstT, secondT)                                 \
  typename std::enable_if<!std::is_same<firstT, secondT>::value>::type * = \
      nullptr

/**
 * \brief Generic type to ContentType converter
 * \return ContentType
 */
template <class T>
inline ContentType typeToContentTypeEnum() noexcept {
  static_assert(false, "Invalid input type in typeToContentTypeEnum");
  return ContentType::NOT_INIT;
}

/**
 * \brief int to ContentType converter
 * \return ContentType
 */
template <>
inline ContentType typeToContentTypeEnum<int>() noexcept {
  return ContentType::INT_VALUE;
}

/**
 * \brief float to ContentType converter
 * \return ContentType
 */
template <>
inline ContentType typeToContentTypeEnum<float>() noexcept {
  return ContentType::FLOAT_VALUE;
}

/**
 * \brief bool to ContentType converter
 * \return ContentType
 */
template <>
inline ContentType typeToContentTypeEnum<bool>() noexcept {
  return ContentType::BOOL_VALUE;
}

/**
 * \brief string to ContentType converter
 * \return ContentType
 */
template <>
inline ContentType typeToContentTypeEnum<std::string>() noexcept {
  return ContentType::STRING_VALUE;
}

/**
 * \brief int_vector to ContentType converter
 * \return ContentType
 */
template <>
inline ContentType typeToContentTypeEnum<int_vector>() noexcept {
  return ContentType::VECTOR_INT_VALUE;
}

/**
 * \brief float_vector to ContentType converter
 * \return ContentType
 */
template <>
inline ContentType typeToContentTypeEnum<float_vector>() noexcept {
  return ContentType::VECTOR_FLOAT_VALUE;
}

/**
 * \brief bool_vector to ContentType converter
 * \return ContentType
 */
template <>
inline ContentType typeToContentTypeEnum<bool_vector>() noexcept {
  return ContentType::VECTOR_BOOL_VALUE;
}

/**
 * \brief strt_vector to ContentType converter
 * \return ContentType
 */
template <>
inline ContentType typeToContentTypeEnum<str_vector>() noexcept {
  return ContentType::VECTOR_STRING_VALUE;
}

/**
 * \brief Generic type to string converter
 * \return type name
 */
template <class T>
inline const char *typeToString() noexcept {
  static_assert(false, "Invalid input type in typeToString");
  return "FATAL_ERROR";
}

/**
 * \brief int to string converter
 * \return type name
 */
template <>
inline const char *typeToString<int>() noexcept {
  return "INT";
}

/**
 * \brief float to string converter
 * \return type name
 */
template <>
inline const char *typeToString<float>() noexcept {
  return "FLOAT";
}

/**
 * \brief bool to string converter
 * \return type name
 */
template <>
inline const char *typeToString<bool>() noexcept {
  return "BOOL";
}

/**
 * \brief string to string converter
 * \return type name
 */
template <>
inline const char *typeToString<std::string>() noexcept {
  return "STRING";
}

/**
 * \brief int_vector to string converter
 * \return type name
 */
template <>
inline const char *typeToString<int_vector>() noexcept {
  return "ARRAY INT";
}

/**
 * \brief float_vector to string converter
 * \return type name
 */
template <>
inline const char *typeToString<float_vector>() noexcept {
  return "ARRAY FLOAT";
}

/**
 * \brief bool_vector to string converter
 * \return type name
 */
template <>
inline const char *typeToString<bool_vector>() noexcept {
  return "ARRAY BOOL";
}

/**
 * \brief str_vector to string converter
 * \return type name
 */
template <>
inline const char *typeToString<str_vector>() noexcept {
  return "ARRAY STRING";
}

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
  if (type == ContentType::INT_VALUE) {
    return typeToString<int>();
  }
  if (type == ContentType::FLOAT_VALUE) {
    return typeToString<float>();
  }
  if (type == ContentType::BOOL_VALUE) {
    return typeToString<bool>();
  }
  if (type == ContentType::STRING_VALUE) {
    return typeToString<std::string>();
  }
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
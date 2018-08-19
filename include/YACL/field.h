#pragma once
#include <string>

#include <YACL/exception.h>
#include <YACL/types.h>
#include <YACL/types_conversation.h>


namespace yacl {


#define GENERATE_OPERATOR(for_type) \
  inline operator for_type() const { return genericOperator<for_type>(); }

/**
 * \brief YACL Field class, that contains simple types of config
 */
class Field {
public:

  /**
   * \brief Default constructor deleted 
   */
  Field() = delete;


  /**
   * \brief Copy constructor deleted
   */
  Field(const Field &other) = delete;


  /**
   * \brief operator= deleted
   */
  Field &operator=(const Field &other) = delete;

  /**
   * \brief Move operator= deleted
   */
  Field& operator=(Field &&other) = delete;

  /**
   * \brief Constructor with string
   * \param name field name
   */
  explicit Field(std::string name) noexcept;


  /**
   * \brief Move constructor 
   */
  Field(Field &&other) noexcept;


  /**
   * \brief 
   * \tparam T type supported by ContentType, for example int, float, str_vector etc.
   * \param name field name
   * \param value value for field
   */
  template <class T>
  Field(const std::string &name, const T &value) noexcept;


  /**
   * \brief Destructor 
   */
  ~Field();


  /**
   * \brief Get field name const ref
   * \return field name
   */
  const std::string &getName() const;


  /**
   * \brief Get string for debug and show field content
   * \return field description
   */
  std::string debugString() const;


  // setValue specializations for primitive types
  inline void setValue(const bool &value);
  inline void setValue(const std::string &value);
  inline void setValue(const int &value);
  inline void setValue(const float &value);

  // setValue specializations for vectors
  inline void setValue(const int_vector &value);
  inline void setValue(const float_vector &value);
  inline void setValue(const bool_vector &value);
  inline void setValue(const str_vector &value);

  // setValue specializations for initializer lists
  inline void setValue(const int_initlist &value);
  inline void setValue(const float_initlist &value);
  inline void setValue(const str_initlist &value);
  inline void setValue(const bool_initlist &value);


  // cast operators overload
  GENERATE_OPERATOR(float)
  GENERATE_OPERATOR(int)
  GENERATE_OPERATOR(bool)
  GENERATE_OPERATOR(std::string)
  GENERATE_OPERATOR(int_vector)
  GENERATE_OPERATOR(float_vector)
  GENERATE_OPERATOR(bool_vector)
  GENERATE_OPERATOR(str_vector)


  // getValue specializations
  template <class T>
  T getValue() const noexcept { static_assert(false, "Invalid input type"); }

  template <>
  inline int getValue<int>() const noexcept;

  template <>
  inline float getValue<float>() const noexcept;

  template <>
  inline bool getValue<bool>() const noexcept;

  template <>
  inline std::string getValue<std::string>() const noexcept;

  template <>
  inline float_vector getValue<float_vector>() const noexcept;

  template <>
  inline int_vector getValue<int_vector>() const noexcept;

  template <>
  inline bool_vector getValue<bool_vector>() const noexcept;

  template <>
  str_vector getValue<str_vector>() const noexcept;


private:

  /**
  * \brief Support function for operator overloading
  * \tparam T type supported by ContentType, for example int, float, str_vector
  * etc. \return field value
  */
  template <class T>
  T genericOperator() const;

  std::string toString() const;


  /**
   * \brief Update debug string for vectors
   * \param vector vector from union
   * \param str string ptr to be updated, it's can't be nullptr
   */
  template <class T>
  void updateStringWithVector(T *vector, std::string *str) const;

  /**
   * \brief updateStringWithVector specialization for str_vector
   */
  template <>
  void updateStringWithVector(std::vector<std::string*> *vector,
                              std::string *str) const;


  /**
   * \brief Deallocate union field if required
   */
  void deallocVectorsAndStringIfNeeded() noexcept;


  // biggest = 4 bytes
  // TODO: int to int32_t
  union {
    int int_value = 0;
    bool bool_value;
    float float_value;
    std::string *string_value;
    std::vector<int> *vector_int_value;
    std::vector<bool> *vector_bool_value;
    std::vector<float> *vector_float_value;
    std::vector<std::string*> *vector_string_value;
  };

  /**
   * \brief field name
   */
  std::string field_name;


  /**
   * \brief field type
   */
  ContentType type = ContentType::NOT_INIT;
};

// Field declaration ends here
///////////////////////////////////////////////////////////////////////

template <class T>
Field::Field(const std::string &name, const T &value) noexcept
  : field_name(name) { setValue(value); }

template <class T>
T Field::genericOperator() const {
  if (type != typeToContentTypeEnum<T>())
    throw YACLException(
                        formatException(typeToString<T>(),
                                        runtimeContentToString(type)));
  return getValue<T>();
}

template <class T>
void Field::updateStringWithVector(T *vector, std::string *str) const {
  (*str) += "size = " + std::to_string(vector->size()) + ", content = [ ";
  if (vector->size() < 7) {
    for (size_t i = 0; i < vector->size() - 1; ++i) {
      (*str) += std::to_string((*vector)[i]) + ", ";
    }
    (*str) += std::to_string((*vector).back());
  } else {
    for (size_t i = 0; i < 3; ++i) {
      (*str) += std::to_string((*vector)[i]) + ", ";
    }
    (*str) += "... ";
    for (size_t i = vector->size() - 4; i < vector->size() - 1; ++i) {
      (*str) += std::to_string((*vector)[i]) + ", ";
    }
    (*str) += std::to_string((*vector).back());
  }
  (*str) += " ]";
}


inline void Field::setValue(const str_vector &value) {
  deallocVectorsAndStringIfNeeded();
  type = ContentType::VECTOR_STRING_VALUE;

  // need to create each string manually
  vector_string_value = new std::vector<std::string*>();

  for (const std::string &str : value)
    vector_string_value->push_back(new std::string(str));
}


inline void Field::setValue(const int_vector &value) {
  deallocVectorsAndStringIfNeeded();

  type = ContentType::VECTOR_INT_VALUE;
  vector_int_value = new std::vector<int>(value);
}

inline void Field::setValue(const int_initlist &value) {
  setValue(std::vector<int>(value));
}

inline void Field::setValue(const float_initlist &value) {
  setValue(std::vector<float>(value));
}

inline void Field::setValue(const bool_initlist &value) {
  setValue(std::vector<bool>(value));
}

inline void Field::setValue(const str_initlist &value) {
  setValue(std::vector<std::string>(value));
}

inline void Field::setValue(const float_vector &value) {
  deallocVectorsAndStringIfNeeded();

  type = ContentType::VECTOR_FLOAT_VALUE;
  vector_float_value = new std::vector<float>(value);
}

inline void Field::setValue(const bool_vector &value) {
  deallocVectorsAndStringIfNeeded();

  type = ContentType::VECTOR_BOOL_VALUE;
  vector_bool_value = new std::vector<bool>(value);
}

inline void Field::setValue(const std::string &value) {
  deallocVectorsAndStringIfNeeded();

  type = ContentType::STRING_VALUE;
  string_value = new std::string(value);
}


inline void Field::setValue(const int &value) {
  deallocVectorsAndStringIfNeeded();

  type = ContentType::INT_VALUE;
  int_value = value;
}

inline void Field::setValue(const bool &value) {
  deallocVectorsAndStringIfNeeded();

  type = ContentType::BOOL_VALUE;
  bool_value = value;
}

inline void Field::setValue(const float &value) {
  deallocVectorsAndStringIfNeeded();

  type = ContentType::FLOAT_VALUE;
  float_value = value;
}

template <>
inline int Field::getValue<int>() const noexcept { return int_value; }

template <>
inline float Field::getValue<float>() const noexcept { return float_value; }

template <>
inline bool Field::getValue<bool>() const noexcept { return bool_value; }

template <>
inline std::string Field::getValue<std::basic_string<char>>() const noexcept {
  return *string_value;
}

template <>
inline float_vector Field::getValue<float_vector>() const noexcept {
  return *vector_float_value;
}

template <>
inline int_vector Field::getValue<int_vector>() const noexcept {
  return *vector_int_value;
}


template <>
inline bool_vector Field::getValue<bool_vector>() const noexcept {
  return *vector_bool_value;
}

template <>
inline str_vector Field::getValue<str_vector>() const noexcept {

  // manually copy strings
  std::vector<std::string> copy;
  for (const std::string *str_ptr : *vector_string_value) {
    copy.emplace_back(*str_ptr);
  }

  return copy;
}
} // yacl

#include <YACL/field.h>
#include <helper/utils.h>


yacl::Field::Field(std::string name) noexcept
  : field_name(std::move(name)) {}

yacl::Field::Field(Field &&other) noexcept
  : field_name(std::move(other.field_name)),
    type(other.type) {
  int_value = other.int_value;
  other.int_value = 0;
  other.type = ContentType::NOT_INIT;
}

yacl::Field::~Field() { deallocVectorsAndStringIfNeeded(); }


const std::string &yacl::Field::getName() const { return field_name; }
std::string yacl::Field::debugString() const { return toString(); }

template <>
void yacl::Field::updateStringWithVector<std::vector<std::basic_string<char>*>>(
  std::vector<std::string*> *vector, std::string *str) const {
  (*str) += "size = " + std::to_string(vector->size()) + ", content = [ ";
  if (vector->size() < 6) {
    for (size_t i = 0; i < vector->size() - 1; ++i) {
      (*str) += *((*vector)[i]) + ", ";
    }
    (*str) += *((*vector).back());
  } else {
    for (size_t i = 0; i < 3; ++i) { (*str) += *((*vector)[i]) + ", "; }
    (*str) += " ... ";
    for (size_t i = vector->size() - 4; i < vector->size() - 1; ++i) {
      (*str) += *((*vector)[i]) + ", ";
    }
    (*str) += *((*vector).back());
  }
  (*str) += " ]";
}

void yacl::Field::deallocVectorsAndStringIfNeeded() noexcept {
  if (type == ContentType::VECTOR_INT_VALUE
  ) { DELETE_AND_FORCE_NULL(vector_int_value); } else if (
    type == ContentType::STRING_VALUE
  ) { DELETE_AND_FORCE_NULL(string_value); } else if (
    type == ContentType::VECTOR_BOOL_VALUE
  ) { DELETE_AND_FORCE_NULL(vector_bool_value); } else if (
    type == ContentType::VECTOR_FLOAT_VALUE) {
    DELETE_AND_FORCE_NULL(vector_float_value);
  } else if (type == ContentType::VECTOR_STRING_VALUE) {
    // need to delete each string manually
    for (std::string *str : *vector_string_value) {
      DELETE_AND_FORCE_NULL(str);
    }
    DELETE_AND_FORCE_NULL(vector_string_value);
  }
}


std::string yacl::Field::toString() const {
  std::string str("DebugString for \"" + field_name + "\". Value = <\"");

  if (type == ContentType::INT_VALUE) { str += std::to_string(int_value); } else
    if (type == ContentType::FLOAT_VALUE
    ) { str += std::to_string(float_value); } else if (
      type == ContentType::BOOL_VALUE
    ) { str += (bool_value ? "true" : "false"); } else if (
      type == ContentType::STRING_VALUE) { str += *string_value; } else if (
      type == ContentType::VECTOR_INT_VALUE
    ) { updateStringWithVector(vector_int_value, &str); } else if (
      type == ContentType::VECTOR_BOOL_VALUE
    ) { updateStringWithVector(vector_bool_value, &str); } else if (
      type == ContentType::VECTOR_FLOAT_VALUE) {
      updateStringWithVector(vector_float_value, &str);
    } else if (type == ContentType::VECTOR_STRING_VALUE) {
      updateStringWithVector(vector_string_value, &str);
    }

  str += "\">\n";
  return str;
}

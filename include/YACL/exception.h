#pragma once

#include <utility>


namespace yacl {
/**
 * \brief YACL Exception class
 */
class YACLException : public std::exception {
public:
  YACLException(const std::string &message)
    : msg(std::move(message)) {}

  const char *what() const override { return msg.c_str(); }

private:
  const std::string msg;
};



/**
 * \brief Format string for wrong type conversation
 * \param user_try type user try to get
 * \param real_type real type of field
 * \return formatted string
 */
static std::string formatException(const char *user_try,
                                   const char *real_type) noexcept {
  const static std::string pattern =
      "Invalid cast operator, you're trying to get ";
  return std::string(pattern + user_try + ", but current type is " + real_type +
                     '\n');
}
}

#pragma once

#include <vector>
#include <map>
#include <functional>

#include <string>
#include <locale>

#include <algorithm>

#include <cctype>
#include <cassert>

#include <YACL/types.h>

namespace yacl {

#define DELETE_AND_FORCE_NULL(ptr) \
    delete(ptr); \
    ptr = nullptr


// TODO: fast trims


/**
 * \brief Left Trim
 * \param s string for trim
 * \return trimmed string
 */
static inline std::string &ltrim(std::string &s) noexcept {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                  std::not1(std::ptr_fun<
                                              int, int>(std::isspace))));
  return s;
}

/**
 * \brief Right Trim
 * \param s string for trim
 * \return trimmed string
 */
static inline std::string &rtrim(std::string &s) noexcept {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
          s.end());
  return s;
}

/**
* \brief Left + Right Trim
* \param s string for trim
* \return trimmed string
*/
static inline std::string &trim(std::string &s) { return ltrim(rtrim(s)); }


/**
 * \brief Function to print std::vector
 * \tparam T std::vector
 * \param vector vector for print
 * \param sep separator
 */
template <class T>
static inline void print_vector(const T &vector, char sep = ' ') noexcept {
  for (const auto &e : vector)
    std::cout << e << sep;
  std::cout << std::endl;
}


/**
 * \brief Fast erase from vector.
 *        Swap idx with last and pop.
 * \tparam T std::vector
 * \param vector vector to pop
 * \param idx idx to erase
 */
template <class T>
static inline void vector_fast_pop(T &vector, size_t idx) noexcept {
  std::swap(vector[idx], vector.back());
  vector.pop_back();
}


/**
 * \brief Erase N idx from vector, using slow erase
 * \tparam T std::vector
 * \param vector vector to pop
 * \param idxs idxs to pop
 */
template <class T>
static inline void vector_multi_erase(T &vector,
                                      std::vector<size_t> &idxs) noexcept {
  std::sort(idxs.begin(), idxs.end());
  for (auto i = idxs.rbegin(); i != idxs.rend(); ++i) {
    // create var here not necessary
    // but core guidlines told us to always get return value
    const auto it = vector.erase(vector.begin() + *i);
  }
}


/**
 * \brief Check if list contains char
 * \param ch char to find
 * \param list list where we looking for
 * \return true if found, false otherwise
 */
static inline bool isCharEqualsOneOfInTheList(const char ch,
                                              const std::initializer_list<char>
                                              &list) noexcept {
  for (char c : list)
    if (c == ch)
      return true;
  return false;
}


/**
* \brief Check if char closed for same token.
*        Same token example ', " etc
* \param ch char to find
* \param map map char with cnt 
* \return true if closed, false otherwise
*/
static inline bool isCharClosedForSameTokens(const char ch,
                                             std::map<char, uint16_t> &map)
noexcept {
  auto it = map.find(ch);
  if (it != map.end()) { return ++it->second % 2 == 0; }

  return false;
}


/**
 * \brief Check if char opened for same token.
 *        Same token example ', " etc
 * \param ch char to find
 * \param map map char with cnt
 * \return true if opened, false otherwise
 */
static inline bool isCharOpenedForSameTokens(const char ch,
                                             std::map<char, uint16_t> &map)
noexcept {
  auto it = map.find(ch);
  if (it != map.end()) { return ++it->second % 2 != 0; }

  return false;
}


/**
 * \brief Smart splitting line.
 *        Сonsidering opend/close tokens
 * \param str string for split
 * \param sep separator
 * \param ext_sep second separator
 * \return splitted string as vector
 */
static inline std::vector<std::string> smart_bracket_split(
  const std::string &str, char sep,
  char ext_sep = '~') noexcept {
  const static std::initializer_list<char> token_chars_open = {'{', '['};
  const static std::initializer_list<char> token_chars_close = {'}', ']'};
  static std::map<char, uint16_t> token_same_open_close =
    {{'\'', 0}, {'\"', 0}};
  token_same_open_close.at('\'') = 0;
  token_same_open_close.at('\"') = 0;

  size_t first_word = 0,
         second_word = 0;

  std::vector<std::string> splitted;
  bool locked = false;
  bool locked_same_token = false;

  size_t locked_cnt = 0;

  for (size_t i = 0; i < str.size(); ++i) {
    ++second_word;

    if (!locked && !locked_same_token) {
      if (isCharEqualsOneOfInTheList(str[i], token_chars_open)) {
        locked = true;
        locked_cnt++;
      } else if (str[i] == sep || str[i] == ext_sep) {
        splitted.emplace_back(std::move(str.substr(first_word,
                                                   second_word - first_word -
                                                   1)));
        first_word = second_word;
      } else if (isCharOpenedForSameTokens(str[i], token_same_open_close)) {
        locked_same_token = true;
      }
    } else {
      if (locked) {
        if (isCharEqualsOneOfInTheList(str[i], token_chars_close))
          locked_cnt--;
        else if (isCharEqualsOneOfInTheList(str[i], token_chars_open))
          locked_cnt++;

        locked = (locked_cnt != 0);
      } else if (locked_same_token) {
        if (str[i - 1] != '\\') {
          locked_same_token = !
            isCharClosedForSameTokens(str[i], token_same_open_close);
        }
      }
    }
  }

  if (first_word != second_word) {
    splitted.emplace_back(std::move(str.substr(first_word,
                                               second_word - first_word)));
  }

  for (auto &it_str : splitted) { trim(it_str); }

  return splitted;
}


/**
 * \brief Checking is filed unnamed
 * \param str string for check
 * \return true if anon, false otherwise
 */
static inline bool isFieldAnon(const std::string &str) noexcept {
  bool found_eq = false;
  for (char c : str) {
    if (c == '=')
      found_eq = true;
    else if (c == '{') { return found_eq; }
  }

  assert((str + " FAILED TO DETECT IS FIELD ANON").c_str());
  return false;
}


/**
 * \brief Simple string split with separator
 * \param str str for split
 * \param sep separator
 * \param ext_sep second separator
 * \return splitted string as vector
 */
static std::vector<std::string> split(const std::string &str,
                                      char sep,
                                      char ext_sep = ';') {

  size_t first_word = 0,
         second_word = 0;

  std::vector<std::string> splitted;

  for (size_t i = 0; i < str.size(); ++i) {
    ++second_word;

    if (str[i] == sep || str[i] == ext_sep) {
      splitted.
        emplace_back(str.substr(first_word, second_word - first_word - 1));
      first_word = second_word;
    }
  }

  if (first_word != second_word) {
    splitted.emplace_back(str.substr(first_word, second_word - first_word));
  }

  for (auto &it_str : splitted) { trim(it_str); }

  return splitted;
}

} // yacl

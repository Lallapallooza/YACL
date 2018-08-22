#pragma once

#include <string>

#include <map>
#include <vector>

#include <YACL/settings.h>
#include <YACL/settings_ptr.h>

namespace yacl {

/**
* \brief Main YACL config class
*/
class Config {
public:
  

  /**
   * \brief Parse config from file
   * \param path file path
   * \return config root
   */
  static SettingsUniquePtr parseConfigFromFile(const std::string &path) noexcept;


  /**
   * \brief Parse config from string
   * \param conf raw config
   * \return config root
   */
  static SettingsUniquePtr parseConfig(std::string conf) noexcept;


  /**
   * \brief Print config rec
   * \param sett root 
   * \param n_tabs tabs for formatting
   */
  static void printConfig(const Setting *sett, uint16_t n_tabs = 1) noexcept;

private:


  /**
   * \brief Nested class in Config for bfs
   */
  struct PairPrior {
    uint16_t prior;
    std::string content;

  };


  /**
   * \brief Nested helper class in Config 
   */
  struct DoubleCharSize_tPair {
    char first_open_bracket = ' ';
    char last_closed_bracket = ' ';

    size_t ind_first = 0;
    size_t ind_second = 0;

  };


  /**
   * \brief Nested helper class in Config 
   */
  struct GraphPair {
    std::map<std::string, Setting*> inner;
    std::vector<Setting*> anon;
  };


  /**
   * \brief Conver vector to map
   * \param fields vector of settings
   * \return map of settings
   */
  static GraphPair vectorGraphToMapGraph(
    const std::vector<Setting*> &fields) noexcept;


  /**
   * \brief Convert graph vec to map
   * \param graph graph node
   */
  static void convertGraphRec(Setting *graph) noexcept;


  /**
   * \brief Parse name and mark begin, end
   * \param raw_config raw config
   * \param name config name to be filled
   * \return pair of first open, last closed chars and 2 idx 
   */
  static DoubleCharSize_tPair fillNameAndMarkFields(
    const std::string &raw_config, std::string *name) noexcept;


  /**
   * \brief Convert vector of fields to map
   * \param fields vectors of fields
   * \return map of fields
   */
  static std::map<std::string, Field> vectorFieldsToMapFields(
    std::vector<Field> fields) noexcept;


  /**
   * \brief Parse all fields from bracker {}
   * \param raw_bracket_content bracket string
   * \return map of fields
   */
  static std::map<std::string, std::string> bracketIntoMap(
    const std::string &raw_bracket_content) noexcept;


  /**
   * \brief Parse all fields in vector from raw string
   * \param raw_bracket_content string with fields
   * \return vector of fields
   */
  static std::vector<Field> parseBracketIntoFields(
    const std::string &raw_bracket_content) noexcept;
};
}

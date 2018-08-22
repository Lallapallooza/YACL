#pragma once

#include <map>
#include <string>
#include <vector>

#include <YACL/field.h>

namespace yacl {
/**
 * \brief YACL node class
 */

class Setting {
 public:
  /**
   * \brief Constructor with params
   * \param base parent
   * \param dep node depth
   */
  explicit Setting(Setting *base = nullptr, uint16_t dep = 0) noexcept;

  /**
   * \brief Recursively delete graph starting from node
   * \param sett node
   */
  static void recDeleteGraph(Setting *sett);

  /**
   * \brief Get field with name
   *        if field doesnt exists, std::terminate be called
   * \param name field name
   * \return const Field ptr
   */
  inline const Field *field(const std::string &name) const noexcept;

  /**
   * \brief Get setting with name
   *        if setting doesnt exists, std::terminate be called
   * \param name setting name
   * \return const setting ptr
   */
  inline const Setting *setting(const std::string &name) const noexcept;

  /**
   * \brief Get all anon fields from node
   * \return const vector of anon fields
   */
  inline std::vector<Setting *> anons() const noexcept;

  /**
   * \brief Check is setting filled
   * \return filled
   */
  inline bool isFilled() const noexcept;

  /**
   * \brief Get setting name
   * \return setting name
   */
  const std::string &getName() const noexcept;

 private:
  // This class always filling by Config
  // So instead of created dozen methods
  // Just declate friend
  friend class Config;

  /**
   * \brief Means setting filled or not, need for parsing
   */

  bool filled = false;

  /**
   * \brief Means field anon or names
   */
  bool anon = false;

  /**
   * \brief Means node depth in graph
   */
  uint16_t depth;

  /**
   * \brief Parent node
   */
  Setting *father;

  /**
   * \brief Node name
   */
  std::string setting_name;

  /**
   * \brief Node primitive fields
   */
  std::map<std::string, Field> fields;

  /**
   * \brief Node childs
   */
  std::map<std::string, Setting *> inner_graph;

  /**
   * \brief Node anon childs
   */
  std::vector<Setting *> anon_inner_graph;

  // TODO: delete field
  /**
   * \brief Vector where we holding setting while parsing
   */
  std::vector<Setting *> inner_graph_vec;
};

inline Setting::Setting(Setting *base, uint16_t dep) noexcept
    : depth(dep), father(base) {}

inline void Setting::recDeleteGraph(Setting *sett) {
  for (const auto &pair : sett->inner_graph) {
    recDeleteGraph(pair.second);
  }

  for (const auto &anons : sett->anon_inner_graph) {
    recDeleteGraph(anons);
  }

  delete sett;
}

inline const Field *Setting::field(const std::string &name) const noexcept {
  const auto find = fields.find(name);
  return find == fields.end() ? nullptr : &(find->second);
}

inline const Setting *Setting::setting(const std::string &name) const noexcept {
  const auto find = inner_graph.find(name);
  return find == inner_graph.end() ? nullptr : (find->second);
}

inline std::vector<Setting *> Setting::anons() const noexcept {
  return anon_inner_graph;
}

inline bool Setting::isFilled() const noexcept { return filled; }

inline const std::string &Setting::getName() const noexcept {
  return setting_name;
}

}  // namespace yacl

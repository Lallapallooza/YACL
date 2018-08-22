#pragma once

#include <YACL/settings.h>

namespace yacl {

/**
 * \brief Unique ptr wrapper for auto memory managment for raw ptr
 */
class SettingsUniquePtr {
public:
  /**
   * \brief Constructor with param
   * \param sett root node 
   */
  SettingsUniquePtr(Setting *sett);


  /**
   * \brief Default Constructor deleted
   */
  SettingsUniquePtr() = delete;


  /**
   * \brief Copy Constructor deleted
   */
  SettingsUniquePtr(const SettingsUniquePtr &other) = delete;


  /**
   * \brief copy operator= deleted
   */
  SettingsUniquePtr &operator=(const SettingsUniquePtr &other) = delete;


  /**
   * \brief Move Constructor 
   * \param other other node
   */
  SettingsUniquePtr(SettingsUniquePtr &&other) noexcept;


  /**
   * \brief Mode operator=
   * \param other other node
   * \return node with settings inside
   */
  SettingsUniquePtr &operator=(SettingsUniquePtr &&other) noexcept;


  /**
   * \brief Destructor 
   */
  ~SettingsUniquePtr();


  /**
   * \brief operator-> return node
   * \return node 
   */
  const Setting *operator->() const noexcept { return settings; }
  

  /**
   * \brief operator* return node
   * \return node
   */
  const Setting *operator*() const noexcept { return settings; }

private:
  /**
   * \brief node
   */
  Setting *settings = nullptr;
};


} // yacl

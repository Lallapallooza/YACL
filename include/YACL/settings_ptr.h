#pragma once

#include <YACL/settings.h>

namespace yacl {

class SettingsUniquePtr {
public:
  SettingsUniquePtr(Setting *sett);

  SettingsUniquePtr() = delete;
  SettingsUniquePtr(const SettingsUniquePtr &other) = delete;
  SettingsUniquePtr &operator=(const SettingsUniquePtr &other) = delete;


  SettingsUniquePtr(SettingsUniquePtr &&other) noexcept;

  SettingsUniquePtr &operator=(SettingsUniquePtr &&other) noexcept;


  ~SettingsUniquePtr();

  inline const Setting* operator->() const noexcept { return settings; }
  inline const Setting *operator*() const noexcept { return settings; }
  
private:
  Setting *settings = nullptr;
};


} // yacl
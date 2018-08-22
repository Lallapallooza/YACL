#include <YACL/settings_ptr.h>

yacl::SettingsUniquePtr::SettingsUniquePtr(Setting *sett)
  : settings(sett) {}

yacl::SettingsUniquePtr::
SettingsUniquePtr(SettingsUniquePtr &&other) noexcept
  : settings(other.settings) { other.settings = nullptr; }

yacl::SettingsUniquePtr &yacl::SettingsUniquePtr::operator=(
  SettingsUniquePtr &&other) noexcept {
  settings = other.settings;
  other.settings = nullptr;
  return *this;
}

yacl::SettingsUniquePtr::~SettingsUniquePtr() {
  Setting::recDeleteGraph(settings);
}

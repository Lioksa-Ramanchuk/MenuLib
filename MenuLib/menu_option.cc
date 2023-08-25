#include "pch.h"

#include "menu_option.h"

namespace menu_lib {

#pragma region Constructors
MenuOption::MenuOption(std::string_view key, std::string_view value,
                       const std::function<void()>& on_selected,
                       const std::function<void()>& on_deselected,
                       bool is_selected)
    : key_{key},
      value_{value},
      on_selected_{on_selected},
      on_deselected_{on_deselected},
      is_selected_{is_selected} {}

MenuOption::MenuOption(const Config& config)
    : MenuOption(config.key, config.value, config.on_selected,
                 config.on_deselected, config.is_selected) {}
#pragma endregion

#pragma region General
void MenuOption::Select() {
  is_selected_ = true;
  if (on_selected_) {
    on_selected_();
  }
}

void MenuOption::Deselect() {
  is_selected_ = false;
  if (on_deselected_) {
    on_deselected_();
  }
}
#pragma endregion

#pragma region Setters& Getters
void MenuOption::set_key(std::string_view key) { key_ = key; }
std::string MenuOption::key() const { return key_; }

void MenuOption::set_value(std::string_view value) { value_ = value; }
std::string MenuOption::value() const { return value_; }

void MenuOption::set_on_selected(const std::function<void()>& on_selected) {
  on_selected_ = on_selected;
}
std::function<void()> MenuOption::on_selected() const { return on_selected_; }

void MenuOption::set_on_deselected(const std::function<void()>& on_deselected) {
  on_deselected_ = on_deselected;
}
std::function<void()> MenuOption::on_deselected() const {
  return on_deselected_;
}

void MenuOption::set_is_selected(bool is_selected) {
  is_selected_ = is_selected;
}
bool MenuOption::is_selected() const { return is_selected_; }
#pragma endregion

}  // namespace menu_lib
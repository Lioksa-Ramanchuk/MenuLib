#ifndef MENU_LIB_MENU_OPTION_H
#define MENU_LIB_MENU_OPTION_H

#include "pch.h"

namespace menu_lib {

class MenuOption {
 public:
  struct Config {
   public:
    std::string key = "";
    std::string value = "";

    // Warning: Do not change the size of the menu options_ list!
    std::function<void()> on_selected = {};

    // Warning: Do not change the size of the menu options_ list!
    std::function<void()> on_deselected = {};

    bool is_selected = false;
  };

#pragma region Constructors
  // Warning: on_selected and on_deselected must not change the size of the menu
  // options_ list!
  explicit MenuOption(std::string_view key, std::string_view value,
                      const std::function<void()>& on_selected = {},
                      const std::function<void()>& on_deselected = {},
                      bool is_selected = false);

  explicit MenuOption(const Config& config);
#pragma endregion

#pragma region General
  void Select();
  void Deselect();
#pragma endregion

#pragma region Setters& Getters
  void set_key(std::string_view key);
  std::string key() const;

  void set_value(std::string_view value);
  std::string value() const;

  // Warning: Do not change the size of the menu options_ list!
  void set_on_selected(const std::function<void()>& on_selected);
  std::function<void()> on_selected() const;

  // Warning: Do not change the size of the menu options_ list!
  void set_on_deselected(const std::function<void()>& on_deselected);
  std::function<void()> on_deselected() const;

  void set_is_selected(bool is_selected);
  bool is_selected() const;
#pragma endregion

 private:
  std::string key_;
  std::string value_;

  // Warning: Do not change the size of the menu options_ list!
  std::function<void()> on_selected_;

  // Warning: Do not change the size of the menu options_ list!
  std::function<void()> on_deselected_;

  bool is_selected_;
};

}  // namespace menu_lib

#endif  // MENU_LIB_MENU_OPTION_H
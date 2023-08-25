#ifndef MENU_LIB_MENU_H
#define MENU_LIB_MENU_H

#include "pch.h"

#include "menu_option.h"

namespace menu_lib {

class Menu {
 public:
#pragma region General
  void Display(std::ostream& out = std::cout) const;

  // Calls `func` for menu options where `predicate` returns true (the default is
  // for all options), starting from the first option to the last.
  Menu& ApplyToOptions(
      const std::function<void(MenuOption& option, size_t index)>& func,
      const std::function<bool(const MenuOption& option, size_t index)>&
          predicate = [](const MenuOption&, size_t) { return true; });
#pragma endregion

#pragma region Selection
  // Use `SelectAll()` for selecting all the menu options instead.
  Menu& Select() = delete;

  Menu& Select(const std::function<bool(const MenuOption& option,
                                        size_t index)>& predicate);

  Menu& Select(const std::vector<size_t>& option_indices);

  template <std::convertible_to<size_t>... Indices>
  Menu& Select(Indices... option_index) {
    return Select({static_cast<size_t>(option_index)...});
  }

  Menu& Select(const std::vector<std::string_view>& option_keys);

  template <std::convertible_to<std::string_view>... Keys>
  Menu& Select(const Keys&... option_key) {
    return Select({static_cast<std::string_view>(option_key)...});
  }

  Menu& SelectAll();
#pragma endregion

#pragma region Deselection
  // Use `DeselectAll()` for deselecting all the menu options instead.
  Menu& Deselect() = delete;

  Menu& Deselect(const std::function<bool(const MenuOption& option,
                                          size_t index)>& predicate);

  Menu& Deselect(const std::vector<size_t>& option_indices);

  template <std::convertible_to<size_t>... Indices>
  Menu& Deselect(Indices... option_index) {
    return Deselect({static_cast<size_t>(option_index)...});
  }

  Menu& Deselect(const std::vector<std::string_view>& keys);

  template <std::convertible_to<std::string_view>... Keys>
  Menu& Deselect(const Keys&... option_key) {
    return Deselect({static_cast<std::string_view>(option_key)...});
  }

  Menu& DeselectAll();
#pragma endregion

#pragma region Getting menu info
  std::vector<MenuOption> GetOptions() const;

  std::vector<MenuOption> GetOptions(
      const std::function<bool(const MenuOption& option, size_t index)>&
          predicate) const;

  std::vector<MenuOption> GetOptions(
      const std::vector<size_t>& option_indices) const;

  template <std::convertible_to<size_t>... Indices>
  std::vector<MenuOption> GetOptions(const Indices&... option_index) const {
    return GetOptions({static_cast<size_t>(option_index)...});
  }

  std::vector<MenuOption> GetOptions(
      const std::vector<std::string_view>& option_keys) const;

  template <std::convertible_to<std::string_view>... Keys>
  std::vector<MenuOption> GetOptions(const Keys&... option_key) const {
    return GetOptions({static_cast<std::string_view>(option_key)...});
  }

  std::vector<MenuOption> GetSelected() const;

  std::vector<MenuOption> GetDeselected() const;
#pragma endregion

#pragma region Adding options
  Menu& AddOptions(const std::vector<MenuOption>& options);

  template <std::same_as<MenuOption>... Options>
  Menu& AddOptions(const Options&... option) {
    return AddOptions({{option...}});
  }
#pragma endregion

#pragma region Removing options
  Menu& RemoveOptions();

  Menu& RemoveOptions(const std::function<bool(const MenuOption& option,
                                               size_t index)>& predicate);

  Menu& RemoveOptions(const std::vector<size_t>& option_indices);

  template <std::convertible_to<size_t>... Indices>
  Menu& RemoveOptions(Indices... option_index) {
    return RemoveOptions({static_cast<size_t>(option_index)...});
  }

  Menu& RemoveOptions(const std::vector<std::string_view>& option_keys);

  template <std::convertible_to<std::string_view>... Keys>
  Menu& RemoveOptions(Keys... option_key) {
    return RemoveOptions({static_cast<std::string_view>(option_key)...});
  }
#pragma endregion

#pragma region Setters& Getters
  void set_id(int id);
  int id() const;

  void set_title(std::string_view title);
  std::string title() const;

  void set_options(const std::vector<MenuOption>& options);
  std::vector<MenuOption> options() const;

  // Warning: Do not change the size of the menu options_ list!
  void set_option_to_string(
      const std::function<std::string(const MenuOption& option, size_t index)>&
          option_to_string);
  std::function<std::string(const MenuOption& option, size_t index)>
  option_to_string() const;
#pragma endregion

 protected:
  explicit Menu(
      int id,
      const std::function<std::string(const MenuOption& option, size_t index)>&
          option_to_string =
              [](const MenuOption& option, size_t index) {
                return std::format("{} - {}", index + 1, option.value());
              },
      const std::string& title = "",
      const std::vector<MenuOption>& options = {});

 private:
  friend class IMenuBuilder;

  int id_;
  std::string title_;
  std::vector<MenuOption> options_;
  std::function<std::string(const MenuOption& option, size_t index)>
      option_to_string_;
};

}  // namespace menu_lib

#endif  // MENU_LIB_MENU_H
#include "pch.h"

#include "menu.h"

namespace {

template <typename T>
std::vector<T> SortUnique(const std::vector<T>& vec) {
  std::vector<T> distinct_vec = vec;
  std::ranges::sort(distinct_vec);
  auto [duplicates_start, duplicates_end] = std::ranges::unique(distinct_vec);
  distinct_vec.erase(duplicates_start, duplicates_end);
  return distinct_vec;
}

}  // namespace

namespace menu_lib {

Menu::Menu(int id,
           const std::function<std::string(const MenuOption& option,
                                           size_t index)>& option_to_string,
           const std::string& title, const std::vector<MenuOption>& options)
    : id_{id},
      title_{title},
      options_{options},
      option_to_string_{option_to_string} {}

#pragma region General
void Menu::Display(std::ostream& out) const {
  std::stringstream ss;
  if (!title_.empty()) {
    ss << title_ << std::endl;
  }
  for (size_t i = 0; const auto& option : options_) {
    ss << option_to_string_(option, i++) << std::endl;
  }
  out << ss.str();
}

Menu& Menu::ApplyToOptions(
    const std::function<void(MenuOption& option, size_t index)>& func,
    const std::function<bool(const MenuOption& option, size_t index)>&
        predicate) {
  for (size_t i = 0; auto& option : options_) {
    if (predicate(option, i)) {
      func(option, ++i);
    }
  }
  return *this;
}
#pragma endregion

#pragma region Selection
Menu& Menu::Select(const std::function<bool(const MenuOption& option,
                                            size_t index)>& predicate) {
  ApplyToOptions([](MenuOption& option, size_t) { option.Select(); },
                 predicate);
  return *this;
}

Menu& Menu::Select(const std::vector<size_t>& option_indices) {
  auto n_options = options_.size();
  if (n_options == 0) {
    return *this;
  }

  for (auto i : SortUnique(option_indices)) {
    if (i < n_options) {
      options_[i].Select();
    }
  }
  return *this;
}

Menu& Menu::Select(const std::vector<std::string_view>& option_keys) {
  auto n_options = options_.size();
  if (n_options == 0) {
    return *this;
  }

  return Select(
      [keys = SortUnique(option_keys)](const MenuOption& option, size_t) {
        return std::ranges::binary_search(keys, option.key());
      });
}

Menu& Menu::SelectAll() {
  return Select([](const MenuOption&, size_t) { return true; });
}
#pragma endregion

#pragma region Deselection
Menu& Menu::Deselect(const std::function<bool(const MenuOption& option,
                                              size_t index)>& predicate) {
  ApplyToOptions([](MenuOption& option, size_t) { option.Deselect(); },
                 predicate);
  return *this;
}

Menu& Menu::Deselect(const std::vector<size_t>& option_indices) {
  auto n_options = options_.size();
  if (n_options == 0) {
    return *this;
  }

  for (auto i : SortUnique(option_indices)) {
    if (i < n_options) {
      options_[i].Deselect();
    }
  }
  return *this;
}

Menu& Menu::Deselect(const std::vector<std::string_view>& option_keys) {
  auto n_options = options_.size();
  if (n_options == 0) {
    return *this;
  }

  return Deselect(
      [keys = SortUnique(option_keys)](const MenuOption& option, size_t) {
        return std::ranges::binary_search(keys, option.key());
      });
}

Menu& Menu::DeselectAll() {
  return Deselect([](const MenuOption&, size_t) { return true; });
}
#pragma endregion

#pragma region Getting menu info
std::vector<MenuOption> Menu::GetOptions() const { return options_; }

std::vector<MenuOption> Menu::GetOptions(
    const std::function<bool(const MenuOption& option, size_t index)>&
        predicate) const {
  auto n_options = options_.size();
  if (n_options == 0) {
    return {};
  }

  std::vector<MenuOption> options;
  options.reserve(n_options);
  for (size_t i = 0; const auto& option : options_) {
    if (predicate(option, i++)) {
      options.push_back(option);
    }
  }
  return options;
}

std::vector<MenuOption> Menu::GetOptions(
    const std::vector<size_t>& option_indices) const {
  auto n_options = options_.size();
  if (n_options == 0) {
    return {};
  }
  std::vector<bool> to_get(n_options, false);
  for (auto i : SortUnique(option_indices)) {
    if (i < n_options) {
      to_get[i] = true;
    }
  }
  return GetOptions(
      [&to_get](const MenuOption&, size_t i) { return to_get[i]; });
}

std::vector<MenuOption> Menu::GetOptions(
    const std::vector<std::string_view>& option_keys) const {
  auto n_options = options_.size();
  if (n_options == 0) {
    return {};
  }
  return GetOptions(
      [keys = SortUnique(option_keys)](const MenuOption& option, size_t) {
        return std::ranges::binary_search(keys, option.key());
      });
}

std::vector<MenuOption> Menu::GetSelected() const {
  return GetOptions(
      [](const MenuOption& option, size_t) { return option.is_selected(); });
}

std::vector<MenuOption> Menu::GetDeselected() const {
  return GetOptions(
      [](const MenuOption& option, size_t) { return !option.is_selected(); });
}
#pragma endregion

#pragma region Adding options
Menu& Menu::AddOptions(const std::vector<MenuOption>& options) {
  options_.insert(std::end(options_), std::begin(options), std::end(options));
  return *this;
}
#pragma endregion

#pragma region Removing options
Menu& Menu::RemoveOptions(const std::function<bool(const MenuOption& option,
                                                   size_t index)>& predicate) {
  auto n_options = options_.size();
  if (n_options == 0) {
    return *this;
  }
  options_.erase(std::begin(std::ranges::remove_if(
                     options_,
                     [&predicate, i = static_cast<size_t>(0)](
                         const MenuOption& option) mutable {
                       return predicate(option, i++);
                     })),
                 std::end(options_));
  return *this;
}

Menu& Menu::RemoveOptions(const std::vector<size_t>& option_indices) {
  auto n_options = options_.size();
  if (n_options == 0) {
    return *this;
  }
  std::vector<bool> to_remove(n_options, false);
  for (auto i : SortUnique(option_indices)) {
    if (i < n_options) {
      to_remove[i] = true;
    }
  }
  return RemoveOptions(
      [&to_remove](const MenuOption&, size_t i) { return to_remove[i]; });
}

Menu& Menu::RemoveOptions(const std::vector<std::string_view>& option_keys) {
  auto n_options = options_.size();
  if (n_options == 0) {
    return *this;
  }
  return RemoveOptions(
      [keys = SortUnique(option_keys)](const MenuOption& option, size_t) {
        return std::ranges::binary_search(keys, option.key());
      });
}

Menu& Menu::RemoveOptions() {
  options_.clear();
  return *this;
}
#pragma endregion

#pragma region Setters& Getters
void Menu::set_id(int id) { id_ = id; }
int Menu::id() const { return id_; }

void Menu::set_title(std::string_view title) { title_ = title; }
std::string Menu::title() const { return title_; }

void Menu::set_options(const std::vector<MenuOption>& options) {
  options_ = options;
}
std::vector<MenuOption> Menu::options() const { return options_; }

void Menu::set_option_to_string(
    const std::function<std::string(const MenuOption& option, size_t index)>&
        option_to_string) {
  option_to_string_ = option_to_string;
}
std::function<std::string(const MenuOption& option, size_t index)>
Menu::option_to_string() const {
  return option_to_string_;
}
#pragma endregion

}  // namespace menu_lib
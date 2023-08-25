#include "pch.h"

#include "menu_builder.h"

namespace menu_lib {

MenuBuilder::MenuBuilder(int id) { menu_.reset(ConstructMenu(id)); }

IMenuBuilder& MenuBuilder::WithTitle(std::string_view title) {
  if (menu_ != nullptr) {
    menu_->set_title(title);
  }
  return *this;
}

IMenuBuilder& MenuBuilder::WithOptionToString(
    const std::function<std::string(const MenuOption& option, size_t index)>&
        option_to_string) {
  if (menu_ != nullptr) {
    menu_->set_option_to_string(option_to_string);
  }
  return *this;
}

IMenuBuilder& MenuBuilder::WithOption(const MenuOption& option) {
  if (menu_ != nullptr) {
    menu_->AddOptions(option);
  }
  return *this;
}

std::unique_ptr<Menu> MenuBuilder::Build() { return std::move(menu_); }

}  // namespace menu_lib
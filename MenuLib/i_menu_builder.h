#ifndef MENU_LIB_I_MENU_BUILDER_H
#define MENU_LIB_I_MENU_BUILDER_H

#include <functional>
#include <memory>
#include <string>

#include "menu.h"
#include "menu_option.h"

namespace menu_lib {

class IMenuBuilder {
 public:
  virtual ~IMenuBuilder() = default;

  virtual IMenuBuilder& WithTitle(std::string_view title) = 0;

  virtual IMenuBuilder& WithOptionToString(
      const std::function<std::string(const MenuOption& option, size_t index)>&
          option_to_string) = 0;

  virtual IMenuBuilder& WithOption(const MenuOption& option) = 0;

  virtual std::unique_ptr<Menu> Build() = 0;

 protected:
  static Menu* ConstructMenu(int id);
};

}  // namespace menu_lib

#endif  // MENU_LIB_I_MENU_BUILDER_H
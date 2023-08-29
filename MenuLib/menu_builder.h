#ifndef MENU_LIB_MENU_BUILDER_H
#define MENU_LIB_MENU_BUILDER_H

#include <functional>
#include <memory>
#include <string>

#include "i_menu_builder.h"
#include "menu_option.h"
#include "menu.h"

namespace menu_lib {

// Builds a single menu. Can't be reused.
class MenuBuilder : public IMenuBuilder {
 public:
  explicit MenuBuilder(int id);

  IMenuBuilder& WithTitle(std::string_view title) override;

  IMenuBuilder& WithOptionToString(
      const std::function<std::string(const MenuOption& option, size_t index)>&
          option_to_string) override;

  IMenuBuilder& WithOption(const MenuOption& option) override;

  std::unique_ptr<Menu> Build() override;

 private:
  std::unique_ptr<Menu> menu_;
};

}  // namespace menu_lib

#endif  // MENU_LIB_MENU_BUILDER_H
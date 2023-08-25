#include "pch.h"

#include "i_menu_builder.h"

#include "menu.h"

namespace menu_lib {

Menu* IMenuBuilder::ConstructMenu(int id) { return new Menu(id); }

}  // namespace menu_lib
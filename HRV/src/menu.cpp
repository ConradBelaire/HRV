#include "menu.h"


/**
 * @brief Menu constructor
 *
 * @param n The name of the menu
 * @param list The list of menu items
 * @param p The parent menu
 */
Menu::Menu(QString n, QStringList list, Menu* p):
    name(n), menuItems(list), parent(p) {}

/**
 * @brief Menu destructor
 */
Menu::~Menu() {
    for(int x = 0; x < childMenus.length(); x++) {
        delete childMenus[x];
    }
}

/**
 * @brief Get the parent menu
 * @return parent menu as a Menu pointer
 */
Menu* Menu::getParent() { return parent; }

/**
 * @brief Get a specific child menu
 * @param i Index of the child menu
 * @return child menu at the specified index as a Menu pointer
*/
Menu* Menu::getChildMenu(int i) { return childMenus[i]; }

/**
 * @brief Get the name of the menu
 * @return menu name as a QString
*/
QString Menu::getName() { return name; }

/**
 * @brief Get a specific menu item
 * @param i Index of the menu item
 * @return menu item at the specified index as a QString
*/
QString Menu::getMenuItem(int i) { return menuItems[i]; }

/**
 * @brief Get all menu items-
 * @return menu items as a QStringList
*/
QStringList Menu::getMenuItems() { return menuItems; }

/**
 * @brief Add a child menu
 * @param m Pointer to the Menu object to be added as a child
*/
void Menu::addChildMenu(Menu *m) {
    childMenus.push_back(m);
}

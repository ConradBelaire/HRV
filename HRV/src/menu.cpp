#include "menu.h"

/**
 * Menu constructor
 *
 * @param n The name of the menu
 * @param list The list of menu items
 * @param p The parent menu
 */
Menu::Menu(QString n, QStringList list, Menu* p):
    name(n), menuItems(list), parent(p) {}

/**
 * Menu destructor
 */
Menu::~Menu() {
    for(int x = 0; x < childMenus.length(); x++) {
        delete childMenus[x];
    }
}

// getters
/**
Get the parent menu
@return parent menu as a Menu pointer
*/
Menu Menu::getParent() { return parent; }
/**
Get a specific child menu
@param i Index of the child menu
@return child menu at the specified index as a Menu pointer
*/
Menu Menu::getChildMenu(int i) { return childMenus[i]; }
/**
Get the name of the menu
@return menu name as a QString
*/
QString Menu::getName() { return name; }
/**
Get a specific menu item
@param i Index of the menu item
@return menu item at the specified index as a QString
*/
QString Menu::getMenuItem(int i) { return menuItems[i]; }
/**
Get all menu items-
@return menu items as a QStringList
*/
QStringList Menu::getMenuItems() { return menuItems; }

// methods 
/**
Add a child menu
@param m Pointer to the Menu object to be added as a child
*/
void Menu::addChildMenu(Menu m) {
    childMenus.push_back(m);
}
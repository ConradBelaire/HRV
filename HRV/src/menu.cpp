#include "menu.h"

Menu::Menu(QString n, QStringList list, Menu* p):
    name(n), menuItems(list), parent(p) {}

Menu::~Menu() {
    for(int x = 0; x < childMenus.length(); x++) {
        delete childMenus[x];
    }
}


void Menu::addChildMenu(Menu* m) {
    childMenus.push_back(m);
}


// getters
QString Menu::getName() { return name; }
QStringList Menu::getMenuItems() { return menuItems; }
QString Menu::getMenuItem(int i) { return menuItems[i]; }
Menu* Menu::getParent() { return parent; }
Menu* Menu::getChildMenu(int i) { return childMenus[i]; }

#ifndef MENU_H
#define MENU_H

#include <QString>
#include <QStringList>
#include <QVector>


/* Class Purpose: Menu class serves as an objet that will be displayed and interacted with on the main window
 *
 * Data Members:
 * -QString name: a QString representation of the menu title
 * -QStringList menuItems: a list of sub menu's titles a menu has
 * -int position: the position of the menu in a menu list
 * -QVector<Menu*> subMenus: a list of subMenu objects that a menu has
 * -Menu* parent: The parent menu that a menu has in the tree
 *
 * Class Functions:
 * -Getters and Setters
 * -void addChildMenu(Menu*): adds a menu to the subMenus vector
 */

class Menu {

public:
    explicit Menu(QString, QStringList, Menu*);
    ~Menu();

    QString getName();
    QStringList getMenuItems();
    Menu* getParent();
    int getPos();
    Menu* get(int);
    void addChildMenu(Menu*);

private:
    QString name;
    QStringList menuItems;
    int position;
    QVector<Menu*> subMenus;
    Menu* parent;

};

#endif // MENU_H

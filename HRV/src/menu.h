#ifndef MENU_H
#define MENU_H

#include <QString>
#include <QStringList>
#include <QVector>

class Menu {
    public:
        Menu(QString, QStringList, Menu*);
        ~Menu();

        QString getName();
        QStringList getMenuItems();
        Menu* getParent();
        void addChildMenu(Menu*);
        Menu* getChildMenu(int);
    private:
        QString name;
        QStringList menuItems;
        Menu* parent;
        QVector<Menu*> childMenus;

};

#endif

#ifndef MENU_H
#define MENU_H

#include <QString>
#include <QStringList>
#include <QVector>

class Menu {
    public:
        // constructor
        Menu(QString, QStringList, Menu*);
        // destructor
        ~Menu();
        
        // getters
        Menu* getParent();
        Menu* getChildMenu(int);
        QString getName();
        QString getMenuItem(int i);
        QStringList getMenuItems();

        // methods
        void addChildMenu(Menu*);

    private:
        QString name;
        QStringList menuItems;
        Menu* parent;
        QVector<Menu*> childMenus;

};

#endif

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

#include "menu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Menu* masterMenu;
    Menu* mainMenuOG;

    Ui::MainWindow *ui;
    QListWidget *activeQListWidget;


    void updateMenu(const QString, const QStringList);
    void initializeMainMenu(Menu*);


    // For testing purposes!
    bool redLED;
    bool greenLED;
    bool blueLED;


private slots:
    void navigateDownMenu();
    void navigateUpMenu();
    void navigateSubMenu();
    void navigateToMainMenu();
    void navigateBack();
    void toggleRedLED();
    void toggleGreenLED();
    void toggleBlueLED();
};
#endif // MAINWINDOW_H

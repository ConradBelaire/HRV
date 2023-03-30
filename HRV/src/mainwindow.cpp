#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->redButton, SIGNAL(released()), this, SLOT (toggleRedLED()));
    connect(ui->greenButton, SIGNAL(released()), this, SLOT (toggleGreenLED()));
    connect(ui->blueButton, SIGNAL(released()), this, SLOT (toggleBlueLED()));

    redLED = true;
    greenLED = true;
    blueLED = true;



    // Create menu tree
    masterMenu = new Menu("MAIN MENU", {"START NEW SESSION","SETTINGS","LOG/HISTORY"}, nullptr);
            //new Menu("MAIN MENU", {"CHALLENGE SETTING","PACER DURATION SETTING","RESET DEVICE"}, nullptr);
    mainMenuOG = masterMenu;
    initializeMainMenu(masterMenu);

    // Initialize the main menu view
    activeQListWidget = ui->mainMenuListView;
    activeQListWidget->addItems(masterMenu->getMenuItems());
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(masterMenu->getName());


    // device interface button connections
    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::navigateUpMenu);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::navigateDownMenu);
    connect(ui->backButton, &QPushButton::pressed, this, &MainWindow::navigateBack);
    connect(ui->okButton, &QPushButton::pressed, this, &MainWindow::navigateSubMenu);
    connect(ui->menuButton, &QPushButton::pressed, this, &MainWindow::navigateToMainMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems) {

    activeQListWidget->clear();
    activeQListWidget->addItems(menuItems);
    activeQListWidget->setCurrentRow(0);

    ui->menuLabel->setText(selectedMenuItem);
}

void MainWindow::initializeMainMenu(Menu *m)
{
    Menu* session = new Menu("SESSION", {}, m);
    Menu* settings = new Menu("SETTINGS", {"CHALLENGE SETTING","PACER DURATION SETTING","RESET DEVICE"}, m);
    Menu* logs = new Menu("LOGS", {}, m);

    m->addChildMenu(session);
    m->addChildMenu(settings);
    m->addChildMenu(logs);

}

void MainWindow::navigateUpMenu() {

    int nextIndex = activeQListWidget->currentRow() - 1;

    if (nextIndex < 0) {
        nextIndex = activeQListWidget->count() - 1;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}

void MainWindow::navigateSubMenu() {

    int index = activeQListWidget->currentRow();
    if (index < 0) return;


    //Check if it's an item with a submenu first
    if (!masterMenu->getMenuItems()[index].compare("SETTINGS") ||
            !masterMenu->getMenuItems()[index].compare("")) { // Add more checks in this style
        masterMenu = masterMenu->get(index);
        MainWindow::updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        return;
    }
    else {
        //cout << masterMenu->getMenuItems()[index].compare("START NEW SESSION") << endl;
        if (!masterMenu->getMenuItems()[index].compare("START NEW SESSION")) {
            cout << "Session time" << endl;
        }
        else if (!masterMenu->getMenuItems()[index].compare("LOG/HISTORY")) {
            cout << "log time" << endl;
        }
    }

}

void MainWindow::navigateToMainMenu()
{
    while (masterMenu->getName() != "MAIN MENU") {
        masterMenu = masterMenu->getParent();
    }

    updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
}


void MainWindow::navigateDownMenu() {

    int nextIndex = activeQListWidget->currentRow() + 1;

    if (nextIndex > activeQListWidget->count() - 1) {
        nextIndex = 0;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}

void MainWindow::navigateBack() {

    if (masterMenu->getName() == "MAIN MENU") {
        activeQListWidget->setCurrentRow(0);
    }
    else {
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    }

    //ui->programViewWidget->setVisible(false);
    //ui->electrodeLabel->setVisible(false);
}



void MainWindow::toggleRedLED()
{
    if (redLED) { // Disabling
        ui->redLED->setStyleSheet("background-color: rgb(80, 0, 0)");
        redLED = false;
    }
    else { // Enabling
        ui->redLED->setStyleSheet("background-color: rgb(255, 0, 0)");
        redLED = true;
    }
    //cout << "red" << endl;
}

void MainWindow::toggleGreenLED()
{
    if (greenLED) { // Disabling
        ui->greenLED->setStyleSheet("background-color: rgb(0, 50, 0)");
        greenLED = false;
    }
    else { // Enabling
        ui->greenLED->setStyleSheet("background-color: rgb(0, 128, 0)");
        greenLED = true;
    }

}

void MainWindow::toggleBlueLED()
{
    if (blueLED) { // Disabling
        ui->blueLED->setStyleSheet("background-color: rgb(0, 0, 80)");
        blueLED = false;
    }
    else { // Enabling
        ui->blueLED->setStyleSheet("background-color: rgb(0, 0, 255)");
        blueLED = true;
    }
}


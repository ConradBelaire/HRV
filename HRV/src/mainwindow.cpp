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

    batteryLevel = 100.0;

    // TODO: connect charge button
    //connect(ui->chargeAdminButton, &QPushButton::released, this, &MainWindow::rechargeBattery);


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

    // battery
    connect(ui->chargeBatteryButton, &QPushButton::released, this, &MainWindow::rechargeBattery);
    connect(ui->batteryLevelAdminSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::changeBatteryLevel);
    ui->batteryLevelAdminSpinBox->setValue(batteryLevel);
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


// function will be used as a reference to the recharge button
void MainWindow::rechargeBattery(){
    changeBatteryLevel(100);
}

void MainWindow::changeBatteryLevel(double newLevel) {
    if (newLevel >= 0.0 && newLevel <= 100.0) {
        /*
        if (newLevel == 0.0 && powerStatus == true) {
            powerChange();
            profile->setBLvl(0);
        }else{
            profile->setBLvl(newLevel);
        }
        */

        batteryLevel = newLevel;

        ui->batteryLevelAdminSpinBox->setValue(newLevel);
        int newLevelInt = int(newLevel);
        ui->batteryLevelBar->setValue(newLevelInt);

        QString highBatteryHealth = "QProgressBar { selection-background-color: rgb(78, 154, 6); background-color: rgb(0, 0, 0); }";
        QString mediumBatteryHealth = "QProgressBar { selection-background-color: rgb(196, 160, 0); background-color: rgb(0, 0, 0); }";
        QString lowBatteryHealth = "QProgressBar { selection-background-color: rgb(164, 0, 0); background-color: rgb(0, 0, 0); }";

        if (newLevelInt >= 50) {
            ui->batteryLevelBar->setStyleSheet(highBatteryHealth);
        }
        else if (newLevelInt >= 20) {
            ui->batteryLevelBar->setStyleSheet(mediumBatteryHealth);
        }
        else {
            ui->batteryLevelBar->setStyleSheet(lowBatteryHealth);
        }
    }
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


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
}

MainWindow::~MainWindow()
{
    delete ui;
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
        ui->greenLED->setStyleSheet("background-color: rgb(0, 80, 0)");
        greenLED = false;
    }
    else { // Enabling
        ui->greenLED->setStyleSheet("background-color: rgb(0, 255, 0)");
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


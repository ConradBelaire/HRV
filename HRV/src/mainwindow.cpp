#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start_session(Session* session)
{
    QDateTime date = QDateTime::currentDateTime();
    QTimer timer;
    // make graph visible
    // start the timer
    init_timer(&timer);
    // initalize timer
    currentDurationCount = 0;

}

void MainWindow::init_timer(QTimer* timer)
{
    connect(timer, &QTimer::timeout, this, &MainWindow::update_timer);
    if (currentDurationCount == 10){
        std::cout << "Session Ended" << "\n";
    }else{
        std::cout << "Session time: "<< std::to_string(currentDurationCount) << "\n";
    }

    if (connectedStatus){
        timer->start();
    }
}

void MainWindow::update_timer()
{
    //drainBattery();
    // take in reading
    // update session

    // various scenarios for ending the session
    //if ()
    std::cout << "updated" << "\n";
}

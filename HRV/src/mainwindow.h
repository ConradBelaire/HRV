#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTimer>
#include <QDateTime>
#include <iostream>
#include <QtGlobal>
#include <QObject>
#include <QLabel>
#include <sstream>
#include <iomanip>

#include "session.h"
#include "menu.h"
#include "dbmanager.h"
#include "log.h"
#include "profile.h"
#include "qcustomplot.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        void UpdateMenu();
        void start_session(Session* session);
        void drainBattery();
        void resetDevice();
        void powerChange();


    private:
        Menu* masterMenu;
        Menu* mainMenuOG;
        Ui::MainWindow *ui;
        QListWidget* activeQListWidget;

        DBManager* dbmanager;
        Profile* profile;

        bool powerStatus;

        void changePowerStatus();
        //void powerChange();
        QVector<Log*> sessions;

        QTimer* timer;
        QString timeString;

        int currentTimerCount;
        bool connectedStatus;
        // int currentDurationCount;
        Session* currentSession;


        int pacer_dur;
        int challenge_level;
        // QString durationString;

        QStringList allLogs;
        void updateMenu(const QString&, const QStringList&);
        void initializeMainMenu(Menu*);
        void init_timer(QTimer* timer);
        void applyToSkin(bool checked);
        void start_session();
        void changeBatteryLevel(double newLevel);
        void rechargeBattery();


        void displaySummary();

        // util fucntions
        Menu* create_history_menu(Menu* m);
        Menu* create_settings_menu(Menu* m);
        bool is_session_num(QString log_id);
        QString floatToStringWithOneDecimalPlace(float value);
        void updatePacer();
        void turnOffLights();

        // For testing purposes!
        bool redLED;
        bool greenLED;
        bool blueLED;

        // HRV data
        int generateHR();
        float generateCS();


    private slots:
        void navigateDownMenu();
        void navigateUpMenu();
        void navigateSubMenu();
        void navigateToMainMenu();
        void navigateBack();
        void toggleRedLED();
        void toggleBlueLED();
        void toggleGreenLED();
        void update_timer();
};
#endif // MAINWINDOW_H

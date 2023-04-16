#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTimer>

#include "session.h"
#include "menu.h"
#include "dbmanager.h"
#include "log.h"
#include "profile.h"
#include "qcustomplot.h"

#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        // constructor
        MainWindow(QWidget *parent = nullptr);
        // destructor
        ~MainWindow();

        // public methods
        void start_session(Session* session);
        void updateMenu(const QString&, const QStringList&);
        void resetDevice();
    private:
        // menus
        Menu* masterMenu;   // current menu
        Menu* mainMenuOG;   // holds main menu

        Ui::MainWindow *ui;
        QListWidget* activeQListWidget; // for updating and getting the menu on screen
        DBManager* dbmanager;   // how we talk to the database in mainwindow
        Profile* profile;   // stores device profile for current session index and battery
        QVector<Session*> sessions; // list of previous sessions
        QTimer* timer;  // timer for calling update_timer()
        Session* currentSession;    // stores the current session

        QVector<QVector<double>> heartRateData; // stores harccoded heart rate data
        QStringList allLogs;    // stores list of logs: Session 1, Session 2 ...

        bool powerStatus;   // is the device on or off
        bool pacerCountUp;  // are we in the stage of counting up on the pacer?
        bool pacerWait; // are we in the stage of making the user hold their breath on the pacer?
        bool pacerCountDown;    // are we in the stage of counting down on the pacer?
        bool connectedStatus;   // is sensor connected?
        bool sessionSummaryVisible; // is the session summary visible>
        bool inSessionView; // is the session view visible?
        bool startSession;  // has user clicked ok to begin session
        int currentTimerCount;  // how long has this session been going
        int pacerCounter;   // keeps track of where we are in the pacer sequence
        int pacer_dur;  // how long should the user hold their breath
        int challenge_level;    // stores challenge level
        int currentHRvector;    // current vector we are obtaining heart rates from
        int vectorHRcount;  // keeps track for when we need to switch to a new hr vector
        int maxHR;  // max for graph y axis
        int minHR;  // min for graph y axis

        // Holds CSS strings for colouring LEDs
        QString redOn;
        QString redOff;
        QString greenOn;
        QString greenOff;
        QString blueOn;
        QString blueOff;
        QString connectionOn;
        QString connectionOff;

        // methods / util fucntions
        void initializeMainMenu(Menu*);
        void init_timer(QTimer* timer);
        void applyToSkin(bool checked);
        void start_session();
        void changeBatteryLevel(double newLevel);
        void rechargeBattery();
        void changePowerStatus();
        void displaySummary(Session* session, bool is_history);
        void clearSessionSummary();
        Menu* create_history_menu(Menu* m);
        Menu* create_settings_menu(Menu* m);
        bool is_session_num(QString log_id);
        void updatePacer();
        void turnOffLights();
        int generateHR();

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
        void toggleSkin();
        void dropTables();
        void drainBattery();
        void powerChange();
};
#endif // MAINWINDOW_H

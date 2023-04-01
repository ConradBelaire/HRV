#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTimer>
#include <QDateTime>
#include <iostream>
#include <QtGlobal>
#include <QObject>

#include "session.h"
#include "menu.h"
#include "dbmanager.h"
#include "log.h"
#include "profile.h"


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
        void powerChange();
        QVector<Log*> sessions;

        QTimer* timer;
        QString timeString;


        bool connectedStatus;
        int currentDurationCount;
        Session* currentSession;


        int pacer_dur;
        int challenge_level;
        // QString durationString;

        QStringList allLogs;
        void updateMenu(const QString&, const QStringList&);
        // void initMainMenu(Menu*);
        void init_timer(QTimer* timer);
        void update_timer();
    private slots:

};
#endif // MAINWINDOW_H

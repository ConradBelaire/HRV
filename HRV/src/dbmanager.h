#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QList>
#include <QApplication>

#include "profile.h"
#include "log.h"

class DBManager {

public:
    const QString DATE_FORMAT = "yyyy-MM-dd hh:mm";
    static const QString DATABASE_PATH;

    DBManager();
    bool addProfile(int id, double batterLvl);
    Profile* getProfile(int id);
    bool addLog(int profileId, int challengeLevel, float isLow, float isMed, float isHigh, float avgCoherence, int sessionTime, float achievementScore, QString graph, QString date);
    Log* getLog(int id);
    Log** getProfileLogs(int profileId);
    bool deleteLog(int id);
    bool deleteLogs();

private:
    QSqlDatabase denasDB;

    bool DBInit();

};

#endif // DBMANAGER_

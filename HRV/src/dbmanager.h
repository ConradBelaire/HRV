#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QList>
#include <QApplication>


class DBManager {

public:
    const QString DATE_FORMAT = "yyyy-MM-dd hh:mm";
    static const QString DATABASE_PATH;

    DBManager();
    bool addProfile(int id, double batterLvl);
    Profile* getProfile(int id);
    bool addSession(int profileId, int challengeLevel, float isLow, float isMed, float isHigh, float avgCoherence, int sessionTime, float achievementScore, QString graph, QString date);
    Session* getSession(int id);
    Session** getSessions(int profileId);
    bool deleteSession(int id);
    bool deleteSessions();

private:
    QSqlDatabase denasDB;

    bool DBInit();

};

#endif // DBMANAGER_

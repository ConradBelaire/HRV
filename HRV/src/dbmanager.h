#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QList>
#include <QApplication>
#include <string>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

#include "profile.h"
#include "log.h"

class DBManager {

public:
    const QString DATE_FORMAT = "yyyy-MM-dd hh:mm";
    static const QString DATABASE_PATH;

    DBManager();
    bool addProfile(int id, double batterLvl, int sessionAmt);
    Profile* getProfile(int id);
    bool updateProfile(int id, double batteryLvl, int sessionAmt);
    bool deleteProfile(int id);
    Log* getLog(int id);
    bool addLog(Log* log);
    QVector<Log*>* getLogs(int id);
    bool deleteLog(int id);
    bool deleteLogs();
    bool doesLogExist(int id);

private:
    QSqlDatabase hrvDB;

    bool DBInit();

};

#endif // DBMANAGER_H

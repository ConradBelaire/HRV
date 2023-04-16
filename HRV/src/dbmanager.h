#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
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
    // database path
    static const QString DATABASE_PATH;

    // constructor
    DBManager();

    // methods
    void dropTables();
    bool doesLogExist(int id);

    // CRUD operations
    bool deleteLogs();
    bool addLog(Log* log);
    bool deleteLog(int id);
    bool deleteProfile(int id);
    bool updateProfile(int id, double batteryLvl);
    bool addProfile(int id, double batterLvl, int sessionAmt);
    Log* getLog(int id);
    QVector<Log*>* getLogs();
    Profile* getProfile(int id);
    int getLogCount(int profile_id);

private:
    QSqlDatabase hrvDB;
    bool DBInit();
};

#endif // DBMANAGER_H
